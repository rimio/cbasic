#include <iostream>
#include <cassert>
#include <string>
#include <getopt.h>

#include "configure.h"
#include "error/error.h"
#include "parser/parser-context.h"
#include "parser/constant-folding.h"
#include "parser/find-symbols.h"
#include "symbols/symbol-table.h"

//
// getopt_long options
//
static struct option long_options[] =
{
	{ "version",	no_argument,		NULL, 		'v' },
	{ "help",		no_argument,		NULL,		'h' },
	{ "output",		required_argument,	NULL,		'o' },

	// End
	{ NULL,			0,					NULL, 		0 }
};

//
// Program arguments
//
static std::string *output_file = nullptr;
static std::string *input_file = nullptr;

//
// Print version
//
void print_version ()
{
	std::cout << "CBASIC compiler " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
}

//
// Print legal info
//
void print_legal ()
{
	std::cout << "Copyright (C) 2015 Vasile Vilvoiu" << std::endl;
	std::cout << "This software is released under the MIT license." << std::endl;
	std::cout << "This is free software; see the source for copying conditions.  There is NO" << std::endl;
	std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;

}

//
// Print help and usage text
//
void print_usage ()
{
	print_version ();
	std::cout << std::endl;
	std::cout << "Usage: cbasic [OPTION]... FILE" << std::endl;

}

//
// Parse arguments
//
int parse_args (int argc, char **argv)
{
	while (1)
	{
		// get option
		int option_index = -1;
		int c = getopt_long (argc, argv, "vho:", long_options, &option_index);
		if (c == -1)
		{
			// Finished
			break;
		}

		// parse option
		switch (c)
		{
			case 0:
				// Flag case
				if (long_options[option_index].flag != 0)
				{
					break;
				}
				else
				{
					// This is not a flag and the value char given was zero.
					// Should not happen
					Error::internalError ("short name is zero for non-flag parameter");
					return ER_FAILED;
				}

			case 'v':
				print_version ();
				std::cout << std::endl;
				print_legal ();
				return NO_ERROR;

			case 'h':
				print_usage ();
				return NO_ERROR;

			case 'o':
				if (optarg != NULL)
				{
					output_file = new std::string (optarg);
				}
				else
				{
					Error::internalError ("output file not provided");
					return ER_FAILED;
				}
				break;

			case '?':
				/* error message should have been printed */
				return ER_FAILED;

			default:
				Error::internalError ("fatal error parsing input arguments");
				return ER_FAILED;
		}
	}

	// Parse filename
	if (argc == optind + 1)
	{
		if (argv[optind - 1] != NULL)
		{
			input_file = new std::string (argv[optind]);
			return NO_ERROR;
		}
		else
		{
			Error::internalError ("file argument pointer is NULL");
			return ER_FAILED;
		}
	}
	else if (argc > optind + 1)
	{
		Error::internalError ("multiple input files not allowed");
		return ER_FAILED;
	}

	// All ok
	return NO_ERROR;
}


//
// Compiler entry point
//
int main (int argc, char **argv)
{
	if (argc <= 1)
	{
		// Print usage and exit
		print_usage ();
		return NO_ERROR;
	}
	else
	{
		// Parse arguments
		if (parse_args (argc, argv) != NO_ERROR)
		{
			return ER_FAILED;
		}

		// Check minimum set of arguments
		if (input_file == nullptr)
		{
			Error::error ("input file was not provided");
			return ER_FAILED;
		}
	}

	// Parse
	ParserContext *pc = new ParserContext ();
	if (pc->parseFile (*input_file) != NO_ERROR)
	{
		// Error should have been printed
		return ER_FAILED;
	}

	std::cout << std::endl << "Reprinted program: " << std::endl;
	pc->printProgram (std::cout);
	std::cout << std::endl << std::endl << "AST:" << std::endl;
	pc->printTree (std::cout);

	// Walk check
	std::cout << std::endl << "Original program: " << std::endl;
	pc->printProgram (std::cout);

	TreeWalker::leafToRoot (pc->getRoot (), fold_constants, false);

	std::cout << std::endl << std::endl << "Optimized program: " << std::endl;
	pc->printProgram (std::cout);

	// Find symbols
	TreeWalker::leafToRoot (pc->getRoot (), find_symbols, false);
	std::cout << std::endl << std::endl << "Symbol table: " << std::endl;
	SymbolTable::debugPrint ();

	// All ok
	return 0;
}

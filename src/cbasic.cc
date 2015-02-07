#include <iostream>
#include <cassert>
#include <string>
#include <getopt.h>

#include "configure.h"
#include "error/error.h"
#include "verbose.h"
#include "parser/parser-context.h"
#include "symbols/symbol-table.h"
#include "ilang/il-program.h"

//
// getopt_long options
//
static struct option long_options[] =
{
	{ "version",	no_argument,		NULL, 		'v' },
	{ "help",		no_argument,		NULL,		'h' },
	{ "output",		required_argument,	NULL,		'o' },
	{ "verbose",	required_argument,	NULL,		'V' },

	// End
	{ NULL,			0,					NULL, 		0 }
};

//
// Program arguments
//
static std::string *output_file = nullptr;
static std::string *input_file = nullptr;
static std::string *assembly_file = nullptr;

unsigned int verbose_flags = 0;

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
	std::cout << "Usage: cbasic [OPTIONS]... FILE" << std::endl;
	std::cout << std::endl;
	std::cout << "Mandatory arguments to long options are mandatory for short options too." << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -h, --help              display this help screen" << std::endl;
	std::cout << "  -v, --version           output version information" << std::endl;
	std::cout << "  -o, --output=FILE       specify the binary output file" << std::endl;
	std::cout << "  -V, --verbose=LEVEL     specify verbose level as a sum of the following flags:" << std::endl;
	std::cout << "                            0 - nothing is printed (default when oprion is not present)" << std::endl;
	std::cout << "                            1 - print program after parsing" << std::endl;
	std::cout << "                            2 - print abstract syntax tree after parsing" << std::endl;
	std::cout << "                            4 - print symbol tables after semantic analysis" << std::endl;
	std::cout << "                            8 - print program after semantic analysis" << std::endl;
	std::cout << "                           16 - print generated intermediate language program" << std::endl;
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
		int c = getopt_long (argc, argv, "vho:V:", long_options, &option_index);
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

			case 'V':
				if (optarg != NULL)
				{
					verbose_flags = atoi (optarg);
					if (verbose_flags > VERBOSE_FLAG_MAX)
					{
						Error::internalError("verbose flags out of bounds");
						return ER_FAILED;
					}
				}
				else
				{
					Error::internalError ("verbose flags not provided");
					return ER_FAILED;
				}
				break;

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

		// Set assembly code file
		assembly_file = new std::string (*input_file + ".nasm");

		// Default output file if one not provided
		if (output_file == nullptr)
		{
			output_file = new std::string (*input_file + ".out");
		}
	}

	//
	// LEXICAL AND SYNTACTIC ANALYSIS
	//
	ParserContext *pc = new ParserContext ();
	if (pc->parseFile (*input_file) != NO_ERROR)
	{
		// Error should have been printed
		return ER_FAILED;
	}

	// VERBOSE code
	if (VERBOSE_PRINT_AST)
	{
		std::cout << std::endl << "[VERBOSE] Abstract Syntax Tree:" << std::endl;
		pc->printTree (std::cout);
		std::cout << "[VERBOSE END]" << std::endl << std::endl;
	}
	if (VERBOSE_PRINT_PARSED)
	{
		std::cout << std::endl << "[VERBOSE] Original program: " << std::endl;
		pc->printProgram (std::cout);
		std::cout << "[VERBOSE END]" << std::endl << std::endl;
	}

	//
	// SEMANTIC ANALYSIS
	//
	if (pc->semanticAnalysis () != NO_ERROR)
	{
		// Error should have been printed
		return ER_FAILED;
	}

	// VERBOSE code
	if (VERBOSE_PRINT_SYMBOLS)
	{
		std::cout << std::endl << "[VERBOSE] Symbol table: " << std::endl;
		SymbolTable::debugPrint ();
		std::cout << "[VERBOSE END]" << std::endl << std::endl;
	}
	if (VERBOSE_PRINT_FINAL)
	{
		std::cout << std::endl << "[VERBOSE] Final program: " << std::endl;
		pc->printProgram (std::cout);
		std::cout << "[VERBOSE END]" << std::endl << std::endl;
	}

	//
	// INTERMEDIATE CODE GENERATION
	//
	IlProgram *program = pc->generateIlCode ();
	if (program == nullptr)
	{
		// Error message should have been set
		return ER_FAILED;
	}

	// VERBOSE code
	if (VERBOSE_PRINT_GENERATED_IL)
	{
		std::cout << std::endl << "[VERBOSE] Generated IL program: " << std::endl;
		program->debugPrint ();
		std::cout <<  "[VERBOSE END]" << std::endl << std::endl;
	}

	// All ok
	return 0;
}

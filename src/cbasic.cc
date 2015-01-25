#include <iostream>
#include <cassert>
#include <getopt.h>

#include "configure.h"
#include "error.h"

//
// getopt_long options
//
static struct option long_options[] =
{
	{ "version",	no_argument,		NULL, 		'v' },
	{ "help",		no_argument,		NULL,		'h' },

	// End
	{ NULL,			0,					NULL, 		0 }
};

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
		int c = getopt_long (argc, argv, "vh", long_options, &option_index);
		if (c == -1)
		{
			// Finished
			return NO_ERROR;
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

			case '?':
				/* error message should have been printed */
				return ER_FAILED;

			default:
				Error::internalError ("fatal error parsing input arguments");
				return ER_FAILED;
		}
	}
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
	}

	// All ok
	return 0;
}

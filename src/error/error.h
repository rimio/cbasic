#ifndef ERROR_H_
#define ERROR_H_

#include "parser/nodes/parser-node.h"

//
// Error codes
//
#define NO_ERROR				0
#define ER_FAILED				-1

//
// Errors handling routines gathered in a static class
//
class Error
{
private:
	// Common call point for all error routines
	static void reportError (const std::string error);

public:
	// Report a generic error
	static void error (const std::string error);

	// Report an internal error (unexpected behavior by the compiler)
	static void internalError (const std::string error);

	// Report a syntax error in the code
	static void syntaxError (const std::string error, int line, int column);

	// Report a semantic error in the code
	static void semanticError (const std::string error, ParserNode *node);
};

#endif

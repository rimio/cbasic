#include <iostream>
#include <cassert>
#include "error.h"

void Error::reportError (const std::string error)
{
	std::cerr << error << std::endl;
}

void Error::error (const std::string error)
{
	Error::reportError ("Error: " + error);
}

void Error::internalError (const std::string error)
{
	Error::reportError ("Internal error: " + error);
}

void Error::syntaxError (const std::string error, int line, int column)
{
	Error::reportError ("Syntax error at line " + std::to_string (line) + " column " + std::to_string (column) + ": " + error);
}

void Error::semanticError (const std::string error, ParserNode *node)
{
	Error::reportError ("Semantic error at line ??? column ???: " + error);
}

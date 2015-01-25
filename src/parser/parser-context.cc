#include <fstream>
#include "parser-context.h"

ParserContext::ParserContext ()
{
	lexer = nullptr;
	parser = nullptr;
}

ParserContext::~ParserContext ()
{
	if (lexer != nullptr)
	{
		delete lexer;
		lexer = nullptr;
	}

	if (parser != nullptr)
	{
		delete parser;
		parser = nullptr;
	}
}

int ParserContext::parseFile (std::string &filename)
{
	// Open file
	std::ifstream input_file (filename);
	if (!input_file.good ())
	{
		Error::error ("failed to open file '" + filename + "'");
		return ER_FAILED;
	}

	// Allocate lexer
	if (lexer != nullptr)
	{
		delete lexer;
	}
	lexer = new Lexer (&input_file);

	// Allocate parser
	if (parser != nullptr)
	{
		delete parser;
	}
	parser = new Parser (*lexer, *this);

	// Parse
	if (parser->parse () != NO_ERROR)
	{
		Error::error ("file parsing failed");
		return ER_FAILED;
	}

	// All ok
	return NO_ERROR;
}

int ParserContext::printTree (std::ostream &stream)
{
	return 0;
}

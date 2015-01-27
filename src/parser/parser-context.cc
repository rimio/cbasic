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
	parser = new Parser (*lexer, *this, &root_node);

	// Parse
	if (parser->parse () != NO_ERROR)
	{
		Error::error ("file parsing failed");
		return ER_FAILED;
	}

	// All ok
	return NO_ERROR;
}

void ParserContext::printTreeRecursive (std::ostream &stream, int level, ParserNode *node)
{
	if (node == nullptr)
		return;

	// Pad for level
	for (int i = 0; i < level; i ++)
		std::cout << "  ";

	// Print text
	std::cout << node->toString () << std::endl;

	// Print children
	std::list<ParserNode *> ch = node->getChildren();
	for (std::list<ParserNode *>::iterator it = ch.begin(); it != ch.end(); it ++)
	{
		printTreeRecursive (stream, level+1, *it);
	}
}

int ParserContext::printTree (std::ostream &stream)
{
	if (root_node != nullptr)
	{
		printTreeRecursive (stream, 0, root_node);
		return NO_ERROR;
	}
	else
	{
		Error::internalError ("empty parse tree, can't print");
		return ER_FAILED;
	}
}

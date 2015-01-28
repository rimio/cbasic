#include <fstream>
#include "parser-context.h"

ParserContext::ParserContext ()
{
	lexer_ = nullptr;
	parser_ = nullptr;
	root_node_ = nullptr;
}

ParserContext::~ParserContext ()
{
	if (lexer_ != nullptr)
	{
		delete lexer_;
		lexer_ = nullptr;
	}

	if (parser_ != nullptr)
	{
		delete parser_;
		parser_ = nullptr;
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
	if (lexer_ != nullptr)
	{
		delete lexer_;
	}
	lexer_ = new Lexer (&input_file);

	// Allocate parser
	if (parser_ != nullptr)
	{
		delete parser_;
	}
	parser_ = new Parser (*lexer_, *this, &root_node_);

	// Parse
	if (parser_->parse () != NO_ERROR)
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

	// Print list
	if (node->getNodeType () == PT_STATEMENT)
	{
		StatementNode *st = (StatementNode *) node;
		printTreeRecursive (stream, level, st->getNext ());
	}
}

int ParserContext::printTree (std::ostream &stream)
{
	if (root_node_ != nullptr)
	{
		printTreeRecursive (stream, 0, root_node_);
		return NO_ERROR;
	}
	else
	{
		Error::internalError ("empty parse tree, can't print");
		return ER_FAILED;
	}
}

int ParserContext::printProgram (std::ostream &stream)
{
	if (root_node_ != nullptr)
	{
		stream << root_node_->print ();
		return NO_ERROR;
	}
	else
	{
		Error::internalError ("empty parse tree, can't print");
		return ER_FAILED;
	}
}

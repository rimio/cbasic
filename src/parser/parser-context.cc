#include <fstream>
#include "parser-context.h"

#include "parser/operations/type-checking.h"
#include "parser/operations/find-symbols.h"
#include "parser/operations/resolve-identifiers.h"
#include "parser/operations/constant-folding.h"
#include "symbols/symbol-table.h"
#include "tree-walker.h"

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

int ParserContext::semanticAnalysis ()
{
	// Find symbols
	root_node_ = TreeWalker::leafToRoot (root_node_, find_symbols, false);

	// Resolve identifiers
	root_node_ = TreeWalker::leafToRoot (root_node_, resolve_identifiers, false);

	// Check types
	root_node_ = TreeWalker::leafToRoot (root_node_, check_types, false);

	// Fold constants
	root_node_ = TreeWalker::leafToRoot (root_node_, fold_constants, false);

	// All ok
	return NO_ERROR;
}

void ParserContext::printTreeRecursive (std::ostream &stream, int level, ParserNode *node)
{
	if (node == nullptr)
		return;

	// Print address
	std::cout << "<addr=" << node << ">  ";

	// Pad for level
	for (int i = 0; i < level; i ++)
		std::cout << "  ";

	// Print text
	std::cout << node->toString ();
	if (node->getNext () != nullptr)
	{
		std::cout << "  <next=" << node->getNext () << ">";
	}
	std::cout << std::endl;

	// Print children
	std::list<ParserNode *> ch = node->getChildren();
	for (std::list<ParserNode *>::iterator it = ch.begin(); it != ch.end(); it ++)
	{
		printTreeRecursive (stream, level+1, *it);
	}

	// Print list
	printTreeRecursive (stream, level, node->getNext ());
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
		stream << root_node_->print ("") << std::endl;
		return NO_ERROR;
	}
	else
	{
		Error::internalError ("empty parse tree, can't print");
		return ER_FAILED;
	}
}

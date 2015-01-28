#ifndef PARSER_CONTEXT_H_
#define PARSER_CONTEXT_H_

#include <string>
#include "error/error.h"
#include "parser.hh"
#include "lexer.h"
#include "nodes/parser-node.h"

using namespace yy;

class ParserContext
{
private:
	Parser *parser_;
	Lexer *lexer_;
	ParserNode *root_node_;

	// Print a tree node to a stream and call itself on all children.
	// "level" determines indentation level and increases with each layer.
	void printTreeRecursive (std::ostream &stream, int level, ParserNode *node);

public:
	ParserContext ();
	~ParserContext ();

	// Get the root node
	ParserNode *getRoot () const { return root_node_; }

	// Parse a file, given the filename
	int parseFile (std::string& filename);

	// Print the parse tree
	int printTree (std::ostream &stream);

	// Print the program (from AST)
	int printProgram (std::ostream &stream);
};

#endif

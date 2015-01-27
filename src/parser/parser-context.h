#ifndef PARSER_CONTEXT_H_
#define PARSER_CONTEXT_H_

#include <string>
#include "error/error.h"
#include "parser.hh"
#include "lexer.h"
#include "parser-node.h"

using namespace yy;

class ParserContext
{
private:
	Parser *parser;
	Lexer *lexer;
	ParserNode *root_node;

	// Print a tree node to a stream and call itself on all children.
	// "level" determines indentation level and increases with each layer.
	void printTreeRecursive (std::ostream &stream, int level, ParserNode *node);

public:
	ParserContext ();
	~ParserContext ();

	// Parse a file, given the filename
	int parseFile (std::string& filename);

	// Print the parse tree
	int printTree (std::ostream &stream);
};

#endif

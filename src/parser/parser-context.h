#ifndef PARSER_CONTEXT_H_
#define PARSER_CONTEXT_H_

#include <string>
#include "error/error.h"
#include "parser.hh"
#include "lexer.h"

using namespace yy;

class ParserContext
{
private:
	Parser *parser;
	Lexer *lexer;

public:
	ParserContext ();
	~ParserContext ();

	// Parse a file, given the filename
	int parseFile (std::string& filename);

	// Print the parse tree
	int printTree (std::ostream &stream);
};

#endif

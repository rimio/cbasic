#ifndef PARSER_CONTEXT_H_
#define PARSER_CONTEXT_H_

#include <string>
#include "error.h"
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

	int parseFile (std::string& filename);
	int printTree (std::ostream &stream);
};

#endif

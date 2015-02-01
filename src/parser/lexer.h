#ifndef LEXER_H_
#define LEXER_H_

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"
using namespace yy;

#undef  YY_DECL
#define YY_DECL int Lexer::yylex()

class Lexer : public yyFlexLexer
{
private:
	int yylex ();
	Parser::semantic_type *yylval;
	yy::location location_;

public:
	Lexer (std::istream *input) : yyFlexLexer (input), yylval (NULL) { };

	// Get and set location
	yy::location getLocation () const { return location_; }
	void setLocation (yy::location loc) { location_ = loc; }

	int yylex (Parser::semantic_type *lval)
	{
		yylval = lval;
		return yylex ();
	}
};

#endif

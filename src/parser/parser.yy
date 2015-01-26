%skeleton "lalr1.cc"
%require  "3.0"
%locations
%debug
%defines
%define parser_class_name {Parser}

%code requires
{
	class ParserContext;
	class Lexer;
}

%lex-param		{ Lexer &lexer  }
%parse-param	{ Lexer &lexer  }

%lex-param		{ ParserContext &context }
%parse-param	{ ParserContext &context }

%code
{

#include <iostream>
#include "error.h"
#include "parser-context.h"
#include "lexer.h"

static int yylex (Parser::semantic_type *yylval, Parser::location_type *loc, Lexer &lexer, ParserContext &context);

}

%union {
	int ival;
	float fval;
	std::string *sval;
}

%token END 0
%token MINUS
%token NEWLINE
%token PLUS
%token POWER
%token SLASH
%token STAR

%token <ival> ILITERAL
%token <fval> FLITERAL
%token <sval> SLITERAL

%%

statement
	: plus_minus
	| plus_minus NEWLINE;

plus_minus
	: plus_minus PLUS plus_minus
	| plus_minus MINUS plus_minus
	| literal
	;

literal
	: ILITERAL
	| FLITERAL
	| SLITERAL
	;

%%

void Parser::error (const Parser::location_type& loc, const std::string &err)
{
	Error::syntaxError (err, loc.begin.line, loc.begin.column);
}

static int yylex (Parser::semantic_type *yylval, Parser::location_type *loc, Lexer &lexer, ParserContext &context)
{
	return lexer.yylex (yylval);
}

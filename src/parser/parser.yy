%skeleton "lalr1.cc"
%require  "3.0"
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

static int yylex (Parser::semantic_type *yylval, Lexer &lexer, ParserContext &context);

}

%union {
	int ival;
	float fval;
	char *sval;
}

%token END 0
%token NEWLINE

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%%

start: INT FLOAT STRING

%%

void Parser::error (const std::string &err)
{
	Error::syntaxError (err); 
}

static int yylex (Parser::semantic_type *yylval, Lexer &lexer, ParserContext &context)
{
	return lexer.yylex (yylval);
}

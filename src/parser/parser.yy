%skeleton "lalr1.cc"
%require  "3.0"
%locations
%debug
%defines
%define parser_class_name {Parser}

%code requires
{
	#include "parser-node.h"
	#include "value-nodes.h"
	#include "operator-nodes.h"

	class ParserContext;
	class Lexer;
}

%lex-param		{ Lexer &lexer  }
%parse-param	{ Lexer &lexer  }

%lex-param		{ ParserContext &context }
%parse-param	{ ParserContext &context }

%parse-param	{ ParserNode **root_node }

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

	ParserNode *parser_node;
	ValueNode *value_node;
	OperatorNode *operator_node;
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


%type <value_node> literal
%type <parser_node> plus_minus
%type <parser_node> program

%%

program
	: plus_minus
		{
			*root_node = $1;
		}
	| plus_minus NEWLINE
		{
			*root_node = $1;
		}
	;

plus_minus
	: plus_minus PLUS plus_minus
		{
			$$ = new PlusOperatorNode ($1, $3);
		}
	| plus_minus MINUS plus_minus
		{
			$$ = new MinusOperatorNode ($1, $3);
		}
	| literal
		{
			$$ = $1;
		}
	;

literal
	: ILITERAL
		{
			$$ = new IntegerValueNode ($1);
		}
	| FLITERAL
		{
			$$ = new FloatValueNode ($1);
		}
	| SLITERAL
		{
			$$ = new StringValueNode (*$1);
		}
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

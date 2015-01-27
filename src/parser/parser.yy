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

%token BACKSLASH
%token END 0
%token MINUS
%token MODULO
%token NEWLINE
%token PLUS
%token POWER
%token SLASH
%token STAR

%token <ival> ILITERAL
%token <fval> FLITERAL
%token <sval> SLITERAL


%type <parser_node>		intdiv_op
%type <value_node>		literal
%type <parser_node>		modulo_op
%type <parser_node>		mul_div_op
%type <parser_node>		negation_op
%type <parser_node>		operand
%type <parser_node>		plus_minus_op
%type <parser_node>		power_op
%type <parser_node>		program

%%

program
	: plus_minus_op
		{
			*root_node = $1;
		}
	| plus_minus_op NEWLINE
		{
			*root_node = $1;
		}
	;

plus_minus_op
	: plus_minus_op PLUS plus_minus_op
		{
			$$ = new PlusOperatorNode ($1, $3);
		}
	| plus_minus_op MINUS plus_minus_op
		{
			$$ = new MinusOperatorNode ($1, $3);
		}
	| modulo_op
		{
			$$ = $1;
		}
	;

modulo_op
	: modulo_op MODULO modulo_op
		{
			$$ = new ModuloOperatorNode ($1, $3);
		}
	| intdiv_op
		{
			$$ = $1;
		}

intdiv_op
	: intdiv_op BACKSLASH intdiv_op
		{
			$$ = new IntDivisionOperatorNode ($1, $3);
		}
	| mul_div_op
		{
			$$ = $1;
		}

mul_div_op
	: mul_div_op STAR mul_div_op
		{
			$$ = new MultiplicationOperatorNode ($1, $3);
		}
	| mul_div_op SLASH mul_div_op
		{
			$$ = new DivisionOperatorNode ($1, $3);
		}
	| negation_op
		{
			$$ = $1;
		}
	;

negation_op
	: MINUS negation_op
		{
			$$ = new MinusOperatorNode ($2, nullptr);

			// Check for chained negations
			if ($2->getNodeType () == PT_OPERATOR)
			{
				OperatorNode *opn = (OperatorNode *) $2;
				if (opn->getOperatorType () == OT_MINUS)
				{
					MinusOperatorNode *mopn = (MinusOperatorNode *) opn;
					if (mopn->getRight () == nullptr)
					{
						error (@2, "chained negation not allowed");
					}
				}
			}
		}
	| power_op
		{
			$$ = $1;
		}

power_op
	: power_op POWER power_op
		{
			$$ = new PowerOperatorNode ($1, $3);
		}
	| operand
		{
			$$ = $1;
		}
	;

operand
	: literal
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

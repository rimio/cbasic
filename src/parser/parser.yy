%skeleton "lalr1.cc"
%require  "3.0"
%locations
%debug
%defines
%define parser_class_name {Parser}
%define parse.error verbose

%code requires
{
	#include "nodes/parser-node.h"
	#include "nodes/value-nodes.h"
	#include "nodes/operator-nodes.h"
	#include "nodes/identifier-node.h"
	#include "nodes/statement-nodes.h"

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
	TypedParserNode *typed_node;
	ValueNode *value_node;
	IdentifierNode *identifier_node;
	StatementNode *statement_node;
}

%token AND
%token BACKSLASH
%token COMMA
%token DIM
%token END 0
%token EQUAL
%token GT
%token GT_EQ
%token LET
%token LT
%token LT_EQ
%token MINUS
%token MODULO
%token NEWLINE
%token NOT
%token NOT_EQUAL
%token OR
%token PAR_CLOSE
%token PAR_OPEN
%token PLUS
%token POWER
%token SLASH
%token STAR
%token XOR

%token <ival> ILITERAL
%token <fval> FLITERAL
%token <sval> SLITERAL
%token <sval> IIDENTIFIER
%token <sval> FIDENTIFIER
%token <sval> SIDENTIFIER


%type <statement_node>	allocation_statement_part
%type <statement_node>	allocation_statement_part_list
%type <typed_node>		and_op
%type <typed_node>		comparison_op
%type <typed_node>		expression
%type <typed_node>		expression_list
%type <identifier_node>	identifier
%type <typed_node>		intdiv_op
%type <value_node>		literal
%type <typed_node>		modulo_op
%type <typed_node>		mul_div_op
%type <typed_node>		negation_op
%type <typed_node>		not_op
%type <typed_node>		operand
%type <typed_node>		or_xor_op
%type <typed_node>		plus_minus_op
%type <typed_node>		power_op
%type <statement_node>	statement
%type <statement_node>	statement_list
%type <statement_node>	statement_with_newline

%%

program
	: statement_with_newline END
		{
			*root_node = $1;
		}
	| statement END
		{
			*root_node = $1;
		}
	| statement_list END
		{
			*root_node = $1;
		}
	;

statement_list
	: statement_with_newline statement_list
		{
			$$ = $1;

			// Some statement rules like the allocation statement can generate node lists.
			// Walk to end and chain.
			ParserNode *list = $$;
			while (list->getNext () != nullptr)
			{
				list = list->getNext ();
			}
			list->setNext ($2);
		}
	| statement_with_newline
		{
			$$ = $1;
		}
	;

statement_with_newline
	: statement NEWLINE
		{
			$$ = $1;
		}
	| NEWLINE statement
		{
			$$ = $2;
		}
	;

statement
	: LET identifier EQUAL expression
		{
			$$ = new AssignmentStatementNode ($2, $4);
		}
	| DIM allocation_statement_part_list
		{
			$$ = $2;
		}
	;

allocation_statement_part_list
	: allocation_statement_part COMMA allocation_statement_part_list
		{
			$$ = $1;
			$$->setNext ($3);
		}
	| allocation_statement_part
		{
			$$ = $1;
		}
	;

allocation_statement_part
	: identifier PAR_OPEN expression_list PAR_CLOSE
		{
			$$ = new AllocationStatementNode ($1, $3);
		}
	;

expression_list
	: expression COMMA expression_list
		{
			$$ = $1;
			$$->setNext ($3);
		}
	| expression
		{
			$$ = $1;
		}
	;

expression
	: or_xor_op
		{
			$$ = $1;
		}
	;

or_xor_op
	: or_xor_op OR or_xor_op
		{
			$$ = new OrOperatorNode ($1, $3);
		}
	| or_xor_op XOR or_xor_op
		{
			$$ = new XorOperatorNode ($1, $3);
		}
	| and_op
		{
			$$ = $1;
		}
	;

and_op
	: and_op AND and_op
		{
			$$ = new AndOperatorNode ($1, $3);
		}
	| not_op
		{
			$$ = $1;
		}
	;

not_op
	: NOT not_op
		{
			$$ = new NotOperatorNode ($2);
		}
	| comparison_op
		{
			$$ = $1;
		}
	;

comparison_op
	: comparison_op LT comparison_op
		{	
			$$ = new LessThanOperatorNode ($1, $3);
		}
	| comparison_op LT_EQ comparison_op
		{
			$$ = new LessThanOrEqualOperatorNode ($1, $3);
		}
	| comparison_op GT comparison_op
		{
			$$ = new GreaterThanOperatorNode ($1, $3);
		}
	| comparison_op GT_EQ comparison_op
		{
			$$ = new GreaterThanOrEqualOperatorNode ($1, $3);
		}
	| comparison_op EQUAL comparison_op
		{
			$$ = new EqualOperatorNode ($1, $3);
		}
	| comparison_op NOT_EQUAL comparison_op
		{
			$$ = new NotEqualOperatorNode ($1, $3);
		}
	| plus_minus_op
		{
			$$ = $1;
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
	| identifier
		{
			$$ = $1;
		}
	;

identifier
	: IIDENTIFIER
		{
			$$ = new IdentifierNode (*$1, BT_INT);
		}
	| FIDENTIFIER
		{
			$$ = new IdentifierNode (*$1, BT_FLOAT);
		}
	| SIDENTIFIER
		{
			$$ = new IdentifierNode (*$1, BT_STRING);
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

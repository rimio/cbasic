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
	ExpressionNode *expr_node;
	ValueNode *value_node;
	IdentifierNode *identifier_node;
	StatementNode *statement_node;
}

%token AND
%token BACKSLASH				"'\\'"
%token COMMA					"','"
%token DIM
%token ELSE
%token END					0	"end of file"
%token ENDIF
%token EQUAL					"="
%token GT						"GT"
%token GT_EQ					"GE"
%token IF
%token LET
%token LT						"LT"
%token LT_EQ					"LE"
%token MINUS					"-"
%token MODULO					"MOD"
%token NEWLINE					"newline"
%token NOT
%token NOT_EQUAL				"<>"
%token OR
%token PAR_CLOSE				")"
%token PAR_OPEN					"("
%token PLUS						"+"
%token POWER					"^"
%token PRINT
%token SEMICOLON				";"
%token SLASH					"/"
%token STAR						"*"
%token THEN
%token WHILE
%token WEND
%token XOR

%left OR XOR
%left AND
%left NOT
%left LT LT_EQ GT GT_EQ EQUAL NOT_EQUAL
%left PLUS MINUS
%left MODULO
%left BACKSLASH
%left STAR SLASH
%precedence NEGATION
%left POWER

%token <ival> ILITERAL			"integer literal"
%token <fval> FLITERAL			"float literal"
%token <sval> SLITERAL			"string literal"
%token <sval> IIDENTIFIER		"integer identifier"
%token <sval> FIDENTIFIER		"float identifier"
%token <sval> SIDENTIFIER		"string identifier"


%type <statement_node>	allocation_statement_part
%type <statement_node>	allocation_statement_part_list
%type <expr_node>		expression
%type <expr_node>		expression_list
%type <identifier_node>	identifier
%type <statement_node>	if_statement
%type <value_node>		literal
%type <expr_node>		operand
%type <expr_node>		print_expression_list
%type <statement_node>	print_statement
%type <statement_node>	statement
%type <statement_node>	statement_list
%type <statement_node>	statement_with_newline
%type <statement_node>	while_statement;

/* this has dummy type */
%type <ival>			opt_newline

%%

program
	: opt_newline statement_with_newline END
		{
			*root_node = $2;
		}
	| opt_newline statement END
		{
			*root_node = $2;
		}
	| opt_newline statement_list END
		{
			*root_node = $2;
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
	;

statement
	: LET identifier EQUAL expression
		{
			$$ = new AssignmentStatementNode ($2, $4);
			$$->setLocation (@2);
		}
	| DIM allocation_statement_part_list
		{
			$$ = $2;
		}
	| while_statement
		{
			$$ = $1;
		}
	| if_statement
		{
			$$ = $1;
		}
	| print_statement
		{
			$$ = $1;
		}
	;

print_statement
	: PRINT print_expression_list
		{
			$$ = new PrintStatementNode ($2);
			$$->setLocation (@1);
		}
	;

print_expression_list
	: expression COMMA print_expression_list
		{
			$$ = $1;
			$$->setNext ($3);
		}
	| expression SEMICOLON print_expression_list
		{
			$$ = $1;
			$$->setNext ($3);
		}
	| expression
		{
			$$ = $1;
		}
	;

if_statement
	: IF expression THEN NEWLINE statement_list ELSE NEWLINE statement_list ENDIF
		{
			$$ = new IfStatementNode ($2, $5, $8);
			$$->setLocation (@1);
		}
	| IF expression THEN NEWLINE statement_list ENDIF
		{
			$$ = new IfStatementNode ($2, $5, nullptr);
			$$->setLocation (@1);
		}
	;

while_statement
	: WHILE expression NEWLINE statement_list WEND
		{
			$$ = new WhileStatementNode ($2, $4);
			$$->setLocation (@1);
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
			$$->setLocation (@1);
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
	: expression OR expression
		{
			$$ = new OrOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression XOR expression
		{
			$$ = new XorOperatorNode ($1, $3);
			$$->setLocation (@1);
		}

	| expression AND expression
		{
			$$ = new AndOperatorNode ($1, $3);
			$$->setLocation (@1);
		}

	| NOT expression
		{
			$$ = new NotOperatorNode ($2);
			$$->setLocation (@1);
		}

	| expression LT expression
		{	
			$$ = new LessThanOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression LT_EQ expression
		{
			$$ = new LessThanOrEqualOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression GT expression
		{
			$$ = new GreaterThanOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression GT_EQ expression
		{
			$$ = new GreaterThanOrEqualOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression EQUAL expression
		{
			$$ = new EqualOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression NOT_EQUAL expression
		{
			$$ = new NotEqualOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
		
	| expression PLUS expression
		{
			$$ = new PlusOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression MINUS expression
		{
			$$ = new MinusOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
		
	| expression MODULO expression
		{
			$$ = new ModuloOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
		
	| expression BACKSLASH expression
		{
			$$ = new IntDivisionOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
		
	| expression STAR expression
		{
			$$ = new MultiplicationOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
	| expression SLASH expression
		{
			$$ = new DivisionOperatorNode ($1, $3);
			$$->setLocation (@1);
		}
		
	| MINUS expression %prec NEGATION
		{
			$$ = new MinusOperatorNode ($2, nullptr);
			$$->setLocation (@1);

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
		
	| expression POWER expression
		{
			$$ = new PowerOperatorNode ($1, $3);
			$$->setLocation (@1);
		}

	| PAR_OPEN expression PAR_CLOSE
		{
			$$ = $2;
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
	| PAR_OPEN operand PAR_CLOSE
		{
			$$ = $2;
		}
	;

identifier
	: IIDENTIFIER
		{
			$$ = new IdentifierNode (*$1, BT_INT);
			$$->setLocation (@1);
		}
	| FIDENTIFIER
		{
			$$ = new IdentifierNode (*$1, BT_FLOAT);
			$$->setLocation (@1);
		}
	| SIDENTIFIER
		{
			$$ = new IdentifierNode (*$1, BT_STRING);
			$$->setLocation (@1);
		}
	;

literal
	: ILITERAL
		{
			$$ = new IntegerValueNode ($1);
			$$->setLocation (@1);
		}
	| FLITERAL
		{
			$$ = new FloatValueNode ($1);
			$$->setLocation (@1);
		}
	| SLITERAL
		{
			$$ = new StringValueNode (*$1);
			$$->setLocation (@1);
		}
	;

opt_newline
	: /* empty */
		{
			$$ = 0;
		}
	| NEWLINE
		{
			$$ = 0;
		}
	;

%%

void Parser::error (const Parser::location_type& loc, const std::string &err)
{
	Error::syntaxError (err, loc.begin.line, loc.begin.column);
}

static int yylex (Parser::semantic_type *yylval, Parser::location_type *loc, Lexer &lexer, ParserContext &context)
{
	int rc = lexer.yylex (yylval);
	*loc = lexer.getLocation ();
	return rc;
}


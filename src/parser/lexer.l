%{

#include <string>
#include "error/error.h"
#include "lexer.h"

#define yyterminate()	return Parser::token::END

%}

%option noyywrap
%option yyclass="Lexer"
%option c++

%{
static yy::location loc;
#define YY_USER_ACTION loc.columns(yyleng);
%}

%%

%{
loc.step();
%}

"^"								{ return Parser::token::POWER; }
"+"								{ return Parser::token::PLUS; }
"-"								{ return Parser::token::MINUS; }
"*"								{ return Parser::token::STAR; }
"/"								{ return Parser::token::SLASH; }
"\\"							{ return Parser::token::BACKSLASH; }
"="								{ return Parser::token::EQUAL; }
[Mm][Oo][Dd]					{ return Parser::token::MODULO; }

[0-9]+								{ yylval->ival = atoi (yytext); return Parser::token::ILITERAL; }
[0-9]*[.][0-9]+						{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
[0-9]*[.][0-9]+[Ee][-]?[0-9]+		{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
[0-9]+[Ee][-]?[0-9]+				{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
["][^"]*["]							{ yylval->sval = new std::string (yytext); return Parser::token::SLITERAL; }

[a-zA-Z][a-zA-Z0-9.]*[%]			{ yylval->sval = new std::string (yytext, strlen (yytext) - 1); return Parser::token::IIDENTIFIER; }
[a-zA-Z][a-zA-Z0-9.]*[$]			{ yylval->sval = new std::string (yytext, strlen (yytext) - 1); return Parser::token::SIDENTIFIER; }
[a-zA-Z][a-zA-Z0-9.]*				{ yylval->sval = new std::string (yytext); return Parser::token::FIDENTIFIER; }

[ \t]							{ /* eat them */ loc.step (); }
[\n]+							{ loc.lines (yyleng); loc.step (); return Parser::token::NEWLINE; }

.								{ Error::syntaxError ("unrecognized token '" + std::string (yytext) + "'", loc.begin.line, loc.begin.column); }

%%

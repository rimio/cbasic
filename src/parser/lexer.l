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

"("								{ return Parser::token::PAR_OPEN; }
")"								{ return Parser::token::PAR_CLOSE; }

","								{ return Parser::token::COMMA; }

"^"								{ return Parser::token::POWER; }
"+"								{ return Parser::token::PLUS; }
"-"								{ return Parser::token::MINUS; }
"*"								{ return Parser::token::STAR; }
"/"								{ return Parser::token::SLASH; }
"\\"							{ return Parser::token::BACKSLASH; }
"="								{ return Parser::token::EQUAL; }
"<"								{ return Parser::token::LT; }
"<="							{ return Parser::token::LT_EQ; }
">"								{ return Parser::token::GT; }
">="							{ return Parser::token::GT_EQ; }
"<>"							{ return Parser::token::NOT_EQUAL; }

[Mm][Oo][Dd]					{ return Parser::token::MODULO; }
[Ll][Tt]						{ return Parser::token::LT; }
[Ll][Ee]						{ return Parser::token::LT_EQ; }
[Gg][Tt]						{ return Parser::token::GT; }
[Gg][Ee]						{ return Parser::token::GT_EQ; }
[Nn][Ee]						{ return Parser::token::NOT_EQUAL; }
[Nn][Oo][Tt]					{ return Parser::token::NOT; }
[Aa][Nn][Dd]					{ return Parser::token::AND; }
[Oo][Rr]						{ return Parser::token::OR; }
[Xx][Oo][Rr]					{ return Parser::token::XOR; }

[Ll][Ee][Tt]					{ return Parser::token::LET; }
[Dd][Ii][Mm]					{ return Parser::token::DIM; }

[0-9]+								{ yylval->ival = atoi (yytext); return Parser::token::ILITERAL; }
[0][0-9a-fA-F]*[Hh]					{ yylval->ival = strtol (yytext, NULL, 16); return Parser::token::ILITERAL; }
[0-1]+[Bb]							{ yylval->ival = strtol (yytext, NULL, 2); return Parser::token::ILITERAL; }
[0-9]*[.][0-9]+						{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
[0-9]*[.][0-9]+[Ee][-]?[0-9]+		{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
[0-9]+[Ee][-]?[0-9]+				{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
["][^"]*["]							{
										std::string strq (yytext);
										yylval->sval = new std::string (strq.substr (1, strq.length () - 2));
										return Parser::token::SLITERAL;
									}

[a-zA-Z][a-zA-Z0-9.]*[%]			{ yylval->sval = new std::string (yytext, strlen (yytext) - 1); return Parser::token::IIDENTIFIER; }
[a-zA-Z][a-zA-Z0-9.]*[$]			{ yylval->sval = new std::string (yytext, strlen (yytext) - 1); return Parser::token::SIDENTIFIER; }
[a-zA-Z][a-zA-Z0-9.]*				{ yylval->sval = new std::string (yytext); return Parser::token::FIDENTIFIER; }

[ \t]							{ /* eat them */ loc.step (); }
[\n]+							{ loc.lines (yyleng); loc.step (); return Parser::token::NEWLINE; }

.								{ Error::syntaxError ("unrecognized token '" + std::string (yytext) + "'", loc.begin.line, loc.begin.column); }

%%

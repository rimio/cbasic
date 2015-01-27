%{

#include <string>
#include "error/error.h"
#include "lexer.h"

#define yyterminate()	return Parser::token::END

%}

%option noyywrap
%option yyclass="Lexer"
%option c++

%%

"^"								{ return Parser::token::POWER; }
"+"								{ return Parser::token::PLUS; }
"-"								{ return Parser::token::MINUS; }
"*"								{ return Parser::token::STAR; }
"/"								{ return Parser::token::SLASH; }
"\\"							{ return Parser::token::BACKSLASH; }
[Mm][Oo][Dd]					{ return Parser::token::MODULO; }

[0-9]+									{ yylval->ival = atoi (yytext); return Parser::token::ILITERAL; }
[0-9]*[.][0-9]+							{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
[0-9]*[.][0-9]+[Ee][-]?[0-9]*[1-9]+		{ yylval->fval = atof (yytext); return Parser::token::FLITERAL; }
["][a-zA-Z]*["]							{ yylval->sval = new std::string (yytext); return Parser::token::SLITERAL; }

[ \t]							{ /* eat them */ }
\n								{ return Parser::token::NEWLINE; }

.								{ Error::syntaxError ("unrecognized token '" + std::string (yytext) + "'", 0, 0); }

%%

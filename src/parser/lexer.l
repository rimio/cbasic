%{

#include <string>
#include "lexer.h"

#define yyterminate()	return Parser::token::END

%}

%option noyywrap
%option yyclass="Lexer"
%option c++

%%

[ \t]							;
\n								{ return Parser::token::NEWLINE; }

%%

%{
#include "y.tab.h" // Include the generated header file from YACC
#include <stdlib.h>
#include <string.h>
%}

%%
[ \t\n]+             ;             // Ignore whitespace
[0-9]+               { yylval.num = atoi(yytext); return NUM; }
[a-zA-Z][a-zA-Z0-9]* { yylval.id = strdup(yytext); return ID; }
"+"                  return '+';
"-"                  return '-';
"*"                  return '*';
"/"                  return '/';
"="                  return '=';
";"                  return ';';

%%

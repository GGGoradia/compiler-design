%{
#include <stdio.h>
#include "y.tab.h"
int lineno = 1;
%}

%%
[0-9]+      { yylval = atoi(yytext); return NUMBER; }
"+"         return PLUS;
"-"         return MINUS;
"*"         return TIMES;
"/"         return DIVIDE;
"%"         return MOD;
"("         return LPAREN;
")"         return RPAREN;
"{"         return LCURLY;
"}"         return RCURLY;
"["         return LSQUARE;
"]"         return RSQUARE;
[\n]        { lineno++; return NEWLINE; }
[ \t]       ; 
.           { printf("Unknown character: %s\n", yytext); }
%%

int yywrap(void) {
    return 1;
}

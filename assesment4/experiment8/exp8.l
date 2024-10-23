%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

void yyerror(const char* s);  
%}

%%
[0-9]+      { 
                yylval.value = atoi(yytext); 
                return NUMBER; 
            }
[a-zA-Z]    { 
                yylval.id = strdup(yytext); 
                return IDENTIFIER; 
            }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"^"         { return POWER; }
"("         { return LPAREN; }
")"         { return RPAREN; }
[\n]        { return EOL; }
[ \t]       ; 
.           { printf("Unknown character: %s\n", yytext); }
%%

int yywrap(void) {
    return 1;
}
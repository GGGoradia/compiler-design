%{
#include "y.tab.h"
#include <stdlib.h>
%}

digit    [0-9]+
id       [a-zA-Z_][a-zA-Z0-9_]*

%%

{digit}  { 
    yylval.num = atoi(yytext); 
    return NUMBER; 
}

{id}  { 
    strncpy(yylval.id, yytext, sizeof(yylval.id) - 1); 
    yylval.id[sizeof(yylval.id) - 1] = '\0'; 
    return ID; 
}

"+"    { return PLUS; }
"-"    { return MINUS; }
"*"    { return MUL; }
"/"    { return DIV; }
"="    { return ASSIGN; }
";"    { return SEMI; }

[ \t\n]+  { /* ignore whitespace */ }
.         { /* ignore other characters */ }

%%

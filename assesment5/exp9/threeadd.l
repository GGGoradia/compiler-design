%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "three_address.h"
#include "y.tab.h"

void yyerror(const char* s);
int line_no = 1;
%}

%%
"int"       { return INT; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"="         { return ASSIGN; }
"=="        { return EQ; }
"!="        { return NE; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LE; }
">="        { return GE; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }
";"         { return SEMICOLON; }
[a-zA-Z][a-zA-Z0-9_]*  { 
    yylval.string = strdup(yytext);
    return IDENTIFIER; 
}
[0-9]+      { 
    yylval.number = atoi(yytext);
    return NUMBER; 
}
[\n]        { line_no++; }
[ \t]       ; 
.           { printf("Unknown character: %s\n", yytext); }
%%

int yywrap(void) {
    return 1;
}
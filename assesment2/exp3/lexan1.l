%{

#include <stdio.h>

#include <string.h>

%}



%option noyywrap



%%



[0-9]+          { printf("NUMBER: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]* { printf("IDENTIFIER: %s\n", yytext); }

[+\-*/=]        { printf("OPERATOR: %s\n", yytext); }

[(){};,]        { printf("DELIMITER: %s\n", yytext); }

[ \t\n]         { /* Ignore whitespace */ }

.               { printf("UNKNOWN: %s\n", yytext); }



%%



int main() {

    char input[] = "int x = 5 + 3;";

    yy_scan_string(input);

    yylex();

    return 0;

}
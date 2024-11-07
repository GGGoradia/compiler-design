%{

#include <stdio.h>

#include <string.h>



int line_num = 1;

%}



%option noyywrap



%%



"int"|"float"|"char"|"if"|"else"|"while"|"for" { printf("KEYWORD: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENTIFIER: %s\n", yytext); }

[0-9]+                  { printf("INTEGER: %s\n", yytext); }

[0-9]+\.[0-9]+          { printf("FLOAT: %s\n", yytext); }

"+"                     { printf("OPERATOR: %s\n", yytext); }

"-"                     { printf("OPERATOR: %s\n", yytext); }

"*"                     { printf("OPERATOR: %s\n", yytext); }

"/"                     { printf("OPERATOR: %s\n", yytext); }

"="                     { printf("OPERATOR: %s\n", yytext); }

"=="                    { printf("OPERATOR: %s\n", yytext); }

"<"                     { printf("OPERATOR: %s\n", yytext); }

">"                     { printf("OPERATOR: %s\n", yytext); }

"("                     { printf("DELIMITER: %s\n", yytext); }

")"                     { printf("DELIMITER: %s\n", yytext); }

"{"                     { printf("DELIMITER: %s\n", yytext); }

"}"                     { printf("DELIMITER: %s\n", yytext); }

";"                     { printf("DELIMITER: %s\n", yytext); }

[ \t]+                  { /* Ignore whitespace */ }

\n                      { line_num++; }

.                       { printf("UNKNOWN: %s\n", yytext); }



%%



int main(int argc, char** argv) {

    if (argc != 2) {

        printf("Usage: %s <input_file>\n", argv[0]);

        return 1;

    }



    FILE* input_file = fopen(argv[1], "r");

    if (!input_file) {

        perror("Error opening file");

        return 1;

    }



    yyin = input_file;



    printf("Lexical Analysis:\n");

    yylex();



    fclose(input_file);

    printf("\nTotal lines: %d\n", line_num - 1);



    return 0;

}
%{

#include <stdio.h>

int line_num = 1;

%}



%option noyywrap



%%



[ \t]+                  /* ignore whitespace */

\n                      { line_num++; }



"int"|"float"|"char"|"if"|"else"|"while"|"for"|"return"  { printf("KEYWORD: %s\n", yytext); }



[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENTIFIER: %s\n", yytext); }



[0-9]+                  { printf("INTEGER: %s\n", yytext); }

[0-9]+\.[0-9]+          { printf("FLOAT: %s\n", yytext); }



"+"|"-"|"*"|"/"|"="     { printf("OPERATOR: %s\n", yytext); }

"=="|"!="|"<"|">"|"<="|">="  { printf("COMPARISON: %s\n", yytext); }



"("|")"|"{"|"}"|";"|","  { printf("DELIMITER: %s\n", yytext); }



\"[^\"\n]*\"            { printf("STRING: %s\n", yytext); }



"//".*                  { printf("COMMENT: %s\n", yytext); }



.                       { printf("UNKNOWN: %s\n", yytext); }



%%



int main(int argc, char **argv) {

    if (argc > 1) {

        FILE *file = fopen(argv[1], "r");

        if (!file) {

            perror(argv[1]);

            return 1;

        }

        yyin = file;

    }



    yylex();



    printf("\nTotal lines: %d\n", line_num);

    return 0;

}
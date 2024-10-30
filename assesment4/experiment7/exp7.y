%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex(void);
extern int lineno;
void print_result(int result);
%}

%token NUMBER NEWLINE
%token PLUS MINUS TIMES DIVIDE MOD
%token LPAREN RPAREN LCURLY RCURLY LSQUARE RSQUARE

%%
input:  
        | input line
        ;

line:   NEWLINE
        | expr NEWLINE            { print_result($1); }
        ;

expr:   term                      { $$ = $1; }
        | expr PLUS term          { $$ = $1 + $3; }
        | expr MINUS term         { $$ = $1 - $3; }
        ;

term:   factor                    { $$ = $1; }
        | term TIMES factor       { $$ = $1 * $3; }
        | term DIVIDE factor      { 
            if ($3 == 0) {
                yyerror("Division by zero!");
                YYABORT;
            }
            $$ = $1 / $3; 
        }
        | term MOD factor         { 
            if ($3 == 0) {
                yyerror("Modulus by zero!");
                YYABORT;
            }
            $$ = $1 % $3; 
        }
        ;

factor: NUMBER                    { $$ = $1; }
        | MINUS factor            { $$ = -$2; }
        | LPAREN expr RPAREN      { $$ = $2; }
        | LCURLY expr RCURLY      { $$ = $2; }
        | LSQUARE expr RSQUARE    { $$ = $2; }
        ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", lineno, s);
}

void print_result(int result) {
    printf("Result of expression: %d\n", result);
}

int main(void) {
    yyparse();
    return 0;
}

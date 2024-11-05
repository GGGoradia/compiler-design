%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare yylex and yyerror to resolve implicit declaration warnings
int yylex();
int yyerror(char *s);

int temp_var = 0; // Temporary variable counter for TAC

void emit(char *op, char *arg1, char *arg2, char *result) {
    printf("%s = %s %s %s\n", result, arg1, op, arg2);
}

void emit_assign(char *arg1, char *result) {
    printf("%s = %s\n", result, arg1);
}

%}

%union {
    int num;
    char *id;
}

%token <num> NUM
%token <id> ID
%left '+' '-'
%left '*' '/' '%'
%type <id> expr

%%
program: 
    program stmt ';' 
    | /* empty */
    ;

stmt:
    ID '=' expr  { 
        emit_assign($3, $1);
    }
    ;

expr: 
    expr '+' expr { 
        char temp[10];
        sprintf(temp, "t%d", temp_var++);
        emit("+", $1, $3, temp);
        $$ = strdup(temp);
    }
    | expr '-' expr {
        char temp[10];
        sprintf(temp, "t%d", temp_var++);
        emit("-", $1, $3, temp);
        $$ = strdup(temp);
    }
    | expr '*' expr {
        char temp[10];
        sprintf(temp, "t%d", temp_var++);
        emit("*", $1, $3, temp);
        $$ = strdup(temp);
    }
    | expr '/' expr {
        char temp[10];
        sprintf(temp, "t%d", temp_var++);
        emit("/", $1, $3, temp);
        $$ = strdup(temp);
    }
    | expr '%' expr {
        char temp[10];
        sprintf(temp, "t%d", temp_var++);
        emit("%", $1, $3, temp);
        $$ = strdup(temp);
    }
    | NUM {
        char temp[10];
        sprintf(temp, "%d", $1);
        $$ = strdup(temp);
    }
    | ID {
        $$ = $1;
    }
    ;

%%
int main() {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}

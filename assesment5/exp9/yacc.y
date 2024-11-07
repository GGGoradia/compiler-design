%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strdup

int tempVarCount = 1;

void emit(char *op, char *arg1, char *arg2, char *result) {
    if (arg2 != NULL) {
        printf("%s = %s %s %s\n", result, arg1, op, arg2);
    } else {
        printf("%s = %s\n", result, arg1);
    }
}

int yylex();
int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
%}

%union {
    int num;
    char id[10];
    char *str;
}

%token <id> ID
%token <num> NUMBER
%token ASSIGN PLUS MINUS MUL DIV SEMI
%type <str> E T F

%%

stmt: ID ASSIGN E SEMI {
    emit("=", $3, NULL, $1);
}
;

E: E PLUS T {
        $$ = (char*) malloc(10);
        sprintf($$, "t%d", tempVarCount++);
        emit("+", $1, $3, $$);
    }
    | E MINUS T {
        $$ = (char*) malloc(10);
        sprintf($$, "t%d", tempVarCount++);
        emit("-", $1, $3, $$);
    }
    | T {
        $$ = $1;
    }
;

T: T MUL F {
        $$ = (char*) malloc(10);
        sprintf($$, "t%d", tempVarCount++);
        emit("*", $1, $3, $$);
    }
    | T DIV F {
        $$ = (char*) malloc(10);
        sprintf($$, "t%d", tempVarCount++);
        emit("/", $1, $3, $$);
    }
    | F {
        $$ = $1;
    }
;

F: ID {
        $$ = strdup($1);
    }
    | NUMBER {
        $$ = (char*) malloc(10);
        sprintf($$, "%d", $1);
    }
;

%%

int main() {
    yyparse();
    return 0;
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "three_address.h"

void yyerror(const char* s);
int yylex(void);
extern int line_no;

int temp_count = 0;
int label_count = 0;

char* new_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

char* new_label() {
    char* label = (char*)malloc(10);
    sprintf(label, "L%d", label_count++);
    return label;
}

List* merge(List* l1, List* l2) {
    if (!l1) return l2;
    if (!l2) return l1;
    List* temp = l1;
    while (temp->next) temp = temp->next;
    temp->next = l2;
    return l1;
}

%}

%union {
    int number;
    char* string;
    Attrs attrs;
}

%token <number> NUMBER
%token <string> IDENTIFIER
%token INT IF ELSE WHILE
%token ASSIGN EQ NE LT GT LE GE
%token PLUS MINUS MULTIPLY DIVIDE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON

%type <attrs> expr stmt stmt_list bool_expr

%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc UMINUS
%nonassoc IFX
%nonassoc ELSE

%%

program: stmt_list {
    printf("\nGenerated Three Address Code:\n");
    printf("%s", $1.code);
}
;

stmt_list: stmt {
    $$.code = $1.code;
}
| stmt_list stmt {
    int len = strlen($1.code) + strlen($2.code) + 1;
    char* temp = (char*)malloc(len);
    sprintf(temp, "%s%s", $1.code, $2.code);
    $$.code = temp;
}
;

stmt: IDENTIFIER ASSIGN expr SEMICOLON {
    int len = ($3.code ? strlen($3.code) : 0) + strlen($1) + 20;
    char* temp = (char*)malloc(len);
    sprintf(temp, "%s%s = %s\n", $3.code ? $3.code : "", $1, $3.addr);
    $$.code = temp;
}
| IF LPAREN bool_expr RPAREN stmt %prec IFX {
    char* label = new_label();
    int len = strlen($3.code) + strlen($5.code) + 50;
    char* temp = (char*)malloc(len);
    sprintf(temp, "%sif %s goto %s\n%s%s:\n", 
        $3.code, $3.addr, label, $5.code, label);
    $$.code = temp;
}
| WHILE LPAREN bool_expr RPAREN stmt {
    char* label1 = new_label();
    char* label2 = new_label();
    int len = strlen($3.code) + strlen($5.code) + 100;
    char* temp = (char*)malloc(len);
    sprintf(temp, "%s:\n%sif %s goto %s\ngoto %s\n%s%s:\n",
        label1, $3.code, $3.addr, label2, label1, $5.code, label2);
    $$.code = temp;
}
| LBRACE stmt_list RBRACE {
    $$.code = $2.code;
}
;

bool_expr: expr EQ expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s == %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
| expr NE expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s != %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
| expr LT expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s < %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
;

expr: NUMBER {
    char* temp = new_temp();
    char* code = (char*)malloc(30);
    sprintf(code, "%s = %d\n", temp, $1);
    $$.addr = temp;
    $$.code = code;
}
| IDENTIFIER {
    $$.addr = strdup($1);
    $$.code = NULL;
}
| expr PLUS expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s + %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
| expr MINUS expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s - %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
| expr MULTIPLY expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s * %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
| expr DIVIDE expr {
    char* temp = new_temp();
    int len = ($1.code ? strlen($1.code) : 0) + ($3.code ? strlen($3.code) : 0) + 100;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s%s = %s / %s\n", 
        $1.code ? $1.code : "", $3.code ? $3.code : "", temp, $1.addr, $3.addr);
    $$.addr = temp;
    $$.code = code;
}
| LPAREN expr RPAREN {
    $$.addr = $2.addr;
    $$.code = $2.code;
}
| MINUS expr %prec UMINUS {
    char* temp = new_temp();
    int len = ($2.code ? strlen($2.code) : 0) + 50;
    char* code = (char*)malloc(len);
    sprintf(code, "%s%s = -%s\n", $2.code ? $2.code : "", temp, $2.addr);
    $$.addr = temp;
    $$.code = code;
}
;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error at line %d: %s\n", line_no, s);
}

int main() {
    printf("Enter program (press Ctrl+D to end):\n");
    yyparse();
    return 0;
}
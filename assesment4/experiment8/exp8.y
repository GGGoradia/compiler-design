%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char* s);

typedef struct ASTNode {
    int type;
    char* operator;
    int value;
    char* id;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* createOperatorNode(char* op, ASTNode* left, ASTNode* right);
ASTNode* createNumberNode(int value);
ASTNode* createIdentifierNode(char* id);
void printPostfix(ASTNode* root);
void freeAST(ASTNode* root);

ASTNode* ast_root = NULL;
%}

%union {
    int value;
    char* id;
    struct ASTNode* node;
}

%token <value> NUMBER
%token <id> IDENTIFIER
%token PLUS MINUS MULTIPLY DIVIDE POWER LPAREN RPAREN EOL
%type <node> expr term factor line

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER

%%
input:  
        | input line
        ;

line:   EOL             { $$ = NULL; }
        | expr EOL      { 
            ast_root = $1;
            printf("Postfix expression: ");
            printPostfix(ast_root);
            printf("\n");
            freeAST(ast_root);
            ast_root = NULL;
        }
        ;

expr:   expr PLUS term   { $$ = createOperatorNode("+", $1, $3); }
        | expr MINUS term { $$ = createOperatorNode("-", $1, $3); }
        | term           { $$ = $1; }
        ;

term:   term MULTIPLY factor { $$ = createOperatorNode("*", $1, $3); }
        | term DIVIDE factor { $$ = createOperatorNode("/", $1, $3); }
        | factor             { $$ = $1; }
        ;

factor: NUMBER            { $$ = createNumberNode($1); }
        | IDENTIFIER      { $$ = createIdentifierNode($1); }
        | LPAREN expr RPAREN { $$ = $2; }
        | factor POWER factor { $$ = createOperatorNode("^", $1, $3); }
        ;
%%

ASTNode* createOperatorNode(char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = 0;
    node->operator = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* createNumberNode(int value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = 1;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* createIdentifierNode(char* id) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = 2;
    node->id = strdup(id);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printPostfix(ASTNode* root) {
    if (root != NULL) {
        printPostfix(root->left);
        printPostfix(root->right);

        if (root->type == 0) {
            printf("%s ", root->operator);
        } else if (root->type == 1) {
            printf("%d ", root->value);
        } else if (root->type == 2) {
            printf("%s ", root->id);
        }
    }
}

void freeAST(ASTNode* root) {
    if (root != NULL) {
        freeAST(root->left);
        freeAST(root->right);
        if (root->type == 0) free(root->operator);
        if (root->type == 2) free(root->id);
        free(root);
    }
}

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter infix expressions (one per line, press Ctrl+D to end):\n");  
    yyparse();
    return 0;
}
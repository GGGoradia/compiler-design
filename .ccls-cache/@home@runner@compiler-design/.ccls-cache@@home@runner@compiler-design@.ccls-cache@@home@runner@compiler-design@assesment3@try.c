#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX 100
#define MAX_RULES 20
#define MAX_SYMBOLS 10

char stack[MAX];
int top = -1;

char terminals[MAX_SYMBOLS];
char non_terminals[MAX_SYMBOLS];
char productions[MAX_RULES][MAX_SYMBOLS];
int num_terminals = 0;
int num_non_terminals = 0;
int num_productions = 0;

void push(char symbol) {
    if (top < MAX - 1) {
        stack[++top] = symbol;
    }
}

void pop() {
    if (top >= 0) {
        top--;
    }
}

char peek() {
    return (top >= 0) ? stack[top] : '$';
}

bool is_terminal(char symbol) {
    for (int i = 0; i < num_terminals; i++) {
        if (terminals[i] == symbol) {
            return true;
        }
    }
    return false;
}

bool is_non_terminal(char symbol) {
    for (int i = 0; i < num_non_terminals; i++) {
        if (non_terminals[i] == symbol) {
            return true;
        }
    }
    return false;
}

void add_production(char* rule) {
    if (num_productions < MAX_RULES) {
        strcpy(productions[num_productions], rule);
        num_productions++;
    }
}

char* get_production(char non_terminal, char input) {
    for (int i = 0; i < num_productions; i++) {
        if (productions[i][0] == non_terminal) {
            char* rhs = &productions[i][3];
            if (rhs[0] == input || rhs[0] == 'e' || is_non_terminal(rhs[0])) {
                return rhs;
            }
        }
    }
    return NULL;
}

bool predictiveParse(char* input) {
    int index = 0;
    char current_input = input[index];
    push(non_terminals[0]);  // Start symbol

    while (top >= 0) {
        char top_symbol = peek();
        if (is_terminal(top_symbol)) {
            if (top_symbol == current_input) {
                pop();
                current_input = input[++index];
            } else {
                return false;
            }
        } else if (is_non_terminal(top_symbol)) {
            char* production = get_production(top_symbol, current_input);
            if (production != NULL) {
                pop();
                if (production[0] != 'e') {  // 'e' represents epsilon
                    for (int i = strlen(production) - 1; i >= 0; i--) {
                        push(production[i]);
                    }
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    return (current_input == '\0' || current_input == '$');
}

void input_symbols(char* symbols, int* count, const char* type) {
    printf("Enter %s (space-separated): ", type);
    char input[100];
    fgets(input, sizeof(input), stdin);
    char* token = strtok(input, " \n");
    while (token != NULL && *count < MAX_SYMBOLS) {
        symbols[(*count)++] = token[0];
        token = strtok(NULL, " \n");
    }
}

void input_productions() {
    printf("Enter productions (one per line, format 'A->aB' or 'A->e' for epsilon, empty line to finish):\n");
    char input[100];
    while (num_productions < MAX_RULES) {
        if (!fgets(input, sizeof(input), stdin) || input[0] == '\n') break;
        input[strcspn(input, "\n")] = 0;
        add_production(input);
    }
}

int main() {
    input_symbols(terminals, &num_terminals, "terminals");
    input_symbols(non_terminals, &num_non_terminals, "non-terminals");
    input_productions();

    printf("Enter input string to parse ($ at the end is optional): ");
    char input[MAX];
    scanf("%s", input);

    if (predictiveParse(input)) {
        printf("Input is valid.\n");
    } else {
        printf("Input is invalid.\n");
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_RULES 100
#define MAX_STRING 1000

char terminals[MAX_SYMBOLS][10];
char non_terminals[MAX_SYMBOLS][10];
char productions[MAX_RULES][20];
int num_terminals, num_non_terminals, num_rules;
char input[MAX_STRING];
int input_index = 0;

int parse_E();
int parse_T();
int parse_F();

int match(char expected) {
    if (input[input_index] == expected) {
        printf("Matched %c\n", expected);
        input_index++;
        return 1;
    }
    return 0;
}

int parse_E() {
    printf("Parsing E\n");
    if (parse_T()) {
        while (input[input_index] == '+') {
            if (match('+') && parse_T()) {
                continue;
            }
            break;
        }
        return 1;
    }
    return 0;
}

int parse_T() {
    printf("Parsing T\n");
    if (parse_F()) {
        while (input[input_index] == '*') {
            if (match('*') && parse_F()) {
                continue;
            }
            break;
        }
        return 1;
    }
    return 0;
}

int parse_F() {
    printf("Parsing F\n");
    if (input[input_index] == '(') {
        if (match('(') && parse_E() && match(')')) {
            return 1;
        }
    } else if (input[input_index] == 'i' && input[input_index + 1] == 'd') {
        if (match('i') && match('d')) {
            return 1;
        }
    }
    return 0;
}

int main() {
    printf("Enter the number of terminals: ");
    scanf("%d", &num_terminals);
    printf("Enter the terminals:\n");
    for (int i = 0; i < num_terminals; i++) {
        scanf("%s", terminals[i]);
    }

    printf("Enter the number of non-terminals: ");
    scanf("%d", &num_non_terminals);
    printf("Enter the non-terminals:\n");
    for (int i = 0; i < num_non_terminals; i++) {
        scanf("%s", non_terminals[i]);
    }

    printf("Enter the number of production rules: ");
    scanf("%d", &num_rules);
    printf("Enter the production rules (format: A->abc):\n");
    for (int i = 0; i < num_rules; i++) {
        scanf("%s", productions[i]);
    }

    printf("Enter the string to parse: ");
    scanf("%s", input);

    if (parse_E() && input[input_index] == '\0') {
        printf("String is valid!\n");
    } else {
        printf("String is invalid!\n");
    }

    return 0;
}
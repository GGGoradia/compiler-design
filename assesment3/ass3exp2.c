#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

int parsingTable[MAX][MAX] = {
    {1, 0, 0, 1, 0, 0},
    {0, 2, 0, 0, 3, 3},
    {4, 0, 0, 4, 0, 0},
    {0, 6, 5, 0, 6, 6},
    {8, 0, 0, 7, 0, 0}
};

char *rules[] = {
    "E -> TH",
    "H -> +TH",
    "H -> @",
    "T -> FI",
    "I -> *FI",
    "I -> @",
    "F -> (E)",
    "F -> i"
};

char stack[MAX];
int top = -1;

void pushStack(char symbol) {
    if (top == MAX - 1) {
        printf("Stack Overflow\n");
    } else {
        stack[++top] = symbol;
    }
}

char popStack() {
    if (top == -1) {
        printf("Stack Underflow\n");
        return '\0';
    } else {
        return stack[top--];
    }
}

int isStackEmpty() {
    return top == -1;
}

void showStack(char input[], int index) {
    printf("Stack: ");
    for (int i = top; i >= 0; i--) {
        printf("%c", stack[i]);
    }
    printf("\nInput: ");
    for (int i = index; input[i] != '\0'; i++) {
        printf("%c", input[i]);
    }
    printf("\n");
}

int getTerminalIndex(char symbol) {
    switch (symbol) {
        case '+': return 1;
        case '*': return 2;
        case '(': return 3;
        case ')': return 4;
        case 'i': return 0;
        case '$': return 5;
        default: return -1;
    }
}

int getNonTerminalIndex(char symbol) {
    switch (symbol) {
        case 'E': return 0;
        case 'H': return 1;
        case 'T': return 2;
        case 'I': return 3;
        case 'F': return 4;
        default: return -1;
    }
}

void parseInput(char input[]) {
    int i = 0;
    pushStack('$');
    pushStack('E');

    while (!isStackEmpty()) {
        showStack(input, i);
        char stackTop = stack[top];
        char currentInput = input[i];

        if (currentInput == '$' && stackTop == '$') {
            printf("Input string is successfully parsed.\n");
            return;
        }

        if (stackTop == currentInput) {
            printf("Match: %c\n", currentInput);
            popStack();
            i++;
        } else if (isupper(stackTop)) {
            int row = getNonTerminalIndex(stackTop);
            int col = getTerminalIndex(currentInput);

            if (row != -1 && col != -1 && parsingTable[row][col] != 0) {
                popStack();
                printf("Production used: %s\n", rules[parsingTable[row][col] - 1]);

                char *prod = rules[parsingTable[row][col] - 1];
                for (int j = strlen(prod) - 1; j > 3; j--) {
                    if (prod[j] != ' ' && prod[j] != '@') {
                        pushStack(prod[j]);
                    }
                }
            } else {
                printf("Error: No matching production found for %c\n", stackTop);
                return;
            }
        } else {
            printf("Error: Unexpected terminal symbol at stack: %c and input: %c\n", stackTop, currentInput);
            return;
        }
    }

    if (input[i] == '$') {
        printf("Input string is successfully parsed.\n");
    } else {
        printf("Error: Input string is not fully consumed.\n");
    }
}

int main() {
    char input[50];

    printf("Enter the input string (terminated by $): ");
    scanf("%s", input);

    parseInput(input);

    return 0;
}

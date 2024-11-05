#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAX_INPUT_SIZE 100

typedef struct {
    int state;
    char action; // 's' for shift, 'r' for reduce, 'a' for accept
    int rule;    // Rule number for reduction, or next state for shift
} Action;

typedef struct {
    int top;
    int stack[MAX_STACK_SIZE];
} Stack;

// Function prototypes
void initStack(Stack* s);
int isEmpty(Stack* s);
void push(Stack* s, int value);
int pop(Stack* s);
void parse(const char* input, Action parsingTable[][256], int numStates);

int main() {
    // Example parsing table based on your input
    Action parsingTable[5][256] = {0};

    // Filling the parsing table with the actions you provided
    parsingTable[0]['a'] = (Action){0, 's', 3}; // s3
    parsingTable[0]['b'] = (Action){0, '\0', 0}; // -
    parsingTable[0]['$'] = (Action){0, '\0', 0}; // -
    parsingTable[0]['S'] = (Action){0, 's', 1}; // 1
    parsingTable[0]['A'] = (Action){0, 's', 2}; // 2
    parsingTable[0]['B'] = (Action){0, '\0', 0}; // -

    parsingTable[1]['a'] = (Action){1, '\0', 0}; // -
    parsingTable[1]['b'] = (Action){1, '\0', 0}; // -
    parsingTable[1]['$'] = (Action){1, 'a', 0}; // accept

    parsingTable[2]['a'] = (Action){2, '\0', 0}; // -
    parsingTable[2]['b'] = (Action){2, '\0', 0}; // -
    parsingTable[2]['$'] = (Action){2, '\0', 0}; // -
    parsingTable[2]['S'] = (Action){2, '\0', 0}; // -
    parsingTable[2]['A'] = (Action){2, '\0', 0}; // -
    parsingTable[2]['B'] = (Action){2, 'r', 4}; // r4

    parsingTable[3]['a'] = (Action){3, '\0', 0}; // -
    parsingTable[3]['b'] = (Action){3, '\0', 0}; // -
    parsingTable[3]['$'] = (Action){3, 'r', 2}; // r2

    parsingTable[4]['a'] = (Action){4, '\0', 0}; // -
    parsingTable[4]['b'] = (Action){4, '\0', 0}; // -
    parsingTable[4]['$'] = (Action){4, 'r', 1}; // r1

    char input[MAX_INPUT_SIZE];
    printf("Enter a string to parse (end with $): ");
    scanf("%s", input);

    // Parse the input string
    parse(input, parsingTable, 5);

    return 0;
}

void initStack(Stack* s) {
    s->top = -1;
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, int value) {
    if (s->top < MAX_STACK_SIZE - 1) {
        s->stack[++(s->top)] = value;
    } else {
        printf("Stack overflow\n");
    }
}

int pop(Stack* s) {
    if (!isEmpty(s)) {
        return s->stack[(s->top)--];
    } else {
        printf("Stack underflow\n");
        return -1; // Error value
    }
}

void parse(const char* input, Action parsingTable[][256], int numStates) {
    Stack stack;
    initStack(&stack);
    push(&stack, 0); // Start with state 0

    char* inputCopy = malloc(strlen(input) + 1);
    strcpy(inputCopy, input);

    strcat(inputCopy, "$"); // Add end marker

    int index = 0;
    char currentSymbol;

    while (1) {
        currentSymbol = inputCopy[index];
        int currentState = stack.stack[stack.top];

        Action action = parsingTable[currentState][(int)currentSymbol];

        if (action.action == 's') { // Shift action
            printf("Shift to state %d\n", action.rule);
            push(&stack, action.rule);
            index++;
        } else if (action.action == 'r') { // Reduce action
            printf("Reduce by rule %d\n", action.rule);
            // Pop states based on the number of symbols on the right side of the rule
            // For simplicity, assuming a fixed number of pops for each rule
            // Adjust this based on your grammar rules
            int pops = (action.rule == 1) ? 2 : (action.rule == 2) ? 1 : 0; // Example
            for (int i = 0; i < pops; i++) {
                pop(&stack);
            }
            // Push the new state from FOLLOW set (assuming it is state 0 for simplicity)
            push(&stack, 0);
        } else if (action.action == 'a') { // Accept action
            printf("Accepted\n");
            break;
        } else {
            printf("Error: No action found for state %d and symbol '%c'.\n", currentState, currentSymbol);
            break;
        }
    }

    free(inputCopy);
}

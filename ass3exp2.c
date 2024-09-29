#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define STACK_SIZE 100

char grammar[MAX][MAX][MAX];  // Grammar rules
char first[MAX][MAX], follow[MAX][MAX], terminals[MAX], nonTerminals[MAX];
char parseTable[MAX][MAX];  // Parse table
int numProductions, numTerminals = 0, numNonTerminals = 0;

char stack[STACK_SIZE];  // Stack for parsing
int top = -1;

void push(char c) {
    if (top < STACK_SIZE - 1) {
        stack[++top] = c;
    }
}

char pop() {
    if (top >= 0) {
        return stack[top--];
    }
    return '\0';  // Empty stack case
}

// Function to add a symbol to a set without duplication
void addToSet(char set[], char symbol) {
    if (!strchr(set, symbol)) {
        int len = strlen(set);
        set[len] = symbol;
        set[len + 1] = '\0';
    }
}

// Function to find FIRST of a non-terminal
void findFirst(char nonTerminal, char result[]) {
    for (int i = 0; i < numProductions; i++) {
        if (grammar[i][0][0] == nonTerminal) {
            if (islower(grammar[i][1][0]) || grammar[i][1][0] == 'e') {
                addToSet(result, grammar[i][1][0]);  // Terminal or epsilon
            } else {
                findFirst(grammar[i][1][0], result);  // Recursive call for non-terminal
            }
        }
    }
}

// Function to find FOLLOW of a non-terminal
void findFollow(char nonTerminal, char result[]) {
    if (nonTerminal == grammar[0][0][0]) {
        addToSet(result, '$');  // Add $ to FOLLOW of start symbol
    }

    for (int i = 0; i < numProductions; i++) {
        for (int j = 0; j < strlen(grammar[i][1]); j++) {
            if (grammar[i][1][j] == nonTerminal) {
                if (grammar[i][1][j + 1] != '\0') {
                    if (islower(grammar[i][1][j + 1])) {
                        addToSet(result, grammar[i][1][j + 1]);
                    } else {
                        findFirst(grammar[i][1][j + 1], result);
                    }
                }
                if (grammar[i][1][j + 1] == '\0' || strchr(first[grammar[i][1][j + 1] - 'A'], 'e')) {
                    if (grammar[i][0][0] != nonTerminal) {
                        findFollow(grammar[i][0][0], result);
                    }
                }
            }
        }
    }
}

// Function to construct the parse table
void constructParseTable() {
    for (int i = 0; i < numNonTerminals; i++) {
        for (int j = 0; j < numTerminals; j++) {
            parseTable[i][j] = '-';  // Initialize with empty '-'
        }
    }

    for (int i = 0; i < numProductions; i++) {
        char firstSet[MAX] = "";
        findFirst(grammar[i][0][0], firstSet);

        for (int j = 0; j < strlen(firstSet); j++) {
            if (firstSet[j] != 'e') {
                for (int k = 0; k < numTerminals; k++) {
                    if (terminals[k] == firstSet[j]) {
                        parseTable[grammar[i][0][0] - 'A'][k] = i + '1';
                    }
                }
            } else {
                char followSet[MAX] = "";
                findFollow(grammar[i][0][0], followSet);
                for (int k = 0; k < strlen(followSet); k++) {
                    for (int l = 0; l < numTerminals; l++) {
                        if (terminals[l] == followSet[k]) {
                            parseTable[grammar[i][0][0] - 'A'][l] = i + '1';
                        }
                    }
                }
            }
        }
    }
}

// Function to display the FIRST and FOLLOW sets
void displayFirstFollow() {
    printf("FIRST Sets:\n");
    for (int i = 0; i < numNonTerminals; i++) {
        printf("FIRST(%c) = { %s }\n", nonTerminals[i], first[i]);
    }

    printf("\nFOLLOW Sets:\n");
    for (int i = 0; i < numNonTerminals; i++) {
        printf("FOLLOW(%c) = { %s }\n", nonTerminals[i], follow[i]);
    }
}

// Function to display the Parse Table
void displayParseTable() {
    printf("\nPredictive Parse Table:\n");
    printf("    ");
    for (int i = 0; i < numTerminals; i++) {
        printf(" %c  |", terminals[i]);
    }
    printf("\n-------------------------\n");

    for (int i = 0; i < numNonTerminals; i++) {
        printf(" %c |", nonTerminals[i]);
        for (int j = 0; j < numTerminals; j++) {
            if (parseTable[i][j] != '-') {
                printf(" %c  |", parseTable[i][j]);
            } else {
                printf(" -  |");
            }
        }
        printf("\n");
    }
}

// Predictive parsing algorithm
void predictiveParsing(char *input) {
    push('$');
    push(grammar[0][0][0]);  // Start symbol

    int i = 0;
    char currentInput = input[i];

    printf("\nParsing process:\n");
    while (stack[top] != '$') {
        char topSymbol = pop();

        if (topSymbol == currentInput) {
            printf("Matched: %c\n", currentInput);
            i++;
            currentInput = input[i];
        } else if (topSymbol >= 'A' && topSymbol <= 'Z') {
            int row = topSymbol - 'A';
            int col = -1;
            for (int j = 0; j < numTerminals; j++) {
                if (terminals[j] == currentInput) {
                    col = j;
                    break;
                }
            }

            if (col != -1 && parseTable[row][col] != '-') {
                int ruleIndex = parseTable[row][col] - '1';
                printf("Applying rule: %s -> %s\n", grammar[ruleIndex][0], grammar[ruleIndex][1]);

                if (strcmp(grammar[ruleIndex][1], "e") != 0) {
                    for (int j = strlen(grammar[ruleIndex][1]) - 1; j >= 0; j--) {
                        push(grammar[ruleIndex][1][j]);
                    }
                }
            } else {
                printf("Error: No rule for %c on input %c\n", topSymbol, currentInput);
                return;
            }
        } else {
            printf("Error: Unexpected symbol %c\n", topSymbol);
            return;
        }
    }

    if (input[i] == '$') {
        printf("Input string is successfully parsed.\n");
    } else {
        printf("Error: Input string not fully parsed.\n");
    }
}

void inputGrammar() {
    printf("Enter the number of production rules: ");
    scanf("%d", &numProductions);
    getchar();  // Consume the newline character

    for (int i = 0; i < numProductions; i++) {
        printf("Enter production rule %d (format: A->BC or A->a): ", i + 1);
        char lhs[2], rhs[MAX];
        scanf("%s -> %s", lhs, rhs);

        strcpy(grammar[i][0], lhs);
        strcpy(grammar[i][1], rhs);

        // Add non-terminals
        if (!strchr(nonTerminals, lhs[0])) {
            nonTerminals[numNonTerminals++] = lhs[0];
        }

        // Add terminals
        for (int j = 0; j < strlen(rhs); j++) {
            if (islower(rhs[j]) && !strchr(terminals, rhs[j])) {
                terminals[numTerminals++] = rhs[j];
            }
        }
    }
    terminals[numTerminals++] = '$';  // End marker
}

int main() {
    inputGrammar();

    for (int i = 0; i < numNonTerminals; i++) {
        findFirst(nonTerminals[i], first[i]);
    }

    for (int i = 0; i < numNonTerminals; i++) {
        findFollow(nonTerminals[i], follow[i]);
    }

    displayFirstFollow();

    constructParseTable();

    displayParseTable();

    char input[MAX];
    printf("\nEnter input string to parse (append $ at the end): ");
    scanf("%s", input);

    predictiveParsing(input);

    return 0;
}
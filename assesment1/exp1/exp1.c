#include <stdio.h>

#include <string.h>



#define MAX_STATES 100

#define MAX_SYMBOLS 26

#define MAX_LABEL_LENGTH 10



typedef struct {

    int transition[MAX_STATES][MAX_SYMBOLS];

    int numStates;

    int startState;

    int acceptStates[MAX_STATES];

    char stateLabels[MAX_STATES][MAX_LABEL_LENGTH];

    char symbols[MAX_SYMBOLS];

    int numSymbols;

} DFA;



void initializeDFA(DFA *dfa) {

    memset(dfa->transition, -1, sizeof(dfa->transition));

    memset(dfa->acceptStates, 0, sizeof(dfa->acceptStates));

}



int findStateIndex(DFA *dfa, char *label) {

    for (int i = 0; i < dfa->numStates; i++) {

        if (strcmp(dfa->stateLabels[i], label) == 0) {

            return i;

        }

    }

    return -1;

}



int findSymbolIndex(DFA *dfa, char symbol) {

    for (int i = 0; i < dfa->numSymbols; i++) {

        if (dfa->symbols[i] == symbol) {

            return i;

        }

    }

    return -1;

}



void createDFA(DFA *dfa) {

    printf("Enter the number of states: ");

    scanf("%d", &(dfa->numStates));

    printf("Enter the state labels:\n");

    for (int i = 0; i < dfa->numStates; i++) {

        printf("State %d: ", i + 1);

        scanf("%s", dfa->stateLabels[i]);

    }

    printf("Enter the number of input symbols: ");

    scanf("%d", &(dfa->numSymbols));

    printf("Enter the symbols:\n");

    for (int i = 0; i < dfa->numSymbols; i++) {

        printf("Symbol %d: ", i + 1);

        scanf(" %c", &(dfa->symbols[i]));

    }

    char startLabel[MAX_LABEL_LENGTH];

    printf("Enter the start state label: ");

    scanf("%s", startLabel);

    dfa->startState = findStateIndex(dfa, startLabel);

    int numAcceptStates;

    printf("Enter the number of accept states: ");

    scanf("%d", &numAcceptStates);

    printf("Enter the accept state labels:\n");

    for (int i = 0; i < numAcceptStates; i++) {

        char acceptLabel[MAX_LABEL_LENGTH];

        scanf("%s", acceptLabel);

        int index = findStateIndex(dfa, acceptLabel);

        if (index != -1) {

            dfa->acceptStates[index] = 1;

        }

    }

    printf("Enter transitions:\n");

    for (int i = 0; i < dfa->numStates; i++) {

        for (int j = 0; j < dfa->numSymbols; j++) {

            char nextState[MAX_LABEL_LENGTH];

            printf("Enter for D(%s,%c): ", dfa->stateLabels[i], dfa->symbols[j]);

            scanf("%s", nextState);

            int nextIndex = findStateIndex(dfa, nextState);

            if (nextIndex != -1) {

                dfa->transition[i][j] = nextIndex;

            }

        }

    }

}



void displayTransitionTable(DFA *dfa) {

    printf("\nTransition Table:\n");

    printf("States\t");

    for (int i = 0; i < dfa->numSymbols; i++) {

        printf("%c\t", dfa->symbols[i]);

    }

    printf("\n");

    for (int i = 0; i < dfa->numStates; i++) {

        printf("%s\t", dfa->stateLabels[i]);

        for (int j = 0; j < dfa->numSymbols; j++) {

            if (dfa->transition[i][j] != -1) {

                printf("%s\t", dfa->stateLabels[dfa->transition[i][j]]);

            } else {

                printf("-\t");

            }

        }

        printf("\n");

    }

}



int isAccepted(DFA *dfa, const char *input) {

    int currentState = dfa->startState;

    printf("Transition path: ");

    for (int i = 0; i < strlen(input); i++) {

        char symbol = input[i];

        int symbolIndex = findSymbolIndex(dfa, symbol);

        int nextState = dfa->transition[currentState][symbolIndex];

        if (nextState != -1) {

            printf("d(%s,%c)->", dfa->stateLabels[currentState], symbol);

            currentState = nextState;

        } else {

            printf("Invalid transition on symbol '%c'.\n", symbol);

            return 0;

        }

    }

    printf("%s\n", dfa->stateLabels[currentState]);

    return dfa->acceptStates[currentState];

}



int main() {

    DFA dfa;

    initializeDFA(&dfa);

    createDFA(&dfa);

   

    displayTransitionTable(&dfa);

   

    char input[100];

   

    printf("Enter a string to check: ");

   

    scanf("%s", input);

   

    if (isAccepted(&dfa, input)) {

        printf("The string is accepted by the DFA.\n");

    } else {

        printf("The string is rejected by the DFA.\n");

    }

   

    return 0;

}
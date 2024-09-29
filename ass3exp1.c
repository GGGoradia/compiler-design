#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NUM_TERMINALS 6
#define MAX_NON_TERMINALS 10

typedef struct {
  char non_terminal[10];
  char productions[MAX_NON_TERMINALS][20];
} ParseTableEntry;

char terminals[NUM_TERMINALS][10] = {"(", "id", "+", "*", ")", "$"};

bool containsEpsilon(char *set) { return strchr(set, 'ε') != NULL; }

void addToSet(char *set, char c) {
  if (!strchr(set, c)) {
    strncat(set, &c, 1);
  }
}

void calculateFirstSet(int num_non_terminals, ParseTableEntry parseTable[],
                       char firstSets[][20]) {
  bool changed = true;

  while (changed) {
    changed = false;

    for (int i = 0; i < num_non_terminals; i++) {
      for (int j = 0; j < MAX_NON_TERMINALS; j++) {
        char *prod = parseTable[i].productions[j];
        if (prod[0] >= 'a' && prod[0] <= 'z') { // Terminal
          if (!strchr(firstSets[i], prod[0])) {
            strncat(firstSets[i], prod, 1);
            changed = true;
          }
        } else if (prod[0] >= 'A' && prod[0] <= 'Z') { // Non-terminal
          int nt_index = prod[0] - 'A'; // Assuming A-Z map to 0-25
          for (int k = 0; k < strlen(firstSets[nt_index]); k++) {
            char first_char = firstSets[nt_index][k];
            if (first_char != 'ε' && !strchr(firstSets[i], first_char)) {
              strncat(firstSets[i], &first_char, 1);
              changed = true;
            }
          }
          if (containsEpsilon(firstSets[nt_index])) {
            addToSet(firstSets[i], 'ε');
          }
        }
      }
    }
  }
}

void calculateFollowSet(int num_non_terminals, ParseTableEntry parseTable[],
                        char firstSets[][20], char followSets[][20]) {
  strcat(followSets[0], "$"); // FOLLOW of start symbol includes $

  bool changed = true;

  while (changed) {
    changed = false;

    for (int i = 0; i < num_non_terminals; i++) {
      for (int j = 0; j < MAX_NON_TERMINALS; j++) {
        char *prod = parseTable[i].productions[j];
        int len = strlen(prod);

        for (int k = 0; k < len; k++) {
          if (prod[k] >= 'A' && prod[k] <= 'Z') { // Non-terminal
            int nt_index = prod[k] - 'A';

            // If there is something after the non-terminal
            if (k + 1 < len) {
              if (prod[k + 1] >= 'a' && prod[k + 1] <= 'z') { // Terminal
                addToSet(followSets[nt_index], prod[k + 1]);
              } else if (prod[k + 1] >= 'A' &&
                         prod[k + 1] <= 'Z') { // Non-terminal
                int next_nt_index = prod[k + 1] - 'A';
                for (int m = 0; m < strlen(firstSets[next_nt_index]); m++) {
                  if (firstSets[next_nt_index][m] != 'ε') {
                    addToSet(followSets[nt_index], firstSets[next_nt_index][m]);
                  }
                }
              }
            }

            // If it's the last symbol or followed by ε, add FOLLOW of LHS
            // non-terminal
            if (k == len - 1 || containsEpsilon(firstSets[prod[k + 1] - 'A'])) {
              for (int n = 0; n < strlen(followSets[i]); n++) {
                if (!strchr(followSets[nt_index], followSets[i][n])) {
                  addToSet(followSets[nt_index], followSets[i][n]);
                  changed = true;
                }
              }
            }
          }
        }
      }
    }
  }
}

int main() {
  int num_non_terminals;

  printf("Enter the number of non-terminals: ");
  scanf("%d", &num_non_terminals);

  ParseTableEntry parseTable[num_non_terminals];
  char firstSets[num_non_terminals][20];
  char followSets[num_non_terminals][20];

  for (int i = 0; i < num_non_terminals; i++) {
    memset(firstSets[i], 0, sizeof(firstSets[i]));
    memset(followSets[i], 0, sizeof(followSets[i]));

    printf("Enter non-terminal %d: ", i + 1);
    scanf("%s", parseTable[i].non_terminal);

    printf("Enter productions for %s (use 'NULL' for empty slots, max %d "
           "productions):\n",
           parseTable[i].non_terminal, MAX_NON_TERMINALS);
    for (int j = 0; j < MAX_NON_TERMINALS; j++) {
      printf("Production %d: ", j + 1);
      scanf("%s", parseTable[i].productions[j]);
    }
  }

  calculateFirstSet(num_non_terminals, parseTable, firstSets);
  calculateFollowSet(num_non_terminals, parseTable, firstSets, followSets);

  printf("\nFIRST and FOLLOW Sets:\n");
  for (int i = 0; i < num_non_terminals; i++) {
    printf("FIRST(%s) = %s\n", parseTable[i].non_terminal, firstSets[i]);
    printf("FOLLOW(%s) = %s\n", parseTable[i].non_terminal, followSets[i]);
  }

  return 0;
}

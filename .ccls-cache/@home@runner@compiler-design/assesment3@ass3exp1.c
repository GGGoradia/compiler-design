#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100
#define MAX_RULES 100
#define MAX_RULE_LENGTH 20

char terminals[MAX_SYMBOLS];
char non_terminals[MAX_SYMBOLS];
char productions[MAX_RULES][MAX_RULE_LENGTH];
int num_terminals, num_non_terminals, num_rules;

char first_sets[MAX_SYMBOLS][MAX_SYMBOLS];
char follow_sets[MAX_SYMBOLS][MAX_SYMBOLS];
char parse_table[MAX_SYMBOLS][MAX_SYMBOLS][MAX_RULE_LENGTH];

void read_input() {
  printf("Enter number of terminals: ");
  scanf("%d", &num_terminals);
  printf("Enter terminals: ");
  for (int i = 0; i < num_terminals; i++) {
    scanf(" %c", &terminals[i]);
  }

  printf("Enter number of non-terminals: ");
  scanf("%d", &num_non_terminals);
  printf("Enter non-terminals: ");
  for (int i = 0; i < num_non_terminals; i++) {
    scanf(" %c", &non_terminals[i]);
  }

  printf("Enter number of production rules: ");
  scanf("%d", &num_rules);
  printf("Enter production rules (e.g., S->aB):\n");
  for (int i = 0; i < num_rules; i++) {
    scanf("%s", productions[i]);
  }
}

bool is_terminal(char symbol) {
  for (int i = 0; i < num_terminals; i++) {
    if (terminals[i] == symbol)
      return true;
  }
  return false;
}

bool is_non_terminal(char symbol) {
  for (int i = 0; i < num_non_terminals; i++) {
    if (non_terminals[i] == symbol)
      return true;
  }
  return false;
}

int get_non_terminal_index(char symbol) {
  for (int i = 0; i < num_non_terminals; i++) {
    if (non_terminals[i] == symbol)
      return i;
  }
  return -1;
}

int get_terminal_index(char symbol) {
  for (int i = 0; i < num_terminals; i++) {
    if (terminals[i] == symbol)
      return i;
  }
  return -1;
}

bool add_to_set(char *set, char symbol) {
  for (int i = 0; set[i] != '\0'; i++) {
    if (set[i] == symbol)
      return false;
  }
  int len = strlen(set);
  set[len] = symbol;
  set[len + 1] = '\0';
  return true;
}

void calculate_first_sets() {
  bool changed;
  do {
    changed = false;
    for (int i = 0; i < num_rules; i++) {
      char non_terminal = productions[i][0];
      char *rhs = productions[i] + 3; // Skip "X->"

      if (is_terminal(rhs[0]) || rhs[0] == '$') {
        if (add_to_set(first_sets[get_non_terminal_index(non_terminal)],
                       rhs[0])) {
          changed = true;
        }
      } else if (is_non_terminal(rhs[0])) {
        int first_nt_index = get_non_terminal_index(rhs[0]);
        for (int j = 0; first_sets[first_nt_index][j] != '\0'; j++) {
          if (add_to_set(first_sets[get_non_terminal_index(non_terminal)],
                         first_sets[first_nt_index][j])) {
            changed = true;
          }
        }
      }
    }
  } while (changed);
}

void calculate_follow_sets() {
  add_to_set(follow_sets[0], '$'); // Add $ to follow set of start symbol

  bool changed;
  do {
    changed = false;
    for (int i = 0; i < num_rules; i++) {
      char non_terminal = productions[i][0];
      char *rhs = productions[i] + 3; // Skip "X->"

      for (int j = 0; rhs[j] != '\0'; j++) {
        if (is_non_terminal(rhs[j])) {
          int current_nt_index = get_non_terminal_index(rhs[j]);

          if (rhs[j + 1] == '\0') { // If it's the last symbol
            int nt_index = get_non_terminal_index(non_terminal);
            for (int k = 0; follow_sets[nt_index][k] != '\0'; k++) {
              if (add_to_set(follow_sets[current_nt_index],
                             follow_sets[nt_index][k])) {
                changed = true;
              }
            }
          } else if (is_terminal(rhs[j + 1])) {
            if (add_to_set(follow_sets[current_nt_index], rhs[j + 1])) {
              changed = true;
            }
          } else if (is_non_terminal(rhs[j + 1])) {
            int next_nt_index = get_non_terminal_index(rhs[j + 1]);
            for (int k = 0; first_sets[next_nt_index][k] != '\0'; k++) {
              if (first_sets[next_nt_index][k] != '$') {
                if (add_to_set(follow_sets[current_nt_index],
                               first_sets[next_nt_index][k])) {
                  changed = true;
                }
              }
            }
          }
        }
      }
    }
  } while (changed);
}

void construct_parse_table() {
  for (int i = 0; i < num_rules; i++) {
    char non_terminal = productions[i][0];
    char *rhs = productions[i] + 3; // Skip "X->"
    int nt_index = get_non_terminal_index(non_terminal);

    if (is_terminal(rhs[0]) || rhs[0] == '$') {
      int terminal_index = get_terminal_index(rhs[0]);
      if (terminal_index != -1) {
        strcpy(parse_table[nt_index][terminal_index], productions[i]);
      }
    } else {
      int first_nt_index = get_non_terminal_index(rhs[0]);
      for (int j = 0; first_sets[first_nt_index][j] != '\0'; j++) {
        char terminal = first_sets[first_nt_index][j];
        if (terminal != '$') {
          int terminal_index = get_terminal_index(terminal);
          if (terminal_index != -1) {
            strcpy(parse_table[nt_index][terminal_index], productions[i]);
          }               
        }
      }

      if (strchr(first_sets[first_nt_index], '$') != NULL) {
        for (int j = 0; follow_sets[nt_index][j] != '\0'; j++) {
          char terminal = follow_sets[nt_index][j];
          if (terminal != '$') {
            int terminal_index = get_terminal_index(terminal);
            if (terminal_index != -1) {
              strcpy(parse_table[nt_index][terminal_index], productions[i]);
            }
          }
        }
      }
    }
  }
}

void display_results() {
  printf("\nFirst Sets:\n");
  for (int i = 0; i < num_non_terminals; i++) {
    printf("%c: { ", non_terminals[i]);
    for (int j = 0; first_sets[i][j] != '\0'; j++) {
      printf("%c ", first_sets[i][j]);
    }
    printf("}\n");
  }

  printf("\nFollow Sets:\n");
  for (int i = 0; i < num_non_terminals; i++) {
    printf("%c: { ", non_terminals[i]);
    for (int j = 0; follow_sets[i][j] != '\0'; j++) {
      printf("%c ", follow_sets[i][j]);
    }
    printf("}\n");
  }

  printf("\nPredictive Parse Table:\n");
  printf("   ");
  for (int i = 0; i < num_terminals; i++) {
    printf("%-10c", terminals[i]);
  }
  printf("\n");
  for (int i = 0; i < num_non_terminals; i++) {
    printf("%c: ", non_terminals[i]);
    for (int j = 0; j < num_terminals; j++) {
      if (parse_table[i][j][0] != '\0') {
        printf("%-10s", parse_table[i][j]);
      } else {
        printf("%-10s", "");
      }
    }
    printf("\n");
  }
}

int main() {
  read_input();
  calculate_first_sets();
  calculate_follow_sets();
  construct_parse_table();
  display_results();
  return 0;
}
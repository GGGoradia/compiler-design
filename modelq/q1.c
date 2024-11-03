#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODS 50
#define MAX_LEN 100
#define MAX_NT 20
#define MAX_T 20
#define MAX_STATES 100
#define MAX_ITEMS 100
#define MAX_FOLLOW 20

struct Production {
    char left;
    char right[MAX_LEN];
};

struct Item {
    char left;
    char right[MAX_LEN];
    int dot_pos;
    char lookahead[MAX_T];
};

struct State {
    struct Item items[MAX_ITEMS];
    int num_items;
};

struct Production prods[MAX_PRODS];
int num_prods = 0;
char non_terminals[MAX_NT];
int num_nt = 0;
char terminals[MAX_T];
int num_t = 0;
struct State states[MAX_STATES];
int num_states = 0;

char first[MAX_NT][MAX_T];
char follow[MAX_NT][MAX_T];
int first_len[MAX_NT] = {0};
int follow_len[MAX_NT] = {0};

// Forward declarations
int states_equal(struct State s1, struct State s2);
int can_be_epsilon(char nt);
void compute_lookahead(struct Item item, int pos, char* result);
int add_to_first(char nt, char t);
int add_to_follow(char nt, char t);
int is_non_terminal(char c);


void augment_grammar() {
    // Shift all existing productions one position to the right
    for(int i = num_prods; i > 0; i--) {
        prods[i] = prods[i-1];
    }
    
    // Add new start production S' -> S
    prods[0].left = 'S';  // Using 'S' for S'
    prods[0].right[0] = prods[1].left;  // Original start symbol
    prods[0].right[1] = '\0';
    num_prods++;
}

int states_equal(struct State s1, struct State s2) {
    if(s1.num_items != s2.num_items) return 0;
    
    for(int i = 0; i < s1.num_items; i++) {
        int found = 0;
        for(int j = 0; j < s2.num_items; j++) {
            if(s1.items[i].left == s2.items[j].left &&
               strcmp(s1.items[i].right, s2.items[j].right) == 0 &&
               s1.items[i].dot_pos == s2.items[j].dot_pos &&
               strcmp(s1.items[i].lookahead, s2.items[j].lookahead) == 0) {
                found = 1;
                break;
            }
        }
        if(!found) return 0;
    }
    return 1;
}

int can_be_epsilon(char nt) {
    for(int i = 0; i < num_prods; i++) {
        if(prods[i].left == nt && strlen(prods[i].right) == 0)
            return 1;
    }
    return 0;
}

int is_non_terminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

void compute_lookahead(struct Item item, int pos, char* result) {
    result[0] = '\0';
    int result_len = 0;
    
    if(pos >= strlen(item.right)) {
        strcpy(result, item.lookahead);
        return;
    }
    
    char next = item.right[pos];
    if(!is_non_terminal(next)) {
        result[result_len++] = next;
        result[result_len] = '\0';
        return;
    }
    
    int nt_idx = next - 'A';
    for(int i = 0; i < first_len[nt_idx]; i++) {
        result[result_len++] = first[nt_idx][i];
    }
    result[result_len] = '\0';
}

int add_to_first(char nt, char t) {
    int idx = nt - 'A';
    for(int i = 0; i < first_len[idx]; i++) {
        if(first[idx][i] == t) return 0;
    }
    first[idx][first_len[idx]++] = t;
    return 1;
}

int add_to_follow(char nt, char t) {
    int idx = nt - 'A';
    for(int i = 0; i < follow_len[idx]; i++) {
        if(follow[idx][i] == t) return 0;
    }
    follow[idx][follow_len[idx]++] = t;
    return 1;
}

void collect_symbols() {
    // Add augmented start symbol first
    non_terminals[num_nt++] = 'S';  // S' is represented as 'S'
    
    for(int i = 1; i < num_prods; i++) {  // Start from 1 to skip augmented production
        char nt = prods[i].left;
        int found = 0;
        for(int j = 0; j < num_nt; j++) {
            if(non_terminals[j] == nt) {
                found = 1;
                break;
            }
        }
        if(!found) non_terminals[num_nt++] = nt;

        for(int j = 0; j < strlen(prods[i].right); j++) {
            char c = prods[i].right[j];
            if(is_non_terminal(c)) {
                found = 0;
                for(int k = 0; k < num_nt; k++) {
                    if(non_terminals[k] == c) {
                        found = 1;
                        break;
                    }
                }
                if(!found) non_terminals[num_nt++] = c;
            } else {
                found = 0;
                for(int k = 0; k < num_t; k++) {
                    if(terminals[k] == c) {
                        found = 1;
                        break;
                    }
                }
                if(!found && c != '\0') terminals[num_t++] = c;
            }
        }
    }
    terminals[num_t++] = '$';
}

void compute_first_sets() {
    int changed;
    do {
        changed = 0;
        for(int i = 0; i < num_prods; i++) {
            char nt = prods[i].left;
            char *rhs = prods[i].right;
            
            if(strlen(rhs) == 0) {
                // Handle epsilon productions
                if(add_to_first(nt, 'e')) changed = 1;
                continue;
            }
            
            if(!is_non_terminal(rhs[0])) {
                if(add_to_first(nt, rhs[0])) changed = 1;
            } else {
                for(int j = 0; j < strlen(rhs); j++) {
                    if(!is_non_terminal(rhs[j])) {
                        if(add_to_first(nt, rhs[j])) changed = 1;
                        break;
                    }
                    
                    char cur_nt = rhs[j];
                    int idx = cur_nt - 'A';
                    for(int k = 0; k < first_len[idx]; k++) {
                        if(first[idx][k] != 'e' && add_to_first(nt, first[idx][k])) 
                            changed = 1;
                    }
                    
                    if(!can_be_epsilon(cur_nt)) break;
                }
            }
        }
    } while(changed);
}

void compute_follow_sets() {
    // Add $ to follow set of augmented start symbol S'
    add_to_follow('S', '$');
    
    int changed;
    do {
        changed = 0;
        for(int i = 0; i < num_prods; i++) {
            char *rhs = prods[i].right;
            int len = strlen(rhs);
            
            for(int j = 0; j < len; j++) {
                if(!is_non_terminal(rhs[j])) continue;
                
                if(j == len-1) {
                    int idx = prods[i].left - 'A';
                    for(int k = 0; k < follow_len[idx]; k++) {
                        if(add_to_follow(rhs[j], follow[idx][k])) 
                            changed = 1;
                    }
                }
                
                for(int k = j+1; k < len; k++) {
                    if(!is_non_terminal(rhs[k])) {
                        if(add_to_follow(rhs[j], rhs[k])) 
                            changed = 1;
                        break;
                    }
                    
                    int idx = rhs[k] - 'A';
                    for(int l = 0; l < first_len[idx]; l++) {
                        if(first[idx][l] != 'e' && add_to_follow(rhs[j], first[idx][l])) 
                            changed = 1;
                    }
                    
                    if(!can_be_epsilon(rhs[k])) break;
                    
                    if(k == len-1) {
                        idx = prods[i].left - 'A';
                        for(int l = 0; l < follow_len[idx]; l++) {
                            if(add_to_follow(rhs[j], follow[idx][l])) 
                                changed = 1;
                        }
                    }
                }
            }
        }
    } while(changed);
}
struct Item create_item(char left, char *right, int dot_pos, char *lookahead) {
    struct Item item;
    item.left = left;
    strcpy(item.right, right);
    item.dot_pos = dot_pos;
    strcpy(item.lookahead, lookahead);
    return item;
}

void add_item_to_state(struct State *state, struct Item item) {
    if(state->num_items >= MAX_ITEMS) return;
    
    for(int i = 0; i < state->num_items; i++) {
        if(state->items[i].left == item.left && 
           strcmp(state->items[i].right, item.right) == 0 &&
           state->items[i].dot_pos == item.dot_pos &&
           strcmp(state->items[i].lookahead, item.lookahead) == 0) {
            return;
        }
    }
    state->items[state->num_items++] = item;
}

void compute_closure(struct State *state) {
    int changed;
    do {
        changed = 0;
        for(int i = 0; i < state->num_items; i++) {
            struct Item item = state->items[i];
            if(item.dot_pos >= strlen(item.right)) continue;
            
            char next = item.right[item.dot_pos];
            if(!is_non_terminal(next)) continue;
            
            char lookahead[MAX_T];
            compute_lookahead(item, item.dot_pos + 1, lookahead);
            if(strlen(lookahead) == 0) strcpy(lookahead, item.lookahead);
            
            for(int j = 0; j < num_prods; j++) {
                if(prods[j].left != next) continue;
                
                struct Item new_item = create_item(prods[j].left, prods[j].right, 0, lookahead);
                
                int old_num = state->num_items;
                add_item_to_state(state, new_item);
                if(state->num_items > old_num) changed = 1;
            }
        }
    } while(changed);
}

struct State goto_state(struct State state, char symbol) {
    struct State new_state;
    new_state.num_items = 0;
    
    for(int i = 0; i < state.num_items; i++) {
        struct Item item = state.items[i];
        if(item.dot_pos >= strlen(item.right)) continue;
        
        if(item.right[item.dot_pos] == symbol) {
            struct Item new_item = item;
            new_item.dot_pos++;
            add_item_to_state(&new_state, new_item);
        }
    }
    
    compute_closure(&new_state);
    return new_state;
}

void compute_states() {
    struct State initial_state;
    initial_state.num_items = 0;
    
    char initial_lookahead[2] = "$";
    struct Item initial_item = create_item(prods[0].left, prods[0].right, 0, initial_lookahead);
    add_item_to_state(&initial_state, initial_item);
    compute_closure(&initial_state);
    
    states[num_states++] = initial_state;
    
    for(int i = 0; i < num_states && i < MAX_STATES; i++) {
        for(int j = 0; j < num_nt; j++) {
            struct State new_state = goto_state(states[i], non_terminals[j]);
            if(new_state.num_items > 0) {
                int exists = 0;
                for(int k = 0; k < num_states; k++) {
                    if(states_equal(new_state, states[k])) {
                        exists = 1;
                        break;
                    }
                }
                if(!exists && num_states < MAX_STATES) states[num_states++] = new_state;
            }
        }
        
        for(int j = 0; j < num_t; j++) {
            struct State new_state = goto_state(states[i], terminals[j]);
            if(new_state.num_items > 0) {
                int exists = 0;
                for(int k = 0; k < num_states; k++) {
                    if(states_equal(new_state, states[k])) {
                        exists = 1;
                        break;
                    }
                }
                if(!exists && num_states < MAX_STATES) states[num_states++] = new_state;
            }
        }
    }
}

void print_parse_table() {
    printf("\nCLR(1) PARSING TABLE\n\n");
    
    printf("STATE\t");
    for(int i = 0; i < num_t; i++) printf("%c\t", terminals[i]);
    for(int i = 0; i < num_nt; i++) printf("%c\t", non_terminals[i]);
    printf("\n");
    
    for(int i = 0; i < num_states; i++) {
        printf("%d\t", i);
        
        // Action table
        for(int j = 0; j < num_t; j++) {
            int action_found = 0;
            
            // Check for shift
            struct State next_state = goto_state(states[i], terminals[j]);
            if(next_state.num_items > 0) {
                for(int k = 0; k < num_states; k++) {
                    if(states_equal(next_state, states[k])) {
                        printf("s%d\t", k);
                        action_found = 1;
                        break;
                    }
                }
            }
            
            // Check for reduce/accept
            if(!action_found) {
                for(int k = 0; k < states[i].num_items; k++) {
                    struct Item item = states[i].items[k];
                    if(item.dot_pos == strlen(item.right)) {
                        for(int l = 0; l < strlen(item.lookahead); l++) {
                            if(terminals[j] == item.lookahead[l]) {
                                if(item.left == prods[0].left && strcmp(item.right, prods[0].right) == 0) {
                                    printf("acc\t");
                                } else {
                                    for(int m = 0; m < num_prods; m++) {
                                        if(prods[m].left == item.left && 
                                           strcmp(prods[m].right, item.right) == 0) {
                                            printf("r%d\t", m);
                                            break;
                                        }
                                    }
                                }
                                action_found = 1;
                                break;
                            }
                        }
                        if(action_found) break;
                    }
                }
            }
            
            if(!action_found) printf("-\t");
        }
        
        // Goto table
        for(int j = 0; j < num_nt; j++) {
            int goto_found = 0;
            struct State next_state = goto_state(states[i], non_terminals[j]);
            if(next_state.num_items > 0) {
                for(int k = 0; k < num_states; k++) {
                    if(states_equal(next_state, states[k])) {
                        printf("%d\t", k);
                        goto_found = 1;
                        break;
                    }
                }
            }
            if(!goto_found) printf("-\t");
        }
        printf("\n");
    }
}

void print_first_follow_sets() {
    printf("\nFIRST SETS:\n");
    for(int i = 0; i < num_nt; i++) {
        printf("FIRST(%c) = { ", non_terminals[i]);
        for(int j = 0; j < first_len[i]; j++) {
            printf("%c ", first[i][j]);
        }
        printf("}\n");
    }
    
    printf("\nFOLLOW SETS:\n");
    for(int i = 0; i < num_nt; i++) {
        printf("FOLLOW(%c) = { ", non_terminals[i]);
        for(int j = 0; j < follow_len[i]; j++) {
            printf("%c ", follow[i][j]);
        }
        printf("}\n");
    }
}

int main() {
    printf("Enter number of productions (not including augmented production): ");
    scanf("%d", &num_prods);
    getchar(); // consume newline
    
    printf("Enter productions (e.g., E->E+T or E-> for epsilon):\n");
    printf("Note: First production should be the start production\n");
    for(int i = 0; i < num_prods; i++) {
        char line[MAX_LEN];
        fgets(line, MAX_LEN, stdin);
        line[strcspn(line, "\n")] = 0; // remove newline
        
        prods[i].left = line[0];
        if(strlen(line) <= 3) {
            prods[i].right[0] = '\0';
        } else {
            strcpy(prods[i].right, line + 3);
        }
    }
    
    // Augment the grammar before processing
    augment_grammar();
    
    collect_symbols();
    compute_first_sets();
    compute_follow_sets();
    compute_states();
    
    printf("\nAugmented Grammar:\n");
    printf("S' -> %c\n", prods[0].right[0]);
    printf("Original Productions:\n");
    for(int i = 1; i < num_prods; i++) {
        printf("%c -> %s\n", prods[i].left, prods[i].right);
    }
    
    printf("\nSymbols:\n");
    printf("Non-terminals: ");
    for(int i = 0; i < num_nt; i++) printf("%c ", non_terminals[i]);
    printf("\nTerminals: ");
    for(int i = 0; i < num_t; i++) printf("%c ", terminals[i]);
    printf("\n");
    
    print_first_follow_sets();
    
    printf("\nSTATES:\n");
    for(int i = 0; i < num_states; i++) {
        printf("\nState %d:\n", i);
        for(int j = 0; j < states[i].num_items; j++) {
            struct Item item = states[i].items[j];
            printf("%c -> ", item.left);
            for(int k = 0; k < strlen(item.right); k++) {
                if(k == item.dot_pos) printf(".");
                printf("%c", item.right[k]);
            }
            if(item.dot_pos == strlen(item.right)) printf(".");
            printf(", %s\n", item.lookahead);
        }
    }
    
    print_parse_table();
    
    return 0;
}
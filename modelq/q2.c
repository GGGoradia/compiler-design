#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK 100
#define MAX_STRING 100
#define MAX_PRODUCTIONS 20
#define MAX_SYMBOLS 50
#define MAX_RHS_LENGTH 20

typedef struct {
    char action;     
    int next_state;  
} TableEntry;

typedef struct {
    char lhs;
    char rhs[MAX_RHS_LENGTH];
    int rhs_length;
} Production;

// Global variables
TableEntry parsing_table[MAX_STACK][MAX_SYMBOLS];
Production productions[MAX_PRODUCTIONS];
int num_productions = 0;
int num_states = 0;
char terminals[MAX_SYMBOLS];
char non_terminals[MAX_SYMBOLS];
int num_terminals = 0;
int num_non_terminals = 0;

// Function to check if a character is in array
int contains(char* arr, int size, char c) {
    for(int i = 0; i < size; i++) {
        if(arr[i] == c) return 1;
    }
    return 0;
}

// Function to read grammar from user
void read_grammar() {
    printf("Enter number of productions: ");
    scanf("%d", &num_productions);
    getchar(); // consume newline

    printf("\nEnter productions in format (A=BC or A=a):\n");
    printf("Use capital letters for non-terminals and small letters for terminals\n");
    printf("Use '=' instead of '->' for production rules\n");
    printf("Do not use spaces in production\n\n");

    for(int i = 0; i < num_productions; i++) {
        char production[MAX_STRING];
        printf("Production %d: ", i+1);
        scanf("%s", production);
        
        productions[i].lhs = production[0];
        if(!contains(non_terminals, num_non_terminals, production[0])) {
            non_terminals[num_non_terminals++] = production[0];
        }

        int j = 0;
        for(int k = 2; k < strlen(production); k++) {  // start after '='
            productions[i].rhs[j] = production[k];
            if(isupper(production[k])) {  // non-terminal
                if(!contains(non_terminals, num_non_terminals, production[k])) {
                    non_terminals[num_non_terminals++] = production[k];
                }
            } else {  // terminal
                if(!contains(terminals, num_terminals, production[k])) {
                    terminals[num_terminals++] = production[k];
                }
            }
            j++;
        }
        productions[i].rhs[j] = '\0';
        productions[i].rhs_length = j;
    }

    // Add $ to terminals if not present
    if(!contains(terminals, num_terminals, '$')) {
        terminals[num_terminals++] = '$';
    }

    // Print collected symbols
    printf("\nTerminals: ");
    for(int i = 0; i < num_terminals; i++) {
        printf("%c ", terminals[i]);
    }
    printf("\nNon-terminals: ");
    for(int i = 0; i < num_non_terminals; i++) {
        printf("%c ", non_terminals[i]);
    }
    printf("\n");
}

// Function to generate canonical LR(1) items (simplified version)
void generate_parsing_table() {
    // Initialize table with error entries
    for(int i = 0; i < MAX_STACK; i++) {
        for(int j = 0; j < MAX_SYMBOLS; j++) {
            parsing_table[i][j].action = 'e';
            parsing_table[i][j].next_state = -1;
        }
    }

    // Here we would normally generate the actual LR parsing table
    // This is a complex process involving:
    // 1. Computing LR(1) items
    // 2. Computing closure of item sets
    // 3. Computing goto function
    // 4. Building action and goto tables

    // For demonstration, we'll create a simple table for the input grammar
    // This is a placeholder - in a real implementation, this would be generated
    // based on the grammar rules
    
    num_states = num_productions + 2;  // Simplified state count

    // Add some basic shifts and reduces based on the grammar
    for(int i = 0; i < num_states; i++) {
        for(int j = 0; j < num_terminals; j++) {
            if(i < num_productions) {
                parsing_table[i][terminals[j]].action = 's';
                parsing_table[i][terminals[j]].next_state = i + 1;
            }
        }
        for(int j = 0; j < num_non_terminals; j++) {
            if(i < num_productions) {
                parsing_table[i][non_terminals[j]].action = 's';
                parsing_table[i][non_terminals[j]].next_state = i + 1;
            }
        }
    }
}

void print_parsing_table() {
    printf("\nLR Parsing Table:\n");
    
    // Print header
    printf("State\t");
    for(int i = 0; i < num_terminals; i++) {
        printf("%c\t", terminals[i]);
    }
    for(int i = 0; i < num_non_terminals; i++) {
        printf("%c\t", non_terminals[i]);
    }
    printf("\n");
    
    // Print separator
    for(int i = 0; i < (num_terminals + num_non_terminals + 1) * 8; i++) {
        printf("-");
    }
    printf("\n");
    
    // Print table contents
    for(int i = 0; i < num_states; i++) {
        printf("%d\t", i);
        
        // Print actions for terminals
        for(int j = 0; j < num_terminals; j++) {
            if(parsing_table[i][terminals[j]].action != 'e') {
                printf("%c%d\t", 
                    parsing_table[i][terminals[j]].action,
                    parsing_table[i][terminals[j]].next_state);
            } else {
                printf("err\t");
            }
        }
        
        // Print actions for non-terminals
        for(int j = 0; j < num_non_terminals; j++) {
            if(parsing_table[i][non_terminals[j]].action != 'e') {
                printf("%c%d\t",
                    parsing_table[i][non_terminals[j]].action,
                    parsing_table[i][non_terminals[j]].next_state);
            } else {
                printf("err\t");
            }
        }
        printf("\n");
    }
}

int parse(char* input) {
    int stack[MAX_STACK];
    int top = 0;
    stack[top] = 0;
    
    int input_pos = 0;
    char current_symbol = input[input_pos];
    
    printf("\nParsing Steps:\n");
    printf("Stack\tInput\tAction\n");
    printf("-------------------------\n");
    
    while(1) {
        int current_state = stack[top];
        TableEntry entry = parsing_table[current_state][current_symbol];
        
        printf("Stack: ");
        for(int i = 0; i <= top; i++) {
            printf("%d ", stack[i]);
        }
        printf("\tInput: %s\tAction: ", input + input_pos);
        
        if(entry.action == 'e') {
            printf("Error\n");
            return 0;
        }
        else if(entry.action == 's') {
            top++;
            stack[top] = entry.next_state;
            input_pos++;
            current_symbol = input[input_pos];
            printf("Shift %d\n", entry.next_state);
        }
        else if(entry.action == 'r') {
            int prod_num = entry.next_state;
            top -= productions[prod_num].rhs_length;
            current_state = stack[top];
            
            TableEntry goto_entry = parsing_table[current_state][productions[prod_num].lhs];
            top++;
            stack[top] = goto_entry.next_state;
            
            printf("Reduce by %c -> %s\n", 
                   productions[prod_num].lhs, 
                   productions[prod_num].rhs);
        }
        else if(entry.action == 'a') {
            printf("Accept\n");
            return 1;
        }
    }
    
    return 0;
}

int main() {
    printf("LR Parser\n");
    printf("=========\n");
    
    // Read grammar
    read_grammar();
    
    // Generate parsing table
    generate_parsing_table();
    
    // Print parsing table
    print_parsing_table();
    
    // Read input string
    char input[MAX_STRING];
    printf("\nEnter input string: ");
    scanf("%s", input);
    
    // Parse input
    if(parse(input)) {
        printf("\nInput string accepted!\n");
    } else {
        printf("\nInput string rejected!\n");
    }
    
    return 0;
}
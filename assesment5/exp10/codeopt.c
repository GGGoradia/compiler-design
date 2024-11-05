#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000
#define MAX_TOKENS 4
#define MAX_CODE 1000

typedef struct {
    char result[20];      
    char operand1[20];    
    char operator[5];     
    char operand2[20];    
} Instruction;

Instruction code[MAX_CODE];
int code_size = 0;

int is_number(const char* str) {
    if (!str || strlen(str) == 0) return 0;
    char* endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}

int evaluate_constant(int a, const char* op, int b) {
    if (strcmp(op, "+") == 0) return a + b;
    if (strcmp(op, "-") == 0) return a - b;
    if (strcmp(op, "*") == 0) return a * b;
    if (strcmp(op, "/") == 0) return b != 0 ? a / b : 0;
    return 0;
}

void constant_folding() {
    for(int i = 0; i < code_size; i++) {
        if (is_number(code[i].operand1) && is_number(code[i].operand2)) {
            int a = atoi(code[i].operand1);
            int b = atoi(code[i].operand2);
            int result = evaluate_constant(a, code[i].operator, b);

            sprintf(code[i].operand1, "%d", result);
            strcpy(code[i].operator, "=");
            strcpy(code[i].operand2, "");

            printf("Constant folding: %s = %d\n", code[i].result, result);
        }
    }
}

void strength_reduction() {
    for(int i = 0; i < code_size; i++) {

        if (strcmp(code[i].operator, "*") == 0 && strcmp(code[i].operand2, "2") == 0) {
            strcpy(code[i].operator, "<<");
            strcpy(code[i].operand2, "1");
            printf("Strength reduction: Multiplication by 2 -> Left shift\n");
        }

        else if (strcmp(code[i].operator, "/") == 0 && strcmp(code[i].operand2, "2") == 0) {
            strcpy(code[i].operator, ">>");
            strcpy(code[i].operand2, "1");
            printf("Strength reduction: Division by 2 -> Right shift\n");
        }

        else if (strcmp(code[i].operator, "*") == 0 && strcmp(code[i].operand2, "4") == 0) {
            strcpy(code[i].operator, "<<");
            strcpy(code[i].operand2, "2");
            printf("Strength reduction: Multiplication by 4 -> Left shift\n");
        }

        else if (strcmp(code[i].operator, "*") == 0 && strcmp(code[i].operand2, "8") == 0) {
            strcpy(code[i].operator, "<<");
            strcpy(code[i].operand2, "3");
            printf("Strength reduction: Multiplication by 8 -> Left shift\n");
        }
    }
}

void algebraic_transformation() {
    for(int i = 0; i < code_size; i++) {

        if (strcmp(code[i].operator, "*") == 0 && strcmp(code[i].operand2, "1") == 0) {
            strcpy(code[i].operator, "=");
            strcpy(code[i].operand2, "");
            printf("Algebraic transformation: Multiplication by 1 eliminated\n");
        }

        else if ((strcmp(code[i].operator, "+") == 0 || strcmp(code[i].operator, "-") == 0) && 
                 strcmp(code[i].operand2, "0") == 0) {
            strcpy(code[i].operator, "=");
            strcpy(code[i].operand2, "");
            printf("Algebraic transformation: Addition/Subtraction by 0 eliminated\n");
        }

        else if (strcmp(code[i].operator, "*") == 0 && strcmp(code[i].operand2, "0") == 0) {
            strcpy(code[i].operator, "=");
            strcpy(code[i].operand1, "0");
            strcpy(code[i].operand2, "");
            printf("Algebraic transformation: Multiplication by 0 -> 0\n");
        }

        else if (strcmp(code[i].operator, "/") == 0 && strcmp(code[i].operand2, "1") == 0) {
            strcpy(code[i].operator, "=");
            strcpy(code[i].operand2, "");
            printf("Algebraic transformation: Division by 1 eliminated\n");
        }
    }
}

void read_input(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open input file %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file)) {

        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        char* token;

        token = strtok(line, "=");
        if (!token) continue;

        sscanf(token, "%s", code[code_size].result);

        token = strtok(NULL, "=");
        if (!token) continue;

        sscanf(token, "%s %s %s", 
               code[code_size].operand1, 
               code[code_size].operator, 
               code[code_size].operand2);

        code_size++;
    }

    fclose(file);
}

void write_output(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Cannot open output file %s\n", filename);
        exit(1);
    }

    for(int i = 0; i < code_size; i++) {
        fprintf(file, "%s = %s", code[i].result, code[i].operand1);
        if (strcmp(code[i].operator, "=") != 0) {
            fprintf(file, " %s %s", code[i].operator, code[i].operand2);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {

    read_input("input.tac");

    printf("Applying optimizations...\n\n");

    constant_folding();
    strength_reduction();
    algebraic_transformation();

    write_output("output.tac");

    return 0;
}
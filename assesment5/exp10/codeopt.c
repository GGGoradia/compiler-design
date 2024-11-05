#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    char a[25][50]; // Array to store TAC lines
    int i = 0;     // Index for input lines
    FILE *fi;

    printf("Enter three address code (end with Ctrl-D for EOF):\n");

    // Read lines of TAC until EOF
    while (fgets(a[i], sizeof(a[i]), stdin) != NULL) {
        a[i][strcspn(a[i], "\n")] = 0; // Remove newline character
        i++;
    }
    
    int count = i; // Number of TAC instructions

    printf("\nUnoptimized input block:\n");
    for (int j = 0; j < count; j++) {
        printf("%s\n", a[j]);
    }

    printf("\nOptimized three address code:\n");

    // Process the TAC instructions for optimizations
    for (int j = 0; j < count; j++) {
        char result[10], op1[10], op2[10], operator;
        int num1, num2, resultValue;
        
        // Parse the current line into result, op1, operator, op2
        if (sscanf(a[j], "%[^=]=%[^ ]%c%s", result, op1, &operator, op2) == 4) {
            // Check for constant folding
            if (isdigit(op1[0]) && isdigit(op2[0])) {
                num1 = atoi(op1);
                num2 = atoi(op2);

                switch (operator) {
                    case '+':
                        resultValue = num1 + num2;
                        break;
                    case '-':
                        resultValue = num1 - num2;
                        break;
                    case '*':
                        resultValue = num1 * num2;
                        break;
                    case '/':
                        resultValue = num1 / num2;
                        break;
                    default:
                        printf("Error: Unknown operator\n");
                        continue;
                }

                printf("%s=%d\n", result, resultValue);
                continue;
            }
        }

        // Handle other cases without optimization
        printf("%s\n", a[j]);
    }

    return 0;
}

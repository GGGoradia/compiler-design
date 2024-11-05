#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // For system()

int count = 0, i = 0;
char str[100][100];

void gen();

int main() {
    // Clear screen based on the operating system
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    printf("\n CODE GENERATOR \n");
    printf("\n ENTER THREE ADDRESS CODE (Type QUIT to end): \n\n");
    
    do {
        printf("\t");
        fgets(str[i], sizeof(str[i]), stdin);
        str[i][strcspn(str[i], "\n")] = 0; // Remove newline character
        i++;
    } while (strcmp(str[i - 1], "QUIT") != 0);
    
    i = 0;
    printf("\n ASSEMBLY LANGUAGE CODE: \n");
    
    while (strcmp(str[i - 1], "QUIT") != 0) {
        gen();
        printf("\n");
        i++;
    }
    
    printf("\n PRESS ENTER TO EXIT FROM CODE GENERATOR\n");
    getchar(); // Wait for user input to exit
    return 0;
}

void gen() {
    int j;
    printf("\n");
    
    for (j = strlen(str[i]) - 1; j >= 0; j--) {
        char reg = 'R';
        
        if (isdigit(str[i][j]) || isalpha(str[i][j]) || 
            str[i][j] == '+' || str[i][j] == '-' || 
            str[i][j] == '*' || str[i][j] == '/' || 
            str[i][j] == ' ' || str[i][j] == '|' || 
            str[i][j] == '&' || str[i][j] == ':' || 
            str[i][j] == '=') {
            
            switch (str[i][j]) {
                case '+':
                    printf("\n\t MOV\t%c,%c%d", str[i][j - 1], reg, count);
                    printf("\n\t ADD\t%c,%c%d", str[i][j + 1], reg, count);
                    break;
                case '-':
                    printf("\n\t MOV\t%c,%c%d", str[i][j - 1], reg, count);
                    printf("\n\t SUB\t%c,%c%d", str[i][j + 1], reg, count);
                    break;
                case '*':
                    printf("\n\t MOV\t%c,%c%d", str[i][j - 1], reg, count);
                    printf("\n\t MUL\t%c,%c%d", str[i][j + 1], reg, count);
                    break;
                case '/':
                    printf("\n\t MOV\t%c,%c%d", str[i][j - 1], reg, count);
                    printf("\n\t DIV\t%c,%c%d", str[i][j + 1], reg, count);
                    break;
                case '|':
                    printf("\n\t MOV\t%c,%c%d", str[i][j - 1], reg, count);
                    printf("\n\t OR\t%c,%c%d", str[i][j + 1], reg, count);
                    break;
                case '&':
                    printf("\n\t MOV\t%c,%c%d", str[i][j - 1], reg, count);
                    printf("\n\t AND\t%c,%c%d", str[i][j + 1], reg, count);
                    break;
                case ':':
                    if (str[i][j + 1] == '=') {
                        printf("\n\t MOV\t%c%d,%c", reg, count, str[i][j - 1]);
                        count++;
                    } else {
                        printf("\n syntax error...\n");
                    }
                    break;
                default:
                    break;
            }
        } else {
            printf("\n Error\n");
        }
    }
}

#include <stdio.h>

#include <stdlib.h>



#define MAX_LINES 5

#define MAX_LINE_LENGTH 100



int main() {

    FILE *file;

    char filename[] = "input_program.txt";

    char line[MAX_LINE_LENGTH];

    int line_count = 0;



    file = fopen(filename, "w");

    if (file == NULL) {

        printf("Error opening file!\n");

        return 1;

    }



    printf("Enter a small program (up to 5 lines):\n");

    printf("Press Enter twice to finish input.\n");



    while (line_count < MAX_LINES) {

        if (fgets(line, sizeof(line), stdin) == NULL) {

            break;

        }

        if (line[0] == '\n') {

            break;

        }

        fputs(line, file);

        line_count++;

    }



    fclose(file);

    printf("Input saved to %s\n", filename);



    return 0;

}
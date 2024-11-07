%{

#include <stdio.h>

int state = 0;

%}



%%

1   { 

    if (state == 0 || state == 1) state++;

}

0   { state = 0; }

\n  {

    if (state == 2) {

        printf("Accepted\n");

    } else {

        printf("Rejected\n");

    }

    state = 0;

}

.   { printf("Invalid input\n"); }



%%



int main() {

    yylex();

    return 0;

}
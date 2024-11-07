%{

#include<stdio.h>

#include<string.h>

%}

%s A B C D

%%

<INITIAL>0 BEGIN A;

<INITIAL>1 BEGIN C;

<INITIAL>[^01\n] BEGIN D;

<INITIAL>\n BEGIN INITIAL; printf("Not Accepted\n");



<A>0 BEGIN INITIAL;

<A>1 BEGIN B;

<A>\n BEGIN INITIAL; printf("Accepted \n");

<A>[^01\n] BEGIN D;



<B>1 BEGIN A;

<B>0 BEGIN C;

<B>\n BEGIN INITIAL; printf("Not Accepted\n");

<B>[^01\n] BEGIN D;



<C>1 BEGIN INITIAL;

<C>0 BEGIN B;

<C>\n BEGIN INITIAL; printf("Not Accepted\n");

<C>[^01\n] BEGIN D;



<D>[^\n] BEGIN D;

<D>\n BEGIN INITIAL; printf("Not Accepted\n");



%%



int main(){

  printf("Enter String\n");

  yylex();

  return 0;

}

int yywrap() {

    return 1;

}
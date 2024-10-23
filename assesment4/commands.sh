yacc -d exp1.y
lex exp1.l
gcc y.tab.c lex.yy.c -o exp1
./exp1
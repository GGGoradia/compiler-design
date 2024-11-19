#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_CHARS 100
#define MAX_RULES 10
#define MAX_TERMS 20
#define MAX_NON_TERMS 20
typedef struct {
char head;
char bodies[MAX_RULES][MAX_CHARS];
int bodyCount;
} Rule;
typedef struct {
char terminalSymbol;
char productionRule[MAX_CHARS];
} TermRule;
typedef struct {
char nonTermSymbol;
TermRule entries[MAX_TERMS];
int entryCount;
} TableEntry;
Rule grammarRules[MAX_NON_TERMS];
int ruleCount = 0;
char termSymbols[MAX_TERMS];
int termCount = 0;
char nonTermSymbols[MAX_NON_TERMS];
int nonTermCount = 0;
char firstSet[MAX_NON_TERMS][MAX_CHARS][MAX_CHARS];
int firstSetSize[MAX_NON_TERMS];
char followSet[MAX_NON_TERMS][MAX_CHARS][MAX_CHARS];
int followSetSize[MAX_NON_TERMS];
TableEntry parseTableEntries[MAX_NON_TERMS];
int parseTableEntriesCount = 0;
int isNonTerminal(char ch) {
return isupper(ch);
}
int symbolExists(char set[][MAX_CHARS], int size, char *symbol) {
for(int i = 0; i < size; i++) {
if(strcmp(set[i], symbol) == 0)
return 1;
}
return 0;
}
void insertTerminal(char ch) {
if(!isNonTerminal(ch) && ch != 'e' && ch != '$') {
for(int i = 0; i < termCount; i++) {
if(termSymbols[i] == ch)
return;
}
termSymbols[termCount++] = ch;
}
}
void insertNonTerminal(char ch) {
for(int i = 0; i < nonTermCount; i++) {
if(nonTermSymbols[i] == ch)
return;
}
nonTermSymbols[nonTermCount++] = ch;
}
int getRuleIndex(char head) {
for(int i = 0; i < ruleCount; i++) {
if(grammarRules[i].head == head)
return i;
}
return -1;
}
void addRule(char head, const char *body) {
int idx = getRuleIndex(head);
if(idx == -1) {
grammarRules[ruleCount].head = head;
strcpy(grammarRules[ruleCount].bodies[grammarRules[ruleCount].bodyCount++], body);
insertNonTerminal(head);
ruleCount++;
}
else {
strcpy(grammarRules[idx].bodies[grammarRules[idx].bodyCount++], body);
}
}
void getFirst(char symbol, char result[][MAX_CHARS], int *resSize) {
if(!isNonTerminal(symbol)) {
char temp[2] = {symbol, '\0'};
if(!symbolExists(result, *resSize, temp)) {
strcpy(result[*resSize], temp);
(*resSize)++;
}
return;
}
int idx = getRuleIndex(symbol);
if(idx == -1) return;
for(int i = 0; i < grammarRules[idx].bodyCount; i++) {
char firstChar = grammarRules[idx].bodies[i][0];
if(firstChar == 'e') {
char temp[] = "e";
if(!symbolExists(result, *resSize, temp)) {
strcpy(result[*resSize], temp);
(*resSize)++;
}
}
else if(!isNonTerminal(firstChar)) {
char temp[2] = {firstChar, '\0'};
if(!symbolExists(result, *resSize, temp)) {
strcpy(result[*resSize], temp);
(*resSize)++;
}
insertTerminal(firstChar);
}
else {
getFirst(firstChar, result, resSize);
}
}
}
void generateFirstSets() {
    for(int i = 0; i < nonTermCount; i++) {
firstSetSize[i] = 0;
getFirst(nonTermSymbols[i], firstSet[i], &firstSetSize[i]);
}
}
void getFollow(char symbol, char result[][MAX_CHARS], int *resSize) {
if(symbol == grammarRules[0].head) {
char endSym[] = "$";
if(!symbolExists(result, *resSize, endSym)) {
strcpy(result[*resSize], endSym);
(*resSize)++;
}
}
for(int i = 0; i < ruleCount; i++) {
for(int j = 0; j < grammarRules[i].bodyCount; j++) {
char *production = grammarRules[i].bodies[j];
for(int k = 0; k < strlen(production); k++) {
if(production[k] == symbol) {
if(k + 1 < strlen(production)) {
char nextChar = production[k + 1];
if(nextChar == 'e') {
getFollow(grammarRules[i].head, result, resSize);
}
else if(!isNonTerminal(nextChar)) {
char temp[2] = {nextChar, '\0'};
if(!symbolExists(result, *resSize, temp)) {
strcpy(result[*resSize], temp);
(*resSize)++;
}
}
else {
int nextIdx = getRuleIndex(nextChar);
if(nextIdx != -1) {
for(int f = 0; f < firstSetSize[nextIdx]; f++) {
if(strcmp(firstSet[nextIdx][f], "e") != 0) {
if(!symbolExists(result, *resSize, firstSet[nextIdx][f])) {
strcpy(result[*resSize], firstSet[nextIdx][f]);
(*resSize)++;
}
}
}
int hasEpsilon = 0;
for(int f = 0; f < firstSetSize[nextIdx]; f++) {
if(strcmp(firstSet[nextIdx][f], "e") == 0) {
hasEpsilon = 1;
break;
}
}
if(hasEpsilon) {
    int headIdx = getRuleIndex(grammarRules[i].head);
if(headIdx != -1) {
for(int f = 0; f < followSetSize[headIdx]; f++) {
if(!symbolExists(result, *resSize, followSet[headIdx][f])) {
strcpy(result[*resSize], followSet[headIdx][f]);
(*resSize)++;
}
}
}
}
}
}
}
else {
int headIdx = getRuleIndex(grammarRules[i].head);
if(headIdx != -1) {
for(int f = 0; f < followSetSize[headIdx]; f++) {
if(!symbolExists(result, *resSize, followSet[headIdx][f])) {
strcpy(result[*resSize], followSet[headIdx][f]);
(*resSize)++;
}
}
}
}
}
}
}
}
}
void generateFollowSets() {
for(int i = 0; i < nonTermCount; i++) {
followSetSize[i] = 0;
}
int updated = 1;
while(updated) {
updated = 0;
for(int i = 0; i < nonTermCount; i++) {
char current = nonTermSymbols[i];
char tempSet[MAX_CHARS][MAX_CHARS];
int tempSize = 0;
getFollow(current, tempSet, &tempSize);
for(int j = 0; j < tempSize; j++) {
if(!symbolExists(followSet[i], followSetSize[i], tempSet[j])) {
strcpy(followSet[i][followSetSize[i]++], tempSet[j]);
updated = 1;
}
}
}
}
}
void buildParseTable() {
for(int i = 0; i < nonTermCount; i++) {
parseTableEntries[i].nonTermSymbol = nonTermSymbols[i];
parseTableEntries[i].entryCount = 0;
}
for(int i = 0; i < ruleCount; i++) {
char head = grammarRules[i].head;
int headIdx = -1;
for(int nt = 0; nt < nonTermCount; nt++) {
if(nonTermSymbols[nt] == head) {
headIdx = nt;
break;
}
}
if(headIdx == -1) continue;
for(int p = 0; p < grammarRules[i].bodyCount; p++) {
char *prod = grammarRules[i].bodies[p];
if(prod[0] == 'e') {
for(int f = 0; f < followSetSize[headIdx]; f++) {
parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].terminalSymbol = followSet[headIdx][f][0];
strcpy(parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].productionRule, "e");
parseTableEntries[headIdx].entryCount++;
}
}
else {
char firstChar = prod[0];
if(!isNonTerminal(firstChar)) {
parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].terminalSymbol = firstChar;
strcpy(parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].productionRule, prod);
parseTableEntries[headIdx].entryCount++;
insertTerminal(firstChar);
}
else {
int firstIdx = getRuleIndex(firstChar);
if(firstIdx != -1) {
for(int f = 0; f < firstSetSize[firstIdx]; f++) {
if(strcmp(firstSet[firstIdx][f], "e") != 0) {
parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].terminalSymbol = firstSet[firstIdx][f][0];
strcpy(parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].productionRule, prod);
parseTableEntries[headIdx].entryCount++;
}
}
int hasEpsilon = 0;
for(int f = 0; f < firstSetSize[firstIdx]; f++) {
if(strcmp(firstSet[firstIdx][f], "e") == 0) {
hasEpsilon = 1;
break;
}
}
if(hasEpsilon) {
for(int f = 0; f < followSetSize[headIdx]; f++) {
parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].terminalSymbol = followSet[headIdx][f][0];
strcpy(parseTableEntries[headIdx].entries[parseTableEntries[headIdx].entryCount].productionRule, prod);
parseTableEntries[headIdx].entryCount++;
}
}
}
}
}
}
}
}
void printFirstSets() {
printf("\nFIRST Sets:\n");
for(int i = 0; i < nonTermCount; i++) {
printf("FIRST(%c) = { ", nonTermSymbols[i]);
for(int j = 0; j < firstSetSize[i]; j++) {
printf("%s ", firstSet[i][j]);
}
printf("}\n");
}
}
void printFollowSets() {
printf("\nFOLLOW Sets:\n");
for(int i = 0; i < nonTermCount; i++) {
printf("FOLLOW(%c) = { ", nonTermSymbols[i]);
for(int j = 0; j < followSetSize[i]; j++) {
printf("%s ", followSet[i][j]);
}
printf("}\n");
}
}
void printParseTable() {
printf("\nParse Table:\n");
printf("Non-Terminal\tTerminal\tProduction\n");
for(int i = 0; i < nonTermCount; i++) {
for(int j = 0; j < parseTableEntries[i].entryCount; j++) {
printf("%c\t\t%c\t\t%c->%s\n", parseTableEntries[i].nonTermSymbol, parseTableEntries[i].entries[j].terminalSymbol, parseTableEntries[i].nonTermSymbol, parseTableEntries[i].entries[j].productionRule);
}
}
}
void parseInput(char inputStr[], TableEntry table[], int totalRules) {
char stack[MAX_CHARS];
int top = 0;
stack[top++] = '$';
stack[top++] = 'S';
strcat(inputStr, "$");
int ptr = 0;
printf("\nStack\t\tInput\t\tAction\n");
while(top > 0) {
stack[top] = '\0';
if(strlen(stack) > 4){
printf("%s\t%s\t\t", stack, &inputStr[ptr]);
}
else{
printf("%s\t\t%s\t\t", stack, &inputStr[ptr]);
}
char topSymbol = stack[top-1];
char currentChar = inputStr[ptr];
if(topSymbol == currentChar) {
printf("Match\n");
top--;
ptr++;
if(topSymbol == '$' && currentChar == '$') {
printf("Parsing successful\n");
break;
}
}
else {
int found = 0;
int nonTermIdx = -1;
for(int i = 0; i < nonTermCount; i++) {
if(table[i].nonTermSymbol == topSymbol) {
nonTermIdx = i;
break;
}
}
if(nonTermIdx != -1) {
for(int j = 0; j < table[nonTermIdx].entryCount; j++) {
if(table[nonTermIdx].entries[j].terminalSymbol == currentChar) {
printf("Output %c->%s\n", topSymbol, table[nonTermIdx].entries[j].productionRule);
top--;
if(strcmp(table[nonTermIdx].entries[j].productionRule, "e") != 0) {
int prodLen = strlen(table[nonTermIdx].entries[j].productionRule);
for(int k = prodLen - 1; k >= 0; k--)
stack[top++] = table[nonTermIdx].entries[j].productionRule[k];
}
found = 1;
break;
}
}
if(!found) {
printf("Rejected\n");
break;
}
}
else {
printf("Rejected\n");
break;
}
}
}
}
int main() {
addRule('S', "TA");
addRule('A', "+TA");
addRule('A', "e");
addRule('T', "FB");
addRule('B', "*FB");
addRule('B', "e");
addRule('F', "(S)");
addRule('F', "a");
// addRule('S', "(L)");
// addRule('S', "a");
// addRule('L', "SA");
// addRule('A', ",SA");
// addRule('A', "e");
generateFirstSets();
generateFollowSets();
buildParseTable();
printFirstSets();
printFollowSets();
printParseTable();
char inputString[MAX_CHARS];
printf("\nEnter input string: ");
scanf("%s", inputString);
parseInput(inputString, parseTableEntries, ruleCount);
return 0;
}
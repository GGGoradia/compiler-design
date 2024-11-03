#ifndef THREE_ADDRESS_H
#define THREE_ADDRESS_H

struct List {
    int addr;
    struct List* next;
};
typedef struct List List;

typedef struct {
    char* addr;        
    char* code;        
    List* true_list;   
    List* false_list;  
    List* next_list;   
} Attrs;

#endif
#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "../utils/utils.h"

typedef struct StackNode{
    void* content;
    struct StackNode* previous_node;
} StackNode;

typedef struct Stack{
    size_t type_size;
    u4 max_size;
    u4 size;
    StackNode* top;
    void (*freeContent)(void*); // ponteiro para funcao que da free no conteudo
} Stack;


Stack* createStack(u4 max_size, size_t type_size, void (*freeContent)(void*));

u1 isEmpty(Stack* stack);

void* getTop(Stack* stack);

void pop(Stack* stack);

u1 push(Stack* stack, void* content);

void freeStack(Stack* stack);

#endif
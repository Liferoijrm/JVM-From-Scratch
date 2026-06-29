/**
 * @file stack.c
 * @brief Implementação das funções de manipulação e controle da pilha genérica.
 */

#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

Stack* createStack(u4 max_size, size_t type_size, void (*freeContent)(void*)){
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->type_size = type_size;
    stack->max_size = max_size;
    stack->size = 0;
    stack->top = NULL;
    stack->freeContent = freeContent;
    return stack;
}

u1 isEmpty(Stack* stack){
    return stack->size == 0;
}

void* getTop(Stack* stack){
    if(stack->top == NULL)
        return NULL;
    return stack->top->content;
}

void pop(Stack* stack){
    StackNode* top_ptr = stack->top;
    if(top_ptr == NULL) return;

    StackNode* new_ptr = top_ptr->previous_node;
    stack->top = new_ptr;
    stack->freeContent(top_ptr->content);
    free(top_ptr);
    stack->size--;
}

u1 push(Stack* stack, void* content){
    if(stack->size == stack->max_size) return 0;
    StackNode* top_ptr = stack->top;

    StackNode* new_ptr = (StackNode*) malloc(sizeof(StackNode));

    void* content_ptr = malloc(stack->type_size);
    memcpy(content_ptr, content, stack->type_size);

    new_ptr->content = content_ptr;
    new_ptr->previous_node = top_ptr;
    stack->top = new_ptr;
    stack->size++;
    return 1;
}

void freeStack(Stack* stack){
    u4 size = stack->size;
    for(u4 i = 0; i < size; i++){
        pop(stack);
    }
    free(stack);
}
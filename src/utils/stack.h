/**
 * @file stack.h
 * @brief Declarações das estruturas e funções para a implementação de uma pilha genérica.
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "utils.h"

/**
 * @struct StackNode
 * @brief Estrutura que representa um nó individual da pilha.
 */
typedef struct StackNode{
    void* content;                 /**< Ponteiro genérico para o conteúdo armazenado. */
    struct StackNode* previous_node; /**< Ponteiro para o nó anterior (abaixo na pilha). */
} StackNode;

/**
 * @struct Stack
 * @brief Estrutura principal que gerencia o estado e os dados da pilha genérica.
 */
typedef struct Stack{
    size_t type_size;              /**< Tamanho em bytes do tipo de dado armazenado. */
    u4 max_size;                   /**< Capacidade máxima de elementos da pilha. */
    u4 size;                       /**< Quantidade atual de elementos na pilha. */
    StackNode* top;                /**< Ponteiro para o nó localizado no topo da pilha. */
    void (*freeContent)(void*);    /**< Ponteiro para a função responsável por dar free no conteúdo. */
} Stack;

/**
 * @brief Cria e inicializa uma nova pilha genérica.
 * @param max_size Quantidade máxima de elementos que a pilha suportará.
 * @param type_size Tamanho em bytes do tipo a ser armazenado (ex: sizeof(int)).
 * @param freeContent Ponteiro para a função de desalocação do tipo armazenado.
 * @return Ponteiro para a nova estrutura Stack alocada.
 */
Stack* createStack(u4 max_size, size_t type_size, void (*freeContent)(void*));

/**
 * @brief Verifica se a pilha está vazia.
 * @param stack Ponteiro para a pilha que será verificada.
 * @return 1 se a pilha estiver vazia, 0 caso contrário.
 */
u1 isEmpty(Stack* stack);

/**
 * @brief Obtém o elemento que está no topo da pilha sem removê-lo.
 * @param stack Ponteiro para a pilha.
 * @return Ponteiro (void*) para o conteúdo no topo da pilha, ou NULL se estiver vazia.
 */
void* getTop(Stack* stack);

/**
 * @brief Remove o elemento do topo da pilha e libera a sua memória.
 * @param stack Ponteiro para a pilha da qual o elemento será removido.
 */
void pop(Stack* stack);

/**
 * @brief Insere um novo elemento no topo da pilha.
 * @param stack Ponteiro para a pilha onde o elemento será inserido.
 * @param content Ponteiro para o dado que será copiado para a pilha.
 * @return 1 se a inserção ocorreu com sucesso, 0 se a pilha já atingiu o tamanho máximo.
 */
u1 push(Stack* stack, void* content);

/**
 * @brief Desaloca totalmente a pilha, esvaziando seus elementos e liberando a estrutura.
 * @param stack Ponteiro para a pilha a ser destruída.
 */
void freeStack(Stack* stack);

/**
 * @example
 * // TUTORIAL DE USO:
 * * #define MAX_SIZE 1000000    // valor que caiba em u4
 * * int main(){
 * // CRIACAO: passe o tamanho maximo da stack, o sizeof(tipo) e 
 * // a funcao de free do tipo (se for primitivo, apenas passe "free" como parametro)
 * Stack* stack = createStack(MAX_SIZE, sizeof(int), free);
 * * int a = 30; // primeiro valor a ser colocado na pilha
 * * // PUSH: basta dar um cast para void* e mandar o endereço (retorna 0 se exceder max_size)
 * push(stack, (void*)&a);
 * * // GETTOP: basta dar um cast para type* e desrreferenciar com "*"
 * int b = *((int*) getTop(stack));
 * * // POP: basta dar pop na stack
 * pop(stack);
 * * // ISEMPTY: basta usar a funcao, que retorna true se estiver vazio
 * isEmpty(stack);
 * * // FREESTACK: basta dar free na stack
 * freeStack(stack);
 * * return 0;
 * }
 */

#endif
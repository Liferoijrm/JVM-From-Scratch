/**
 * @file thread_data.h
 * @brief Definições das estruturas de controle de threads e frames na JVM.
 */

#ifndef THREAD_DATA_H
#define THREAD_DATA_H

#include <stdlib.h>
#include "../../utils/utils.h"
#include "../../utils/stack.h"
#include "../../class_loader/loading/classparser.h"

/**
 * @struct JVMThread
 * @brief Representa uma thread de execução da JVM.
 */
typedef struct JVMThread{
    u4 pc;                  /**< Program Counter: armazena o endereço da instrução atual. */
    Stack* frame_stack;     /**< Pilha de quadros (frames) de métodos desta thread. */
} JVMThread;

/**
 * @struct Frame
 * @brief Representa o quadro (frame) de um método, contendo o estado local de execução.
 */
typedef struct Frame{
    u4* local_variables;        /**< Array de variáveis locais do método. */
    Stack* operand_stack;       /**< Pilha de operandos utilizada pela máquina virtual. */
    u4 return_pc;               /**< Endereço de retorno (PC) para o chamador do método. */
    Method_info* method;        /**< Informações do método sendo executado. */
    ClassFile* class_file;      /**< Referência ao arquivo de classe (para acesso ao Constant Pool). */
} Frame;

/**
 * @brief Cria e inicializa uma nova thread na JVM.
 * @param stack_max_size Tamanho máximo da pilha de frames desta thread.
 * @return Ponteiro para a estrutura JVMThread inicializada.
 */
JVMThread* createThread(u4 stack_max_size);

/**
 * @brief Cria um novo frame para um método e empilha na thread atual.
 * @param thread Ponteiro para a thread de execução.
 * @param cf Ponteiro para o ClassFile do método.
 * @param method Ponteiro para as informações do método.
 * @param return_pc Valor do Program Counter para retornar após o término do método.
 */
void pushFrame(JVMThread* thread, ClassFile* cf, Method_info* method, u4 return_pc);

/**
 * @brief Remove o frame do topo da thread e restaura o PC do chamador.
 * @param thread Ponteiro para a thread de execução.
 */
void popFrame(JVMThread* thread);

/**
 * @brief Função de callback para desalocar a memória de um Frame.
 * @param frame_ptr Ponteiro para o frame (cast para void* para ser usado pela Stack).
 */
void freeFrame(void* frame_ptr);

#endif
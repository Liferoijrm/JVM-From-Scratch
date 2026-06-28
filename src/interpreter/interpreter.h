/**
 * @file interpreter.h
 * @brief Declarações do interpretador de bytecode da JVM.
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../utils/utils.h"
#include "../runtime_data/allocation/allocation.h"
#include "../runtime_data/method_area/methodarea.h"
#include "../runtime_data/thread_data/thread_data.h"
#include "interpreter_helpers.h"

/**
 * @struct RuntimeContext
 * @brief Contexto global de execução do interpretador.
 */
typedef struct RuntimeContext{
    JVMThread *thread;          /**< Thread de execução contendo a pilha de frames. */
    MethodArea *method_area;    /**< Área de métodos com todas as classes carregadas. */
    ReferenceMap *reference_map; /**< Mapeamento de referências para objetos na heap. */
} RuntimeContext;

/**
 * @brief Inicia o loop principal de interpretação de bytecode.
 *
 * Executa as instruções bytecode do frame atual até que a thread
 * termine ou ocorra um erro.
 *
 * @param ctx Ponteiro para o contexto de execução.
 */
void interpret(RuntimeContext *ctx);

#endif
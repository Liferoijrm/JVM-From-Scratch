/**
 * @file initialization.h
 * @brief Declarações da etapa de inicialização (initialization) de classes na JVM.
 */

#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "../../runtime_data/method_area/methodarea.h"
#include "../../runtime_data/thread_data/thread_data.h"
#include "../loading/classparser.h"

/**
 * @brief Inicializa uma classe executando seu método <clinit>.
 *
 * Executa o inicializador estático da classe conforme a especificação JVM,
 * garantindo que a superclasse seja inicializada primeiro e lidando com
 * dependências circulares.
 *
 * @param method_area Ponteiro para a Method Area.
 * @param entry Ponteiro para a entrada da classe na Method Area.
 * @param thread Ponteiro para a thread de execução.
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
u1 InitializeClass(MethodArea *method_area, MethodAreaEntry *entry, JVMThread *thread);

#endif
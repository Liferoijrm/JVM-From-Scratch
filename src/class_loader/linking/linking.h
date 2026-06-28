/**
 * @file linking.h
 * @brief Declarações da etapa de ligação (linking) de classes na JVM.
 */

#ifndef LINKING_H
#define LINKING_H

#include "../../runtime_data/method_area/methodarea.h"

/**
 * @brief Executa o processo completo de linking de uma classe.
 *
 * Realiza as etapas de verificação (verification), preparação (preparation)
 * e resolução (resolution) conforme a especificação da JVM.
 *
 * @param entry Ponteiro para a entrada da classe na Method Area.
 * @param ma Ponteiro para a Method Area (usado para lookup de classes).
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
u1 LinkClass(MethodAreaEntry *entry, const MethodArea *ma);

#endif
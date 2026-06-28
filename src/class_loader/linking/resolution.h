/**
 * @file resolution.h
 * @brief Declarações da etapa de resolução de referências simbólicas na JVM.
 */

#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "../../runtime_data/method_area/methodarea.h"

/**
 * @brief Resolve todas as referências simbólicas da constant pool de uma classe.
 *
 * @param entry Ponteiro para a entrada da classe na Method Area.
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
u1 ResolveClass(MethodAreaEntry *entry);

/**
 * @brief Resolve uma referência simbólica específica no constant pool.
 *
 * @param entry Ponteiro para a entrada da classe na Method Area.
 * @param cp_index Índice no constant pool da referência a ser resolvida.
 * @param ma Ponteiro para a Method Area (para lookup de classes).
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
u1 ResolveSymbolicRef(MethodAreaEntry *entry, u2 cp_index, const MethodArea *ma);

#endif
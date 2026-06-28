/**
 * @file preparation.h
 * @brief Declarações da etapa de preparação (preparation) de classes na JVM.
 */

#ifndef PREPARATION_H
#define PREPARATION_H

#include <stddef.h>
#include <stdlib.h>
#include "../../runtime_data/method_area/methodarea.h"

/** @brief Flag de acesso ACC_STATIC para campos e métodos. */
#define ACC_STATIC 0x0008

/**
 * @brief Prepara uma classe alocando seus campos estáticos.
 *
 * Cria o vetor de campos estáticos (StaticField) para a classe e
 * os inicializa com seus valores padrão (zeros).
 *
 * @param entry Ponteiro para a entrada da classe na Method Area.
 */
void PrepareClass(MethodAreaEntry *entry);

#endif
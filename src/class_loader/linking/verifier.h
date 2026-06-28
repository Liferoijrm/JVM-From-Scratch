/**
 * @file verifier.h
 * @brief Declarações da etapa de verificação (verification) de classes na JVM.
 */

#ifndef VERIFIER_H
#define VERIFIER_H

#include "../loading/classparser.h"
#include "../../runtime_data/method_area/methodarea.h"

/**
 * @brief Executa a verificação estrutural e de bytecode de uma classe.
 *
 * Verifica metadados (superclasse, flags de acesso) e a integridade
 * do código bytecode (limites de instruções, tabela de exceções).
 *
 * @param cf Ponteiro para o ClassFile a ser verificado.
 * @param ma Ponteiro para a Method Area (para lookup de classes).
 * @return 1 se a classe é válida, 0 caso contrário.
 */
u1 VerifyClass(ClassFile *cf, const MethodArea *ma);

#endif
/**
 * @file classloader.h
 * @brief Declarações do carregador de classes da JVM.
 */

#ifndef CLASSLOADER_H
#define CLASSLOADER_H

#include "classparser.h"
#include "../linking/preparation.h"
#include "../../runtime_data/method_area/methodarea.h"

/**
 * @brief Carrega uma classe a partir do disco ou retorna do cache na Method Area.
 *
 * Realiza o parsing do arquivo .class, insere a classe na Method Area
 * e carrega recursivamente superclasses e interfaces.
 *
 * @param ma Ponteiro para a Method Area.
 * @param class_name Nome da classe a ser carregada.
 * @return Ponteiro para o ClassFile carregado, ou NULL em caso de falha.
 */
ClassFile* LoadClass(MethodArea* ma, char* class_name);

#endif
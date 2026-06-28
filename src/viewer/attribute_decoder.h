/**
 * @file attribute_decoder.h
 * @brief Declarações das funções de decodificação e exibição dos atributos de classes, campos e métodos
 */
#ifndef ATTRIBUTE_DECODER_H
#define ATTRIBUTE_DECODER_H

#include "../class_loader/loading/classparser.h"

/**
 * @brief Identifica, decodifica e imprime as informações detalhadas de um atributo (como ConstantValue, SourceFile, Code, Exceptions).
 * * @param cpool Ponteiro para o pool de constantes da classe.
 * @param attribute Ponteiro para a estrutura Attribute_info genérica que será analisada e impressa.
 */
void PrintAttributeInfo(Cp_info *cpool, Attribute_info *attribute);

#endif
/**
 * @file classviewer.h
 * @brief Declarações das funções de visualização (viewer) e decodificação de um arquivo .class.
 */

#ifndef CLASSVIEWER_H
#define CLASSVIEWER_H

#include <stdio.h>
#include <stdlib.h>
#include "../class_loader/loading/classparser.h"

/**
 * @def CLEAR
 * @brief Macro para limpar o terminal de forma multiplataforma.
 */
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear" 
#endif

/**
 * @brief Inicia o menu interativo para visualizar as seções do ClassFile.
 * @param cf Ponteiro para a estrutura ClassFile.
 */
void ViewClass(ClassFile *cf);

/**
 * @brief Converte o número da versão major do Java para uma string legível.
 * @param major Número da versão major (ex: 52 para Java 8).
 * @return String constante contendo o nome da versão.
 */
const char* GetJavaVersionText(u2 major);

/**
 * @brief Exibe as informações gerais do ClassFile (Magic, versões, flags, classes e contadores).
 * @param cf Ponteiro para a estrutura ClassFile.
 */
void PrintClassFile(ClassFile *cf);

/**
 * @brief Exibe todas as entradas do Pool de Constantes.
 * @param cpool Ponteiro para o array do pool de constantes.
 * @param count Número de entradas no pool.
 */
void PrintCpool(Cp_info *cpool, u2 count);

/**
 * @brief Exibe os campos (fields) da classe e seus atributos.
 * @param cpool Ponteiro para o pool de constantes.
 * @param fields Ponteiro para o array de fields.
 * @param count Quantidade de fields.
 */
void PrintFields(Cp_info *cpool, Field_info *fields, u2 count);

/**
 * @brief Exibe os métodos da classe e seus atributos associados.
 * @param cpool Ponteiro para o pool de constantes.
 * @param methods Ponteiro para o array de métodos.
 * @param count Quantidade de métodos.
 */
void PrintMethods(Cp_info *cpool, Method_info *methods, u2 count);

/**
 * @brief Exibe os atributos gerais da classe.
 * @param cpool Ponteiro para o pool de constantes.
 * @param attributes Ponteiro para o array de atributos.
 * @param count Quantidade de atributos.
 */
void PrintAttributes(Cp_info *cpool, Attribute_info *attributes, u2 count);

/**
 * @brief Exibe as interfaces implementadas pela classe.
 * @param cpool Ponteiro para o pool de constantes.
 * @param interfaces Ponteiro para o array de índices de interfaces.
 * @param count Quantidade de interfaces.
 */
void PrintInterfaces(Cp_info *cpool, u2 *interfaces, u2 count);

/**
 * @brief Decodifica e imprime o formato de descritores da JVM (ex: Ljava/lang/String; -> java/lang/String).
 * @param descriptor Ponteiro para os bytes do descritor.
 * @param len Tamanho em bytes do descritor.
 */
void DecodeDescriptor(u1 *descriptor, u2 len);

/**
 * @brief Decodifica uma bitmask de flags de acesso gerais em uma string legível.
 * @param bitmask Valor de 16 bits contendo as flags.
 * @return Ponteiro para a string decodificada.
 */
const char *DecodeAccessFlags(u2 bitmask);

/**
 * @brief Decodifica uma bitmask de flags de acesso específicas para métodos.
 * @param bitmask Valor de 16 bits contendo as flags.
 * @return Ponteiro para a string decodificada.
 */
const char *DecodeMethodAccessFlags(u2 bitmask);

/**
 * @brief Imprime uma string no formato Modified UTF-8 utilizado pela JVM.
 * @param bytes Ponteiro para o array de bytes.
 * @param length Tamanho do array de bytes.
 */
void printModifiedUtf8(u1 *bytes, u2 length);

/**
 * @brief Limpa o buffer de entrada do stdin para evitar loops infinitos no menu.
 */
void cleanBuffer();

#endif
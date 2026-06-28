/**
 * @file methodarea.h
 * @brief Definições da Method Area da JVM e das operações de gerenciamento de classes carregadas.
 */

#ifndef METHODAREA_H
#define METHODAREA_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../../class_loader/loading/classparser.h"

#define METHOD_AREA_OK 0
#define METHOD_AREA_NULL_POINTER 1
#define METHOD_AREA_ALLOC_ERROR 2
#define METHOD_AREA_INVALID_CLASS 3
#define METHOD_AREA_DUPLICATE_CLASS 4

/**
 * @enum ClassState
 * @brief Estados do ciclo de vida de uma classe durante o carregamento.
 */
typedef enum {
    CLASS_LOADED,       /**< Classe carregada e parseada. */
    CLASS_LINKED,       /**< Classe verificada e preparada. */
    CLASS_INITIALIZING, /**< Método <clinit> em execução. */
    CLASS_INITIALIZED,  /**< Classe completamente inicializada. */
} ClassState;

/**
 * @struct StaticField
 * @brief Representa um campo estático pertencente a uma classe.
 */
typedef struct StaticField {
    u2 field_index; /**< Índice do campo na tabela de campos do ClassFile. */
    u4 value[2];    /**< Valor armazenado (até dois slots da JVM). */
} StaticField;

/**
 * @struct MethodAreaEntry
 * @brief Representa uma classe armazenada na Method Area.
 */
typedef struct MethodAreaEntry {
    char *class_name;           /**< Nome interno da classe. */
    ClassFile *class_file;      /**< Estrutura ClassFile correspondente. */
    StaticField *static_fields; /**< Vetor de campos estáticos preparados. */
    u2 static_field_count;      /**< Quantidade de campos estáticos. */
    ClassState state;           /**< Estado atual da classe. */
    u1 *resolved_flags;         /**< Indica quais entradas da Constant Pool já foram resolvidas. */
} MethodAreaEntry;

/**
 * @struct MethodArea
 * @brief Estrutura responsável por armazenar todas as classes carregadas pela JVM.
 */
typedef struct MethodArea {
    MethodAreaEntry *entries; /**< Vetor de entradas da Method Area. */
    u2 count;                 /**< Quantidade de classes armazenadas. */
    u2 capacity;              /**< Capacidade atual do vetor. */
} MethodArea;

/**
 * @brief Cria uma nova Method Area vazia.
 *
 * @return Ponteiro para a Method Area criada ou NULL em caso de falha.
 */
MethodArea *CreateMethodArea(void);

/**
 * @brief Libera todos os recursos associados à Method Area.
 *
 * Desaloca todas as classes armazenadas, seus campos estáticos e as
 * estruturas auxiliares utilizadas pela Method Area.
 *
 * @param method_area Ponteiro para a Method Area.
 */
void DestroyMethodArea(MethodArea *method_area);

/**
 * @brief Adiciona uma nova classe à Method Area.
 *
 * A classe é inserida apenas se ainda não existir uma classe com o mesmo
 * nome interno.
 *
 * @param method_area Ponteiro para a Method Area.
 * @param class_file Ponteiro para o ClassFile da classe.
 *
 * @return Código de status da operação.
 */
u1 MethodAreaAddClass(MethodArea *method_area, ClassFile *class_file);

/**
 * @brief Prepara uma classe para execução.
 *
 * Realiza a etapa de preparação da JVM, incluindo a alocação dos campos
 * estáticos e demais estruturas necessárias antes da inicialização.
 *
 * @param method_area Ponteiro para a Method Area.
 * @param class_file Ponteiro para o ClassFile da classe.
 *
 * @return Código de status da operação.
 */
u1 MethodAreaPrepareClass(MethodArea *method_area, ClassFile *class_file);

/**
 * @brief Procura uma classe pelo nome interno.
 *
 * @param method_area Ponteiro para a Method Area.
 * @param class_name Nome interno da classe.
 *
 * @return Ponteiro para o ClassFile correspondente ou NULL caso a classe
 * não seja encontrada.
 */
ClassFile *MethodAreaFindClass(const MethodArea *method_area, const char *class_name);

/**
 * @brief Retorna o número de classes armazenadas.
 *
 * @param method_area Ponteiro para a Method Area.
 *
 * @return Quantidade de classes carregadas.
 */
u2 MethodAreaCount(const MethodArea *method_area);

/**
 * @brief Conta o número total de slots ocupados pelos campos de instância.
 *
 * A contagem considera também os campos herdados das superclasses.
 *
 * @param method_area Ponteiro para a Method Area.
 * @param class_file Classe cuja hierarquia será analisada.
 *
 * @return Número total de slots de campos de instância.
 */
u2 MethodAreaCountInstanceFields(const MethodArea *method_area, ClassFile *class_file);

/**
 * @brief Conta os campos estáticos declarados pela classe.
 *
 * @param method_area Ponteiro para a Method Area.
 * @param class_file Ponteiro para o ClassFile.
 *
 * @return Quantidade de slots destinados aos campos estáticos.
 */
u2 MethodAreaCountStaticFields(const MethodArea *method_area, ClassFile *class_file);

/**
 * @brief Obtém a entrada correspondente a uma classe na Method Area.
 *
 * @param ma Ponteiro para a Method Area.
 * @param class_name Nome interno da classe.
 *
 * @return Ponteiro para a entrada encontrada ou NULL caso ela não exista.
 */
MethodAreaEntry* MethodAreaGetEntry(MethodArea* ma, const char* class_name);

#endif
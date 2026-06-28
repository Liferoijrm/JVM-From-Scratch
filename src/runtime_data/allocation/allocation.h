/**
 * @file allocation.h
 * @brief Definições de estruturas para alocação de objetos, arrays e buffers na heap da JVM.
 */

#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>
#include <stdint.h>
#include "../method_area/methodarea.h"

/* Códigos de identificação de tipos primitivos (Array Types) */
#define JVM_ATYPE_BOOLEAN 4
#define JVM_ATYPE_CHAR    5
#define JVM_ATYPE_FLOAT   6
#define JVM_ATYPE_DOUBLE  7
#define JVM_ATYPE_BYTE    8
#define JVM_ATYPE_SHORT   9
#define JVM_ATYPE_INT     10
#define JVM_ATYPE_LONG    11
#define JVM_ATYPE_OBJECT  12

#define MAX_REF_MAP 100000

/**
 * @struct ReferenceMap
 * @brief Mapeamento de referências (cat1) para ponteiros reais na heap do sistema.
 */
typedef struct ReferenceMap {
    void** entries; /**< Array de ponteiros para os objetos/arrays na memória. */
    u4 size;        /**< Quantidade atual de referências mapeadas. */
} ReferenceMap;

/**
 * @struct JVMObject
 * @brief Representação de uma instância de classe na JVM.
 */
typedef struct JVMObject {
    ClassFile *class_ref; /**< Referência à classe que define este objeto. */
    u4 *fields;           /**< Array contendo os valores dos campos de instância. */
} JVMObject;

/**
 * @struct JVMArray
 * @brief Representação de um array na JVM.
 */
typedef struct JVMArray {
    u4 length;  /**< Número de elementos no array. */
    u1 atype;   /**< Tipo do array conforme a especificação da JVM. */
    void *data; /**< Ponteiro para os dados brutos do array. */
} JVMArray;

/**
 * @struct JVMStringBuffer
 * @brief Estrutura auxiliar para manipulação de strings (String Buffers).
 */
typedef struct JVMStringBuffer {
    char *data;       /**< Conteúdo da string. */
    size_t length;    /**< Comprimento atual. */
    size_t capacity;  /**< Capacidade máxima alocada. */
} JVMStringBuffer;

/**
 * @brief Cria e inicializa um novo objeto na heap da JVM.
 * * @param method_area Ponteiro para a área de métodos, usada para calcular os campos da instância.
 * @param class_file Ponteiro para o ClassFile que define a estrutura do objeto a ser criado.
 * @return JVMObject* Ponteiro para o objeto recém-alocado ou NULL em caso de erro.
 */
JVMObject *CreateObject(const MethodArea *method_area, ClassFile *class_file);

/**
 * @brief Cria e inicializa um novo array de tipos primitivos na heap da JVM.
 * * @param atype O tipo de dado do array (ex: JVM_ATYPE_INT).
 * @param length O número de elementos que o array deve conter.
 * @return JVMArray* Ponteiro para o array recém-alocado ou NULL caso length seja inválido ou falte memória.
 */
JVMArray *CreatePrimitiveArray(u1 atype, int32_t length);

/**
 * @brief Calcula o tamanho em bytes de um tipo de array primitivo.
 * * @param atype O código identificador do tipo primitivo da JVM.
 * @return size_t O tamanho em bytes do tipo, ou 0 se o atype não for um tipo primitivo válido.
 */
size_t ArrayTypeSize(u1 atype);

#endif
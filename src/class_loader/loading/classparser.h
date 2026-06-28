/**
 * @file classparser.h
 * @brief Parser de arquivos .class e estruturas de dados da JVM.
 *
 * Define as representações internas para todas as estruturas encontradas em um
 * arquivo .class da JVM, conforme a especificação. Fornece funções para
 * fazer o parsing do arquivo, acessar o constant pool, ler metadados
 * da classe e liberar a memória alocada.
 *
 * @note Todos os valores multibyte são lidos em big-endian conforme a especificação JVM.
 * @todo Suporte ao atributo InnerClasses.
 */

#ifndef CLASSPARSER_H
#define CLASSPARSER_H

#include <stdio.h>
#include "../../utils/utils.h"

/* ====================================================================
 * Tags do Constant Pool
 * ==================================================================== */

/** @brief Tag para entrada inválida/vazia no constant pool. */
#define CONSTANT_Invalid              0
/** @brief Tag para CONSTANT_Class_info. */
#define CONSTANT_Class                7
/** @brief Tag para CONSTANT_Fieldref_info. */
#define CONSTANT_Fieldref             9
/** @brief Tag para CONSTANT_Methodref_info. */
#define CONSTANT_Methodref           10
/** @brief Tag para CONSTANT_InterfaceMethodref_info. */
#define CONSTANT_InterfaceMethodref  11
/** @brief Tag para CONSTANT_String_info. */
#define CONSTANT_String               8
/** @brief Tag para CONSTANT_Integer_info. */
#define CONSTANT_Integer              3
/** @brief Tag para CONSTANT_Float_info. */
#define CONSTANT_Float                4
/** @brief Tag para CONSTANT_Long_info. */
#define CONSTANT_Long                 5
/** @brief Tag para CONSTANT_Double_info. */
#define CONSTANT_Double               6
/** @brief Tag para CONSTANT_NameAndType_info. */
#define CONSTANT_NameAndType         12
/** @brief Tag para CONSTANT_Utf8_info. */
#define CONSTANT_Utf8                 1
/** @brief Tag para CONSTANT_MethodHandle_info. */
#define CONSTANT_MethodHandle        15
/** @brief Tag para CONSTANT_MethodType_info. */
#define CONSTANT_MethodType          16
/** @brief Tag para CONSTANT_InvokeDynamic_info. */
#define CONSTANT_InvokeDynamic       18

/* Forward declarations para definições recursivas */
typedef struct Cp_info Cp_info;
typedef struct Field_info Field_info;
typedef struct Method_info Method_info;
typedef struct Attribute_info Attribute_info;

/**
 * @brief Representação de um arquivo .class parseado.
 *
 * Corresponde à estrutura ClassFile da especificação JVM, Seção 4.1.
 * Todos os arrays de tamanho dinâmico são alocados na heap.
 */
typedef struct ClassFile{
    u4 magic;                       /**< Número mágico (0xCAFEBABE). */
    u2 minor_version;               /**< Versão menor do formato. */
    u2 major_version;               /**< Versão principal (ex: 52 para Java 8). */
    u2 constant_pool_count;         /**< Número de entradas no constant pool (indexado a partir de 1). */
    Cp_info *constant_pool;         /**< Array do constant pool (índice 0 é reservado). */
    u2 access_flags;                /**< Modificadores de acesso da classe. */
    u2 this_class;                  /**< Índice no constant pool para o nome da classe. */
    u2 super_class;                 /**< Índice no constant pool para a superclasse (0 se for Object). */
    u2 interfaces_count;            /**< Número de interfaces implementadas. */
    u2 *interfaces;                 /**< Array de índices no constant pool das interfaces. */
    u2 fields_count;                /**< Número de declarações de campos. */
    Field_info *fields;             /**< Array de declarações de campos. */
    u2 methods_count;               /**< Número de declarações de métodos. */
    Method_info *methods;           /**< Array de declarações de métodos. */
    u2 attributes_count;            /**< Número de atributos ao nível da classe. */
    Attribute_info *attributes;     /**< Array de atributos ao nível da classe. */
} ClassFile;

/**
 * @brief Entrada do constant pool com união tipada para dados variantes.
 *
 * A entrada no índice 0 é reservada pela especificação e sempre inválida.
 * Entradas Long e Double ocupam dois índices consecutivos.
 */
typedef struct Cp_info{
    u1 tag; /**< Tag que identifica o tipo da constante (ver defines CONSTANT_*). */
    union{
        /** @brief CONSTANT_Class_info */
        struct Class{
            u2 name_index;          /**< Índice para uma constante UTF-8 com o nome da classe. */
        } Class;
        /** @brief CONSTANT_Fieldref_info */
        struct Fieldref{
            u2 class_index;         /**< Índice para um CONSTANT_Class_info. */
            u2 name_and_type_index; /**< Índice para um CONSTANT_NameAndType_info. */
        } Fieldref;
        /** @brief CONSTANT_NameAndType_info */
        struct NameAndType{
            u2 name_index;          /**< Índice para uma string UTF-8 com o nome. */
            u2 descriptor_index;    /**< Índice para uma string UTF-8 com o descritor. */
        } NameAndType;
        /** @brief CONSTANT_Methodref_info */
        struct Methodref{
            u2 class_index;         /**< Índice para um CONSTANT_Class_info. */
            u2 name_and_type_index; /**< Índice para um CONSTANT_NameAndType_info. */
        } Methodref;
        /** @brief CONSTANT_InterfaceMethodref_info */
        struct InterfaceMethodref{
            u2 class_index;         /**< Índice para um CONSTANT_Class_info. */
            u2 name_and_type_index; /**< Índice para um CONSTANT_NameAndType_info. */
        } InterfaceMethodref;
        /** @brief CONSTANT_String_info */
        struct String{
            u2 string_index;        /**< Índice para uma constante UTF-8 com o valor da string. */
        } String;
        /** @brief CONSTANT_Integer_info */
        struct Integer{
            u4 bytes;               /**< Valor inteiro em formato big-endian. */
        } Integer;
        /** @brief CONSTANT_Float_info */
        struct Float{
            u4 bytes;               /**< Bits IEEE 754 de precisão simples. */
        } Float;
        /** @brief CONSTANT_Long_info (ocupa duas entradas consecutivas) */
        struct Long{
            u4 high_bytes;          /**< 32 bits altos do long com sinal de 64 bits. */
            u4 low_bytes;           /**< 32 bits baixos do long com sinal de 64 bits. */
        } Long;
        /** @brief CONSTANT_Double_info (ocupa duas entradas consecutivas) */
        struct Double{
            u4 high_bytes;          /**< 32 bits altos do double IEEE 754. */
            u4 low_bytes;           /**< 32 bits baixos do double IEEE 754. */
        } Double;
        /** @brief CONSTANT_Utf8_info */
        struct Utf8{
            u2 length;              /**< Comprimento do array de bytes (sem terminador nulo no arquivo). */
            u1 *bytes;              /**< Bytes da string codificada em UTF-8 (com terminador nulo em memória). */
        } Utf8;
        /** @brief CONSTANT_MethodHandle_info */
        struct MethodHandle{
            u1 reference_kind;      /**< Tipo de referência (1-9). */
            u2 reference_index;     /**< Índice no constant pool conforme o reference_kind. */
        } MethodHandle;
        /** @brief CONSTANT_MethodType_info */
        struct MethodType{
            u2 descriptor_index;    /**< Índice para uma string UTF-8 com o descritor. */
        } MethodType;
        /** @brief CONSTANT_InvokeDynamic_info */
        struct InvokeDynamic{
            u2 bootstrap_method_attr_index;  /**< Índice na tabela de bootstrap methods. */
            u2 name_and_type_index;          /**< Índice para um CONSTANT_NameAndType_info. */
        } InvokeDynamic;
    } info;
} Cp_info;

/**
 * @brief Atributo SourceFile (armazena o nome do arquivo .java de origem).
 */
typedef struct SourceFile_attribute{
    u2 attribute_name_index;    /**< Índice para "SourceFile" no constant pool. */
    u4 attribute_length;        /**< Comprimento do atributo (sempre 2). */
    u2 sourcefile_index;        /**< Índice para uma constante UTF-8 com o nome do arquivo fonte. */
} SourceFile_attributes;

/**
 * @brief Cabeçalho genérico de atributo, usado antes da decodificação em tipos específicos.
 */
typedef struct Attribute_info{
    u2 attribute_name_index;    /**< Índice para o nome do atributo no constant pool. */
    u4 attribute_length;        /**< Comprimento dos dados do atributo (excluindo o cabeçalho). */
    u1 *info;                   /**< Dados brutos do atributo. */
} Attribute_info;

/**
 * @brief Atributo Exceptions (lista as exceções declaradas por um método).
 */
typedef struct Exception_attribute{
    u2 attribute_name_index;            /**< Índice para "Exceptions" no constant pool. */
    u4 attribute_length;                /**< Comprimento do atributo. */
    u2 number_of_exceptions;            /**< Número de entradas de classes de exceção. */
    u2 *exception_index_table;          /**< Array de índices no constant pool para as classes de exceção. */
} Exception_attribute;

/**
 * @brief Entrada na tabela de manipuladores de exceção do atributo Code.
 */
typedef struct Exception_code{
    u2 start_pc;    /**< Início do intervalo do código onde o handler está ativo (inclusivo). */
    u2 end_pc;      /**< Fim do intervalo do código onde o handler está ativo (exclusivo). */
    u2 handler_pc;  /**< Program counter do código do manipulador de exceção. */
    u2 catch_type;  /**< Índice no constant pool para a classe da exceção (0 = qualquer exceção). */
} Exception_code;

/**
 * @brief Atributo Code contendo instruções bytecode e metadados.
 */
typedef struct Code_attribute{
    u2 attribute_name_index;            /**< Índice para "Code" no constant pool. */
    u4 attribute_length;                /**< Comprimento do atributo. */
    u2 max_stack;                       /**< Profundidade máxima da pilha de operandos. */
    u2 max_locals;                      /**< Número de slots de variáveis locais (incluindo 'this'). */
    u4 code_length;                     /**< Comprimento do array de instruções bytecode. */
    u1 *code;                           /**< Array de instruções bytecode. */
    u2 exception_table_length;          /**< Número de entradas na tabela de exceções. */
    Exception_code *exception_table;    /**< Tabela de manipuladores de exceção. */
    u2 attributes_count;                /**< Número de sub-atributos (ex: LineNumberTable). */
    Attribute_info *attributes;         /**< Sub-atributos do atributo Code. */
} Code_attribute;

/**
 * @brief Atributo ConstantValue (valor inicial para um campo estático).
 */
typedef struct ConstantValue_attribute{
    u2 attribute_name_index;    /**< Índice para "ConstantValue" no constant pool. */
    u4 attribute_length;        /**< Comprimento do atributo (sempre 2). */
    u2 constant_value_index;    /**< Índice no constant pool para o valor inicial do campo. */
} ConstantValue_attribute;

/**
 * @brief Entrada na LineNumberTable (mapeia PCs do bytecode para números de linha do fonte).
 */
typedef struct LineNumberTable{
    u2 start_pc;    /**< PC inicial do intervalo do bytecode. */
    u2 line_number; /**< Número da linha correspondente no código fonte. */
} LineNumberTable;

/**
 * @brief Atributo LineNumberTable (para depuração).
 */
typedef struct LineNumberTable_attribute{
    u2 attribute_name_index;            /**< Índice para "LineNumberTable" no constant pool. */
    u4 attribute_length;                /**< Comprimento do atributo. */
    u2 line_number_table_length;        /**< Número de entradas na tabela. */
    LineNumberTable *line_number_table; /**< Array de mapeamentos PC-para-linha. */
} LineNumberTable_attribute;

/**
 * @brief Entrada na LocalVariableTable (mapeia variáveis locais para nomes/tipos).
 */
typedef struct LocalVariable{
    u2 start_pc;        /**< PC inicial do intervalo de escopo. */
    u2 length;          /**< Extensão do intervalo (em instruções bytecode). */
    u2 name_index;      /**< Índice para uma constante UTF-8 com o nome da variável. */
    u2 descriptor_index;/**< Índice para uma constante UTF-8 com o descritor da variável. */
    u2 index;           /**< Índice do slot da variável local. */
} LocalVariable;

/**
 * @brief Atributo LocalVariableTable (para depuração).
 */
typedef struct LocalVariableTable_attribute{
    u2 attribute_name_index;                    /**< Índice para "LocalVariableTable" no constant pool. */
    u4 attribute_length;                        /**< Comprimento do atributo. */
    u2 local_variable_table_length;             /**< Número de entradas na tabela. */
    LocalVariable *local_variable_table;        /**< Array de mapeamentos de variáveis locais. */
} LocalVariableTable_attribute;

/**
 * @brief Descritor de método incluindo flags de acesso, nome e atributos.
 */
typedef struct Method_info{
    u2 access_flags;            /**< Modificadores de acesso deste método. */
    u2 name_index;              /**< Índice para uma constante UTF-8 com o nome do método. */
    u2 descriptor_index;        /**< Índice para uma constante UTF-8 com o descritor do método. */
    u2 attributes_count;        /**< Número de atributos do método. */
    Attribute_info *attributes; /**< Array de atributos do método (Code, Exceptions, etc.). */
} Method_info;

/**
 * @brief Descritor de campo incluindo flags de acesso, nome, tipo e atributos.
 */
typedef struct Field_info{
    u2 access_flags;            /**< Modificadores de acesso deste campo. */
    u2 name_index;              /**< Índice para uma constante UTF-8 com o nome do campo. */
    u2 descriptor_index;        /**< Índice para uma constante UTF-8 com o descritor do campo. */
    u2 attributes_count;        /**< Número de atributos do campo. */
    Attribute_info *attributes; /**< Array de atributos do campo (ConstantValue, etc.). */
} Field_info;

/* ====================================================================
 * Funções de E/S Binária
 * ==================================================================== */

/**
 * @brief Lê um único byte sem sinal (u1) de um arquivo.
 * @param fd Ponteiro para um stream FILE aberto.
 * @return O byte lido (0-255), ou EOF em caso de erro.
 */
u1 u1Read(FILE *fd);

/**
 * @brief Lê um inteiro de 2 bytes sem sinal (u2) em big-endian.
 * @param fd Ponteiro para um stream FILE aberto.
 * @return O valor de 16 bits lido.
 */
u2 u2Read(FILE *fd);

/**
 * @brief Lê um inteiro de 4 bytes sem sinal (u4) em big-endian.
 * @param fd Ponteiro para um stream FILE aberto.
 * @return O valor de 32 bits lido.
 */
u4 u4Read(FILE *fd);

/* ====================================================================
 * Funções de Parsing
 * ==================================================================== */

/**
 * @brief Faz o parsing completo de um arquivo .class do disco.
 * @param class_name Caminho para o arquivo .class.
 * @return Ponteiro para um ClassFile alocado na heap, ou NULL em caso de falha.
 */
ClassFile *ParseClass(char *class_name);

/**
 * @brief Lê o constant pool de um arquivo .class.
 * @param fd Ponteiro para um stream FILE posicionado no constant pool.
 * @param size O valor constant_pool_count do cabeçalho.
 * @param cf Ponteiro para o ClassFile sendo populado.
 * @return DEFAULT (0) em sucesso, ou um código de erro em falha.
 */
u1 Read_cpool(FILE *fd, u2 size, ClassFile *cf);

/**
 * @brief Lê o array de interfaces de um arquivo .class.
 * @param fd Ponteiro para um stream FILE posicionado nas interfaces.
 * @param size O valor interfaces_count.
 * @param cf Ponteiro para o ClassFile sendo populado.
 * @return DEFAULT (0) em sucesso, ou um código de erro em falha.
 */
u1 Read_interfaces(FILE *fd, u2 size, ClassFile *cf);

/**
 * @brief Lê o array de campos de um arquivo .class.
 * @param fd Ponteiro para um stream FILE posicionado nos campos.
 * @param size O valor fields_count.
 * @param cf Ponteiro para o ClassFile sendo populado.
 * @return DEFAULT (0) em sucesso, ou um código de erro em falha.
 */
u1 Read_fields(FILE *fd, u2 size, ClassFile *cf);

/**
 * @brief Lê o array de métodos de um arquivo .class.
 * @param fd Ponteiro para um stream FILE posicionado nos métodos.
 * @param size O valor methods_count.
 * @param cf Ponteiro para o ClassFile sendo populado.
 * @return DEFAULT (0) em sucesso, ou um código de erro em falha.
 */
u1 Read_methods(FILE *fd, u2 size, ClassFile *cf);

/**
 * @brief Lê um array de estruturas Attribute_info de um arquivo .class.
 *
 * @note O array attributes já deve estar alocado antes de chamar esta função.
 * @param fd Ponteiro para um stream FILE posicionado nos atributos.
 * @param size Número de atributos a ler.
 * @param attributes Ponteiro para um array Attribute_info pré-alocado.
 * @param cf Ponteiro para o ClassFile (usado para acesso ao constant pool).
 * @return DEFAULT (0) em sucesso, ou um código de erro em falha.
 */
u1 Read_attributes(FILE *fd, u2 size, Attribute_info *attributes, ClassFile *cf);

/* ====================================================================
 * Gerenciamento de Memória
 * ==================================================================== */

/**
 * @brief Libera toda a memória associada a um ClassFile.
 * @param cf Ponteiro para o ClassFile a ser liberado (passar NULL é seguro).
 */
void FreeClass(ClassFile *cf);

/* ====================================================================
 * Funções Utilitárias
 * ==================================================================== */

/**
 * @brief Registra erros do parser em stderr com mensagens descritivas.
 * @param error_flag Código de erro retornado por uma função de parsing.
 * @param class_name Nome ou caminho do arquivo .class que causou o erro.
 */
void Log_error(u1 error_flag, char *class_name);

/**
 * @brief Decodifica flags de acesso em uma string legível.
 * @param access_flag A bitmask de flags de acesso.
 * @return String com o resumo das flags (buffer estático, não thread-safe).
 */
char *Read_flags(u2 access_flag);

/**
 * @brief Extrai o nome de uma classe do constant pool como string C.
 * @param cf Ponteiro para o ClassFile.
 * @param class_index Índice no constant pool apontando para um CONSTANT_Class_info.
 * @return Ponteiro para a string do nome da classe (dentro do constant pool, não liberar).
 */
char* GetClassName(ClassFile *cf, u2 class_index);

#endif
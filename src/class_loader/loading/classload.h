#ifndef CLASSLOAD_H
#define CLASSLOAD_H

#include <stdint.h>
#include <stdio.h>

// Tamanhos fixos
#define u1 uint8_t
#define u2 uint16_t
#define u4 uint32_t

// Tags para cp_info
#define CONSTANT_Invalid 0
#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1
// TODO: verificar a estrutura dessas tags e modificar a implementação no .c
#define CONSTANT_MethodHandle 15
#define CONSTANT_MethodType 16
#define CONSTANT_InvokeDynamic 18

// Flags de acesso
#define ACC_PUBLIC 0x0001
#define ACC_PRIVATE 0x0002
#define ACC_PROTECTED 0x0004
#define ACC_STATIC 0x0008
#define ACC_FINAL 0x0010
#define ACC_SUPER 0x0020
#define ACC_BRIDGE 0x0040
#define ACC_VARARGS 0x0080
#define ACC_NATIVE 0x0100
#define ACC_INTERFACE 0x0200
#define ACC_ABSTRACT 0x0400
#define ACC_STRICT 0x0800
#define ACC_SYNTHETIC 0x1000
#define ACC_ANNOTATION 0x2000
#define ACC_ENUM 0x4000

// forward declarations para ClassFile
typedef struct Cp_info Cp_info;
typedef struct Field_info Field_info;
typedef struct Method_info Method_info;
typedef struct Attribute_info Attribute_info;

// Formato do .class
typedef struct ClassFile{
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    //Cp_info constant_pool[constant_pool_count - 1];
    Cp_info *constant_pool;
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    //u2 interfaces[interfaces_count];
    u2 *interfaces;
    u2 fields_count;
    //Field_info fields[fields_count];
    Field_info *fields;
    u2 methods_count;
    //Method_info methods[methods_count];
    Method_info *methods;
    u2 attributes_count;
    //Attribute_info attributes[attributes_count];
    Attribute_info *attributes;
}ClassFile;

// Definição do pool de constantes
typedef struct Cp_info{
    u1 tag;
    union{
        struct Class{
            u2 name_index;
        }Class;
        struct Fieldref{
            u2 class_index;
            u2 name_and_type_index;
        }Fieldref;
        struct NameAndType{
            u2 name_index;
            u2 descriptor_index;
        }NameAndType;
        struct Methodref{
            u2 class_index;
            u2 name_and_type_index;
        }Methodref;
        struct InterfaceMethodref{
            u2 class_index;
            u2 name_and_type_index;
        }InterfaceMethodref;
        struct String{
            u2 string_index;
        }String;
        struct Integer{
            u4 bytes;
        }Integer;
        struct Float{
            u4 bytes;
        }Float;
        struct Long{
            u4 high_bytes;
            u4 low_bytes;
        }Long;
        struct Double{
            u4 high_bytes;
            u4 low_bytes;
        }Double;
        struct Utf8{
            u2 length;
            //u1 bytes[length];
            u1 *bytes;
        }Utf8;
        struct MethodHandle{
            u1 reference_kind;
            u2 reference_index;
        }MethodHandle;
        struct MethodType{
            u2 descriptor_index;
        }MethodType;
        struct InvokeDynamic{
            u2 bootstrap_method_attr_index;
            u2 name_and_type_index;
        }InvokeDynamic;
    }info;
}Cp_info;

// Definição dos tipos de atributos
typedef struct SourceFile_attribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 sourcefile_index;
}SourceFile_attributes;

typedef struct Attribute_info{
    u2 attribute_name_index;
    u4 attribute_length;
    //u1 info[attribute_length];
    // TODO: verificar melhor maneira de implementar essa parte de info
    u1 *info;
}Attribute_info;

typedef struct Exception_attribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_exceptions;
    //u2 exception_index_table[number_of_exceptions];
    u2 *exception_index_table;
}Exception_attribute;

typedef struct Exception_code{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
}Exception_code;

typedef struct Code_attribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    //u1 code[code_length];
    u1 *code;
    u2 exception_table_length;
    //Exception_code exception_table[exception_table_length];
    Exception_code *exception_table;
    u2 attributes_count;
    //Attribute_info attributes[attributes_count];
    Attribute_info *attributes;
}Code_attribute;

typedef struct ConstantValue_attribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 constant_value_index;
}ConstantValue_attribute;

typedef struct LineNumberTable{
    u2 start_pc;
    u2 line_number;
}LineNumberTable;

typedef struct LineNumberTable_attribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    //LineNumberTable line_number_table[line_number_table_length];
    LineNumberTable *line_number_table;
}LineNumberTable_attribute;

typedef struct LocalVariable{
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
}LocalVariable;

typedef struct LocalVariableTable_attribute{
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_table_length;
    //LocalVariable local_variable_table[local_variable_table_length];
    LocalVariable *local_variable_table;
}LocalVariableTable_attribute;

// Falta InnerClass

// Definição de métodos
typedef struct Method_info{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    //Attribute_info attributes[attributes_count];
    Attribute_info *attributes;
}Method_info;

// Definição de fields
typedef struct Field_info{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    //Attribute_info attributes[attributes_count];
    Attribute_info *attributes;
}Field_info;

// Métodos de leitura de arquivo para cada tamanho definido
u1 u1Read(FILE *fd);

u2 u2Read(FILE *fd);

u4 u4Read(FILE *fd);

// Leitura do .class
ClassFile *OpenClass(FILE *fd);

// leitura do pool de constantes
void Read_cpool(FILE *fd, u2 size, ClassFile *cf);

// leitura do array de interfaces
void Read_interfaces(FILE *fd, u2 size, ClassFile *cf);

// leitura do array de fields
void Read_fields(FILE *fd, u2 size, ClassFile *cf);

// leitura do array de métodos
void Read_methods(FILE *fd, u2 size, ClassFile *cf);

// leitura do array de atributos
void Read_attributes(FILE *fd, u2 size, Attribute_info *attributes, ClassFile *cf);

// interpretação dos access_flags
char* Read_flags(u2 access_flag);

#endif
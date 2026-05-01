#include "classparser.h"
#include <stdio.h>
#include <stdlib.h>

// flags para log de erros
#define DEFAULT 0
#define FOPEN_ERROR 1
#define CLASS_MALLOC_ERROR 2
#define MAGIC_ERROR 3
#define VERSION_ERROR 4
#define CPOOL_MALLOC_ERROR 5
#define INTERFACE_MALLOC_ERROR 6
#define FIELD_MALLOC_ERROR 7
#define METHOD_MALLOC_ERROR 8
#define FIELD_ATTRIBUTE_ERROR 9
#define METHOD_ATTRIBUTE_ERROR 10
#define ATTRIBUTE_MALLOC_ERROR 11
#define ATTRIBUTE_INFO_MALLOC_ERROR 12

u1 u1Read(FILE *fd){
    u1 value = getc(fd);
    return value;
}

u2 u2Read(FILE *fd){
    u2 value = getc(fd);
    value = (value << 8) | (getc(fd));
    return value;
}

u4 u4Read(FILE *fd){
    u4 value = getc(fd);
    value = (value << 8) | (getc(fd));
    value = (value << 8) | (getc(fd));
    value = (value << 8) | (getc(fd));
    return value;
}

// retorna struct ClassFile preenchida a partir de um .class dado por class_name
// retorna NULL em caso de erro
ClassFile *ParseClass(char *class_name){
    u1 error_flag = DEFAULT;

    FILE *fd = fopen(class_name, "rb");
    if (!fd){
        error_flag = FOPEN_ERROR;
        goto error;
    }

    // uso de calloc para inicializar os campos zerados (0 ou NULL)
    ClassFile *cf = (ClassFile *) calloc(1, sizeof(ClassFile));
    if (!cf){
        error_flag = CLASS_MALLOC_ERROR;
        goto error;
    }

    cf->magic = u4Read(fd);
    if (cf->magic != 0xCAFEBABE){
        error_flag = MAGIC_ERROR;
        goto error;
    }

    cf->minor_version = u2Read(fd);
    cf->major_version = u2Read(fd);
    if (cf->major_version != 46 && cf->major_version != 52){ // Java 1.2 ou Java 8 (TODO: ver com o Ladeira para quais versões tem que permitir)
        error_flag = VERSION_ERROR;
        goto error;
    }

    cf->constant_pool_count = u2Read(fd);
    error_flag = Read_cpool(fd, cf->constant_pool_count, cf);
    if(error_flag != DEFAULT) goto error; // TODO: verificar se cpool está correto sintaticamente (Read_cpool retorna o erro detectado)

    cf->access_flags = u2Read(fd); // TODO: verificar se flags são válidas e não conflitam (bitmask não pode ter final e abstract junto, por exemplo)
    cf->this_class = u2Read(fd); // TODO: verificar se é indice válido para o cpool, se aponta para CLass_info
    cf->super_class = u2Read(fd); // TODO: verificar se é indice válido para o cpool, se aponta para CLass_info

    cf->interfaces_count = u2Read(fd);
    error_flag = Read_interfaces(fd, cf->interfaces_count, cf);
    if(error_flag != DEFAULT) goto error; // TODO: verificar se está correto sintaticamente (Read_interfaces retorna o erro detectado)

    cf->fields_count = u2Read(fd);
    error_flag = Read_fields(fd, cf->fields_count, cf);
    if(error_flag != DEFAULT) goto error; // TODO: verificar se está correto sintaticamente (Read_fields retorna o erro detectado)

    cf->methods_count = u2Read(fd);
    error_flag = Read_methods(fd, cf->methods_count, cf);
    if(error_flag != DEFAULT) goto error; // TODO: verificar se está correto sintaticamente (Read_methods retorna o erro detectado)

    u2 count = u2Read(fd);
    cf->attributes_count = count;
    cf->attributes = (Attribute_info*) malloc(count*sizeof(Attribute_info));
    if(!cf->attributes){
        error_flag = ATTRIBUTE_MALLOC_ERROR;
        goto error;
    } 
    error_flag = Read_attributes(fd, count, cf->attributes, cf);
    if(error_flag != DEFAULT) goto error; // TODO: verificar se está correto sintaticamente (Read_attributes retorna o erro detectado)

    fclose(fd);

    return cf;

    error:
        if (fd) fclose(fd);
        Log_error(error_flag, class_name);
        FreeClass(cf);
        return NULL;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_cpool(FILE *fd, u2 size, ClassFile *cf){
    cf->constant_pool = (Cp_info*) malloc(size*sizeof(Cp_info));
    if (!cf->constant_pool) return CPOOL_MALLOC_ERROR;

    cf->constant_pool[0].tag = 0;

    for(u2 i = 1; i < size; i++){

        u1 tag = u1Read(fd);
        cf->constant_pool[i].tag = tag;

        switch(tag){
            case CONSTANT_Class:
                cf->constant_pool[i].info.Class.name_index = u2Read(fd);
                break;
            case CONSTANT_Fieldref:
                cf->constant_pool[i].info.Fieldref.class_index = u2Read(fd);
                cf->constant_pool[i].info.Fieldref.name_and_type_index = u2Read(fd);
                break;
            case CONSTANT_Methodref:
                cf->constant_pool[i].info.Methodref.class_index = u2Read(fd);
                cf->constant_pool[i].info.Methodref.name_and_type_index = u2Read(fd);
                break;
            case CONSTANT_InterfaceMethodref:
                cf->constant_pool[i].info.InterfaceMethodref.class_index = u2Read(fd);
                cf->constant_pool[i].info.InterfaceMethodref.name_and_type_index = u2Read(fd);
                break;
            case CONSTANT_String:
                cf->constant_pool[i].info.String.string_index = u2Read(fd);
                break;
            case CONSTANT_Integer:
                cf->constant_pool[i].info.Integer.bytes = u4Read(fd);
                break;
            case CONSTANT_Float:
                cf->constant_pool[i].info.Float.bytes = u4Read(fd);
                break;
            case CONSTANT_Long:
                // big-endian
                cf->constant_pool[i].info.Long.high_bytes = u4Read(fd);
                cf->constant_pool[i].info.Long.low_bytes = u4Read(fd);
                i++;
                cf->constant_pool[i].tag = 0; // marca como inválido
                break;
            case CONSTANT_Double:
                // big-endian
                cf->constant_pool[i].info.Double.high_bytes = u4Read(fd);
                cf->constant_pool[i].info.Double.low_bytes = u4Read(fd);
                i++;
                cf->constant_pool[i].tag = 0; // marca como inválido
                break;
            case CONSTANT_NameAndType:
                cf->constant_pool[i].info.NameAndType.name_index = u2Read(fd);
                cf->constant_pool[i].info.NameAndType.descriptor_index = u2Read(fd);
                break;
            case CONSTANT_Utf8:
                ; // resolve "error: a label can only be part of a statement and a declaration is not a statement"
                u2 length = u2Read(fd);
                cf->constant_pool[i].info.Utf8.length = length;
                cf->constant_pool[i].info.Utf8.bytes = (u1*) malloc(length*sizeof(u1));
                for(u2 j = 0; j < length; j++)
                    cf->constant_pool[i].info.Utf8.bytes[j] = u1Read(fd);
                break;
            case CONSTANT_MethodHandle:
                cf->constant_pool[i].info.MethodHandle.reference_kind = u1Read(fd);
                cf->constant_pool[i].info.MethodHandle.reference_index = u2Read(fd);
                break;
            case CONSTANT_MethodType:
                cf->constant_pool[i].info.MethodType.descriptor_index = u2Read(fd);
                break;
            case CONSTANT_InvokeDynamic: 
                cf->constant_pool[i].info.InvokeDynamic.bootstrap_method_attr_index = u2Read(fd);
                cf->constant_pool[i].info.InvokeDynamic.name_and_type_index = u2Read(fd);
                break;
            default : 
                // por enquanto, ignora em silêncio (TODO: avaliar se faz sentido abortar execução ao não identificar tag)
                break;
        }
    }
    return 0;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_interfaces(FILE *fd, u2 size, ClassFile *cf){
    cf->interfaces = (u2*) malloc(size*sizeof(u2));

    if(!cf->interfaces) return INTERFACE_MALLOC_ERROR;

    for(u2 i = 0; i < size; i++)
        cf->interfaces[i] = u2Read(fd);

    return 0;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_fields(FILE *fd, u2 size, ClassFile *cf){
    cf->fields = (Field_info*) malloc(size*sizeof(Field_info));

    if(!cf->fields) return FIELD_MALLOC_ERROR; 

    for(u2 i = 0; i < size; i++){
        cf->fields[i].access_flags = u2Read(fd);
        cf->fields[i].name_index = u2Read(fd);
        cf->fields[i].descriptor_index = u2Read(fd);
        u2 count = u2Read(fd);
        cf->fields[i].attributes_count = count;
        cf->fields[i].attributes = (Attribute_info*) malloc(count*sizeof(Attribute_info));
        if(!cf->fields[i].attributes) return FIELD_ATTRIBUTE_ERROR;
        if(Read_attributes(fd, cf->fields[i].attributes_count, cf->fields[i].attributes, cf))
            return ATTRIBUTE_INFO_MALLOC_ERROR;
    }
    return 0;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_methods(FILE *fd, u2 size, ClassFile *cf){
    cf->methods = (Method_info*) malloc(size*sizeof(Method_info));

    if(!cf->methods) return METHOD_MALLOC_ERROR;

    for(u2 i = 0; i < size; i++){
        cf->methods[i].access_flags = u2Read(fd);
        cf->methods[i].name_index = u2Read(fd);
        cf->methods[i].descriptor_index = u2Read(fd);
        u2 count = u2Read(fd);
        cf->methods[i].attributes_count = count;
        cf->methods[i].attributes = (Attribute_info*) malloc(count*sizeof(Attribute_info));
        if(!cf->methods[i].attributes) return METHOD_ATTRIBUTE_ERROR;
        if(Read_attributes(fd, cf->methods[i].attributes_count, cf->methods[i].attributes, cf))
            return ATTRIBUTE_INFO_MALLOC_ERROR;
    }
    return 0;
}

// recebe ponteiro attributes já inicializado com malloc. Formato diferente para evitar 3 funções "Read_attributes"
// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_attributes(FILE *fd, u2 size, Attribute_info *attributes, ClassFile *cf){
    for(u2 i = 0; i < size; i++){
        attributes[i].attribute_name_index = u2Read(fd);
        attributes[i].attribute_length = u4Read(fd);
        attributes[i].info = NULL;

        if(attributes[i].attribute_length == 0)
            continue;

        attributes[i].info = (u1*) malloc(attributes[i].attribute_length * sizeof(u1));
        
        if(!attributes[i].info) return ATTRIBUTE_INFO_MALLOC_ERROR;

        for(u4 j = 0; j < attributes[i].attribute_length; j++)
            attributes[i].info[j] = u1Read(fd);
    }
    return 0;
}

void FreeClass(ClassFile *cf){
    // implementar aqui a liberação do cf
    // todos os campos não lidos já estão inicializados com 0 ou NULL (calloc usado)
}

// TODO: Read-flags não é um switch-case estático, é uma bitmask
// tem que decodificar a bitmask e fazer o retorno
char* Read_flags(u2 access_flag){
    switch(access_flag){
        case ACC_PUBLIC:
            return "Public";
            break;
        case ACC_PRIVATE:
            return "Private";
            break;
        case ACC_PROTECTED:
            return "Protected";
            break; 
        case ACC_STATIC:
            return "Static";
            break;
        case ACC_FINAL:
            return "Final";
            break;
        case ACC_SUPER:
            return "Super";
            break;
        case ACC_BRIDGE:
            return "Bridge";
            break;
        case ACC_VARARGS:
            return "Variable_args";
            break;
        case ACC_NATIVE:
            return "Native";
            break;
        case ACC_INTERFACE: 
            return "interface";
            break;
        case ACC_ABSTRACT:
            return "Abstract";
            break;
        case ACC_STRICT: 
            return "Strict";
            break;
        case ACC_SYNTHETIC: 
            return "Synthetic";
            break;
        case ACC_ANNOTATION: 
            return "Annotation";
            break;
        case ACC_ENUM: 
            return "Enum";
            break;
        default : 
            return "invalid_flag";
            break;
    }
}

// usa fprintf para gerar a saída no arquivo stderr em vez de stdout
void Log_error(u1 error_flag, char *class_name){
    fprintf(stderr, "\n[CLASS PARSER ERROR]\n");

    if (class_name != NULL) {
        fprintf(stderr, "Class name: %s\n", class_name);
    }

    switch (error_flag) {
        case FOPEN_ERROR:
            fprintf(stderr, "Failed to open .class file.\n");
            break;

        case CLASS_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for ClassFile structure.\n");
            break;

        case MAGIC_ERROR:
            fprintf(stderr, "Invalid magic number. Not a valid .class file.\n");
            break;

        case VERSION_ERROR:
            fprintf(stderr, "Unsupported or invalid class file version (not Java 8).\n");
            break;

        case CPOOL_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for constant pool.\n");
            break;

        case INTERFACE_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for interfaces.\n");
            break;

        case FIELD_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for fields.\n");
            break;

        case METHOD_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for methods.\n");
            break;

        case FIELD_ATTRIBUTE_ERROR:
            fprintf(stderr, "Failed to allocate memory for field attributes.\n");
            break;

        case METHOD_ATTRIBUTE_ERROR:
            fprintf(stderr, "Failed to allocate memory for method attributes.\n");
            break;

        case ATTRIBUTE_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for class attributes.\n");
            break;

        case ATTRIBUTE_INFO_MALLOC_ERROR:
            fprintf(stderr, "Failed to allocate memory for attributes info.\n");
            break;

        default:
            fprintf(stderr, "Unknown error.\n");
            break;
    }

    fprintf(stderr, "Aborting execution.\n\n");
}
#include "classparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
#define INVALID_CLASS_INDEX_ERROR 13
#define INVALID_FLAG_ERROR 14 // TODO: depois tem que tirar esse erro e as partes do código que usam ele
#define INVALID_CLASS_TAG_ERROR 15

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
    /*
    tive que tirar essa verificacao por causa do Object.class
    if (cf->major_version != 46 && cf->major_version != 52){ // Java 1.2 ou Java 8 (TODO: ver com o Ladeira para quais versões tem que permitir)
        error_flag = VERSION_ERROR;
        goto error;
    }
    */

    cf->constant_pool_count = u2Read(fd);
    error_flag = Read_cpool(fd, cf->constant_pool_count, cf);
    if(error_flag != DEFAULT) goto error; // TODO: verificar se cpool está correto sintaticamente (Read_cpool retorna o erro detectado)

    cf->access_flags = u2Read(fd);
    cf->this_class = u2Read(fd);
    cf->super_class = u2Read(fd); 
    if (cf->this_class == 0){
        error_flag = INVALID_CLASS_INDEX_ERROR;
        goto error;
    }
    if (cf->super_class != 0){
        if (cf->super_class >= cf->constant_pool_count || cf->constant_pool[cf->super_class].tag == 0) {
            error_flag = INVALID_CLASS_INDEX_ERROR;
            goto error;
        }
    }
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

    if(count > 0){
        cf->attributes = (Attribute_info*) calloc(count, sizeof(Attribute_info));
        if(!cf->attributes){
            error_flag = ATTRIBUTE_MALLOC_ERROR;
            goto error;
        } 
        error_flag = Read_attributes(fd, count, cf->attributes, cf);
        if(error_flag != DEFAULT) goto error; // TODO: verificar se está correto sintaticamente (Read_attributes retorna o erro detectado)
    }
    else cf->attributes = NULL;

    fclose(fd);

    return cf;

    error:
        if(fd) fclose(fd);
        Log_error(error_flag, class_name);
        FreeClass(cf);
        return NULL;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_cpool(FILE *fd, u2 size, ClassFile *cf){
    cf->constant_pool = (Cp_info*) calloc(size, sizeof(Cp_info));
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
                if(i < size) cf->constant_pool[i].tag = 0; // marca como inválido
                break;
            case CONSTANT_Double:
                // big-endian
                cf->constant_pool[i].info.Double.high_bytes = u4Read(fd);
                cf->constant_pool[i].info.Double.low_bytes = u4Read(fd);
                i++;
                if(i < size) cf->constant_pool[i].tag = 0; // marca como inválido
                break;
            case CONSTANT_NameAndType:
                cf->constant_pool[i].info.NameAndType.name_index = u2Read(fd);
                cf->constant_pool[i].info.NameAndType.descriptor_index = u2Read(fd);
                break;
            case CONSTANT_Utf8:
                ; // resolve "error: a label can only be part of a statement and a declaration is not a statement"
                u2 length = u2Read(fd);
                cf->constant_pool[i].info.Utf8.length = length;
                cf->constant_pool[i].info.Utf8.bytes = (u1*) malloc((length + 1)*sizeof(u1));
                for(u2 j = 0; j < length; j++)
                    cf->constant_pool[i].info.Utf8.bytes[j] = u1Read(fd);
                cf->constant_pool[i].info.Utf8.bytes[length] = '\0'; // adicionar terminador de string
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
                return INVALID_CLASS_TAG_ERROR;
                break;
        }
    }
    return 0;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_interfaces(FILE *fd, u2 size, ClassFile *cf){
    if(!size){
        cf->interfaces = NULL;
        return 0;
    }

    cf->interfaces = (u2*) malloc(size*sizeof(u2));

    if(!cf->interfaces) return INTERFACE_MALLOC_ERROR;

    for(u2 i = 0; i < size; i++)
        cf->interfaces[i] = u2Read(fd);

    return 0;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_fields(FILE *fd, u2 size, ClassFile *cf){
    if(!size){
        cf->fields = NULL;
        return 0;
    }

    cf->fields = (Field_info*) calloc(size, sizeof(Field_info));

    if(!cf->fields) return FIELD_MALLOC_ERROR; 

    for(u2 i = 0; i < size; i++){
        cf->fields[i].access_flags = u2Read(fd);
        cf->fields[i].name_index = u2Read(fd);
        cf->fields[i].descriptor_index = u2Read(fd);
        
        u2 count = u2Read(fd);
        cf->fields[i].attributes_count = count;
        
        if(count > 0){
            cf->fields[i].attributes = (Attribute_info*) calloc(count, sizeof(Attribute_info));
            if(!cf->fields[i].attributes) return FIELD_ATTRIBUTE_ERROR;
            if(Read_attributes(fd, cf->fields[i].attributes_count, cf->fields[i].attributes, cf)) return ATTRIBUTE_INFO_MALLOC_ERROR;
        } 
        else cf->fields[i].attributes = NULL;
    }
    return 0;
}

// retorno: função retorna DEFAULT (0) ou o erro que detectou
u1 Read_methods(FILE *fd, u2 size, ClassFile *cf){
    if(!size){
        cf->methods = NULL;
        return 0;
    }

    cf->methods = (Method_info*) calloc(size, sizeof(Method_info));

    if(!cf->methods) return METHOD_MALLOC_ERROR;

    for(u2 i = 0; i < size; i++){
        cf->methods[i].access_flags = u2Read(fd);
        cf->methods[i].name_index = u2Read(fd);
        cf->methods[i].descriptor_index = u2Read(fd);
        
        u2 count = u2Read(fd);
        cf->methods[i].attributes_count = count;
        
        if(count > 0){
            cf->methods[i].attributes = (Attribute_info*) calloc(count, sizeof(Attribute_info));
            if(!cf->methods[i].attributes) return METHOD_ATTRIBUTE_ERROR;
            if(Read_attributes(fd, cf->methods[i].attributes_count, cf->methods[i].attributes, cf)) return ATTRIBUTE_INFO_MALLOC_ERROR;
        } 
        else cf->methods[i].attributes = NULL;
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
    if(!cf) return;

    if(cf->constant_pool){
        for(u2 i = 1; i < cf->constant_pool_count; i++){
            if(cf->constant_pool[i].tag == CONSTANT_Utf8 && cf->constant_pool[i].info.Utf8.bytes) free(cf->constant_pool[i].info.Utf8.bytes);
        }
        free(cf->constant_pool);
    }

    if(cf->interfaces){
        free(cf->interfaces);
    }

    if(cf->fields){
        for(u2 i = 0; i < cf->fields_count; i++){
            if(cf->fields[i].attributes){
                for(u2 j = 0; j < cf->fields[i].attributes_count; j++){
                    if(cf->fields[i].attributes[j].info) free(cf->fields[i].attributes[j].info);
                }
                free(cf->fields[i].attributes);
            }
        }
        free(cf->fields);
    }

    if(cf->methods){
        for(u2 i = 0; i < cf->methods_count; i++){
            if(cf->methods[i].attributes){
                for(u2 j = 0; j < cf->methods[i].attributes_count; j++){
                    if(cf->methods[i].attributes[j].info) free(cf->methods[i].attributes[j].info);
                }
                free(cf->methods[i].attributes);
            }
        }
        free(cf->methods);
    }

    if(cf->attributes){
        for(u2 i = 0; i < cf->attributes_count; i++){
            if(cf->attributes[i].info) free(cf->attributes[i].info);
        }
        free(cf->attributes);
    }

    free(cf);
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

        case INVALID_CLASS_INDEX_ERROR:
            fprintf(stderr, "Invalid class index in constant pool (this_class or super_class).\n");
            break;
        
        case INVALID_FLAG_ERROR:
            fprintf(stderr, "Invalid access flag value.\n");
            break;

        case INVALID_CLASS_TAG_ERROR:
            fprintf(stderr, "Invalid tag in constant pool.\n");
             break;

        default:
            fprintf(stderr, "Unknown error.\n");
            break;
    }

    fprintf(stderr, "Aborting execution.\n\n");
}

char* GetClassName(ClassFile *cf, u2 class_index) {
    // Validação de segurança básica (ausência de superclasse)
    if (!cf || class_index == 0) {
        return NULL; 
    }

    u2 name_index = cf->constant_pool[class_index].info.Class.name_index;

    char *class_name = (char*) cf->constant_pool[name_index].info.Utf8.bytes;

    return class_name;
}
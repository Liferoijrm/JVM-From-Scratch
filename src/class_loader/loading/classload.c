#include "classload.h"
#include <stdio.h>
#include <stdlib.h>

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

ClassFile *OpenClass(FILE *fd){
    ClassFile *cf = (ClassFile *) malloc(sizeof(ClassFile));

    if (!cf) return NULL;

    cf->magic = u4Read(fd);
    cf->minor_version = u2Read(fd);
    cf->major_version = u2Read(fd);
    cf->constant_pool_count = u2Read(fd);
    Read_cpool(fd, cf->constant_pool_count, cf);
    cf->access_flags = u2Read(fd);
    cf->this_class = u2Read(fd);
    cf->super_class = u2Read(fd);
    cf->interfaces_count = u2Read(fd);
    Read_interfaces(fd, cf->interfaces_count, cf);
    cf->fields_count = u2Read(fd);
    Read_fields(fd, cf->fields_count, cf);
    cf->methods_count = u2Read(fd);
    Read_methods(fd, cf->methods_count, cf);

    u2 count = u2Read(fd);
    cf->attributes_count = count;
    cf->attributes = (Attribute_info*) malloc(count*sizeof(Attribute_info));
    Read_attributes(fd, count, cf->attributes, cf);

    return cf;
}

void Read_cpool(FILE *fd, u2 size, ClassFile *cf){
    cf->constant_pool = (Cp_info*) malloc(size*sizeof(Cp_info));
    if (!cf->constant_pool) return; // TODO: tratar mallocs melhor

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
                for(u1 j = 0; j < length; j++)
                    cf->constant_pool[i].info.Utf8.bytes[j] = u1Read(fd);
                break;
            case CONSTANT_MethodHandle:
                // por enquanto, ignora em silêncio
                break;
            case CONSTANT_MethodType:
                // por enquanto, ignora em silêncio
                break;
            case CONSTANT_InvokeDynamic: 
                // por enquanto, ignora em silêncio
                break;
            default : 
                // por enquanto, ignora em silêncio
                break;
        }
    }
}

void Read_interfaces(FILE *fd, u2 size, ClassFile *cf){
    cf->interfaces = (u2*) malloc(size*sizeof(u2));

    if(!cf->interfaces) return; // TODO: tratar mallocs melhor

    for(u2 i = 0; i < size; i++)
        cf->interfaces[i] = u2Read(fd);
}

void Read_fields(FILE *fd, u2 size, ClassFile *cf){
    cf->fields = (Field_info*) malloc(size*sizeof(Field_info));

    if(!cf->fields) return; // TODO: tratar mallocs melhor

    for(u2 i = 0; i < size; i++){
        cf->fields[i].access_flags = u2Read(fd);
        cf->fields[i].name_index = u2Read(fd);
        cf->fields[i].descriptor_index = u2Read(fd);
        u2 count = u2Read(fd);
        cf->fields[i].attributes_count = count;
        cf->fields[i].attributes = (Attribute_info*) malloc(count*sizeof(Attribute_info));
        Read_attributes(fd, cf->fields[i].attributes_count, cf->fields[i].attributes, cf);
    }
}

void Read_methods(FILE *fd, u2 size, ClassFile *cf){
    cf->methods = (Method_info*) malloc(size*sizeof(Method_info));

    if(!cf->methods) return; // TODO: tratar mallocs melhor

    for(u2 i = 0; i < size; i++){
        cf->methods[i].access_flags = u2Read(fd);
        cf->methods[i].name_index = u2Read(fd);
        cf->methods[i].descriptor_index = u2Read(fd);
        u2 count = u2Read(fd);
        cf->methods[i].attributes_count = count;
        cf->methods[i].attributes = (Attribute_info*) malloc(count*sizeof(Attribute_info));
        Read_attributes(fd, cf->methods[i].attributes_count, cf->methods[i].attributes, cf);
    }
}

// recebe ponteiro attributes já inicializado com malloc. Formato diferente para evitar 3 funções "Read_attributes"
void Read_attributes(FILE *fd, u2 size, Attribute_info *attributes, ClassFile *cf){
    //TODO: decodificar os fields do attributes 
    for(u2 i = 0; i < size; i++){
        attributes[i].attribute_name_index = u2Read(fd);
        attributes[i].attribute_length = u4Read(fd);
        attributes[i].info = NULL;

        if(attributes[i].attribute_length == 0)
            continue;

        attributes[i].info = (u1*) malloc(attributes[i].attribute_length * sizeof(u1));
        
        //if(!attributes[i].info) // TODO: tratar mallocs melhor

        for(u4 j = 0; j < attributes[i].attribute_length; j++)
            attributes[i].info[j] = u1Read(fd);
    }
}

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
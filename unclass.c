#include "unclass.h"

static u1 u1Read(FILE *fd){
    u1 value = getc(fd);
    return value;
}

static u2 u2Read(FILE *fd){
    u2 value = getc(fd);
    value = (value << 8) | (getc(fd));
    return value;
}

static u4 u4Read(FILE *fd){
    u4 value = getc(fd);
    value = (value << 8) | (getc(fd));
    value = (value << 8) | (getc(fd));
    value = (value << 8) | (getc(fd));
    return value;
}

static ClassFile *OpenClass(FILE *fd){
    ClassFile *cf = (ClassFile *) malloc(sizeof(Class));

    if (!cf)    return NULL;

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
    cf->methods = u2Read(fd);
    Read_methods(fd, cf->methods, cf);
    cf->attributes_count = u2Read(fd);
    Read_attributes(fd, cf->attributes_count, cf);

    return cf;
}

//static void Read_cpool(File *fd, u2 constant_pool_count, ClassFile *cf);

//static void Read_interfaces(File *fd, u2 constant_pool_count, ClassFile *cf);

//static void Read_fields(File *fd, u2 constant_pool_count, ClassFile *cf);

//static void Read_methods(File *fd, u2 constant_pool_count, ClassFile *cf);

//static void Read_attributes(File *fd, u2 constant_pool_count, ClassFile *cf);


static char* Read_flags(u2 access_flag){
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
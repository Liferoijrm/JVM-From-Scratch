#include "classloader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// forward declaration para LoadClass
char* BuildClassPath(const char* base_dir, const char* class_name);

ClassFile* LoadClass(MethodArea *ma, char *class_name) {
    if (ma == NULL || class_name == NULL) {
        return NULL;
    }

    // Verifica se a classe já está carregada na Method Area
    ClassFile *already_loaded = MethodAreaFindClass(ma, class_name);
    if (already_loaded != NULL) {
        return already_loaded; // Retorna imediatamente se já existir em memória
    }

    char file_path[256];
    snprintf(file_path, sizeof(file_path), "Examples/%s.class", class_name);

    ClassFile *cf = ParseClass(file_path);

    if (cf == NULL) {
        // O log de erro específico (FOPEN_ERROR, MAGIC_ERROR, etc.) já foi emitido pelo classparser.c
        fprintf(stderr, "Erro de Linkage: Falha ao carregar a classe %s\n", class_name);
        return NULL;
    }

    // Injeta a classe validada na Method Area global
    u1 status = MethodAreaAddClass(ma, cf);
    if (status != METHOD_AREA_OK) {
        switch (status) {
            case METHOD_AREA_NULL_POINTER:
                fprintf(stderr, "InternalError: Ponteiro nulo (Method Area ou ClassFile) ao tentar adicionar a classe %s.\n", class_name);
                break;
                
            case METHOD_AREA_INVALID_CLASS:
                fprintf(stderr, "ClassFormatError: Nao foi possivel extrair o nome da classe %s (Arquivo invalido).\n", class_name);
                break;
                
            case METHOD_AREA_DUPLICATE_CLASS:
                fprintf(stderr, "LinkageError: A classe %s ja foi carregada na Method Area (Duplicata).\n", class_name);
                break;
                
            case METHOD_AREA_ALLOC_ERROR:
                fprintf(stderr, "OutOfMemoryError: Sem memoria para alocar a classe %s na Method Area.\n", class_name);
                break;
                
            default:
                fprintf(stderr, "UnknownError: Falha inesperada ao adicionar a classe %s (Codigo de erro: %d).\n", class_name, status);
                break;
        }
    }

    MethodAreaEntry *added_entry = MethodAreaGetEntry(ma, class_name);
    //MethodAreaPrepareClass(ma, class_name);

    // Carregamento Recursivo da Superclasse
    // O índice 0 indica que não tem superclasse (java.lang.Object)
    if (cf->super_class != 0) {
        char *super_name = GetClassName(cf, cf->super_class);
        if (super_name != NULL){
            ClassFile *super_cf = LoadClass(ma, super_name);
            if (super_cf == NULL) {
                fprintf(stderr, "NoClassDefFoundError: Falha ao carregar a superclasse %s da classe %s\n", super_name, class_name);
                return NULL;
            }
        }
    }

    // Carregamento Recursivo das Interfaces
    for (u2 i = 0; i < cf->interfaces_count; i++) {
        u2 interface_cp_index = cf->interfaces[i];
        char *interface_name = GetClassName(cf, cf->interfaces[i]);
        
        if (interface_name != NULL) {
            ClassFile *interface_cf = LoadClass(ma, interface_name);
            if (interface_cf == NULL) {
                fprintf(stderr, "NoClassDefFoundError: Falha ao carregar a interface %s da classe %s\n", interface_name, class_name);
                return NULL;
            }
        }
    }

    return cf;
}
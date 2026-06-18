// src/class_loader/linking/resolution.c
#include "resolution.h"
#include "../../class_loader/loading/classparser.h"

#include <stdlib.h>
#include <stdio.h>

static u1 IsLiteralTag(u1 tag){
    switch (tag) {
        case CONSTANT_Utf8:
        case CONSTANT_Integer:
        case CONSTANT_Float:
        case CONSTANT_Long:
        case CONSTANT_Double:
        case CONSTANT_NameAndType:
            return 1;
        default:
            return 0;
    }
}

u1 ResolveClass(MethodAreaEntry *entry){
    if (entry == NULL || entry->class_file == NULL) return 0;

    ClassFile *cf = entry->class_file;
    u2 cp_count = cf->constant_pool_count;

    entry->resolved_flags = (u1*) calloc(cp_count, sizeof(u1));

    if (entry->resolved_flags == NULL) return 0;

    // Marca como resolvidas as entradas literais.
    for (u2 i = 1; i < cp_count; i++){
        u1 tag = cf->constant_pool[i].tag;
        if (tag == 0 || IsLiteralTag(tag)){
            entry->resolved_flags[i] = 1;
        }
        // Referências simbólicas (Class, Fieldref etc) ficam 0 (resolvidas sob demanda no interpreter)
    }

    return 1;
}

u1 ResolveSymbolicRef(MethodAreaEntry *entry, u2 cp_index, const MethodArea *ma){
    if (entry == NULL || entry->class_file == NULL || entry->resolved_flags == NULL){
        return 0;
    }

    ClassFile *cf = entry->class_file;

    // Verifica se já foi resolvida (ou marcada como erro)
    if (entry->resolved_flags[cp_index] == 1) return 1;
    if (entry->resolved_flags[cp_index] == 2) return 0;

    if (cp_index == 0 || cp_index >= cf->constant_pool_count){
        return 0;
    }

    // marca como erro
    entry->resolved_flags[cp_index] = 2;
    return 0;
}
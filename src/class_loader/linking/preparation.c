// src/class_loader/linking/preparation.c
#include <stdlib.h>
#include "preparation.h"
#include "../../runtime_data/method_area/methodarea.h"

void PrepareClass(MethodAreaEntry *entry) {
    if (entry == NULL || entry->class_file == NULL) return;

    ClassFile *cf = entry->class_file;
    u2 static_count = 0;

    // Conta quantos campos estáticos existem na classe
    for (u2 i = 0; i < cf->fields_count; i++) {
        if (cf->fields[i].access_flags & ACC_STATIC) {
            static_count++;
        }
    }

    entry->static_field_count = static_count;

    if (static_count == 0) {
        entry->static_fields = NULL;
        return; 
    }

    // Calloc para inicializar os campos com valor padrão 0
    entry->static_fields = (StaticField*) calloc(static_count, sizeof(StaticField));

    // Mapeia o índice do campo estático com a sua memória alocada
    u2 current_static_index = 0;
    for (u2 i = 0; i < cf->fields_count; i++) {
        if (cf->fields[i].access_flags & ACC_STATIC) {
            entry->static_fields[current_static_index].field_index = i;
            current_static_index++;
        }
    }
}
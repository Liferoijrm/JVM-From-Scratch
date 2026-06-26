#include "initialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO: mover essa funcao para o methodarea.c futuramente (aperfeicoar se necessario)
 */
static Method_info *FindMethod(ClassFile *class_file, const char *name, const char *descriptor) {
    if (class_file == NULL || name == NULL) {
        return NULL;
    }

    for (u2 i = 0; i < class_file->methods_count; i++) {
        Method_info *method = &class_file->methods[i];

        char *method_name = (char *)class_file->constant_pool[method->name_index].info.Utf8.bytes;
        char *method_desc = (char *)class_file->constant_pool[method->descriptor_index].info.Utf8.bytes;

        if (strcmp(method_name, name) == 0) {
            // Se o descritor foi passado, valida ele também (essencial para sobrecarga)
            if (descriptor == NULL || strcmp(method_desc, descriptor) == 0) {
                return method;
            }
        }
    }

    return NULL;
}

/*
 * Inicializa a classe
 */
u1 InitializeClass(MethodArea *method_area, MethodAreaEntry *entry, JVMThread *thread) {
    Method_info *clinit;

    if (method_area == NULL || entry == NULL || entry->class_file == NULL) {
        return 0;
    }

    if (entry->state < CLASS_LINKED) return 0;

    // Se já estiver inicializada ou inicializando, não faz nada
    if (entry->state == CLASS_INITIALIZED || entry->state == CLASS_INITIALIZING) {
        return 1;
    }

    entry->state = CLASS_INITIALIZING;

    /*
     * Empurra o <clinit> da própria classe ANTES de inicializar a superclasse.
     * Como a frame_stack é LIFO, o <clinit> da superclasse (empurrado depois)
     * ficará no topo e será executado primeiro pelo interpretador — ordem correta.
     */
    clinit = FindMethod(entry->class_file, "<clinit>", "()V");

    if (clinit != NULL) {
        printf("[INITIALIZATION] <clinit> encontrado na classe %s\n", entry->class_name);
        pushFrame(thread, entry->class_file, clinit, thread->pc);
    } else {
        entry->state = CLASS_INITIALIZED;
    }

    /*
     * Inicializa a superclasse depois (seu frame fica no topo → roda primeiro).
     */
    if (entry->class_file->super_class != 0) {
        char *super_name = GetClassName(entry->class_file, entry->class_file->super_class);

        if (super_name != NULL) {
            MethodAreaEntry *super_entry = MethodAreaGetEntry(method_area, super_name);

            if (super_entry != NULL) {
                if (!InitializeClass(method_area, super_entry, thread)) {
                    return 0;
                }
            }
        }
    }

    printf("[INITIALIZATION] Classe %s inicializada com sucesso.\n", entry->class_name);

    return 1;
}
#include "resolve.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static u1 LinkAllLoaded(MethodArea *method_area);

ClassFile* get_class_from_constant_pool(JVMThread* thread, MethodArea* method_area, Frame* frame, u2 class_index) {
    ClassFile* current_class = frame->class_file;

    // Pega a entrada da classe no pool
    Cp_info class_info = current_class->constant_pool[class_index];
    u2 name_index = class_info.info.Class.name_index;

    // Extrai o nome UTF-8 e converte para string C (null-terminated)
    u2 name_length = current_class->constant_pool[name_index].info.Utf8.length;
    u1* name_bytes = current_class->constant_pool[name_index].info.Utf8.bytes;

    char class_name[name_length + 1];
    memcpy(class_name, name_bytes, name_length);
    class_name[name_length] = '\0';

    // 1. Tenta buscar na MethodArea
    MethodAreaEntry* entry = MethodAreaGetEntry(method_area, class_name);

    // --- PROTEÇÃO E TODO ---
    // 1. Verifica se a classe é nativa/simulada
    if (strstr(class_name, "java/io/") == class_name || 
        strstr(class_name, "java/lang/System") == class_name) {
        
        printf("[DEBUG] Ignorando carregamento de classe nativa/simulada: %s\n", class_name);
        
        /* TODO: Retornar uma estrutura de "Classe Mock" ou um ponteiro especial
           para que a instrução 'new' e 'invokevirtual' saibam como agir.
           Por enquanto, retornamos NULL para impedir que o interpretador tente 
           carregar o arquivo inexistente no disco.
        */
        return NULL; 
    }

    // 2. Se não estiver carregada, faz Load e Link
    if (entry == NULL) {
        printf("[RESOLVE] Carregando a classe '%s' dinamicamente...\n", class_name);
        
        ClassFile* loaded_cf = LoadClass(method_area, class_name);
        if (loaded_cf == NULL) {
            printf("NoClassDefFoundError: %s\n", class_name);
            exit(1);
        }

        entry = MethodAreaGetEntry(method_area, class_name);
        
        if (!LinkAllLoaded(method_area)) {
            printf("LinkageError: falha ao linkar classes dinamicamente\n", class_name);
            exit(1);
        }
    }

    // 3. Verifica se precisa de inicialização (<clinit>)
    // Assumindo que sua MethodAreaEntry tenha um campo 'state' (ex: CLASS_LOADED, CLASS_INITIALIZED)
    if (entry->state < CLASS_INITIALIZED) {
        printf("[RESOLVE] Inicializando a classe '%s'...\n", class_name);
        
        // Esta função deve procurar o método <clinit>. 
        // Se achar, cria o Frame e faz push(thread->frame_stack, clinit_frame).
        // Também deve mudar o entry->state para CLASS_INITIALIZED para não repetir o processo.
        u1 has_clinit = InitializeClass(method_area, entry, thread);
        
        if (has_clinit) {
            // Retornamos NULL como "Sinal" de que a thread foi pausada para rodar o clinit.
            return NULL;
        }
    }

    // Se já estava inicializada (ou se não tinha <clinit>), retorna a classe pronta!
    return entry->class_file;
}

static u1 LinkAllLoaded(MethodArea *method_area){
    for (u2 i = 0; i < method_area->count; i++) {
        MethodAreaEntry *entry = &method_area->entries[i];
        if (entry->state == CLASS_LOADED){
            if (!LinkClass(entry, method_area)){
                fprintf(stderr, "Falha ao linkar '%s'\n",
                        entry->class_name ? entry->class_name : "?");
                return 0;
            }
        }
    }
    return 1;
}
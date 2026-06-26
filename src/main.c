#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "viewer/printer.h"
#include "class_loader/loading/classloader.h"
#include "class_loader/linking/linking.h"
#include "class_loader/initialization/initialization.h"
#include "runtime_data/method_area/methodarea.h"
#include "runtime_data/thread_data/thread_data.h"
#include "interpreter/interpreter.h"

#define FRAME_STACK_MAX 512

// forward declaration para a main
static u1 LinkAllLoaded(MethodArea *method_area);

int main(int argc, char **argv){

    if (argc < 3 || (strcmp(argv[1], "run") && strcmp(argv[1], "view"))) {
        printf("Uso: ./<.exe> run <arquivo>       - Rodar a JVM\n");
        printf("Uso: ./<.exe> view <arquivo>      - Rodar o exibidor de bytecode\n");
        return 1;
    }

    char *class_name = argv[2];

    if (!strcmp(argv[1], "view")) {
        char full_path[512];

        snprintf(full_path, sizeof(full_path), "Examples/%s.class", class_name);

        ClassFile* cf = ParseClass(full_path);
        printClass(cf);
        return 0;
    }

    MethodArea *method_area = CreateMethodArea();
    if (!method_area) {
        fprintf(stderr, "Erro de alocação de memória para Method Area\n");
        return 1;
    }

    JVMThread *thread = createThread(FRAME_STACK_MAX);
    if (thread == NULL) {
        fprintf(stderr, "Falha ao criar JVMThread\n");
        DestroyMethodArea(method_area);
        return 1;
    }

    printf("[LOADING] Carregando '%s'...\n", class_name);

    ClassFile *main_cf = LoadClass(method_area, class_name);
    if (main_cf == NULL){
        fprintf(stderr, "Erro: não foi possível carregar a classe '%s'\n", class_name);
        freeStack(thread->frame_stack);
        free(thread);
        DestroyMethodArea(method_area);
        return 1;
    }

    printf("[LOADING] %u classe(s) carregada(s) na MethodArea\n", MethodAreaCount(method_area));

    printf("[LINKING] Linkando classes...\n");

    if (!LinkAllLoaded(method_area)){
        fprintf(stderr, "Erro: falha durante o linking\n");
        freeStack(thread->frame_stack);
        free(thread);
        DestroyMethodArea(method_area);
        return 1;
    }

    printf("[LINKING] %u classe(s) linkada(s) com sucesso\n", MethodAreaCount(method_area));

    // alocando o frame do metodo main
    MethodAreaEntry *main_entry = MethodAreaGetEntry(method_area, class_name);
    Method_info* main_method = ResolveMethod(method_area, main_entry->class_file, "main", 4, "([String)V", 10, NULL);
    pushFrame(thread, main_entry->class_file, main_method, 0);

    printf("[INITIALIZATION] Inicializando '%s'...\n", class_name);

    if (main_entry == NULL) {
        fprintf(stderr, "Erro interno: entrada da classe principal não encontrada\n");
        freeStack(thread->frame_stack);
        free(thread);
        DestroyMethodArea(method_area);
        return 1;
    }

    if (!InitializeClass(method_area, main_entry, thread)){
        fprintf(stderr, "Erro: falha ao inicializar '%s'\n", class_name);
        freeStack(thread->frame_stack);
        free(thread);
        DestroyMethodArea(method_area);
        return 1;
    }

    /*
        Quando for interpretar:

        cria um ReferenceMap (usar calloc com MAX_REF_MAP para entries)

        cria um
        RuntimeContext ctx = {
            .thread = thread,
            .method_area = method_area
            .reference_map = reference_map
        };

        interpret(&ctx);

        // OU CRIA UM PONTEIRO DE CTX EM VEZ DE PASSAR POR REFERENCIA COM &

        // tem que dar free nessas estruturas dps tambem
    */

    printf("Interpretador nao implementado ainda\n");

    freeStack(thread->frame_stack);
    free(thread);
    DestroyMethodArea(method_area);
    
    return 0;
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
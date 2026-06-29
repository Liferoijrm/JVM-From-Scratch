/**
 * @file main.c
 * @brief Ponto de entrada principal para a Java Virtual Machine (JVM).
 *
 * Este arquivo contém a função principal que inicializa a JVM, processa os argumentos
 * da linha de comando, executa o ciclo de vida das classes (carregamento, linking e 
 * inicialização) e inicia o interpretador de bytecode. Também inclui a funcionalidade 
 * de visualização de arquivos .class.
 */

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

/**
 * @brief Capacidade máxima da pilha de frames (Frame Stack) da thread da JVM.
 */
#define FRAME_STACK_MAX 512

/**
 * @brief Realiza o processo de linking para todas as classes atualmente carregadas.
 *
 * Itera sobre todas as entradas presentes na Method Area e aplica a etapa de linking
 * (verificação, preparação e resolução) nas classes que estão no estado CLASS_LOADED.
 *
 * @param method_area Ponteiro para a Method Area que contém as classes a serem linkadas.
 * @return u1 Retorna 1 se todas as classes foram linkadas com sucesso, ou 0 em caso de falha.
 */
static u1 LinkAllLoaded(MethodArea *method_area);

/**
 * @brief Função principal da JVM.
 *
 * Lida com os argumentos passados por linha de comando para definir a operação:
 * - "run <arquivo>": Carrega e executa a classe principal na JVM.
 * - "view <arquivo>": Atua como um visualizador (leitor) da estrutura do arquivo .class.
 *
 * No modo de execução, gerencia a criação da Method Area, da Thread principal, aloca 
 * o frame para o método "main", inicializa o mapa de referências e chama o interpretador.
 *
 * @param argc Número de argumentos recebidos pela linha de comando.
 * @param argv Vetor de strings contendo os argumentos.
 * @return int Retorna 0 em caso de execução bem-sucedida, ou 1 em caso de erro.
 */
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

    MethodAreaEntry *main_entry = MethodAreaGetEntry(method_area, class_name);
    Method_info* main_method = ResolveMethod(method_area, main_entry->class_file, "main", 4, "([Ljava/lang/String;)V", 22, NULL);
    if(main_method == NULL){
        printf("[ERROR] Classe nao possui metodo main\n");
        return 1;
    }
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

    ReferenceMap* ref_map = (ReferenceMap*) malloc(sizeof(ReferenceMap));
    ref_map->entries = (void**) calloc(MAX_REF_MAP, sizeof(void*));
    ref_map->entries[0] = NULL;
    ref_map->size = 1;

    RuntimeContext ctx = {
        .thread = thread,
        .method_area = method_area,
        .reference_map = ref_map
    };

    interpret(&ctx);

    printf("Interpretado!\n");

    freeStack(thread->frame_stack);
    free(thread);
    DestroyMethodArea(method_area);

    if (ref_map != NULL) {
        if (ref_map->entries != NULL) {
            for (size_t i = 0; i < ref_map->size; i++) {
                free(ref_map->entries[i]);
            }
            free(ref_map->entries);
        }
        free(ref_map);
    }
    
    return 0;
}

/**
 * @brief Aplica o linking em todas as classes carregadas na Method Area.
 *
 * Percorre as entradas da Method Area e executa o linking para cada
 * classe que ainda está no estado CLASS_LOADED.
 *
 * @param method_area Ponteiro para a Method Area.
 * @return 1 se todas as classes foram linkadas com sucesso, 0 caso contrário.
 */
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
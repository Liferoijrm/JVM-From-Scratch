/**
 * @file printer.c
 * @brief Implementação da lógica de exibição (viewer) da estrutura do arquivo bytecode.
 */

#include <stdio.h>
#include <string.h>
#include "./classviewer.h"
#include "../class_loader/loading/classparser.h"

/**
 * @brief Imprime as informações detalhadas de um ClassFile em um arquivo de texto.
 * * A função extrai o nome da classe do Constant Pool e cria um arquivo `.txt`
 * correspondente. Ela resolve conflitos de diretório substituindo barras (`/`)
 * por underlines (`_`) em nomes de pacotes (ex: java/lang/Object -> java_lang_Object.txt).
 * * Após definir o arquivo, a saída padrão (stdout) é redirecionada para ele, e 
 * as seções da classe (Constant Pool, Interfaces, Fields, Methods, Attributes) 
 * são impressas sequencialmente.
 * * @warning Esta função altera o fluxo de saída padrão (stdout) utilizando `freopen` 
 * e o fecha com `fclose(stdout)` no final da execução. Qualquer tentativa de usar 
 * `printf` no terminal após a chamada desta função falhará, a menos que o 
 * stdout seja reaberto.
 * * @param cf Ponteiro para a estrutura ClassFile a ser processada e impressa.
 */
void printClass(ClassFile *cf){

    char filename[1000];

    // pegar o nome da classe
    int c_index = cf->constant_pool[cf->this_class].info.Class.name_index;
    strcpy(filename, cf->constant_pool[c_index].info.Utf8.bytes);
    strcat(filename, ".txt");

    // Substituir '/' por '_' para evitar problemas com diretórios
    for(int i = 0; filename[i] != '\0'; i++) {
        if(filename[i] == '/') {
            filename[i] = '_';
        }
    }

    printf("abrindo: %s\n", filename);

    if(freopen(filename, "w", stdout) == NULL){
        perror("Nao conseguiu abrir\n");
    }
    
    printf("================================================\n\n");
    PrintClassFile(cf);
    printf("\n================================================\n");
    PrintCpool(cf->constant_pool, cf->constant_pool_count);
    printf("\n================================================\n\n");
    PrintInterfaces(cf->constant_pool, cf->interfaces, cf->interfaces_count);
    printf("\n================================================\n\n");
    PrintFields(cf->constant_pool, cf->fields, cf->fields_count);
    printf("\n================================================\n\n");
    PrintMethods(cf->constant_pool, cf->methods, cf->methods_count);
    printf("\n================================================\n\n");
    PrintAttributes(cf->constant_pool, cf->attributes, cf->attributes_count);
    printf("\n================================================\n\n");
    
    fclose(stdout);
    return;
}
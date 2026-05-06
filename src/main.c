#include <stdio.h>
#include <stdlib.h>
#include "class_loader/loading/classparser.h"
#include "viewer/classviewer.h"

#ifdef WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear" 
#endif

// função temporária de print para testes do ClassFile
void PrintClassFile(ClassFile *cf) {
    if (!cf) {
        printf("ClassFile NULL\n");
        return;
    }

    printf("magic: %u\n======\n", cf->magic);
    printf("minor_version: %u\n======\n", cf->minor_version);
    printf("major_version: %u\n======\n", cf->major_version);
    printf("constant_pool_count: %u\n======\n", cf->constant_pool_count);

    printf("access_flags: %s\n======\n", Read_flags(cf->access_flags));
    printf("this_class: %u\n======\n", cf->this_class);
    printf("super_class: %u\n======\n", cf->super_class);

    printf("interfaces_count: %u\n======\n", cf->interfaces_count);

    printf("fields_count: %u\n======\n", cf->fields_count);

    printf("methods_count: %u\n======\n", cf->methods_count);

    printf("attributes_count: %u\n======\n", cf->attributes_count);
}

int main(int argc, char **argv){
    int option, exit = 0;
    if (argc < 2) {
        printf("Uso: %s <arquivo.class>\n", argv[0]);
        return 1;
    }

    ClassFile *cf = ParseClass(argv[1]);

    if (!cf) {
        printf("Falha ao parsear o arquivo .class.\n");
        return 1;
    }

    PrintClassFile(cf);

    while(1) {
        if (exit)
            break;
        printf("\nEscolha a opcao:\n1. Exibir informacoes do CPool\n2. Exibir informacoes dos campos\n3. Exibir informacoes dos metodos\n4. Exibir informacoes dos atributos\n5. Sair\n");
        scanf("%d", &option);
        system(CLEAR);

        switch (option)
        {
        case 1:
            // code for displaying CPool information
            PrintCpool(cf->constant_pool, cf->constant_pool_count);
            printf("\n");
            break;
        case 2:
            // code for displaying field information
            PrintFields(cf->fields, cf->fields_count);
            printf("\n");
            break;
        case 3:
            // code for displaying method information
            PrintMethods(cf->methods, cf->methods_count);
            printf("\n");
            break;
        case 4:
            // code for displaying attribute information
            PrintAttributes(cf->attributes, cf->attributes_count);
            printf("\n");
            break;
        case 5:
            // code for exiting
            printf("Saindo...\n");
            FreeClass(cf);
            exit = 1;
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }

    // alocar memória e inicializar processos

    // chamar funções da JVM e do exibidor

    // liberar memória

    return 0;
}
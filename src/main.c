#include <stdio.h>
#include <stdlib.h>
#include "class_loader/loading/classparser.h"
#include "viewer/classviewer.h"
#include "viewer/printer.h"
#include "runtime_data/method_area/methodarea.h"

int main(int argc, char **argv){

    if (argc < 2) {
        printf("Uso: %s <arquivo.class>\n", argv[0]);
        return 1;
    }

    MethodArea *method_area = CreateMethodArea();
    if (!method_area) {
        fprintf(stderr, "Erro de alocação de memória para Method Area\n");
        return 1;
    }

    ClassFile *cf = ParseClass(argv[1]);

    if (!cf) {
        printf("Falha ao parsear o arquivo .class.\n");
        return 1;
    }

    // testes de methodArea:
    u1 status = MethodAreaAddClass(method_area, cf);
    if (status) {
        fprintf(stderr, "Erro ao adicionar classe a Method Area\n");
        return 1;
    }

    u2 count = MethodAreaCount(method_area);
    printf("Numero de classes na Method Area: %u\n", count);


    // invertendo os dois ele exibe o menu e depois salva o txt
    //ViewClass(cf);
    //printClass(cf);

    // alocar memória e inicializar processos

    // chamar funções da JVM e do exibidor
    // liberar memória
    
    DestroyMethodArea(method_area);
    
    return 0;
}
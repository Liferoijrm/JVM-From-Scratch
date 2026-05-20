#include <stdio.h>
#include <stdlib.h>
#include "class_loader/loading/classparser.h"
#include "viewer/classviewer.h"

int main(int argc, char **argv){
    if (argc < 2) {
        printf("Uso: %s <arquivo.class>\n", argv[0]);
        return 1;
    }

    ClassFile *cf = ParseClass(argv[1]);

    if (!cf) {
        printf("Falha ao parsear o arquivo .class.\n");
        return 1;
    }

    ViewClass(cf);

    // alocar memória e inicializar processos

    // chamar funções da JVM e do exibidor

    // liberar memória

    return 0;
}
#include <stdio.h>
#include "class_loader/loading/classparser.h"

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

    printf("access_flags: %u\n======\n", cf->access_flags);
    printf("this_class: %u\n======\n", cf->this_class);
    printf("super_class: %u\n======\n", cf->super_class);

    printf("interfaces_count: %u\n======\n", cf->interfaces_count);

    printf("fields_count: %u\n======\n", cf->fields_count);

    printf("methods_count: %u\n======\n", cf->methods_count);

    printf("attributes_count: %u\n======\n", cf->attributes_count);
}

int main(int argc, char **argv){

    if (argc < 2) {
        printf("Uso: %s <arquivo.class>\n", argv[0]);
        return 1;
    }

    ClassFile *cf = ParseClass(argv[1]);

    PrintClassFile(cf);

    FreeClass(cf);

    // alocar memória e inicializar processos

    // chamar funções da JVM e do exibidor

    // liberar memória

    return 0;
}
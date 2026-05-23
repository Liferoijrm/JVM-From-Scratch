#include <stdio.h>
#include <string.h>
#include "./classviewer.h"
#include "../class_loader/loading/classparser.h"

void printClass(ClassFile *cf){

    char filename[1000];

    // pegar o nome da classe
    int c_index = cf->constant_pool[cf->this_class].info.Class.name_index;
    strcpy(filename, cf->constant_pool[c_index].info.Utf8.bytes);
    strcat(filename, ".txt");

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
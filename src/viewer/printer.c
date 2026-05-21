#include <stdio.h>
#include <string.h>
#include "./classviewer.h"
#include "../class_loader/loading/classparser.h"

void printClass(ClassFile *cf){

    char filename[100];

    //pegar o nome da classe
    int c_index = cf->constant_pool[cf->this_class].info.Class.name_index;
    strcpy(filename, cf->constant_pool[c_index].info.Utf8.bytes);
    strcat(filename, ".txt");

    if(freopen(filename, "w", stdout) == NULL){
        perror("Nao conseguiu abrir\n");
    }
    
    PrintClassFile(cf);
    PrintCpool(cf->constant_pool, cf->constant_pool_count);
    printf("\n");
    //print interface
    PrintFields(cf->constant_pool, cf->fields, cf->fields_count);
    printf("\n");
    PrintMethods(cf->constant_pool, cf->methods, cf->methods_count);
    printf("\n");
    PrintAttributes(cf->constant_pool, cf->attributes, cf->attributes_count);
    printf("\n");
    fclose(stdout);
    //FreeClass(cf);

    return;
}
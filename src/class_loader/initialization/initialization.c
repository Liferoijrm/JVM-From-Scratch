#include "initialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Lista interna das classes inicializadas
 */
typedef struct InitializedNode {

    ClassFile *class_file;
    struct InitializedNode *next;

} InitializedNode;

static InitializedNode *initialized_classes = NULL;


/*
 * Verifica se a classe já foi inicializada
 */
static u1 IsInitialized(ClassFile *class_file) {

    InitializedNode *current = initialized_classes;

    while (current != NULL) {

        if (current->class_file == class_file) {
            return 1;
        }

        current = current->next;
    }

    return 0;
}


/*
 * Marca a classe como inicializada
 */
static u1 MarkInitialized(ClassFile *class_file) {

    InitializedNode *node =
        (InitializedNode *) malloc(sizeof(InitializedNode));

    if (node == NULL) {
        return 0;
    }

    node->class_file = class_file;
    node->next = initialized_classes;

    initialized_classes = node;

    return 1;
}


/*
 * Procura o <clinit>
 */
static Method_info *FindClinit(ClassFile *class_file) {

    if (class_file == NULL) {
        return NULL;
    }

    for (u2 i = 0; i < class_file->methods_count; i++) {

        Method_info *method =
            &class_file->methods[i];

        char *method_name =
            (char *)
            class_file->constant_pool[
                method->name_index
            ].info.Utf8.bytes;

        if (strcmp(method_name, "<clinit>") == 0) {
            return method;
        }
    }

    return NULL;
}


/*
 * Inicializa a classe
 */
u1 InitializeClass(MethodArea *method_area,
                   ClassFile *class_file) {

    Method_info *clinit;

    if (method_area == NULL ||
        class_file == NULL) {

        return 0;
    }

    if (IsInitialized(class_file)) {
        return 1;
    }

    /*
     * Inicializa a superclasse primeiro
     */
    if (class_file->super_class != 0) {

        char *super_name =
            GetClassName(class_file,
                         class_file->super_class);

        if (super_name != NULL) {

            ClassFile *super_class_file =
                MethodAreaFindClass(method_area,
                                    super_name);

            if (super_class_file != NULL &&
                super_class_file != class_file) {

                if (!InitializeClass(method_area,
                                     super_class_file)) {

                    return 0;
                }
            }
        }
    }

    clinit = FindClinit(class_file);

    if (clinit != NULL) {

        printf(
            "[INITIALIZATION] "
            "<clinit> encontrado na classe %s\n",

            GetClassName(class_file,
                         class_file->this_class)
        );

        /*
         * Futuramente(eu acho): ExecuteMethod(clinit);
         */
    }

    if (!MarkInitialized(class_file)) {
        return 0;
    }

    printf(
        "[INITIALIZATION] "
        "Classe %s inicializada.\n",

        GetClassName(class_file,
                     class_file->this_class)
    );

    return 1;
}
#include <stdio.h>
#include "./classviewer.h"
#include "../class_loader/loading/classparser.h"

// loop principal do bytecode viewer
void ViewClass(ClassFile *cf){
    int option, exit = 0;

    while(1){
        if (exit) break;
        
        printf("\nEscolha a opcao:\n1. Exibir informacoes gerais\n2. Exibir informacoes do CPool\n3. Exibir informacoes das interfaces\n4. Exibir informacoes dos campos\n5. Exibir informacoes dos metodos\n6. Exibir informacoes dos atributos\n7. Sair\n");
        scanf("%d", &option);
        cleanBuffer(); // serve para que inputs quaisquer não quebrem o codigo
        system(CLEAR);

        switch (option){
        case 1:
            // basic cf info
            PrintClassFile(cf);
            printf("\n");
            break;
        case 2:
            // code for displaying CPool information
            PrintCpool(cf->constant_pool, cf->constant_pool_count);
            printf("\n");
            break;
        case 3:
            // TODO: implementar
            break;
        case 4:
            // code for displaying field information
            PrintFields(cf->constant_pool, cf->fields, cf->fields_count);
            printf("\n");
            break;
        case 5:
            // code for displaying method information
            PrintMethods(cf->constant_pool, cf->methods, cf->methods_count);
            printf("\n");
            break;
        case 6:
            // code for displaying attribute information
            PrintAttributes(cf->constant_pool, cf->attributes, cf->attributes_count);
            printf("\n");
            break;
        case 7:
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
}

// função temporária de print para testes do ClassFile
void PrintClassFile(ClassFile *cf){
    int c_index;
    if (!cf) {
        printf("ClassFile NULL\n");
        return;
    }

    printf("magic: %u\n======\n", cf->magic);
    printf("minor_version: %u\n======\n", cf->minor_version);
    printf("major_version: %u\n======\n", cf->major_version);
    printf("constant_pool_count: %u\n======\n", cf->constant_pool_count);

    printf("access_flags: %s\n======\n", Read_flags(cf->access_flags));
    c_index = cf->constant_pool[cf->this_class].info.Class.name_index;
    printf("this_class: %s\n======\n", cf->constant_pool[c_index ].info.Utf8.bytes);
    c_index = cf->constant_pool[cf->super_class].info.Class.name_index;
    printf("super_class: %s\n======\n", cf->constant_pool[c_index].info.Utf8.bytes);

    printf("interfaces_count: %u\n======\n", cf->interfaces_count);

    printf("fields_count: %u\n======\n", cf->fields_count);

    printf("methods_count: %u\n======\n", cf->methods_count);

    printf("attributes_count: %u\n======\n", cf->attributes_count);
}

void PrintCpool(Cp_info *cpool, u2 count) {
    printf("Constant Pool:\n");

    for (u2 i = 1; i < count; i++) { // constant_pool[0] is unused
        Cp_info entry = cpool[i];

        printf("Constant #%u: ", i);

        switch (entry.tag) {
            case CONSTANT_Class:
                printf("Class: name_index=%u\n", entry.info.Class.name_index);
                break;
            case CONSTANT_Fieldref:
                printf("Fieldref: class_index=%u, name_and_type_index=%u\n", entry.info.Fieldref.class_index, entry.info.Fieldref.name_and_type_index);
                break;
            case CONSTANT_Methodref:
                printf("Methodref: class_index=%u, name_and_type_index=%u\n", entry.info.Methodref.class_index, entry.info.Methodref.name_and_type_index);
                break;
            case CONSTANT_InterfaceMethodref:
                printf("InterfaceMethodref: class_index=%u, name_and_type_index=%u\n", entry.info.InterfaceMethodref.class_index, entry.info.InterfaceMethodref.name_and_type_index);
                break;
            case CONSTANT_String:
                printf("String: string_index=%u\n", entry.info.String.string_index);
                break;
            case CONSTANT_Integer:
                printf("Integer: bytes=%u\n", entry.info.Integer.bytes);
                break;
            case CONSTANT_Float:
                printf("Float: bytes=%u\n", entry.info.Float.bytes);
                break;
            case CONSTANT_Long:
                printf("Long: high_bytes=%u, low_bytes=%u\n", entry.info.Long.high_bytes, entry.info.Long.low_bytes);
                break;
            case CONSTANT_Double:
                printf("Double: high_bytes=%u, low_bytes=%u\n", entry.info.Double.high_bytes, entry.info.Double.low_bytes);
                break;
            case CONSTANT_NameAndType:
                printf("NameAndType: name_index=%u, descriptor_index=%u\n", entry.info.NameAndType.name_index, entry.info.NameAndType.descriptor_index);
                break;
            case CONSTANT_Utf8:
                printf("Utf8: length=%u, bytes=", entry.info.Utf8.length);
                for (u2 j = 0; j < entry.info.Utf8.length; j++) {
                    putchar(entry.info.Utf8.bytes[j]);
                }
                printf("\n");
                break;
            default:
                printf("Unknown tag: %u\n", entry.tag);
                break;
        }
    }
}

void PrintFields(Cp_info *cpool, Field_info *fields, u2 count) {
    u1 *name = NULL, *descriptor = NULL;
    u2 len;

    printf("Fields:\n");

    for (u2 i = 0; i < count; i++) {
        name = cpool[fields[i].name_index].info.Utf8.bytes;
        descriptor = cpool[fields[i].descriptor_index].info.Utf8.bytes;
        len = cpool[fields[i].descriptor_index].info.Utf8.length;
        
        printf("Field #%u: access_flags=%s, name=%s, descriptor=", i, Read_flags(fields[i].access_flags), name);

        DecodeDescriptor(descriptor, len);

        printf(" attributes_count=%u\n", fields[i].attributes_count);
    }
}

void PrintMethods(Cp_info *cpool, Method_info *methods, u2 count) {
    u1 *name = NULL, *descriptor = NULL;
    u2 len;

    printf("Methods:\n");

    for (u2 i = 0; i < count; i++) {
        name = cpool[methods[i].name_index].info.Utf8.bytes;
        descriptor = cpool[methods[i].descriptor_index].info.Utf8.bytes;
        len = cpool[methods[i].descriptor_index].info.Utf8.length;

        printf("Method #%u: access_flags=%s, name=%s, descriptor=", i, Read_flags(methods[i].access_flags), name);

        DecodeDescriptor(descriptor, len);

        printf(" attributes_count=%u\n", methods[i].attributes_count);
    }
}

void PrintAttributes(Cp_info *cpool, Attribute_info *attributes, u2 count) {
    printf("Attributes:\n");

    for (u2 i = 0; i < count; i++) {
        char *name = cpool[attributes[i].attribute_name_index].info.Utf8.bytes;

        printf("Attribute #%u: name=%s, attribute_length=%u\n", i, name, attributes[i].attribute_length);
    }
}

void DecodeDescriptor(u1 *descriptor, u2 len){
    int j, k, arr_count = 0;
    char current_letter = 'a';

    j = 0;
    char reference[len], arr_brackets[3] = "[]";

    while(j < len) {
        if (descriptor[j] == '(' || descriptor[j] == ')') {
            printf(" %c", descriptor[j]);
        }
        if (arr_count > 0) {
            for (k = 0; k < arr_count; k++)
                printf("%s", arr_brackets);
            arr_count = 0;
        }
        if (descriptor[j] == 'I'){
            printf(" int %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'D'){
            printf(" double %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'V'){
            printf(" void %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'Z'){
            printf(" boolean %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'B'){
            printf(" byte %c ", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'C'){
            printf(" char %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'S'){
            printf(" short %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'F'){
            printf(" float %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'J'){
            printf(" long %c", current_letter);
            current_letter++;
        }
        else if (descriptor[j] == 'L') {
            k = 0;
            j++;

            while(descriptor[j] != ';') {

                if (descriptor[j] == '/') {
                    k = 0;
                    j++;
                    continue;
                }

                reference[k] = descriptor[j];
                j++;
                k++;
            }
            reference[k] = '\0';

            printf(" %s %c", reference, current_letter);
            
            current_letter++;
        }

        else if (descriptor[j] == '[') {
            k = 0;

            while(descriptor[j] == '[') {
                arr_count++;
                j++;
            }
        }
        j++;
    }
}

// limpa o buffer de entrada
void cleanBuffer(){
    while (getchar() != '\n');
}
#include <stdio.h>
#include <string.h>
#include "./classviewer.h"
#include "./attribute_decoder.h"
#include "../class_loader/loading/classparser.h"

#define ONE_BYTE 1
#define TWO_BYTES 2
#define THREE_BYTES 3

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

    printf("magic: 0x%0x\n======\n", cf->magic);
    printf("minor_version: %u\n======\n", cf->minor_version);
    printf("major_version: %u\n======\n", cf->major_version);
    printf("constant_pool_count: %u\n======\n", cf->constant_pool_count);

    printf("access_flags: %s\n======\n", DecodeAccessFlags(cf->access_flags));
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

    for (u2 i = 1; i < count; i++){ // constant_pool[0] is unused
        Cp_info entry = cpool[i];

        printf("[%u] ", i);

        switch (entry.tag) {
            case CONSTANT_Class:
                printf("CONSTANT_Class_info: name_index=%u\n", entry.info.Class.name_index);
                break;
            case CONSTANT_Fieldref:
                printf("CONSTANT_Fieldref_info: class_index=%u, name_and_type_index=%u\n", entry.info.Fieldref.class_index, entry.info.Fieldref.name_and_type_index);
                break;
            case CONSTANT_Methodref:
                printf("CONSTANT_Methodref_info: class_index=%u, name_and_type_index=%u\n", entry.info.Methodref.class_index, entry.info.Methodref.name_and_type_index);
                break;
            case CONSTANT_InterfaceMethodref:
                printf("CONSTANT_InterfaceMethodref_info: class_index=%u, name_and_type_index=%u\n", entry.info.InterfaceMethodref.class_index, entry.info.InterfaceMethodref.name_and_type_index);
                break;
            case CONSTANT_String:
                printf("CONSTANT_String_info: string_index=%u\n", entry.info.String.string_index);
                break;
            case CONSTANT_Integer:
                printf("CONSTANT_Integer_info: bytes=%u\n", entry.info.Integer.bytes);
                break;
            case CONSTANT_Float:
                printf("CONSTANT_Float_info: bytes=%u\n", entry.info.Float.bytes);
                break;
            case CONSTANT_Long:
                printf("CONSTANT_Long_info: high_bytes=%u, low_bytes=%u\n", entry.info.Long.high_bytes, entry.info.Long.low_bytes);
                break;
            case CONSTANT_Double:
                printf("CONSTANT_Double_info: high_bytes=%u, low_bytes=%u\n", entry.info.Double.high_bytes, entry.info.Double.low_bytes);
                break;
            case CONSTANT_NameAndType:
                printf("CONSTANT_NameAndType_info: name_index=%u, descriptor_index=%u\n", entry.info.NameAndType.name_index, entry.info.NameAndType.descriptor_index);
                break;
            case CONSTANT_Utf8:
                printf("CONSTANT_Utf8_info: length=%u, bytes=", entry.info.Utf8.length);
                printModfiedUtf8(entry.info.Utf8.bytes, entry.info.Utf8.length);
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
        
        printf("[%u] access_flags=%s, name=%s, descriptor=", i, DecodeAccessFlags(fields[i].access_flags), name);

        DecodeDescriptor(descriptor, len);

        printf(" attributes_count=%u\n", fields[i].attributes_count);

        for (u2 j = 0; j < fields[i].attributes_count; j++) {
            PrintAttributeInfo(cpool, &fields[i].attributes[j]);
        }
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

        printf("[%u] access_flags=%s, name=%s, descriptor=", i, DecodeMethodAccessFlags(methods[i].access_flags), name);

        DecodeDescriptor(descriptor, len);

        printf(" attributes_count=%u\n", methods[i].attributes_count);

        for (u2 j = 0; j < methods[i].attributes_count; j++) {
            PrintAttributeInfo(cpool, &methods[i].attributes[j]);
        }
    }
}

void PrintAttributes(Cp_info *cpool, Attribute_info *attributes, u2 attributes_count){

    printf("\nAttributes:\n");

    for(u2 i = 0; i < attributes_count; i++){
        char *name = (char *) cpool[attributes[i].attribute_name_index].info.Utf8.bytes;

        printf("\n[%u] %s\n", i, name);
        printf("Attribute length: %u\n", attributes[i].attribute_length);

        PrintAttributeInfo(cpool, &attributes[i]);
    }
}

void DecodeDescriptor(u1 *descriptor, u2 len){
    int j, k;

    j = 0;
    char reference[len];

    while(j < len) {
        if (descriptor[j] == 'L') {
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

            printf("%s", reference);
        }

        else 
            printf("%c", descriptor[j]);
        j++;
    }
}

u1 NthBitIsLit(u2 bitmask, u1 n){
    if (bitmask & (1 << n)) return 1;
    return 0;
}

char* AddFlag(char *buffer, const char *flag){
    u2 buffer_size = strlen(buffer);
    u1 increment_size = strlen(flag);
    u1 virgula = 0; 

    if(buffer_size != 0){
        increment_size+=2;
        virgula = 1; 
    }

    buffer = (char*) realloc(buffer, (buffer_size + increment_size + 1)*sizeof(char));

    if(virgula) buffer = strcat(buffer, ", ");
    buffer = strcat(buffer, flag);

    return buffer;
}

// recebe a bitmask de 16 bits e retorna string com nomes das access flags separadas por ", "
char *DecodeAccessFlags(u2 bitmask){
    char *buffer = (char*) malloc(sizeof(char));
    buffer[0] = '\0';

    if(NthBitIsLit(bitmask, 0)) 
        buffer = AddFlag(buffer, "Public");

    if(NthBitIsLit(bitmask, 1)) 
        buffer = AddFlag(buffer, "Private");

    if(NthBitIsLit(bitmask, 2)) 
        buffer = AddFlag(buffer, "Protected");

    if(NthBitIsLit(bitmask, 3))
        buffer = AddFlag(buffer, "Static");

    if(NthBitIsLit(bitmask, 4))
        buffer = AddFlag(buffer, "Final");

    if(NthBitIsLit(bitmask, 5))
        buffer = AddFlag(buffer, "Super");

    if(NthBitIsLit(bitmask, 6))
        buffer = AddFlag(buffer, "Volatile");

    if(NthBitIsLit(bitmask, 7))
        buffer = AddFlag(buffer, "Transient");

    if(NthBitIsLit(bitmask, 9))
        buffer = AddFlag(buffer, "Interface");

    if(NthBitIsLit(bitmask, 10))
        buffer = AddFlag(buffer, "Abstract");

    if(NthBitIsLit(bitmask, 11))
        buffer = AddFlag(buffer, "Strict");

    if(NthBitIsLit(bitmask, 12))
        buffer = AddFlag(buffer, "Synthetic");

    if(NthBitIsLit(bitmask, 13))
        buffer = AddFlag(buffer, "Annotation");

    if(NthBitIsLit(bitmask, 14))
        buffer = AddFlag(buffer, "Enum");

    return buffer;
}

// recebe a bitmask de 16 bits e retorna string com nomes das access flags separadas por ", "
// essa função especifica para metodos foi necessaria pois suas flags diferem em relacao a outras estruturas
char* DecodeMethodAccessFlags(u2 bitmask){
    char *buffer = (char*) malloc(sizeof(char));
    buffer[0] = '\0';

    if(NthBitIsLit(bitmask, 0)) 
        buffer = AddFlag(buffer, "Public");

    if(NthBitIsLit(bitmask, 1)) 
        buffer = AddFlag(buffer, "Private");

    if(NthBitIsLit(bitmask, 2)) 
        buffer = AddFlag(buffer, "Protected");

    if(NthBitIsLit(bitmask, 3))
        buffer = AddFlag(buffer, "Static");

    if(NthBitIsLit(bitmask, 4))
        buffer = AddFlag(buffer, "Final");

    if(NthBitIsLit(bitmask, 5))
        buffer = AddFlag(buffer, "Synchronized");

    if(NthBitIsLit(bitmask, 6))
        buffer = AddFlag(buffer, "Bridge");

    if(NthBitIsLit(bitmask, 7))
        buffer = AddFlag(buffer, "Varargs");

    if(NthBitIsLit(bitmask, 8))
        buffer = AddFlag(buffer, "Native");

    if(NthBitIsLit(bitmask, 10))
        buffer = AddFlag(buffer, "Abstract");

    if(NthBitIsLit(bitmask, 11))
        buffer = AddFlag(buffer, "Strict");

    if(NthBitIsLit(bitmask, 12))
        buffer = AddFlag(buffer, "Synthetic");

    return buffer;
}

void printModfiedUtf8(u1 *bytes, u2 length){
    for (u2 i = 0; i < length; i++) {
        // mostra \n literalmente
        if (bytes[i] == '\n') {
            printf("\\n");
            continue;
        }

        // Caso especial do Modified UTF-8 0xC080 representa U+0000 ('\0')
        if (i + 1 < length && bytes[i] == 0xC0 && bytes[i+1] == 0x80){
            putchar('\0');
            i++;
            continue;
        }

        // Para todos os demais casos, basta printar os bytes
        putchar(bytes[i]);
    }

    putchar('\n');
}

// limpa o buffer de entrada
void cleanBuffer(){
    while (getchar() != '\n');
}
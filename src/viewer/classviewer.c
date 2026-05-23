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
            // visualizacao de interfaces
            PrintInterfaces(cf->constant_pool, cf->interfaces, cf->interfaces_count);
            printf("\n");
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

const char* GetJavaVersionText(u2 major){
    switch (major)
    {
    case 45: return "JDK 1.1";
    case 46: return "JDK 1.2";
    case 47: return "JDK 1.3";
    case 48: return "JDK 1.4";
    case 49: return "Java SE 5";
    case 50: return "Java SE 6";
    case 51: return "Java SE 7";
    case 52: return "Java SE 8";
    }
}

// função temporária de print para testes do ClassFile
void PrintClassFile(ClassFile *cf){
    int c_index;
    if (!cf) {
        printf("ClassFile NULL\n");
        return;
    }
    printf("General Information: \n\n======\n");
    printf("magic: 0x%0x\n======\n", cf->magic);
    // Como a JVM deve suportar ate Java 8, deve tratar o erro de encontrar uma versao mais nova ou desconhecida
    if(cf->major_version < 45 && cf->minor_version != 0){
        // TODO: Tratamento de excessao para versao invalida
        printf("minor_version: INVALID\n");
        goto MAJOR;
    }
    printf("minor_version: %u\n======\n", cf->minor_version);
MAJOR:
    if(cf->major_version < 45 || cf->major_version > 52){
        // TODO: Tratamento de excessao para versao invalida
        printf("major_version: INVALID\n");
        goto CP;
    }
    printf("major_version: %u (%s)\n======\n", cf->major_version, GetJavaVersionText(cf->major_version));
CP: printf("constant_pool_count: %u\n======\n", cf->constant_pool_count);

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
    printf("\nConstant Pool:\n");

    if(count <= 1 || !cpool){
        printf("    Nenhuma constante no Constant Pool.\n");
        return;
    }

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
                printf("CONSTANT_Long_info: high_bytes=%u, low_bytes=%u\n", entry.info.Long.high_bytes, entry.info.Long.low_bytes); i++;
                break;
            case CONSTANT_Double:
                printf("CONSTANT_Double_info: high_bytes=%u, low_bytes=%u\n", entry.info.Double.high_bytes, entry.info.Double.low_bytes); i++;
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

    printf("\nFields (%u):\n", count);

    if(count == 0 || !fields){
        printf("    Nenhum campo implementado.\n");
        return;
    }

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

    printf("\nMethods (%u):\n\n", count);

    if(count == 0 || !methods){
        printf("    Nenhum metodo implementado.\n");
        return;
    }

    for (u2 i = 0; i < count; i++) {
        name = cpool[methods[i].name_index].info.Utf8.bytes;
        descriptor = cpool[methods[i].descriptor_index].info.Utf8.bytes;
        len = cpool[methods[i].descriptor_index].info.Utf8.length;

        printf("[%u] %s\n", i, name);
        printf("access_flags=%s\nname=%s\ndescriptor=", DecodeMethodAccessFlags(methods[i].access_flags));

        DecodeDescriptor(descriptor, len);
        printf("\n");

        if(methods[i].attributes_count > 0) printf("\nMethod Attributes (%u):\n", methods[i].attributes_count);
        for (u2 j = 0; j < methods[i].attributes_count; j++) {
            PrintAttributeInfo(cpool, &methods[i].attributes[j]);
        }
        printf("=====\n\n");
    }
}

void PrintAttributes(Cp_info *cpool, Attribute_info *attributes, u2 attributes_count){

    printf("\nAttributes (%u):\n", attributes_count);

    if(attributes_count == 0 || !attributes){
        printf("    Nenhum atributo de classe.\n");
        return;
    }

    for(u2 i = 0; i < attributes_count; i++){
        char *name = (char *) cpool[attributes[i].attribute_name_index].info.Utf8.bytes;

        printf("\n[%u] %s\n", i, name);
        printf("Attribute length: %u\n", attributes[i].attribute_length);

        PrintAttributeInfo(cpool, &attributes[i]);
    }
}

void PrintInterfaces(Cp_info* cpool, u2* interfaces, u2 count){
    printf("Interfaces (%u):\n", count);
    if(count == 0 || !interfaces){
        printf("    Nenhuma interface implementada.\n");
        return;
    }

    for(u2 i = 0 ; i < count ; i++){
        u2 cp_class_index = interfaces[i];
        u2 name_index = cpool[cp_class_index].info.Class.name_index;
        u1 *interface_name = cpool[name_index].info.Utf8.bytes;

        printf("    [%u] Interface: cp_index=%u -> Class Name: %s\n", i, cp_class_index, interface_name);
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

// função auxiliar para construir a string de flags a partir da bitmask
// recebe o buffer, seu tamanho, a bitmask, o número do bit a verificar, o nome da flag e um ponteiro para indicar se é a primeira flag a ser adicionada (para formatação)
static void AddFlag(char *buffer, size_t bufsize, u2 bitmask, u1 n, const char *flag, int *first) {
    if (!NthBitIsLit(bitmask, n)) return;

    size_t len = strlen(buffer);
    if (len >= bufsize) return;

    if (*first) {
        snprintf(buffer + len, bufsize - len, "%s", flag);
        *first = 0;
    } else {
        snprintf(buffer + len, bufsize - len, ", %s", flag);
    }
}

// recebe a bitmask de 16 bits e retorna string com nomes das access flags separadas por ", "
const char *DecodeAccessFlags(u2 bitmask){
    static char buffer[256] = "";
    buffer[0] = '\0';
    int first = 1;

    AddFlag(buffer, sizeof(buffer), bitmask, 0, "Public", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 1, "Private", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 2, "Protected", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 3, "Static", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 4, "Final", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 5, "Super", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 6, "Volatile", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 7, "Transient", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 9, "Interface", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 10, "Abstract", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 11, "Strict", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 12, "Synthetic", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 13, "Annotation", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 14, "Enum", &first);

    return buffer;
}

// recebe a bitmask de 16 bits e retorna string com nomes das access flags separadas por ", "
// essa função especifica para metodos foi necessaria pois suas flags diferem em relacao a outras estruturas
const char* DecodeMethodAccessFlags(u2 bitmask){
    static char buffer[256] = "";
    buffer[0] = '\0';
    int first = 1;

    AddFlag(buffer, sizeof(buffer), bitmask, 0, "Public", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 1, "Private", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 2, "Protected", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 3, "Static", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 4, "Final", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 5, "Synchronized", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 6, "Bridge", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 7, "Varargs", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 8, "Native", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 10, "Abstract", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 11, "Strict", &first);
    AddFlag(buffer, sizeof(buffer), bitmask, 12, "Synthetic", &first);

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
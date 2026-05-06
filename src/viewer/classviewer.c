#include <stdio.h>
#include "../class_loader/loading/classparser.h"

void PrintCpool(Cp_info *cpool, u2 count) {
    printf("Constant Pool:\n");
    for (u2 i = 1; i < count; i++) { // constant_pool[0] is unused
        Cp_info entry = cpool[i];
        printf("#%u: ", i);
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

void PrintFields(Field_info *fields, u2 count) {
    printf("Fields:\n");
    for (u2 i = 0; i < count; i++) {
        printf("Field #%u: access_flags=%s, name_index=%u, descriptor_index=%u, attributes_count=%u\n", i, Read_flags(fields[i].access_flags), fields[i].name_index, fields[i].descriptor_index, fields[i].attributes_count);
    }
}

void PrintMethods(Method_info *methods, u2 count) {
    printf("Methods:\n");
    for (u2 i = 0; i < count; i++) {
        printf("Method #%u: access_flags=%s, name_index=%u, descriptor_index=%u, attributes_count=%u\n", i, Read_flags(methods[i].access_flags), methods[i].name_index, methods[i].descriptor_index, methods[i].attributes_count);
    }
}

void PrintAttributes(Attribute_info *attributes, u2 count) {
    printf("Attributes:\n");
    for (u2 i = 0; i < count; i++) {
        printf("Attribute #%u: attribute_name_index=%u, attribute_length=%u\n", i, attributes[i].attribute_name_index, attributes[i].attribute_length);
    }
}
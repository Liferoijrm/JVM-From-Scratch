#include <stdio.h>
#include <string.h>

#include "./attribute_decoder.h"

static void PrintRawBytes(const u1 *bytes, u4 length) {
    for (u4 i = 0; i < length; i++) {
        printf("%02X", bytes[i]);
        if (i + 1 < length) printf(" ");
    }
}

void PrintAttributeInfo(Cp_info *cpool, Attribute_info *attribute) {
    char *attribute_name = (char *)cpool[attribute->attribute_name_index].info.Utf8.bytes;

    printf("    - name=%s, length=%u", attribute_name, attribute->attribute_length);

    if (!attribute->info || attribute->attribute_length == 0) {
        printf("\n");
        return;
    }

    if (strcmp(attribute_name, "ConstantValue") == 0 && attribute->attribute_length >= 2) {
        u2 constant_value_index = (u2)((attribute->info[0] << 8) | attribute->info[1]);
        printf(", constant_value_index=%u\n", constant_value_index);
        return;
    }

    if (strcmp(attribute_name, "SourceFile") == 0 && attribute->attribute_length >= 2) {
        u2 sourcefile_index = (u2)((attribute->info[0] << 8) | attribute->info[1]);
        printf(", sourcefile_index=%u\n", sourcefile_index);
        return;
    }

    if (strcmp(attribute_name, "Signature") == 0 && attribute->attribute_length >= 2) {
        u2 signature_index = (u2)((attribute->info[0] << 8) | attribute->info[1]);
        printf(", signature_index=%u\n", signature_index);
        return;
    }

    //TODO: implementar o Code e reaproveitar a função na lógica principal

    if (strcmp(attribute_name, "Exceptions") == 0) {
        if (attribute->attribute_length >= 2) {
            u2 number_of_exceptions = (u2)((attribute->info[0] << 8) | attribute->info[1]);
            u4 offset = 2;

            printf(", number_of_exceptions=%u", number_of_exceptions);

            for (u2 i = 0; i < number_of_exceptions; i++) {
                if (offset + 2 > attribute->attribute_length) break;

                u2 exception_index = (u2)((attribute->info[offset] << 8) | attribute->info[offset + 1]);
                printf("\n      exception_index[%u]=%u", i, exception_index);
                offset += 2;
            }

            printf("\n");
            return;
        }
    }

    printf(", raw_bytes=");
    PrintRawBytes(attribute->info, attribute->attribute_length);
    printf("\n");
}
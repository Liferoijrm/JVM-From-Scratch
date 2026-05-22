#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./attribute_decoder.h"

static void PrintRawBytes(const u1 *bytes, u4 length) {
    for (u4 i = 0; i < length; i++) {
        printf("%02X", bytes[i]);
        if (i + 1 < length) printf(" ");
    }
}

Code_attribute *DisassembleCodeAttribute(Cp_info *cpool, Code_attribute *code_attr, Attribute_info *attribute) {

    u2 idx_offset = 0;

    code_attr->max_stack = (u2)((attribute->info[idx_offset] << 8) | (attribute->info[idx_offset + 1]));
    idx_offset += 2;

    code_attr->max_locals = (u2)((attribute->info[idx_offset + 2] << 8) | (attribute->info[idx_offset + 3]));
    idx_offset += 2;

    code_attr->code_length = (u4)((attribute->info[idx_offset] << 24) | (attribute->info[idx_offset + 1] << 16) | (attribute->info[idx_offset + 2] << 8) | attribute->info[idx_offset + 3]);
    code_attr->code = (u1 *) calloc(code_attr->code_length, sizeof(u1));
    idx_offset += 4;

    memcpy(code_attr->code, &attribute->info[idx_offset], code_attr->code_length);

    idx_offset += code_attr->code_length;

    code_attr->exception_table_length = (u2)((attribute->info[idx_offset] << 8) | (attribute->info[idx_offset + 1]));
    idx_offset += 2;

    code_attr->exception_table = (Exception_code *) calloc(code_attr->exception_table_length, sizeof(Exception_code));
    for (u2 i = 0; i < code_attr->exception_table_length; i++) {
        code_attr->exception_table[i].start_pc   = (u2)((attribute->info[idx_offset] << 8) | attribute->info[idx_offset + 1]);
        code_attr->exception_table[i].end_pc     = (u2)((attribute->info[idx_offset + 2] << 8) | attribute->info[idx_offset + 3]);
        code_attr->exception_table[i].handler_pc = (u2)((attribute->info[idx_offset + 4] << 8) | attribute->info[idx_offset + 5]);
        code_attr->exception_table[i].catch_type = (u2)((attribute->info[idx_offset + 6] << 8) | attribute->info[idx_offset + 7]);
        idx_offset += 8;
    }

    idx_offset += code_attr->exception_table_length * sizeof(Exception_code);
    
    code_attr->attributes_count = (u2)((attribute->info[idx_offset] << 8) | (attribute->info[idx_offset + 1]));
    idx_offset += 2;

    code_attr->attributes = (Attribute_info *) calloc(code_attr->attributes_count, sizeof(Attribute_info));
    for (u2 i = 0; i < code_attr->attributes_count; i++) {
        code_attr->attributes[i].attribute_name_index = (u2)((attribute->info[idx_offset] << 8) | attribute->info[idx_offset + 1]);
        code_attr->attributes[i].attribute_length = (u4)((attribute->info[idx_offset + 2] << 24) | (attribute->info[idx_offset + 3] << 16) | (attribute->info[idx_offset + 4] << 8) | attribute->info[idx_offset + 5]);
        idx_offset += 6;
        
        code_attr->attributes[i].info = (u1 *) calloc(code_attr->attributes[i].attribute_length, sizeof(u1));
        if (code_attr->attributes[i].attribute_length > 0) {
            memcpy(code_attr->attributes[i].info, &attribute->info[idx_offset], code_attr->attributes[i].attribute_length);
        }
        idx_offset += code_attr->attributes[i].attribute_length;
    }

    return code_attr;
}

void ReadCode(Cp_info *cpool, Code_attribute *code_attr) {
    printf("\nCode Attribute:\n");
    printf("    max_stack=%u\n", code_attr->max_stack);
    printf("    max_locals=%u\n", code_attr->max_locals);
    printf("    code_length=%u\n", code_attr->code_length);
    printf("    code=");
    
    for(u4 i = 0; i < code_attr->code_length; i++){
        u1 opcode = code_attr->code[i];
        printf("%02X ", opcode);
    }
    printf("\n");

    printf("    exception_table_length=%u\n", code_attr->exception_table_length);
    for (u2 i = 0; i < code_attr->exception_table_length; i++) {
        Exception_code *exc = &code_attr->exception_table[i];
        printf("      Exception %u: start_pc=%u, end_pc=%u, handler_pc=%u, catch_type=%u\n", i, exc->start_pc, exc->end_pc, exc->handler_pc, exc->catch_type);
    }


    printf("\n    attributes_count=%u\n", code_attr->attributes_count);
    for (u2 i = 0; i < code_attr->attributes_count; i++) {
        printf("      Attribute %u:\n", i);
        PrintAttributeInfo(cpool, &code_attr->attributes[i]);
    }
    printf("\n");
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
    if (strcmp(attribute_name, "Code") == 0) {
        Code_attribute *code_attr = (Code_attribute *) calloc(1, sizeof(Code_attribute));
        code_attr->attribute_name_index = attribute->attribute_name_index;
        code_attr->attribute_length = attribute->attribute_length;
        DisassembleCodeAttribute(cpool, code_attr, attribute);
        ReadCode(cpool, code_attr);
        return;
    }

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
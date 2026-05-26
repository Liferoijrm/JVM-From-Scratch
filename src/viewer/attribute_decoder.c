#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./attribute_decoder.h"
//funcao especifica pro tableswitch (pode colocar em outro lugar depois)
static int32_t ReadS4BE(const u1 *code, u4 idx) {
    return (int32_t)((uint32_t)code[idx] << 24 |
                     (uint32_t)code[idx + 1] << 16 |
                     (uint32_t)code[idx + 2] << 8 |
                     (uint32_t)code[idx + 3]);
}

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

    // idx_offset += code_attr->exception_table_length * sizeof(Exception_code);
    
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

void PrintNewArray(u1 atype) {
    char *type_name = "";
    switch(atype){
        case 4: type_name = "boolean"; break;
        case 5: type_name = "char"; break;
        case 6: type_name = "float"; break;
        case 7: type_name = "double"; break;
        case 8: type_name = "byte"; break;
        case 9: type_name = "short"; break;
        case 10: type_name = "int"; break;
        case 11: type_name = "long"; break;
        default: type_name = "unknown"; break;
    }
    printf("newarray %u (%s)\n", atype, type_name);
}

void PrintCPoolItem(Cp_info *cpool, u2 index) {
    u1 tag = cpool[index].tag;
    // trata metodos/campos
    if(tag == CONSTANT_Methodref || tag == CONSTANT_Fieldref || tag == CONSTANT_InterfaceMethodref) {
        u2 class_idx = cpool[index].info.Methodref.class_index;
        u2 nt_idx = cpool[index].info.Methodref.name_and_type_index;
        u2 class_name_idx = cpool[class_idx].info.Class.name_index;
        u2 name_idx = cpool[nt_idx].info.NameAndType.name_index;
        printf("<%s.%s>", cpool[class_name_idx].info.Utf8.bytes, cpool[name_idx].info.Utf8.bytes);
    } 
    // trata classes instanciadas
    else if(tag == CONSTANT_Class) {
        u2 name_idx = cpool[index].info.Class.name_index;
        printf("<%s>", cpool[name_idx].info.Utf8.bytes);
    } 
    // trata strings carregadas na stack
    else if(tag == CONSTANT_String) {
        u2 string_idx = cpool[index].info.String.string_index;
        printf("<\"%s\">", cpool[string_idx].info.Utf8.bytes);
    }
}

// TODO: verificar se está funcionando certinho e implementar outras funções caso necessario
void ReadCode(Cp_info *cpool, Code_attribute *code_attr) {
    printf("\nCode Attribute:\n");
    printf("    max_stack=%u\n", code_attr->max_stack);
    printf("    max_locals=%u\n", code_attr->max_locals);
    printf("    code_length=%u\n", code_attr->code_length);
    printf("    code=\n");
    
    u4 pc = 0;
    u2 cp_index;
    int16_t offset;

    while(pc < code_attr->code_length){
        u1 opcode = code_attr->code[pc];
        printf("        %u: ", pc);
        switch(opcode){
            // constantes
            case 0x01: printf("aconst_null\n"); pc += 1; break;
            case 0x02: printf("iconst_m1\n"); pc += 1; break;
            case 0x03: printf("iconst_0\n"); pc += 1; break;
            case 0x04: printf("iconst_1\n"); pc += 1; break;
            case 0x05: printf("iconst_2\n"); pc += 1; break;
            case 0x06: printf("iconst_3\n"); pc += 1; break;
            case 0x07: printf("iconst_4\n"); pc += 1; break;
            case 0x08: printf("iconst_5\n"); pc += 1; break;
            case 0x0E: printf("dconst_0\n"); pc += 1; break;
            case 0x0F: printf("dconst_1\n"); pc += 1; break;
            // loads
            case 0x1A: printf("iload_0\n"); pc += 1; break;
            case 0x1B: printf("iload_1\n"); pc += 1; break;
            case 0x1C: printf("iload_2\n"); pc += 1; break;
            case 0x1D: printf("iload_3\n"); pc += 1; break;
            case 0x26: printf("dload_0\n"); pc += 1; break;
            case 0x27: printf("dload_1\n"); pc += 1; break;
            case 0x28: printf("dload_2\n"); pc += 1; break;
            case 0x29: printf("dload_3\n"); pc += 1; break;
            case 0x2A: printf("aload_0\n"); pc += 1; break;
            case 0x2B: printf("aload_1\n"); pc += 1; break;
            case 0x2C: printf("aload_2\n"); pc += 1; break;
            case 0x2D: printf("aload_3\n"); pc += 1; break;
            // stores
            case 0x3B: printf("istore_0\n"); pc += 1; break;
            case 0x3C: printf("istore_1\n"); pc += 1; break;
            case 0x3D: printf("istore_2\n"); pc += 1; break;
            case 0x3E: printf("istore_3\n"); pc += 1; break;
            case 0x47: printf("dstore_0\n"); pc += 1; break;
            case 0x48: printf("dstore_1\n"); pc += 1; break;
            case 0x49: printf("dstore_2\n"); pc += 1; break;
            case 0x4A: printf("dstore_3\n"); pc += 1; break;
            case 0x4B: printf("astore_0\n"); pc += 1; break;
            case 0x4C: printf("astore_1\n"); pc += 1; break;
            case 0x4D: printf("astore_2\n"); pc += 1; break;
            case 0x4E: printf("astore_3\n"); pc += 1; break;
            // acesso arrays
            case 0x2E: printf("iaload\n"); pc += 1; break;
            case 0x30: printf("faload\n"); pc += 1; break;
            case 0x31: printf("daload\n"); pc += 1; break;
            case 0x32: printf("aaload\n"); pc += 1; break;
            case 0x33: printf("baload\n"); pc += 1; break;
            case 0x34: printf("caload\n"); pc += 1; break;
            case 0x35: printf("saload\n"); pc += 1; break;
            case 0x4F: printf("iastore\n"); pc += 1; break;
            case 0x52: printf("dastore\n"); pc += 1; break;
            case 0x53: printf("aastore\n"); pc += 1; break;
            case 0x54: printf("bastore\n"); pc += 1; break;
            case 0x55: printf("castore\n"); pc += 1; break;
            case 0x56: printf("sastore\n"); pc += 1; break;
            case 0x51: printf("fastore\n"); pc += 1; break;
            case 0xBE: printf("arraylength\n"); pc += 1; break;
            // aritmetica
            case 0x60: printf("iadd\n"); pc += 1; break;
            case 0x64: printf("isub\n"); pc += 1; break;
            case 0x68: printf("imul\n"); pc += 1; break;
            case 0x6C: printf("idiv\n"); pc += 1; break;
            case 0x70: printf("irem\n"); pc += 1; break;
            case 0x63: printf("dadd\n"); pc += 1; break;
            case 0x67: printf("dsub\n"); pc += 1; break;
            case 0x6B: printf("dmul\n"); pc += 1; break;
            case 0x6F: printf("ddiv\n"); pc += 1; break;
            case 0x73: printf("drem\n"); pc += 1; break;
            // casts
            case 0x59: printf("dup\n"); pc += 1; break;
            case 0x87: printf("i2d\n"); pc += 1; break;
            case 0x8D: printf("f2d\n"); pc += 1; break;
            case 0x8E: printf("d2i\n"); pc += 1; break;
            // retornos
            case 0xAC: printf("ireturn\n"); pc += 1; break;
            case 0xAF: printf("dreturn\n"); pc += 1; break;
            case 0xB0: printf("areturn\n"); pc += 1; break;
            case 0xB1: printf("return\n"); pc += 1; break;
            // bipush
            case 0x10: printf("bipush %d\n", (int8_t)code_attr->code[pc + 1]); pc += 2; break;
            // ldc pequeno
            case 0x12: printf("ldc #%u\n", code_attr->code[pc + 1]); pc += 2; break;
            // loads c/ 1 arg
            case 0x15: printf("iload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x16: printf("lload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x17: printf("fload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x18: printf("dload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x19: printf("aload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            //stores c/ 1 arg
            case 0x36: printf("istore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x37: printf("lstore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x38: printf("fstore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x39: printf("dstore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x3A: printf("astore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            // newarray
            case 0xBC: PrintNewArray(code_attr->code[pc + 1]); pc += 2; break;
            // ldc grande
            case 0x13: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("ldc_w #%u\n", cp_index); pc+=3; break;
            case 0x14: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("ldc2_w #%u\n", cp_index); pc+=3; break;
            // acesso fields
            case 0xB2: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("getstatic #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB3: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("putstatic #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB4: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("getfield #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB5: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("putfield #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            // invocação metodos
            case 0xB6: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("invokevirtual #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB7: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("invokespecial #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB8: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("invokestatic #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            // criação objetos
            case 0xBB: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("new #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xBD: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("anewarray #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xC0: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("checkcast #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            // jumps
            case 0x99: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifeq %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0x9A: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifne %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0x9B: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("iflt %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0x9C: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifge %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0x9D: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifgt %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0x9E: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifle %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0x9F: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpeq %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xA0: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpne %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xA1: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmplt %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xA2: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpge %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xA3: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpgt %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xA4: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmple %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xA7: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("goto %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xC6: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifnull %d (+%d)\n", pc+offset, offset); pc+=3; break;
            case 0xC7: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifnonnull %d (+%d)\n", pc+offset, offset); pc+=3; break;
            // iinc
            case 0x84: printf("iinc %u by %d\n", code_attr->code[pc+1], (int8_t)code_attr->code[pc+2]); pc+=3; break;

            // tableswitch: opcode, padding(0-3), default(4), low(4), high(4), jump_offsets(4 * (high-low+1))
            // offset com sinal relativo ao endereço do opcode (pc), mas nao cuida do branching de vdd
            case 0xAA: {
                u4 start_pc = pc;
                u4 pad = (4 - ((start_pc + 1) % 4)) % 4;
                u4 idx = start_pc + 1 + pad;

                if (idx + 12 > code_attr->code_length) {
                    printf("tableswitch <truncada>\n");
                    pc += 1;
                    break;
                }

                int32_t default_off = ReadS4BE(code_attr->code, idx);
                int32_t low = ReadS4BE(code_attr->code, idx + 4);
                int32_t high = ReadS4BE(code_attr->code, idx + 8);
                idx += 12;

                printf("tableswitch low=%d high=%d default=%d (+%d)\n",
                       (int)low, (int)high, (int)(start_pc + default_off), (int)default_off);

                if (high < low) {
                    printf("            <range invalido>\n");
                    pc = idx;
                    break;
                }

                u4 n = (u4)((high - low) + 1);
                if (idx + (u4)(4 * n) > code_attr->code_length) {
                    printf("            <jump table truncada>\n");
                    pc = code_attr->code_length;
                    break;
                }

                for (u4 i = 0; i < n; i++) {
                    int32_t off = ReadS4BE(code_attr->code, idx + 4 * i);
                    int32_t key = low + (int32_t)i;
                    int32_t target = (int32_t)start_pc + off;
                    printf("            %d: %d (+%d)\n", (int)key, (int)target, (int)off);
                }

                pc = idx + 4 * n;
                break;
            }

            // lookupswitch
            case 0xAB: {
                u4 start_pc = pc;
                u4 pad = (4 - ((start_pc + 1) % 4)) % 4;
                u4 idx = start_pc + 1 + pad;

                if (idx + 8 > code_attr->code_length) {
                    printf("lookupswitch <truncada>\n");
                    pc += 1;
                    break;
                }

                int32_t default_off = ReadS4BE(code_attr->code, idx);
                int32_t npairs = ReadS4BE(code_attr->code, idx + 4);
                idx += 8;

                printf("lookupswitch npairs=%d default=%d (+%d)\n",
                       (int)npairs, (int)(start_pc + default_off), (int)default_off);

                if (idx + (u4)(8 * npairs) > code_attr->code_length) {
                    printf("            <pairs truncados>\n");
                    pc = code_attr->code_length;
                    break;
                }

                for (u4 i = 0; i < npairs; i++) {
                    int32_t key = ReadS4BE(code_attr->code, idx + 8 * i);
                    int32_t off = ReadS4BE(code_attr->code, idx + 8 * i + 4);
                    int32_t target = (int32_t)start_pc + off;
                    printf("            %d: %d (+%d)\n", (int)key, (int)target, (int)off);
                }

                pc = idx + 8 * npairs;
                break;
            }
            // erro
            default: printf("UNKNOWN_OPCODE (0x%02X)\n", opcode); pc+=1; break;
        }
    }
    // for(u4 i = 0; i < code_attr->code_length; i++){
    //     u1 opcode = code_attr->code[i];
    //     printf("%02X ", opcode);
    // }
    // printf("\n");

    // exception table
    if(code_attr->exception_table_length > 0) {
        printf("\n    Exception Table:\n");
        for(u2 i = 0; i < code_attr->exception_table_length; i++) {
            Exception_code *exc = &code_attr->exception_table[i];
            printf("      [%u] start_pc=%u, end_pc=%u, handler_pc=%u, catch_type=%u\n", 
                   i, exc->start_pc, exc->end_pc, exc->handler_pc, exc->catch_type);
        }
    }

    // subatributos do code
    if(code_attr->attributes_count > 0) {
        printf("\n    Code Attributes:\n");
        for(u2 i = 0; i < code_attr->attributes_count; i++) {
            PrintAttributeInfo(cpool, &code_attr->attributes[i]);
        }
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
        char *sourcefile_name = (char *) cpool[sourcefile_index].info.Utf8.bytes;
        printf(", sourcefile_index=%u", sourcefile_index);
        printf(", sourcefile_name=%s\n", sourcefile_name);
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
        // liberar memória alocada por DisassembleCodeAttribute
        if (code_attr->code) free(code_attr->code);
        if (code_attr->exception_table) free(code_attr->exception_table);
        if (code_attr->attributes) {
            for (u2 ai = 0; ai < code_attr->attributes_count; ai++) {
                if (code_attr->attributes[ai].info) free(code_attr->attributes[ai].info);
            }
            free(code_attr->attributes);
        }
        free(code_attr);
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
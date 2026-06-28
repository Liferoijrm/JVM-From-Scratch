/**
 * @file attribute_decoder.c
 * @brief Implementação das funções de decodificação e exibição dos atributos de classes, campos e métodos
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./attribute_decoder.h"

/**
 * @brief Lê um valor de 4 bytes com sinal em Big-Endian (específico para tableswitch).
 * * @param code Ponteiro para o array de bytes do código.
 * @param idx Índice inicial de onde os bytes serão lidos.
 * @return int32_t Valor lido decodificado.
 */
static int32_t ReadS4BE(const u1 *code, u4 idx) {
    return (int32_t)((uint32_t)code[idx] << 24 |
                     (uint32_t)code[idx + 1] << 16 |
                     (uint32_t)code[idx + 2] << 8 |
                     (uint32_t)code[idx + 3]);
}

/**
 * @brief Imprime um array de bytes no formato hexadecimal.
 * * @param bytes Ponteiro para os bytes.
 * @param length Quantidade de bytes a ser impressa.
 */
static void PrintRawBytes(const u1 *bytes, u4 length) {
    for (u4 i = 0; i < length; i++) {
        printf("%02X", bytes[i]);
        if (i + 1 < length) printf(" ");
    }
}

/**
 * @brief Desmonta e extrai as informações de um atributo Code a partir de um atributo genérico.
 * * @param cpool Ponteiro para o pool de constantes da classe.
 * @param code_attr Ponteiro para a estrutura Code_attribute que será preenchida com os dados decodificados.
 * @param attribute Ponteiro para o atributo genérico (Attribute_info) contendo o array de bytes bruto.
 * @return Code_attribute* Ponteiro para a estrutura Code_attribute devidamente preenchida.
 */
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

/**
 * @brief Imprime em formato textual o tipo de array criado pela instrução 'newarray'.
 * * @param atype Código numérico indicando o tipo do array (ex: 4 para boolean, 10 para int).
 */
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

/**
 * @brief Resolve e imprime o conteúdo de um item específico do pool de constantes de forma legível.
 * * @param cpool Ponteiro para o pool de constantes da classe.
 * @param index Índice do item no pool de constantes que deve ser acessado e impresso.
 */
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

    else if(tag == CONSTANT_Integer) {
        printf("<%d>", cpool[index].info.Integer.bytes);
    }
    else if(tag == CONSTANT_Float) {
        float f;
        memcpy(&f, &cpool[index].info.Float.bytes, sizeof(float));
        printf("<%f>", f);
    }
    else if(tag == CONSTANT_Long) {
        int64_t value =
            ((uint64_t)cpool[index].info.Long.high_bytes << 32) |
            cpool[index].info.Long.low_bytes;

        printf("<%lld>", (long long)value);
    }
    else if(tag == CONSTANT_Double) {
        uint64_t bits =
            ((uint64_t)cpool[index].info.Double.high_bytes << 32) |
            cpool[index].info.Double.low_bytes;

        double d;
        memcpy(&d, &bits, sizeof(double));

        printf("<%g>", d);
    }
}

/**
 * @brief Lê, decodifica e imprime sequencialmente as instruções (bytecodes/opcodes) de um atributo Code.
 * * @param cpool Ponteiro para o pool de constantes (utilizado para resolver referências de instruções como ldc, invoke*, etc.).
 * @param code_attr Ponteiro para o atributo de código contendo o array de bytes das instruções e a tabela de exceções.
 */
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
            // 0x00 - 0x0F
            case 0x00: printf("nop\n"); pc += 1; break;
            case 0x01: printf("aconst_null\n"); pc += 1; break;
            case 0x02: printf("iconst_m1\n"); pc += 1; break;
            case 0x03: printf("iconst_0\n"); pc += 1; break;
            case 0x04: printf("iconst_1\n"); pc += 1; break;
            case 0x05: printf("iconst_2\n"); pc += 1; break;
            case 0x06: printf("iconst_3\n"); pc += 1; break;
            case 0x07: printf("iconst_4\n"); pc += 1; break;
            case 0x08: printf("iconst_5\n"); pc += 1; break;
            case 0x09: printf("lconst_0\n"); pc += 1; break;
            case 0x0A: printf("lconst_1\n"); pc += 1; break;
            case 0x0B: printf("fconst_0\n"); pc += 1; break;
            case 0x0C: printf("fconst_1\n"); pc += 1; break;
            case 0x0D: printf("fconst_2\n"); pc += 1; break;
            case 0x0E: printf("dconst_0\n"); pc += 1; break;
            case 0x0F: printf("dconst_1\n"); pc += 1; break;
            
            // 0x10 - 0x1F
            case 0x10: printf("bipush %d\n", (int8_t)code_attr->code[pc + 1]); pc += 2; break;
            case 0x11: { int16_t sipush_val = (int16_t)((code_attr->code[pc + 1] << 8) | code_attr->code[pc + 2]); printf("sipush %d\n", sipush_val); pc += 3; break; }
            case 0x12: cp_index = code_attr->code[pc + 1]; printf("ldc #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc += 2; break;
            case 0x13: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("ldc_w #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0x14: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("ldc2_w #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0x15: printf("iload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x16: printf("lload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x17: printf("fload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x18: printf("dload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x19: printf("aload %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x1A: printf("iload_0\n"); pc += 1; break;
            case 0x1B: printf("iload_1\n"); pc += 1; break;
            case 0x1C: printf("iload_2\n"); pc += 1; break;
            case 0x1D: printf("iload_3\n"); pc += 1; break;
            case 0x1E: printf("lload_0\n"); pc += 1; break;
            case 0x1F: printf("lload_1\n"); pc += 1; break;
            
            // 0x20 - 0x2F
            case 0x20: printf("lload_2\n"); pc += 1; break;
            case 0x21: printf("lload_3\n"); pc += 1; break;
            case 0x22: printf("fload_0\n"); pc += 1; break;
            case 0x23: printf("fload_1\n"); pc += 1; break;
            case 0x24: printf("fload_2\n"); pc += 1; break;
            case 0x25: printf("fload_3\n"); pc += 1; break;
            case 0x26: printf("dload_0\n"); pc += 1; break;
            case 0x27: printf("dload_1\n"); pc += 1; break;
            case 0x28: printf("dload_2\n"); pc += 1; break;
            case 0x29: printf("dload_3\n"); pc += 1; break;
            case 0x2A: printf("aload_0\n"); pc += 1; break;
            case 0x2B: printf("aload_1\n"); pc += 1; break;
            case 0x2C: printf("aload_2\n"); pc += 1; break;
            case 0x2D: printf("aload_3\n"); pc += 1; break;
            case 0x2E: printf("iaload\n"); pc += 1; break;
            case 0x2F: printf("laload\n"); pc += 1; break;
            
            // 0x30 - 0x3F
            case 0x30: printf("faload\n"); pc += 1; break;
            case 0x31: printf("daload\n"); pc += 1; break;
            case 0x32: printf("aaload\n"); pc += 1; break;
            case 0x33: printf("baload\n"); pc += 1; break;
            case 0x34: printf("caload\n"); pc += 1; break;
            case 0x35: printf("saload\n"); pc += 1; break;
            case 0x36: printf("istore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x37: printf("lstore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x38: printf("fstore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x39: printf("dstore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x3A: printf("astore %u\n", code_attr->code[pc + 1]); pc += 2; break;
            case 0x3B: printf("istore_0\n"); pc += 1; break;
            case 0x3C: printf("istore_1\n"); pc += 1; break;
            case 0x3D: printf("istore_2\n"); pc += 1; break;
            case 0x3E: printf("istore_3\n"); pc += 1; break;
            case 0x3F: printf("lstore_0\n"); pc += 1; break;
            
            // 0x40 - 0x4F
            case 0x40: printf("lstore_1\n"); pc += 1; break;
            case 0x41: printf("lstore_2\n"); pc += 1; break;
            case 0x42: printf("lstore_3\n"); pc += 1; break;
            case 0x43: printf("fstore_0\n"); pc += 1; break;
            case 0x44: printf("fstore_1\n"); pc += 1; break;
            case 0x45: printf("fstore_2\n"); pc += 1; break;
            case 0x46: printf("fstore_3\n"); pc += 1; break;
            case 0x47: printf("dstore_0\n"); pc += 1; break;
            case 0x48: printf("dstore_1\n"); pc += 1; break;
            case 0x49: printf("dstore_2\n"); pc += 1; break;
            case 0x4A: printf("dstore_3\n"); pc += 1; break;
            case 0x4B: printf("astore_0\n"); pc += 1; break;
            case 0x4C: printf("astore_1\n"); pc += 1; break;
            case 0x4D: printf("astore_2\n"); pc += 1; break;
            case 0x4E: printf("astore_3\n"); pc += 1; break;
            case 0x4F: printf("iastore\n"); pc += 1; break;
            
            // 0x50 - 0x5F
            case 0x50: printf("lastore\n"); pc += 1; break;
            case 0x51: printf("fastore\n"); pc += 1; break;
            case 0x52: printf("dastore\n"); pc += 1; break;
            case 0x53: printf("aastore\n"); pc += 1; break;
            case 0x54: printf("bastore\n"); pc += 1; break;
            case 0x55: printf("castore\n"); pc += 1; break;
            case 0x56: printf("sastore\n"); pc += 1; break;
            case 0x57: printf("pop\n"); pc += 1; break;
            case 0x58: printf("pop2\n"); pc += 1; break;
            case 0x59: printf("dup\n"); pc += 1; break;
            case 0x5A: printf("dup_x1\n"); pc += 1; break;
            case 0x5B: printf("dup_x2\n"); pc += 1; break;
            case 0x5C: printf("dup2\n"); pc += 1; break;
            case 0x5D: printf("dup2_x1\n"); pc += 1; break;
            case 0x5E: printf("dup2_x2\n"); pc += 1; break;
            case 0x5F: printf("swap\n"); pc += 1; break;
            
            // 0x60 - 0x6F
            case 0x60: printf("iadd\n"); pc += 1; break;
            case 0x61: printf("ladd\n"); pc += 1; break;
            case 0x62: printf("fadd\n"); pc += 1; break;
            case 0x63: printf("dadd\n"); pc += 1; break;
            case 0x64: printf("isub\n"); pc += 1; break;
            case 0x65: printf("lsub\n"); pc += 1; break;
            case 0x66: printf("fsub\n"); pc += 1; break;
            case 0x67: printf("dsub\n"); pc += 1; break;
            case 0x68: printf("imul\n"); pc += 1; break;
            case 0x69: printf("lmul\n"); pc += 1; break;
            case 0x6A: printf("fmul\n"); pc += 1; break;
            case 0x6B: printf("dmul\n"); pc += 1; break;
            case 0x6C: printf("idiv\n"); pc += 1; break;
            case 0x6D: printf("ldiv\n"); pc += 1; break;
            case 0x6E: printf("fdiv\n"); pc += 1; break;
            case 0x6F: printf("ddiv\n"); pc += 1; break;
            
            // 0x70 - 0x7F
            case 0x70: printf("irem\n"); pc += 1; break;
            case 0x71: printf("lrem\n"); pc += 1; break;
            case 0x72: printf("frem\n"); pc += 1; break;
            case 0x73: printf("drem\n"); pc += 1; break;
            case 0x74: printf("ineg\n"); pc += 1; break;
            case 0x75: printf("lneg\n"); pc += 1; break;
            case 0x76: printf("fneg\n"); pc += 1; break;
            case 0x77: printf("dneg\n"); pc += 1; break;
            case 0x78: printf("ishl\n"); pc += 1; break;
            case 0x79: printf("lshl\n"); pc += 1; break;
            case 0x7A: printf("ishr\n"); pc += 1; break;
            case 0x7B: printf("lshr\n"); pc += 1; break;
            case 0x7C: printf("iushr\n"); pc += 1; break;
            case 0x7D: printf("lushr\n"); pc += 1; break;
            case 0x7E: printf("iand\n"); pc += 1; break;
            case 0x7F: printf("land\n"); pc += 1; break;
            
            // 0x80 - 0x8F
            case 0x80: printf("ior\n"); pc += 1; break;
            case 0x81: printf("lor\n"); pc += 1; break;
            case 0x82: printf("ixor\n"); pc += 1; break;
            case 0x83: printf("lxor\n"); pc += 1; break;
            case 0x84: printf("iinc %u by %d\n", code_attr->code[pc+1], (int8_t)code_attr->code[pc+2]); pc+=3; break;
            case 0x85: printf("i2l\n"); pc += 1; break;
            case 0x86: printf("i2f\n"); pc += 1; break;
            case 0x87: printf("i2d\n"); pc += 1; break;
            case 0x88: printf("l2i\n"); pc += 1; break;
            case 0x89: printf("l2f\n"); pc += 1; break;
            case 0x8A: printf("l2d\n"); pc += 1; break;
            case 0x8B: printf("f2i\n"); pc += 1; break;
            case 0x8C: printf("f2l\n"); pc += 1; break;
            case 0x8D: printf("f2d\n"); pc += 1; break;
            case 0x8E: printf("d2i\n"); pc += 1; break;
            case 0x8F: printf("d2l\n"); pc += 1; break;
            
            // 0x90 - 0x9F
            case 0x90: printf("d2f\n"); pc += 1; break;
            case 0x91: printf("i2b\n"); pc += 1; break;
            case 0x92: printf("i2c\n"); pc += 1; break;
            case 0x93: printf("i2s\n"); pc += 1; break;
            case 0x94: printf("lcmp\n"); pc += 1; break;
            case 0x95: printf("fcmpl\n"); pc += 1; break;
            case 0x96: printf("fcmpg\n"); pc += 1; break;
            case 0x97: printf("dcmpl\n"); pc += 1; break;
            case 0x98: printf("dcmpg\n"); pc += 1; break;
            case 0x99: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifeq %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0x9A: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifne %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0x9B: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("iflt %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0x9C: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifge %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0x9D: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifgt %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0x9E: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifle %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0x9F: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpeq %d (%+d)\n", pc+offset, offset); pc+=3; break;
            
            // 0xA0 - 0xAF
            case 0xA0: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpne %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA1: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmplt %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA2: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpge %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA3: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmpgt %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA4: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_icmple %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA5: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_acmpeq %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA6: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("if_acmpne %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA7: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("goto %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA8: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("jsr %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xA9: printf("ret %u\n", code_attr->code[pc+1]); pc+=2; break;
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

                printf("tableswitch low=%d high=%d default=%d (%+d)\n",
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
                    printf("            %d: %d (%+d)\n", (int)key, (int)target, (int)off);
                }

                pc = idx + 4 * n;
                break;
            }
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

                printf("lookupswitch npairs=%d default=%d (%+d)\n",
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
                    printf("            %d: %d (%+d)\n", (int)key, (int)target, (int)off);
                }

                pc = idx + 8 * npairs;
                break;
            }
            case 0xAC: printf("ireturn\n"); pc += 1; break;
            case 0xAD: printf("lreturn\n"); pc += 1; break;
            case 0xAE: printf("freturn\n"); pc += 1; break;
            case 0xAF: printf("dreturn\n"); pc += 1; break;
            
            // 0xB0 - 0xBF
            case 0xB0: printf("areturn\n"); pc += 1; break;
            case 0xB1: printf("return\n"); pc += 1; break;
            case 0xB2: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("getstatic #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB3: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("putstatic #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB4: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("getfield #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB5: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("putfield #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB6: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("invokevirtual #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB7: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("invokespecial #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB8: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("invokestatic #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xB9: { cp_index = (code_attr->code[pc + 1] << 8) | code_attr->code[pc + 2]; u1 count = code_attr->code[pc + 3]; printf("invokeinterface #%u count %u ", cp_index, count); PrintCPoolItem(cpool, cp_index); printf("\n"); pc += 5; break; }
            case 0xBB: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("new #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xBC: PrintNewArray(code_attr->code[pc + 1]); pc += 2; break;
            case 0xBD: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("anewarray #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xBE: printf("arraylength\n"); pc += 1; break;
            case 0xBF: printf("athrow\n"); pc += 1; break;
            
            // 0xC0 - 0xCF
            case 0xC0: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("checkcast #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xC1: cp_index = (code_attr->code[pc+1]<<8)|code_attr->code[pc+2]; printf("instanceof #%u ", cp_index); PrintCPoolItem(cpool, cp_index); printf("\n"); pc+=3; break;
            case 0xC2: printf("monitorenter\n"); pc += 1; break;
            case 0xC3: printf("monitorexit\n"); pc += 1; break;
            case 0xC5: { cp_index = (code_attr->code[pc + 1] << 8) | code_attr->code[pc + 2]; u1 dimensions = code_attr->code[pc + 3]; printf("multianewarray #%u dim %u ", cp_index, dimensions); PrintCPoolItem(cpool, cp_index); printf("\n"); pc += 4; break; }
            case 0xC6: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifnull %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xC7: offset = (int16_t)((code_attr->code[pc+1]<<8)|code_attr->code[pc+2]); printf("ifnonnull %d (%+d)\n", pc+offset, offset); pc+=3; break;
            case 0xC8: { int32_t offset_w = ReadS4BE(code_attr->code, pc + 1); printf("goto_w %d (%+d)\n", pc + offset_w, offset_w); pc += 5; break; }
            case 0xC9: { int32_t offset_w = ReadS4BE(code_attr->code, pc + 1); printf("jsr_w %d (%+d)\n", pc + offset_w, offset_w); pc += 5; break; }
            
            // erro
            default: printf("UNKNOWN_OPCODE (0x%02X)\n", opcode); pc+=1; break;
        }
    }

    // exception table
    if(code_attr->exception_table_length > 0) {
        printf("\n    Exception Table:\n");
        for(u2 i = 0; i < code_attr->exception_table_length; i++) {
            Exception_code *exc = &code_attr->exception_table[i];
            printf("      [%u] start_pc=%u, end_pc=%u, handler_pc=%u, catch_type=%u\n", 
                   i, exc->start_pc, exc->end_pc, exc->handler_pc, exc->catch_type);
        }
    }

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
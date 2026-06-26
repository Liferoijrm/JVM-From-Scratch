#include <stdio.h>
#include <string.h>
#include "verifier.h"
#include "../../utils/stack.h"

u4 getInstructionSize(const u1 *code, u4 pc){
    u1 opcode = code[pc];
    if(opcode == 0xAA){ // tableswitch
        u4 padding = (4 - ((pc + 1) % 4)) % 4;
        u4 offset = pc + 1 + padding;
        int32_t low = (code[offset + 4] << 24) | (code[offset + 5] << 16) | 
                      (code[offset + 6] << 8)  | code[offset + 7];
        int32_t high = (code[offset + 8] << 24) | (code[offset + 9] << 16) | 
                       (code[offset + 10] << 8) | code[offset + 11];
        return 1 + padding + 12 + ((high - low + 1) * 4);
    }
    if(opcode == 0xAB){ // lookupswitch
        u4 padding = (4 - ((pc + 1) % 4)) % 4;
        u4 offset = pc + 1 + padding;
        int32_t npairs = (code[offset + 4] << 24) | (code[offset + 5] << 16) | 
                         (code[offset + 6] << 8)  | code[offset + 7];
        return 1 + padding + 8 + (npairs * 8);
    }
    static const u1 sizes[256] = {
        // 0x00 - 0x0F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x10 - 0x1F
        2, 3, 2, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
        // 0x20 - 0x2F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x30 - 0x3F 
        1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
        // 0x40 - 0x4F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x50 - 0x5F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x60 - 0x6F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x70 - 0x7F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x80 - 0x8F
        1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0x90 - 0x9F
        1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3,
        // 0xA0 - 0xAF
        3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 0, 0, 1, 1, 1, 1,
        // 0xB0 - 0xBF
        1, 1, 3, 3, 3, 3, 3, 3, 3, 5, 5, 3, 2, 3, 1, 1,
        // 0xC0 - 0xCF
        3, 3, 1, 1, 1, 4, 3, 3, 5, 5, 1, 1, 1, 1, 1, 1, 
        // 0xD0 - 0xDF
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0xE0 - 0xEF
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        // 0xF0 - 0xFF
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    if(opcode > 0xCF){
        fprintf(stderr, "Aviso: Opcode nao suportado encontrado (%02X).\\n", opcode);
    }
    return sizes[opcode];
}

u1 VerifyMetadata(ClassFile *cf, const MethodArea *ma){
    // verifica se não tem superclasse, logo se eh a java/lang/Object
    if(cf->super_class == 0){
        char *class_name = GetClassName(cf, cf->this_class);
        if(class_name && strcmp(class_name, "java/lang/Object") != 0){
            fprintf(stderr, "VerifyError: A classe %s nao possui superclasse.\n", class_name);
            return 0;
        }
    }
    else{
        char *super_name = GetClassName(cf, cf->super_class);
        if(super_name == NULL){
            fprintf(stderr, "VerifyError: Superclasse com indice invalido.\n");
            return 0;
        }
        // verifica se o pai da interface eh java/lang/Object
        if((cf->access_flags & 0x0200)) {
            if(strcmp(super_name, "java/lang/Object") != 0){
                fprintf(stderr, "VerifyError: Interfaces devem ter java.lang.Object como superclasse direta.\n");
                return 0;
            }
        } 
        // verifica se o pai da classe comum não eh final
        else{
            ClassFile *super_cf = MethodAreaFindClass(ma, super_name);
            if(super_cf != NULL && (super_cf->access_flags & 0x0010)){
                fprintf(stderr, "VerifyError: A classe nao pode herdar da classe final %s.\n", super_name);
                return 0;
            }
        }
    }
    return 1;
}

u1 VerifyBytecode(ClassFile *cf){
    for(u2 i = 0; i < cf->methods_count; i++){
        Method_info *method = &cf->methods[i];

        // metodos nativos e abstratos não possuem atributo Code
        if((method->access_flags & 0x0100) || (method->access_flags & 0x0400)){
            continue;
        }

        for(u2 j = 0; j < method->attributes_count; j++){
            u2 name_idx = method->attributes[j].attribute_name_index;
            char *attr_name = (char *) cf->constant_pool[name_idx].info.Utf8.bytes;

            if(strcmp(attr_name, "Code") == 0){
                u1 *info = method->attributes[j].info;
                // verificacao de tamanho do codigo
                u4 code_length = (info[4] << 24) | (info[5] << 16) | (info[6] << 8) | info[7];
                if(code_length == 0 || code_length >= 65536){
                    fprintf(stderr, "VerifyError: Comprimento de codigo invalido no metodo %d.\n", i);
                    return 0;
                }
                // verificacao limite das intrucoes
                u1 *code = &info[8];
                u4 pc = 0;
                while(pc < code_length){
                    u1 opcode = code[pc];
                    u4 inst_size = getInstructionSize(code, pc); 
                    if(pc + inst_size > code_length){
                        fprintf(stderr, "VerifyError: Instrucao (pc=%u) le operandos fora dos limites.\n", pc);
                        return 0;
                    }
                    pc += inst_size;
                }
                // verificacao da tabela de excecoes
                u2 exc_table_len = (info[8 + code_length] << 8) | info[8 + code_length + 1];
                u1 *exc_table = &info[8 + code_length + 2];
                for(u2 k = 0; k < exc_table_len; k++){
                    u2 start_pc   = (exc_table[0] << 8) | exc_table[1];
                    u2 end_pc     = (exc_table[2] << 8) | exc_table[3];
                    u2 handler_pc = (exc_table[4] << 8) | exc_table[5];
                    if(start_pc >= code_length || end_pc > code_length || handler_pc >= code_length){
                        fprintf(stderr, "VerifyError: Exception table aponta para fora do codigo.\n");
                        return 0;
                    }
                    if(start_pc >= end_pc){
                        fprintf(stderr, "VerifyError: Exception table start_pc deve ser menor que end_pc.\n");
                        return 0;
                    }
                    exc_table += 8;
                }
                break; 
            }
        }
    }
    return 1;
}

u1 VerifyClass(ClassFile *cf, const MethodArea *ma){
    if(!VerifyMetadata(cf, ma)) return 0;
    if(!VerifyBytecode(cf)) return 0;
    return 1;
}
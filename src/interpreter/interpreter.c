#include "interpreter.h"
#include "instruction_handler.h"
#include <math.h>

//TODO: verificar se o tamanho da stack de frames ou de operandos é excedido e tratar

// avanca o pc
u4 handle_nop(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    return pc + 1;
}

// da push em null na operand stack
u4 handle_aconst_null(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 0; // NULL
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em -1 na operand stack
u4 handle_iconst_m1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = -1; // -1
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 0 na operand stack
u4 handle_iconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 0; // 0
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 1 na operand stack
u4 handle_iconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 1; // 1
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 2 na operand stack
u4 handle_iconst_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 2; // 2
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 3 na operand stack
u4 handle_iconst_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 3; // 3
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 4 na operand stack
u4 handle_iconst_4(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 4; // 4
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 5 na operand stack
u4 handle_iconst_5(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 5; // 5
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push do long 0 na operand stack
u4 handle_lconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = 0;
    u4 low  = 0;

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push do long 1 na operand stack
u4 handle_lconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = 0;
    u4 low  = 1;

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push do double 0.0 na operand stack
u4 handle_dconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    union {
        double d;
        uint64_t u;
    } value = { 0.0 };

    u4 high = (u4)(value.u >> 32);
    u4 low  = (u4)(value.u & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push do double 1.0 na operand stack
u4 handle_dconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    union {
        double d;
        uint64_t u;
    } value = { 1.0 };

    u4 high = (u4)(value.u >> 32);
    u4 low  = (u4)(value.u & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push em um byte sign extended na operand stack
u4 handle_bipush(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = sign_extend_byte(parameter);
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da push em um short sign extended na operand stack
u4 handle_sipush(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter_1 = code[pc+1];
    u1 parameter_2 = code[pc+2];
    u2 parameter = ((parameter_1 << 8) | parameter_2);
    u4 content = sign_extend_short(parameter);
    push(frame->operand_stack, (void*)&content);
    return pc + 3;
}

u4 handle_ldc(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    Cp_info *cp = frame->class_file->constant_pool;
    ReferenceMap *rm = ctx->reference_map;

    u1 index = code_attr->code[pc + 1];
    Cp_info entry = cp[index];

    switch (entry.tag) {
        case CONSTANT_Integer: {
            u4 value = entry.info.Integer.bytes;
            push(frame->operand_stack, (void*)&value);
            break;
        }
        case CONSTANT_Float: {
            u4 value = entry.info.Float.bytes; // já são os bits IEEE754
            push(frame->operand_stack, (void*)&value);
            break;
        }
        case CONSTANT_String: {
            u2 utf8_idx = entry.info.String.string_index;
            u1 *bytes = cp[utf8_idx].info.Utf8.bytes;
            u2 len = cp[utf8_idx].info.Utf8.length;

            char *str = (char*) malloc(len + 1);
            memcpy(str, bytes, len);
            str[len] = '\0';

            u4 ref_key = rm->size++;
            rm->entries[ref_key] = (void*) str;
            push(frame->operand_stack, (void*)&ref_key);
            break;
        }
        // TODO: suportar tags de Class, MethodInfo e MethodType se necessario (nao tem nos .class dos exemplos ent n precisa por enquanto)
        default:
            fprintf(stderr, "ldc: tag não suportada (%d)\n", entry.tag);
            exit(1);
    }
    return pc + 2;
}

u4 handle_ldc_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    Cp_info *cp = frame->class_file->constant_pool;
    ReferenceMap *rm = ctx->reference_map;
    u1* code = code_attr->code;

    u2 index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    Cp_info entry = cp[index];

    switch (entry.tag) {
        case CONSTANT_Integer: {
            u4 value = entry.info.Integer.bytes;
            push(frame->operand_stack, (void*)&value);
            break;
        }
        case CONSTANT_Float: {
            u4 value = entry.info.Float.bytes; // já são os bits IEEE754
            push(frame->operand_stack, (void*)&value);
            break;
        }
        case CONSTANT_String: {
            u2 utf8_idx = entry.info.String.string_index;
            u1 *bytes = cp[utf8_idx].info.Utf8.bytes;
            u2 len = cp[utf8_idx].info.Utf8.length;

            char *str = (char*) malloc(len + 1);
            memcpy(str, bytes, len);
            str[len] = '\0';

            if (rm->size >= MAX_REF_MAP) {
                printf("OutOfMemoryError: ReferenceMap cheio!\n");
                exit(1);
            }
            u4 ref_key = rm->size++;
            rm->entries[ref_key] = (void*) str;
            push(frame->operand_stack, (void*)&ref_key);
            break;
        }
        default:
            fprintf(stderr, "ldc_w: tag não suportada (%d)\n", entry.tag);
            exit(1);
    }

    return pc + 3; // 1 opcode + 2 bytes de índice
}

u4 handle_ldc2_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    Cp_info *cp = frame->class_file->constant_pool;
    u1 *code = code_attr->code;

    u2 index = ((u2)code[pc+1] << 8) | code[pc+2];
    Cp_info entry = cp[index];
    u4 high, low;

    if (entry.tag == CONSTANT_Double) {
        high = entry.info.Double.high_bytes;
        low  = entry.info.Double.low_bytes;
    } else if (entry.tag == CONSTANT_Long) {
        high = entry.info.Long.high_bytes;
        low  = entry.info.Long.low_bytes;
    } else {
        fprintf(stderr, "ldc2_w: tag inválida (%d)\n", entry.tag);
        exit(1);
    }

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);
    return pc + 3;
}

// da push em uma variavel local no indice idx na operand stack
u4 handle_iload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = frame->local_variables[parameter];
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da push em uma variavel local no indice 0 na operand stack
u4 handle_iload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[0];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em uma variavel local no indice 1 na operand stack
u4 handle_iload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[1];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em uma variavel local no indice 2 na operand stack
u4 handle_iload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[2];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em uma variavel local no indice 3 na operand stack
u4 handle_iload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[3];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de um long da variavel local no indice idx na operand stack
u4 handle_lload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u1 parameter = code[pc + 1];

    u4 high = frame->local_variables[parameter];
    u4 low  = frame->local_variables[parameter + 1];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 2;
}

// da push de um long da variavel local no indice 0 na operand stack
u4 handle_lload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[0];
    u4 low  = frame->local_variables[1];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um long da variavel local no indice 1 na operand stack
u4 handle_lload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[1];
    u4 low  = frame->local_variables[2];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um long da variavel local no indice 2 na operand stack
u4 handle_lload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[2];
    u4 low  = frame->local_variables[3];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um long da variavel local no indice 3 na operand stack
u4 handle_lload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[3];
    u4 low  = frame->local_variables[4];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice idx na operand stack
u4 handle_dload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u1 parameter = code[pc + 1];

    u4 high = frame->local_variables[parameter];
    u4 low  = frame->local_variables[parameter + 1];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 2;
}

// da push de um double da variavel local no indice 0 na operand stack
u4 handle_dload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[0];
    u4 low  = frame->local_variables[1];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice 1 na operand stack
u4 handle_dload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[1];
    u4 low  = frame->local_variables[2];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice 2 na operand stack
u4 handle_dload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[2];
    u4 low  = frame->local_variables[3];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice 3 na operand stack
u4 handle_dload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[3];
    u4 low  = frame->local_variables[4];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de uma variavel local de referencia no indice 0 na operand stack
u4 handle_aload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[0];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice idx
u4 handle_istore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[parameter] = content;
    return pc + 2;
}

// da pop na operand stack e armazena em variaveis locais no indice 0
u4 handle_istore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[0] = content;
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice 1
u4 handle_istore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[1] = content;
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice 2
u4 handle_istore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[2] = content;
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice 3
u4 handle_istore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[3] = content;
    return pc + 1;
}

// da pop na operand stack e armazena um long em variaveis locais no indice idx
u4 handle_lstore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u1 parameter = code[pc + 1];

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[parameter]     = high;
    frame->local_variables[parameter + 1] = low;

    return pc + 2;
}

// da pop na operand stack e armazena um long em variaveis locais no indice 0
u4 handle_lstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[0] = high;
    frame->local_variables[1] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um long em variaveis locais no indice 1
u4 handle_lstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[1] = high;
    frame->local_variables[2] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um long em variaveis locais no indice 2
u4 handle_lstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[2] = high;
    frame->local_variables[3] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um long em variaveis locais no indice 3
u4 handle_lstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[3] = high;
    frame->local_variables[4] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um double em variaveis locais no indice idx
u4 handle_dstore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u1 parameter = code[pc + 1];

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[parameter]     = high;
    frame->local_variables[parameter + 1] = low;

    return pc + 2;
}

// da pop na operand stack e armazena um double em variaveis locais no indice 0
u4 handle_dstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[0] = high;
    frame->local_variables[1] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um double em variaveis locais no indice 1
u4 handle_dstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[1] = high;
    frame->local_variables[2] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um double em variaveis locais no indice 2
u4 handle_dstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[2] = high;
    frame->local_variables[3] = low;

    return pc + 1;
}

// da pop na operand stack e armazena um double em variaveis locais no indice 3
u4 handle_dstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    frame->local_variables[3] = high;
    frame->local_variables[4] = low;

    return pc + 1;
}

// da pop na operand stack 2x e da push da soma dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_iadd(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand_1 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 operand_2 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 content = operand_2 + operand_1;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack 2x e da push da diferenca dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_isub(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand_1 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 operand_2 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 content = operand_2 - operand_1;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack 2x e da push do produto dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_imul(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand_1 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 operand_2 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 content = operand_2 * operand_1;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack 2x e da push da divisao inteira dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_idiv(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand_1 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 operand_2 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    if (operand_1 == 0){
        // TODO: lançar excecao para divisao por 0
        printf("divisao por 0 fi\n");
        exit(1);
    }

    int32_t div_result;

    // intercepta o caso INT_MIN % -1
    if(operand_2 == 0x80000000 && operand_1 == 0xFFFFFFFF){
        div_result = 0x80000000; // a JVM exige que seja 0x80000000
    }
    else{
        div_result = (int32_t)operand_2 / (int32_t)operand_1;
    }

    u4 content = (u4)div_result;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack 2x e da push do resto da divisao inteira dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_irem(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand_1 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 operand_2 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    if (operand_1 == 0){
        // TODO: lançar excecao para divisao por 0
        printf("resto por 0 fi\n");
        exit(1);
    }

    int32_t rem_result;

    // intercepta o caso INT_MIN % -1
    if(operand_2 == 0x80000000 && operand_1 == 0xFFFFFFFF){
        rem_result = 0; // a JVM exige que seja 0
    }
    else{
        rem_result = (int32_t)operand_2 % (int32_t)operand_1;
    }

    u4 content = (u4)rem_result;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack e da push do operando negativo
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_ineg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 content = (-operand);
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// incrementa variavel local no indice idx por um byte sign extended 
u4 handle_iinc(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter_1 = code[pc+1];
    u1 parameter_2 = code[pc+2];
    u4 increment = sign_extend_byte(parameter_2);
    frame->local_variables[parameter_1]+=increment;
    return pc + 3;
}

// da pop na operand stack 2x (longs) e da push da divisao inteira dos operandos
// OBS: v2 eh o divisor (termo da direita) e v1 eh o dividendo (termo da esquerda)
u4 handle_ldiv(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v2 = (((int64_t)high2) << 32) | low2;

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v1 = (((int64_t)high1) << 32) | low1;

    if (v2 == 0) {
        // TODO: lançar excecao para divisao por 0
        printf("divisao por 0 fi\n");
        exit(1);
    }

    int64_t div_result;

    // intercepta o caso LONG_MIN / -1
    if (v1 == (int64_t)0x8000000000000000LL && v2 == -1) {
        div_result = (int64_t)0x8000000000000000LL; // a JVM exige que seja LONG_MIN
    } else {
        div_result = v1 / v2;
    }

    u4 high = (u4)(div_result >> 32);
    u4 low  = (u4)(div_result & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da pop na operand stack 2x (longs) e da push do produto dos operandos
u4 handle_lmul(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v2 = (((int64_t)high2) << 32) | low2;

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v1 = (((int64_t)high1) << 32) | low1;

    int64_t mul_result = v1 * v2;

    u4 high = (u4)(mul_result >> 32);
    u4 low  = (u4)(mul_result & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da pop na operand stack 2x (longs) e da push do resto da divisao dos operandos
u4 handle_lrem(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v2 = (((int64_t)high2) << 32) | low2;

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v1 = (((int64_t)high1) << 32) | low1;

    if (v2 == 0) {
        // TODO: lançar excecao para divisao por 0
        printf("resto por 0 fi\n");
        exit(1);
    }

    int64_t rem_result;

    // intercepta o caso LONG_MIN % -1
    if (v1 == (int64_t)0x8000000000000000LL && v2 == -1) {
        rem_result = 0; // a JVM exige que seja 0
    } else {
        rem_result = v1 % v2;
    }

    u4 high = (u4)(rem_result >> 32);
    u4 low  = (u4)(rem_result & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da pop em um int (shift) e um long (v1) e da push do long deslocado para a esquerda
u4 handle_lshl(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    // O valor de shift (int) ocupa apenas 1 palavra de 32 bits no topo da pilha
    u4 s = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int shift = s & 0x3F; // Apenas os 6 bits menos significativos importam para longs (0-63)

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v1 = (((int64_t)high1) << 32) | low1;

    int64_t res = v1 << shift;

    u4 high = (u4)(res >> 32);
    u4 low  = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da pop em um int (shift) e um long (v1) e da push do long com deslocamento aritmetico para a direita (mantem sinal)
u4 handle_lshr(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 s = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int shift = s & 0x3F;

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v1 = (((int64_t)high1) << 32) | low1;

    int64_t res = v1 >> shift;

    u4 high = (u4)(res >> 32);
    u4 low  = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da pop em um int (shift) e um long (v1) e da push do long com deslocamento logico para a direita (preenche com zeros)
u4 handle_lushr(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 s = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int shift = s & 0x3F;

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    
    // Tratamos como unsigned para garantir o shift lógico (com preenchimento por 0 à esquerda)
    uint64_t v1 = (((uint64_t)high1) << 32) | low1;

    uint64_t res = v1 >> shift;

    u4 high = (u4)(res >> 32);
    u4 low  = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da pop na operand stack de um long e da push do operando negativo
u4 handle_lneg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    // Desempilha os 64 bits (primeiro low, depois high)
    u4 low = *((u4*) getTop(frame->operand_stack)); 
    pop(frame->operand_stack);
    
    u4 high = *((u4*) getTop(frame->operand_stack)); 
    pop(frame->operand_stack);

    // Combina os u4 em um int64_t para operação aritmética
    int64_t value = (((int64_t)high) << 32) | low;

    // Aplica a negação
    int64_t neg_result = -value;

    // Divide o resultado novamente em high e low para empilhar
    u4 res_high = (u4)(neg_result >> 32);
    u4 res_low  = (u4)(neg_result & 0xFFFFFFFF);

    // Empilha de volta (primeiro high, depois low)
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);

    return pc + 1;
}

u4 handle_ifeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val == 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_ifne(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val != 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_iflt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val < 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_ifge(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val >= 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_ifgt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val > 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_ifle(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val <= 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_icmpeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 == v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_icmpne(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 != v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_icmplt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 < v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_icmpge(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 >= v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_icmpgt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 > v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_icmple(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 <= v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

// altera o pc de acordo com o short dado como parametro
u4 handle_goto(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u2 branch_bytes = (((u2)code[pc + 1] << 8) | code[pc + 2]);
    return (u4)(pc + compute_branch(branch_bytes));
}

u4 handle_ireturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    u4 return_pc = frame->return_pc;

    MarkClinitDoneIfApplicable(ctx->method_area, frame);

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&value);
    }

    return return_pc;
}

u4 handle_lreturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 low = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 return_pc = frame->return_pc;

    MarkClinitDoneIfApplicable(ctx->method_area, frame);

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&high);
        push(caller->operand_stack, (void*)&low);
    }

    return return_pc;
}

u4 handle_freturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    u4 return_pc = frame->return_pc;

    MarkClinitDoneIfApplicable(ctx->method_area, frame);

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&value);
    }

    return return_pc;
}

u4 handle_dreturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 low = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 return_pc = frame->return_pc;

    MarkClinitDoneIfApplicable(ctx->method_area, frame);

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&high);
        push(caller->operand_stack, (void*)&low);
    }

    return return_pc;
}

u4 handle_areturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    u4 return_pc = frame->return_pc;

    MarkClinitDoneIfApplicable(ctx->method_area, frame);

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&value);
    }

    return return_pc;
}

u4 handle_return(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;

    Frame *frame = (Frame*)getTop(thread->frame_stack);
    u4 return_pc = frame->return_pc;

    MarkClinitDoneIfApplicable(ctx->method_area, frame);

    pop(thread->frame_stack);

    return return_pc;
}

u4 handle_invokestatic(RuntimeContext *ctx, Code_attribute *code_attr) {
    printf("n implementou ainda pai o invokestatic\n");
    //(void)frame;
    //u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve method, create new frame, push to stack
    //return pc + 3;
}

u4 handle_invokevirtual(RuntimeContext *ctx, Code_attribute *code_attr) {
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);
    Cp_info *cp = frame->class_file->constant_pool;
    u4 pc = ctx->thread->pc;
    u1 *code = code_attr->code;

    u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    Cp_info methodref = cp[method_index];

    u2 class_idx = methodref.info.Methodref.class_index;
    u2 class_name_idx = cp[class_idx].info.Class.name_index;
    char *class_name = (char*) cp[class_name_idx].info.Utf8.bytes;
    u2 class_name_len = cp[class_name_idx].info.Utf8.length;

    u2 nat_idx = methodref.info.Methodref.name_and_type_index;
    u2 name_idx = cp[nat_idx].info.NameAndType.name_index;
    u2 desc_idx = cp[nat_idx].info.NameAndType.descriptor_index;
    char *method_name = (char*) cp[name_idx].info.Utf8.bytes;
    u2 method_name_len = cp[name_idx].info.Utf8.length;
    char *descriptor = (char*) cp[desc_idx].info.Utf8.bytes;
    u2 descriptor_len = cp[desc_idx].info.Utf8.length;

    if (class_name_len == 19 && strncmp(class_name, "java/io/PrintStream", 19) == 0) {
        return dispatch_printstream(ctx->thread, ctx->reference_map, method_name_len, descriptor, pc);
    }
    if (class_name_len == 22 && strncmp(class_name, "java/lang/StringBuffer", 22) == 0) {
        return dispatch_stringbuffer(ctx->thread, ctx->reference_map, method_name_len, method_name, descriptor, pc);
    }

    // Despacho de usuário: invokevirtual é polimórfico — a busca tem que começar na
    // classe REAL do objeto (obj->class_ref), nunca na classe estática do Methodref.
    u4 arg_words[256];
    u2 total_slots = PopArguments(frame, descriptor, arg_words);

    u4 objectref = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    if (objectref == 0) {
        printf("NullPointerException em invokevirtual\n");
        exit(1);
    }

    JVMObject *obj = (JVMObject*) ctx->reference_map->entries[objectref];
    if (obj == NULL) {
        printf("NullPointerException: objeto não encontrado no ReferenceMap em invokevirtual\n");
        exit(1);
    }

    ClassFile *owner = NULL;
    Method_info *method = ResolveMethod(ctx->method_area, obj->class_ref,
                                         method_name, method_name_len,
                                         descriptor, descriptor_len, &owner);

    if (method == NULL) {
        fprintf(stderr, "AbstractMethodError: %.*s.%.*s%.*s\n",
                class_name_len, class_name, method_name_len, method_name, descriptor_len, descriptor);
        exit(1);
    }
    if (method->access_flags & 0x0008) { // ACC_STATIC
        fprintf(stderr, "IncompatibleClassChangeError: invokevirtual em método estático (%.*s.%.*s)\n",
                class_name_len, class_name, method_name_len, method_name);
        exit(1);
    }

    return PushUserMethodFrame(ctx->thread, owner, method, objectref, arg_words, total_slots, pc + 3);
}

u4 handle_invokespecial(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);
    Cp_info *cp = frame->class_file->constant_pool;
    u1 *code = code_attr->code;

    u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    Cp_info methodref = cp[method_index];

    u2 class_idx = methodref.info.Methodref.class_index;
    u2 class_name_idx = cp[class_idx].info.Class.name_index;
    char *class_name = (char*) cp[class_name_idx].info.Utf8.bytes;
    u2 class_name_len = cp[class_name_idx].info.Utf8.length;

    u2 nat_idx = methodref.info.Methodref.name_and_type_index;
    u2 name_idx = cp[nat_idx].info.NameAndType.name_index;
    u2 desc_idx = cp[nat_idx].info.NameAndType.descriptor_index;
    char *method_name = (char*) cp[name_idx].info.Utf8.bytes;
    u2 method_name_len = cp[name_idx].info.Utf8.length;
    char *descriptor = (char*) cp[desc_idx].info.Utf8.bytes;
    u2 descriptor_len = cp[desc_idx].info.Utf8.length;

    u1 is_init = (method_name_len == 6 && strncmp(method_name, "<init>", 6) == 0);

    if (is_init && is_native_class(class_name, class_name_len)) {
        // Object/String/StringBuffer: objeto já populado em 'new'.
        pop(frame->operand_stack);
        return pc + 3;
    }

    // invokespecial resolve SEMPRE a partir da classe gravada no constant pool
    // (nunca da classe real do objeto) — cobre <init>, método privado e super.metodo()
    // sem comportamento polimórfico.
    ClassFile *resolved_class = get_class_from_constant_pool(ctx->thread, ctx->method_area, frame, class_name);

    // NULL = um frame de <clinit> foi empilhado (classe ainda não inicializada).
    // O pc não avança, então essa mesma instrução roda de novo quando a execução
    // voltar pra este frame — mesmo padrão que 'new'/'anewarray' já usam.
    if (resolved_class == NULL) {
        return ctx->thread->pc;
    }

    ClassFile *owner = NULL;
    Method_info *method = ResolveMethod(ctx->method_area, resolved_class,
                                         method_name, method_name_len,
                                         descriptor, descriptor_len, &owner);

    if (method == NULL) {
        fprintf(stderr, "NoSuchMethodError: %.*s.%.*s%.*s\n",
                class_name_len, class_name, method_name_len, method_name, descriptor_len, descriptor);
        exit(1);
    }
    if (method->access_flags & 0x0008) { // ACC_STATIC
        fprintf(stderr, "IncompatibleClassChangeError: invokespecial em método estático (%.*s.%.*s)\n",
                class_name_len, class_name, method_name_len, method_name);
        exit(1);
    }

    u4 arg_words[256];
    u2 total_slots = PopArguments(frame, descriptor, arg_words);

    u4 objectref = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    if (objectref == 0) {
        printf("NullPointerException em invokespecial\n");
        exit(1);
    }

    return PushUserMethodFrame(ctx->thread, owner, method, objectref, arg_words, total_slots, pc + 3);
}

u4 handle_invokeinterface(RuntimeContext *ctx, Code_attribute *code_attr) {
    printf("n implementou ainda pai o invokeinterface\n");
    //(void)frame;
    // TODO: parse method reference and optional count field
    //return pc + 5;
}

u4 handle_getstatic(RuntimeContext *ctx, Code_attribute *code_attr) {
    
    // 1. Pega o frame atual e o constant pool da classe
    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;

    // 2. Pega o índice do campo no constant pool a partir do bytecode
    u4 pc = thread->pc;
    u1* code = code_attr->code;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    
    // 3. Checa se o entry do constant pool é do tipo Fieldref
    Cp_info field_ref = constant_pool[field_index];

    if (field_ref.tag != CONSTANT_Fieldref) {
        fprintf(stderr, "Invalid constant pool entry for getstatic\n");
        exit(1);
    }

    u2 class_idx = field_ref.info.Fieldref.class_index;
    u2 class_name_idx = constant_pool[class_idx].info.Class.name_index;
    char *class_name = (char*) constant_pool[class_name_idx].info.Utf8.bytes;
    u2 class_name_len = constant_pool[class_name_idx].info.Utf8.length;

    if (class_name_len == 16 && strncmp(class_name, "java/lang/System", 16) == 0) {
        // sentinela qualquer — invokevirtual nunca vai de-referenciar isso,
        // ele intercepta PrintStream.println/print antes
        u4 sentinel = 0;
        push(frame->operand_stack, (void*)&sentinel);
        return pc + 3;
    }

    // 4. Resgata o array de static fields da classe e o entry do method area correspondente
    MethodArea *method_area = ctx->method_area;
    MethodAreaEntry *entry = MethodAreaGetEntry(method_area, class_name);
    if (entry == NULL) {
        fprintf(stderr,
            "Class %s not found in Method Area\n",
            class_name);
        exit(1);
    }
    StaticField *static_fields = entry->static_fields;

    u2 field_idx = field_ref.info.Fieldref.name_and_type_index;
    Cp_info name_and_type = constant_pool[field_idx];
    u2 target_name_index = name_and_type.info.NameAndType.name_index;
    u2 target_descriptor_index = name_and_type.info.NameAndType.descriptor_index;
    char *descriptor = (char*)constant_pool[target_descriptor_index].info.Utf8.bytes;

    u2 static_field_count = entry->static_field_count;

    // 5. Itera sobre os static fields para encontrar o campo correspondente e empilha seu valor na operand stack
    for (u2 i = 0; i < static_field_count; i++) {
        StaticField *static_field = &static_fields[i];
        Field_info *field = &entry->class_file->fields[static_field->field_index];

        if (field->name_index == target_name_index && field->descriptor_index == target_descriptor_index) {
            if (descriptor[0] == 'J' || descriptor[0] == 'D'){
                push(frame->operand_stack, &static_field->value[1]); // high
                push(frame->operand_stack, &static_field->value[0]); // low
            }
            else{
                push(frame->operand_stack, &static_field->value[0]);
            }
            break;
        }
    }
    
    return pc + 3;
}

u4 handle_getfield(RuntimeContext *ctx, Code_attribute *code_attr) {
    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;

    u4 pc = thread->pc;
    u1* code = code_attr->code;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];

    Cp_info field_ref = constant_pool[field_index];
    u2 class_index = field_ref.info.Fieldref.class_index;
    u2 name_and_type_index = field_ref.info.Fieldref.name_and_type_index;

    // classe alvo apontada pelo fieldref
    u2 class_name_idx = constant_pool[class_index].info.Class.name_index;
    char* target_class_name = (char*)constant_pool[class_name_idx].info.Utf8.bytes;
    u2 target_class_name_len = constant_pool[class_name_idx].info.Utf8.length;

    // ome e descritor do campo procurado
    u2 target_name_idx = constant_pool[name_and_type_index].info.NameAndType.name_index;
    u2 target_desc_idx = constant_pool[name_and_type_index].info.NameAndType.descriptor_index;

    char* target_name = (char*)constant_pool[target_name_idx].info.Utf8.bytes;
    u2 target_name_len = constant_pool[target_name_idx].info.Utf8.length;
    char* target_desc = (char*)constant_pool[target_desc_idx].info.Utf8.bytes;
    u2 target_desc_len = constant_pool[target_desc_idx].info.Utf8.length;

    // desempilha a chave de referência do objeto (objectref)
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); 
    pop(frame->operand_stack);

    if (ref_key == 0) {
        printf("NullPointerException em getfield\n");
        exit(1);
    }

    JVMObject* obj = (JVMObject*) ctx->reference_map->entries[ref_key];
    if (obj == NULL) {
        printf("NullPointerException: Objeto não encontrado no ReferenceMap\n");
        exit(1);
    }

    // determina estritamente qual classe na árvore de tipos introduziu o campo referenciado.
    char res_class_name[target_class_name_len + 1];
    memcpy(res_class_name, target_class_name, target_class_name_len);
    res_class_name[target_class_name_len] = '\0';

    MethodAreaEntry *res_entry = MethodAreaGetEntry(ctx->method_area, res_class_name);
    ClassFile* res_class = res_entry ? res_entry->class_file : NULL;

    ClassFile* declaring_class = NULL;
    Field_info* resolved_field = NULL;

    while (res_class != NULL) {
        for (u2 i = 0; i < res_class->fields_count; i++) {
            Field_info* f = &res_class->fields[i];
            char* f_name = (char*)res_class->constant_pool[f->name_index].info.Utf8.bytes;
            u2 f_name_len = res_class->constant_pool[f->name_index].info.Utf8.length;
            char* f_desc = (char*)res_class->constant_pool[f->descriptor_index].info.Utf8.bytes;
            u2 f_desc_len = res_class->constant_pool[f->descriptor_index].info.Utf8.length;

            if (f_name_len == target_name_len && strncmp(f_name, target_name, target_name_len) == 0 &&
                f_desc_len == target_desc_len && strncmp(f_desc, target_desc, target_desc_len) == 0) {
                declaring_class = res_class;
                resolved_field = f;
                break;
            }
        }
        if (declaring_class != NULL) break;
        
        // sobe na hierarquia da classe alvo se não achar nela
        if (res_class->super_class == 0) {
            res_class = NULL;
        } else {
            u2 s_name_in = res_class->constant_pool[res_class->super_class].info.Class.name_index;
            u2 s_len = res_class->constant_pool[s_name_in].info.Utf8.length;
            char s_name[s_len + 1];
            memcpy(s_name, res_class->constant_pool[s_name_in].info.Utf8.bytes, s_len);
            s_name[s_len] = '\0';
            
            MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
            res_class = entry ? entry->class_file : NULL;
        }
    }

    if (declaring_class == NULL) {
        printf("NoSuchFieldError: Campo não encontrado na resolução de tipos\n");
        exit(1);
    }

    // se o campo resolvido for estático getfield aborta.
    if ((resolved_field->access_flags & 0x0008) != 0) { // ACC_STATIC
        printf("IncompatibleClassChangeError: Tentativa de usar getfield em um campo estático\n");
        exit(1);
    }

    // monta a hierarquia real do objeto instanciado
    ClassFile* hierarchy[128];
    int h_count = 0;
    ClassFile* curr = obj->class_ref;
    
    while (curr != NULL && h_count < 128) {
        hierarchy[h_count++] = curr;
        
        if (curr->super_class == 0) {
            curr = NULL; 
        } 
        else {
            u2 s_name_in = curr->constant_pool[curr->super_class].info.Class.name_index;
            u2 s_len = curr->constant_pool[s_name_in].info.Utf8.length;
            char s_name[s_len + 1];
            memcpy(s_name, curr->constant_pool[s_name_in].info.Utf8.bytes, s_len);
            s_name[s_len] = '\0';
            
            MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
            curr = entry ? entry->class_file : NULL;
        }
    }

    u4 slot_offset = 0;
    u1 field_found = 0;
    char field_type_char = '\0';

    // varre de cima para baixo
    for (int h = h_count - 1; h >= 0; h--) {
        ClassFile* cf_curr = hierarchy[h];
        
        for (u2 i = 0; i < cf_curr->fields_count; i++) {
            Field_info* f = &cf_curr->fields[i];

            if ((f->access_flags & 0x0008) == 0) {
                if (cf_curr == declaring_class && f == resolved_field) {
                    field_type_char = cf_curr->constant_pool[f->descriptor_index].info.Utf8.bytes[0];
                    field_found = 1;
                    break;
                }

                char type = cf_curr->constant_pool[f->descriptor_index].info.Utf8.bytes[0];
                slot_offset += ((type == 'J' || type == 'D') ? 2 : 1);
            }
        }
        if (field_found) break;
    }

    if (!field_found) {
        printf("NoSuchFieldError: Campo resolvido não existe na hierarquia do objeto real instanciado\n");
        exit(1);
    }

    if (field_type_char == 'J' || field_type_char == 'D') {
        u4 high = obj->fields[slot_offset];
        u4 low  = obj->fields[slot_offset + 1];
        push(frame->operand_stack, (void*)&high);
        push(frame->operand_stack, (void*)&low);
    } 
    else {
        u4 value = obj->fields[slot_offset];
        push(frame->operand_stack, (void*)&value);
    }

    return pc + 3;
}

u4 handle_putfield(RuntimeContext *ctx, Code_attribute *code_attr) {
    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;

    u4 pc = thread->pc;
    u1* code = code_attr->code;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];

    Cp_info field_ref = constant_pool[field_index];
    u2 class_index = field_ref.info.Fieldref.class_index;
    u2 name_and_type_index = field_ref.info.Fieldref.name_and_type_index;

    // classe alvo apontada pelo fieldref
    u2 class_name_idx = constant_pool[class_index].info.Class.name_index;
    char* target_class_name = (char*)constant_pool[class_name_idx].info.Utf8.bytes;
    u2 target_class_name_len = constant_pool[class_name_idx].info.Utf8.length;

    // nome e descritor do campo procurado
    u2 target_name_idx = constant_pool[name_and_type_index].info.NameAndType.name_index;
    u2 target_desc_idx = constant_pool[name_and_type_index].info.NameAndType.descriptor_index;

    char* target_name = (char*)constant_pool[target_name_idx].info.Utf8.bytes;
    u2 target_name_len = constant_pool[target_name_idx].info.Utf8.length;
    char* target_desc = (char*)constant_pool[target_desc_idx].info.Utf8.bytes;
    u2 target_desc_len = constant_pool[target_desc_idx].info.Utf8.length;

    // determina estritamente qual classe na árvore de tipos introduziu o campo referenciado
    char res_class_name[target_class_name_len + 1];
    memcpy(res_class_name, target_class_name, target_class_name_len);
    res_class_name[target_class_name_len] = '\0';

    MethodAreaEntry *res_entry = MethodAreaGetEntry(ctx->method_area, res_class_name);
    ClassFile* res_class = res_entry ? res_entry->class_file : NULL;

    ClassFile* declaring_class = NULL;
    Field_info* resolved_field = NULL;

    while (res_class != NULL) {
        for (u2 i = 0; i < res_class->fields_count; i++) {
            Field_info* f = &res_class->fields[i];
            char* f_name = (char*)res_class->constant_pool[f->name_index].info.Utf8.bytes;
            u2 f_name_len = res_class->constant_pool[f->name_index].info.Utf8.length;
            char* f_desc = (char*)res_class->constant_pool[f->descriptor_index].info.Utf8.bytes;
            u2 f_desc_len = res_class->constant_pool[f->descriptor_index].info.Utf8.length;

            if (f_name_len == target_name_len && strncmp(f_name, target_name, target_name_len) == 0 &&
                f_desc_len == target_desc_len && strncmp(f_desc, target_desc, target_desc_len) == 0) {
                declaring_class = res_class;
                resolved_field = f;
                break;
            }
        }
        if (declaring_class != NULL) break;
        
        // sobe na hierarquia da classe alvo se não encontrar nela
        if (res_class->super_class == 0) {
            res_class = NULL;
        } else {
            u2 s_name_in = res_class->constant_pool[res_class->super_class].info.Class.name_index;
            u2 s_len = res_class->constant_pool[s_name_in].info.Utf8.length;
            char s_name[s_len + 1];
            memcpy(s_name, res_class->constant_pool[s_name_in].info.Utf8.bytes, s_len);
            s_name[s_len] = '\0';
            
            MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
            res_class = entry ? entry->class_file : NULL;
        }
    }

    if (declaring_class == NULL) {
        printf("NoSuchFieldError: Campo não encontrado na resolução de tipos\n");
        exit(1);
    }
    
    // se o campo resolvido for estático putfield aborta
    if ((resolved_field->access_flags & 0x0008) != 0) { // ACC_STATIC
        printf("IncompatibleClassChangeError: Tentativa de usar putfield em um campo estático\n");
        exit(1);
    }

    // desempilha os valores
    char field_type_char = declaring_class->constant_pool[resolved_field->descriptor_index].info.Utf8.bytes[0];
    u4 value = 0, high = 0, low = 0;

    if (field_type_char == 'J' || field_type_char == 'D') {
        low = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
        high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    } 
    else {
        value = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    }

    // desempilha a referencia do objeto
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); 
    pop(frame->operand_stack);

    if (ref_key == 0) {
        printf("NullPointerException em putfield\n");
        exit(1);
    }

    JVMObject* obj = (JVMObject*) ctx->reference_map->entries[ref_key];
    if (obj == NULL) {
        printf("NullPointerException: Objeto não encontrado no ReferenceMap\n");
        exit(1);
    }

    // monta a hierarquia real do objeto instanciado
    ClassFile* hierarchy[128];
    int h_count = 0;
    ClassFile* curr = obj->class_ref;
    
    while (curr != NULL && h_count < 128) {
        hierarchy[h_count++] = curr;
        
        if (curr->super_class == 0) {
            curr = NULL; 
        } 
        else {
            u2 s_name_in = curr->constant_pool[curr->super_class].info.Class.name_index;
            u2 s_len = curr->constant_pool[s_name_in].info.Utf8.length;
            char s_name[s_len + 1];
            memcpy(s_name, curr->constant_pool[s_name_in].info.Utf8.bytes, s_len);
            s_name[s_len] = '\0';
            
            MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
            curr = entry ? entry->class_file : NULL;
        }
    }

    u4 slot_offset = 0;
    u1 field_found = 0;

    // varre de cima para baixo para calcular o offset
    for (int h = h_count - 1; h >= 0; h--) {
        ClassFile* cf_curr = hierarchy[h];
        
        for (u2 i = 0; i < cf_curr->fields_count; i++) {
            Field_info* f = &cf_curr->fields[i];

            if ((f->access_flags & 0x0008) == 0) {
                if (cf_curr == declaring_class && f == resolved_field) {
                    field_found = 1;
                    break;
                }

                char type = cf_curr->constant_pool[f->descriptor_index].info.Utf8.bytes[0];
                slot_offset += ((type == 'J' || type == 'D') ? 2 : 1);
            }
        }
        if (field_found) break;
    }

    if (!field_found) {
        printf("NoSuchFieldError: Campo resolvido não existe na hierarquia do objeto real instanciado\n");
        exit(1);
    }

    // atribui os valores no heap do objeto
    if (field_type_char == 'J' || field_type_char == 'D') {
        obj->fields[slot_offset] = high;
        obj->fields[slot_offset + 1] = low;
    } 
    else {
        obj->fields[slot_offset] = value;
    }

    return pc + 3;
}

u4 handle_putstatic(RuntimeContext *ctx, Code_attribute *code_attr) {

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;
    u4 pc = thread->pc;
    u1 *code = code_attr->code;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];

    Cp_info field_ref = constant_pool[field_index];

    if (field_ref.tag != CONSTANT_Fieldref) {
        fprintf(stderr, "Invalid constant pool entry for putstatic\n");
        exit(1);
    }

    u2 class_idx = field_ref.info.Fieldref.class_index;
    u2 class_name_idx = constant_pool[class_idx].info.Class.name_index;
    char *class_name = (char*)constant_pool[class_name_idx].info.Utf8.bytes;

    MethodArea *method_area = ctx->method_area;
    MethodAreaEntry *entry = MethodAreaGetEntry(method_area, class_name);

    if (entry == NULL) {
        fprintf(stderr,
                "Class %s not found in Method Area\n",
                class_name);
        exit(1);
    }

    StaticField *static_fields = entry->static_fields;
    u2 static_field_count = entry->static_field_count;
    u2 nt_index = field_ref.info.Fieldref.name_and_type_index;
    Cp_info name_and_type = constant_pool[nt_index];
    u2 target_name_index = name_and_type.info.NameAndType.name_index;
    u2 target_descriptor_index = name_and_type.info.NameAndType.descriptor_index;

    char *descriptor = (char*)constant_pool[target_descriptor_index].info.Utf8.bytes;

    for (u2 i = 0; i < static_field_count; i++) {

        StaticField *static_field = &static_fields[i];

        Field_info *field =
            &entry->class_file->fields[static_field->field_index];

        if (field->name_index == target_name_index && field->descriptor_index == target_descriptor_index){
            if (descriptor[0] == 'J' || descriptor[0] == 'D'){
                u4 low = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
                u4 high = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);

                static_field->value[0] = low;
                static_field->value[1] = high;
            }
            else{
                u4 value = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);

                static_field->value[0] = value;
                static_field->value[1] = 0;
            }

            return pc + 3;
        }
    }

    fprintf(stderr, "Static field not found\n");
    exit(1);
}

u4 handle_new(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    // 1. Pega o índice no constant pool
    u2 class_index = (code_attr->code[pc + 1] << 8) | code_attr->code[pc + 2];

    ClassFile* current_class = frame->class_file;

    // Pega a entrada da classe no pool
    Cp_info class_info = current_class->constant_pool[class_index];
    u2 name_index = class_info.info.Class.name_index;

    // Extrai o nome UTF-8 e converte para string C (null-terminated)
    u2 name_length = current_class->constant_pool[name_index].info.Utf8.length;
    u1* name_bytes = current_class->constant_pool[name_index].info.Utf8.bytes;

    char class_name[name_length + 1];
    memcpy(class_name, name_bytes, name_length);
    class_name[name_length] = '\0';

    // caso nativo java/lang/String
    if (name_length == 16 && strncmp(class_name, "java/lang/String", 16) == 0) {
        char *str = (char*) malloc(1);
        str[0] = '\0'; // new String() == ""

        if (reference_map->size >= MAX_REF_MAP) {
            printf("OutOfMemoryError: ReferenceMap cheio!\n");
            exit(1);
        }
        u4 ref_key = reference_map->size++;
        reference_map->entries[ref_key] = (void*) str;
        push(frame->operand_stack, (void*)&ref_key);
        return pc + 3;
    }

    // caso nativo java/lang/StringBuffer
    if (name_length == 22 && strncmp(class_name, "java/lang/StringBuffer", 22) == 0) {
        JVMStringBuffer *buf = (JVMStringBuffer*) malloc(sizeof(JVMStringBuffer));
        buf->capacity = 16;
        buf->length = 0;
        buf->data = (char*) malloc(buf->capacity);
        buf->data[0] = '\0';

        if (reference_map->size >= MAX_REF_MAP) {
            printf("OutOfMemoryError: ReferenceMap cheio!\n"); exit(1);
        }
        u4 ref_key = reference_map->size++;
        reference_map->entries[ref_key] = (void*) buf;
        push(frame->operand_stack, (void*)&ref_key);
        return pc + 3;
    }

    // 2. Resolve a classe!
    ClassFile* resolved_class = get_class_from_constant_pool(ctx->thread, ctx->method_area, frame, class_name);

    // SE RETORNOU NULL, UM NOVO FRAME (<clinit>) FOI EMPILHADO!
    if (resolved_class == NULL) {
        return ctx->thread->pc; 
    }

    JVMObject* new_obj = CreateObject(ctx->method_area, resolved_class);
    if (new_obj == NULL){ 
        printf("OutOfMemoryError em 'new'\n"); 
        exit(1); 
    }

    u4 ref_key = reference_map->size;
    reference_map->size++;

    if (ref_key >= MAX_REF_MAP) {
        printf("OutOfMemoryError: ReferenceMap cheio!\n");
        exit(1);
    }

    reference_map->entries[ref_key] = (void*)new_obj;
    push(frame->operand_stack, (void*)&ref_key);

    return pc + 3;
}

u4 handle_newarray(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    // 1. O tipo primitivo do array (ex: JVM_ATYPE_INT, JVM_ATYPE_FLOAT)
    u1 atype = code_attr->code[pc + 1];

    // 2. Pop do tamanho do array (count)
    int32_t count = (int32_t)(*((u4*) getTop(frame->operand_stack))); pop(frame->operand_stack);

    // 3. Verificação de segurança obrigatória para arrays
    if (count < 0) {
        printf("NegativeArraySizeException em newarray (count = %d)\n", count);
        exit(1);
    }

    // 4. Alocação da estrutura do Array
    JVMArray* new_array = (JVMArray*) malloc(sizeof(JVMArray));
    if (new_array == NULL) {
        printf("OutOfMemoryError: Não foi possível alocar JVMArray em newarray\n");
        exit(1);
    }

    new_array->length = count;
    new_array->atype = atype;
    
    // 5. Alocar o bloco de dados correto usando o ArrayTypeSize do seu allocation.h
    size_t element_size = ArrayTypeSize(atype);
    
    // Usamos calloc para garantir que valores numéricos iniciem com 0 (ou 0.0) e booleanos com false
    new_array->data = calloc(count, element_size);
    
    if (count > 0 && new_array->data == NULL) {
        printf("OutOfMemoryError: Não foi possível alocar dados do array em newarray\n");
        free(new_array);
        exit(1);
    }

    u4 ref_key = reference_map->size;
    reference_map->size++;

    if (ref_key > MAX_REF_MAP) {
        printf("OutOfMemoryError: ReferenceMap cheio!\n");
        exit(1);
    }

    reference_map->entries[ref_key] = (void*)new_array;

    // 7. Push da nova chave (ref_key) para a pilha de operandos
    push(frame->operand_stack, (void*)&ref_key);

    // newarray tem 2 bytes: 1 de opcode + 1 de atype
    return pc + 2;
}

u4 handle_anewarray(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    u1 *code = code_attr->code;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    // 1. O tipo de referência do array (índice no constant pool)
    u2 idx = ((u2)code[pc + 1] << 8) | (u2)code[pc + 2];
    u1 atype = 12; // Arrays de referência sempre têm atype = JVM_ATYPE_REF

    // 2. Pop do tamanho do array (count)
    int32_t count = (int32_t)(*((u4*) getTop(frame->operand_stack))); pop(frame->operand_stack);

    // 3. Verificação de segurança obrigatória para arrays
    if (count < 0) {
        printf("NegativeArraySizeException em newarray (count = %d)\n", count);
        exit(1);
    }

    // 4. Alocação da estrutura do Array
    JVMArray* new_array = (JVMArray*) malloc(sizeof(JVMArray));
    if (new_array == NULL) {
        printf("OutOfMemoryError: Não foi possível alocar JVMArray em newarray\n");
        exit(1);
    }

    new_array->length = count;
    new_array->atype = atype; 
    
    // 5. Alocar o bloco de dados correto usando o tamanho do elemento de referência (tamanho de ponteiro)
    MethodArea *method_area = ctx->method_area;
    Cp_info *constant_pool = frame->class_file->constant_pool;
    Cp_info class = constant_pool[idx];
    if (class.tag != CONSTANT_Class) {
        printf("Invalid constant pool entry for anewarray\n");
        exit(1);
    }
    else {
        u2 name_index = class.info.Class.name_index;
        char *comp_name = (char*) constant_pool[name_index].info.Utf8.bytes;
        u2 comp_name_len = constant_pool[name_index].info.Utf8.length;

        // Classes nativas (String, StringBuffer, ...) não têm ClassFile real
        // carregado na MethodArea, então pulamos a validação pra elas.
        if (!is_native_class(comp_name, comp_name_len)) {
            ClassFile *resolved_class = get_class_from_constant_pool(ctx->thread, ctx->method_area, frame, comp_name);
            // <se for NULL, clinit foi empilhado e volta pra executar
            if (resolved_class == NULL) {
                return ctx->thread->pc;
            }
        }
    }

    // 6. Alocar o bloco de dados para o array de referências (tamanho de ponteiro para as referências)
    size_t element_size = sizeof(u4);
    
    // Usamos calloc para garantir que valores numéricos iniciem com 0 (ou 0.0) e booleanos com false
    new_array->data = calloc(count, element_size);
    
    if (count > 0 && new_array->data == NULL) {
        printf("OutOfMemoryError: Não foi possível alocar dados do array em newarray\n");
        free(new_array);
        exit(1);
    }

    if (reference_map->size >= MAX_REF_MAP) {
        printf("OutOfMemoryError: ReferenceMap cheio!\n");
        exit(1);
    }
    u4 ref_key = reference_map->size++;

    reference_map->entries[ref_key] = (void*)new_array;

    // 7. Push da nova chave (ref_key) para a pilha de operandos
    push(frame->operand_stack, (void*)&ref_key);

    // anewarray tem 3 bytes: 1 de opcode + 2 de índice no constant pool
    return pc + 3;
}

// da pop de uma referencia de array e da push do seu comprimento
u4 handle_arraylength(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    // 1. pop da referencia do array
    u4 ref_key = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    // 2. checa se a referencia existe no reference map
    if (ref_key >= reference_map->size) {
        printf("NullPointerException: ref_key (%u) invalida em arraylength\n", ref_key);
        exit(1);
    }

    // 3. resgata o array do reference map
    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    // 4. da push do comprimento do array como int (u4)
    u4 length = arrayref->length;
    push(frame->operand_stack, (void*)&length);

    return pc + 1;
}

// da push de uma variavel local de referencia no indice idx na operand stack
u4 handle_aload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = frame->local_variables[parameter];
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da pop na operand stack e armazena referencia em variaveis locais no indice idx
u4 handle_astore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[parameter] = content;
    return pc + 2;
}

// pop na stack de operandos
u4 handle_pop(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    pop(frame->operand_stack);
    return pc + 1;
}

// Remove os dois valores do topo da stack de operandos
u4 handle_pop2(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    pop(frame->operand_stack);
    pop(frame->operand_stack);

    return pc + 1;
}

// duplica o topo da stack
u4 handle_dup(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand = *((u4*)getTop(frame->operand_stack));
    push(frame->operand_stack, (void*)&operand);
    return pc + 1;
}

// faz o swap do topo da stack com o valor imediatamente anterior
u4 handle_swap(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand_1 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 operand_2 = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    push(frame->operand_stack, (void*)&operand_1);
    push(frame->operand_stack, (void*)&operand_2);
    return pc + 1;
}

// verifica se a classe thrown_class eh subclasse ou a mesma que target_class_name
// percorre a hierarquia de superclasses usando o MethodArea
static u1 is_subclass_of(JVMObject *obj, MethodArea *method_area, const char *target_name, u2 target_len) {
    ClassFile *cf = obj->class_ref;
    while (cf != NULL) {
        // compara o nome da classe atual
        char *current_name = GetClassName(cf, cf->this_class);
        u2 current_len = (u2)strlen(current_name);
        if (current_len == target_len && strncmp(current_name, target_name, target_len) == 0) {
            return 1;
        }
        // sobe pra superclasse
        if (cf->super_class == 0) {
            break;
        }
        char *super_name = GetClassName(cf, cf->super_class);
        if (super_name == NULL) break;
        MethodAreaEntry *entry = MethodAreaGetEntry(method_area, super_name);
        cf = (entry != NULL) ? entry->class_file : NULL;
    }
    return 0;
}

// procura um handler de excecao na exception_table do frame atual
// retorna o handler_pc ou 0 se nao encontrar
static u2 find_exception_handler(Frame *frame, Code_attribute *code_attr, u4 exception_pc, JVMObject *exc_obj, MethodArea *method_area) {
    u2 table_len = code_attr->exception_table_length;
    for (u2 i = 0; i < table_len; i++) {
        Exception_code *entry = &code_attr->exception_table[i];
        // verifica se o PC esta dentro do intervalo protegido [start_pc, end_pc)
        if (exception_pc < entry->start_pc || exception_pc >= entry->end_pc) {
            continue;
        }
        // catch_type == 0 captura qualquer excecao (finally)
        if (entry->catch_type == 0) {
            return entry->handler_pc;
        }
        // catch_type > 0: verifica se a excecao IS-A a classe referenciada
        Cp_info *cp = frame->class_file->constant_pool;
        u2 class_name_idx = cp[entry->catch_type].info.Class.name_index;
        char *catch_name = (char*) cp[class_name_idx].info.Utf8.bytes;
        u2 catch_name_len = cp[class_name_idx].info.Utf8.length;
        if (is_subclass_of(exc_obj, method_area, catch_name, catch_name_len)) {
            return entry->handler_pc;
        }
    }
    return 0; // nenhum handler encontrado
}

// pop da referencia de excecao e propaga (athrow)
u4 handle_athrow(RuntimeContext *ctx, Code_attribute *code_attr) {
    JVMThread *thread = ctx->thread;
    ReferenceMap *reference_map = ctx->reference_map;
    MethodArea *method_area = ctx->method_area;

    // 1. pop da referencia da excecao da operand stack
    Frame *current_frame = (Frame*)getTop(thread->frame_stack);
    u4 ref_key = *((u4*)getTop(current_frame->operand_stack));
    pop(current_frame->operand_stack);

    if (ref_key == 0) {
        printf("NullPointerException em athrow\n");
        exit(1);
    }

    JVMObject *exc_obj = (JVMObject*) reference_map->entries[ref_key];
    if (exc_obj == NULL) {
        printf("NullPointerException: objeto de excecao invalido em athrow\n");
        exit(1);
    }

    u4 exception_pc = thread->pc; // PC onde a excecao foi lancada

    // 2. loop de desempilhamento procurando handler
    while (!isEmpty(thread->frame_stack)) {
        Frame *frame = (Frame*)getTop(thread->frame_stack);
        // obtem o Code_attribute do metodo do frame atual
        Code_attribute *frame_code = getCodeAttributeFromTopFrame(thread->frame_stack);
        if (frame_code == NULL) {
            // metodo abstrato/nativo sem Code — desempilha
            u4 ret_pc = frame->return_pc;
            MarkClinitDoneIfApplicable(method_area, frame);
            pop(thread->frame_stack);
            FreeCodeAttribute(frame_code);
            thread->pc = ret_pc;
            continue;
        }

        // procura handler no frame atual
        u2 handler_pc = find_exception_handler(frame, frame_code, exception_pc, exc_obj, method_area);

        if (handler_pc > 0) {
            // handler encontrado! limpa operand stack, empilha excecao e ajusta PC
            Stack *opstack = frame->operand_stack;
            while (opstack->size > 0) pop(opstack);
            push(opstack, (void*)&ref_key);
            FreeCodeAttribute(frame_code);
            return handler_pc; // continua execucao no handler
        }

        // sem handler neste frame: desempilha
        u4 ret_pc = frame->return_pc;
        MarkClinitDoneIfApplicable(method_area, frame);
        pop(thread->frame_stack);
        FreeCodeAttribute(frame_code);

        // exception_pc passa a ser o return_pc do frame desempilhado
        // (para verificar o catch do caller no ponto certo)
        exception_pc = ret_pc;
        thread->pc = ret_pc;
    }

    // 3. nenhum frame restante: excecao nao capturada
    printf("Uncaught exception!\n");
    exit(1);
}

u4 handle_ifnull(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref == 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_ifnonnull(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref != 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_lcmp(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t v1 = (int64_t)(((uint64_t)high1 << 32) | low1);
    int64_t v2 = (int64_t)(((uint64_t)high2 << 32) | low2);
    int32_t result = (v1 > v2) ? 1 : (v1 == v2) ? 0 : -1;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

u4 handle_fcmpl(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } v1 = { raw1 }, v2 = { raw2 };
    int32_t result;
    if (isnan(v1.f) || isnan(v2.f)) result = -1;
    else result = (v1.f > v2.f) ? 1 : (v1.f == v2.f) ? 0 : -1;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

u4 handle_fcmpg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } v1 = { raw1 }, v2 = { raw2 };
    int32_t result;
    if (isnan(v1.f) || isnan(v2.f)) result = 1;
    else result = (v1.f > v2.f) ? 1 : (v1.f == v2.f) ? 0 : -1;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

u4 handle_dcmpl(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { uint64_t u; double d; } v1 = { ((uint64_t)high1 << 32) | low1 };
    union { uint64_t u; double d; } v2 = { ((uint64_t)high2 << 32) | low2 };
    int32_t result;
    if (isnan(v1.d) || isnan(v2.d)) result = -1;
    else result = (v1.d > v2.d) ? 1 : (v1.d == v2.d) ? 0 : -1;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

u4 handle_dcmpg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { uint64_t u; double d; } v1 = { ((uint64_t)high1 << 32) | low1 };
    union { uint64_t u; double d; } v2 = { ((uint64_t)high2 << 32) | low2 };
    int32_t result;
    if (isnan(v1.d) || isnan(v2.d)) result = 1;
    else result = (v1.d > v2.d) ? 1 : (v1.d == v2.d) ? 0 : -1;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

// da pop na operand stack 2x (4x para dois longs) e da push da soma dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_ladd(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    
    u4 low2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)high1 << 32) | low1;
    uint64_t v2 = ((uint64_t)high2 << 32) | low2;
    uint64_t res = v2 + v1;

    u4 res_high = (u4)(res >> 32);
    u4 res_low = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

// da pop na operand stack 2x (4x para dois longs) e da push da diferenca dos operandos
// OBS: op_1 eh sempre o termo da direita da operacao
u4 handle_lsub(RuntimeContext *ctx, Code_attribute *code_attr) {
   u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    
    u4 low2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)high1 << 32) | low1;
    uint64_t v2 = ((uint64_t)high2 << 32) | low2;
    uint64_t res = v2 - v1;

    u4 res_high = (u4)(res >> 32);
    u4 res_low = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

u4 handle_iand(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 val1 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 val2 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 res = val2 & val1;
    push(frame->operand_stack, (void*)&res);
    return pc + 1;
}

u4 handle_ior(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 val1 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 val2 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 res = val2 | val1;
    push(frame->operand_stack, (void*)&res);
    return pc + 1;
}

u4 handle_ixor(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 val1 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 val2 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 res = val2 ^ val1;
    push(frame->operand_stack, (void*)&res);
    return pc + 1;
}

// converte int para long
u4 handle_i2l(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    
    int64_t val64 = (int64_t)val;
    u4 high = (u4)(val64 >> 32);
    u4 low = (u4)(val64 & 0xFFFFFFFF);
    
    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);
    return pc + 1;
}

u4 handle_i2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    float result = (float)val;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

u4 handle_i2d(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    double result = (double)val;
    union { double d; uint64_t u; } res = { result };
    u4 high = (u4)(res.u >> 32);
    u4 low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);
    return pc + 1;
}

// converte long para int (descarta high)
u4 handle_l2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    pop(frame->operand_stack);

    push(frame->operand_stack, (void*)&low);
    
    return pc + 1;
}

u4 handle_l2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t val = (int64_t)(((uint64_t)high << 32) | low);
    float result = (float)val;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

u4 handle_l2d(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t val = (int64_t)(((uint64_t)high << 32) | low);
    double result = (double)val;
    union { double d; uint64_t u; } res = { result };
    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

u4 handle_f2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val = { raw };
    int32_t result;
    if (isnan(val.f))                 result = 0;
    else if (val.f >= 2147483648.0f)  result = INT32_MAX;  // >= 2^31
    else if (val.f < -2147483648.0f)  result = INT32_MIN;  // < -2^31
    else                              result = (int32_t)val.f;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

u4 handle_f2l(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val = { raw };
    int64_t result;
    if (isnan(val.f))                              result = 0LL;
    else if (val.f >= 9223372036854775808.0f)      result = INT64_MAX;  // >= 2^63
    else if (val.f < -9223372036854775808.0f)      result = INT64_MIN;  // < -2^63
    else                                           result = (int64_t)val.f;
    u4 high = (u4)((uint64_t)result >> 32);
    u4 low  = (u4)((uint64_t)result & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);
    return pc + 1;
}

u4 handle_f2d(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val = { raw };
    double result = (double)val.f;
    union { double d; uint64_t u; } res = { result };
    u4 high = (u4)(res.u >> 32);
    u4 low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);
    return pc + 1;
}

u4 handle_d2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits = ((uint64_t)high << 32) | low;
    union { uint64_t u; double d; } val = { bits };
    int32_t result;
    if (isnan(val.d))                result = 0;
    else if (val.d >= 2147483648.0)  result = INT32_MAX;  // >= 2^31
    else if (val.d < -2147483648.0)  result = INT32_MIN;  // < -2^31
    else                             result = (int32_t)val.d;
    push(frame->operand_stack, (void*)&result);
    return pc + 1;
}

u4 handle_d2l(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits = ((uint64_t)high << 32) | low;
    union { uint64_t u; double d; } val = { bits };
    int64_t result;
    if (isnan(val.d))                             result = 0LL;
    else if (val.d >= 9223372036854775808.0)      result = INT64_MAX;  // >= 2^63
    else if (val.d < -9223372036854775808.0)      result = INT64_MIN;  // < -2^63
    else                                          result = (int64_t)val.d;
    u4 res_high = (u4)((uint64_t)result >> 32);
    u4 res_low  = (u4)((uint64_t)result & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

u4 handle_d2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits = ((uint64_t)high << 32) | low;
    union { uint64_t u; double d; } val = { bits };
    float result = (float)val.d;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

u4 handle_land(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 l1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 l2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)h1 << 32) | l1;
    uint64_t v2 = ((uint64_t)h2 << 32) | l2;
    uint64_t res = v2 & v1;

    u4 res_high = (u4)(res >> 32);
    u4 res_low = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

u4 handle_lor(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 l1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 l2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)h1 << 32) | l1;
    uint64_t v2 = ((uint64_t)h2 << 32) | l2;
    uint64_t res = v2 | v1;

    u4 res_high = (u4)(res >> 32);
    u4 res_low  = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);

    return pc + 1;
}

u4 handle_lxor(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 l1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 l2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)h1 << 32) | l1;
    uint64_t v2 = ((uint64_t)h2 << 32) | l2;
    uint64_t res = v2 ^ v1;

    u4 res_high = (u4)(res >> 32);
    u4 res_low  = (u4)(res & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);

    return pc + 1;
}

u4 handle_i2b(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 val = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    
    int32_t truncated = (int32_t)(int8_t)(val & 0xFF);
    
    u4 content = (u4)truncated;
    push(frame->operand_stack, (void*)&content);
    
    return pc + 1;
}

u4 handle_i2c(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 val = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 content = val & 0xFFFF;
    
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

u4 handle_i2s(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 val = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    int32_t truncated = (int32_t)(int16_t)(val & 0xFFFF);
    
    u4 content = (u4)truncated;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// carrega o endereco de retorno de uma variavel local e pula pra ele (subrotina)
u4 handle_ret(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u1 index = code[pc + 1];
    u4 return_addr = frame->local_variables[index];

    return return_addr;
}

u4 handle_tableswitch(RuntimeContext *ctx, Code_attribute *code_attr) {

    // 1. Pegar o pc atual, o array de bytes do code_attr e o frame atual
    u4 pc = ctx->thread->pc;
    u1 *code = code_attr->code;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    // 2. Calcular o padding necessário para alinhar o próximo valor a 4 bytes e pegar os valores default, low e high
    u1 padding = (4 - ((pc + 1) % 4)) % 4;
    int32_t default_value = (((int32_t)code[pc + 1 + padding] << 24) | ((int32_t) code[pc + 2 + padding] << 16) | ((int32_t) code[pc + 3 + padding] << 8) | ((int32_t) code[pc + 4 + padding]));
    padding += 4;

    int32_t low = (((int32_t)code[pc + 1 + padding] << 24) | ((int32_t) code[pc + 2 + padding] << 16) | ((int32_t) code[pc + 3 + padding] << 8) | ((int32_t) code[pc + 4 + padding]));
    padding += 4;

    int32_t high = (((int32_t)code[pc + 1 + padding] << 24) | ((int32_t) code[pc + 2 + padding] << 16) | ((int32_t) code[pc + 3 + padding] << 8) | ((int32_t) code[pc + 4 + padding]));
    padding += 4;

    if (high < low) {
        fprintf(stderr, "Invalid tableswitch\n");
        exit(1);
    }

    // 3. Alocar memória para armazenar os offsets e preencher o array de offsets
    int32_t *offsets = calloc(high - low + 1, sizeof(int32_t));
    for (int32_t i = low; i <= high; i++) {
        offsets[i - low] = (((int32_t)code[pc + 1 + padding] << 24) | ((int32_t) code[pc + 2 + padding] << 16) | ((int32_t) code[pc + 3 + padding] << 8) | ((int32_t) code[pc + 4 + padding]));
        padding += 4;
    }

    // 4. Pegar o índice da operand stack, verificar se está dentro do intervalo e calcular o próximo pc
    int32_t index = *((int32_t*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    if (index < low || index > high) {
        free(offsets);
        return pc + default_value;
    }
    int32_t offset = offsets[index - low];
    free(offsets);
    return pc + offset;
}

u4 handle_lookupswitch(RuntimeContext *ctx, Code_attribute *code_attr) {

    // PC atual e vetor de bytecodes
    u4 pc = ctx->thread->pc;
    u1 *code = code_attr->code;

    // Frame atual
    Frame *frame =
        (Frame*)getTop(ctx->thread->frame_stack);

    // Calcula o padding
    u1 padding =
        (4 - ((pc + 1) % 4)) % 4;

    // Lê o offset default
    int32_t default_offset =
        ((int32_t)code[pc + 1 + padding] << 24) |
        ((int32_t)code[pc + 2 + padding] << 16) |
        ((int32_t)code[pc + 3 + padding] << 8)  |
        ((int32_t)code[pc + 4 + padding]);

    padding += 4;

    // Lê npairs
    int32_t npairs =
        ((int32_t)code[pc + 1 + padding] << 24) |
        ((int32_t)code[pc + 2 + padding] << 16) |
        ((int32_t)code[pc + 3 + padding] << 8)  |
        ((int32_t)code[pc + 4 + padding]);

    padding += 4;

    // Valor procurado
    int32_t key =
        *((int32_t*)getTop(frame->operand_stack));

    pop(frame->operand_stack);

    // Percorre todos os pares
    for (int32_t i = 0; i < npairs; i++) {

        int32_t match =
            ((int32_t)code[pc + 1 + padding] << 24) |
            ((int32_t)code[pc + 2 + padding] << 16) |
            ((int32_t)code[pc + 3 + padding] << 8)  |
            ((int32_t)code[pc + 4 + padding]);

        padding += 4;

        int32_t offset =
            ((int32_t)code[pc + 1 + padding] << 24) |
            ((int32_t)code[pc + 2 + padding] << 16) |
            ((int32_t)code[pc + 3 + padding] << 8)  |
            ((int32_t)code[pc + 4 + padding]);

        padding += 4;

        // Encontrou o match
        if (key == match) {
            return pc + offset;
        }
    }

    // Nenhum match encontrado
    return pc + default_offset;
}

u4 handle_wide(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    // proxima instruçao vai ser estendida
    u1 modified_opcode = code[pc + 1];
    u2 index = ((u2)code[pc + 2] << 8) | code[pc + 3];

    switch (modified_opcode) {
        case OP_ILOAD:
        case OP_FLOAD:
        case OP_ALOAD: {
            u4 content = frame->local_variables[index];
            push(frame->operand_stack, (void*)&content);
            return pc + 4; 
        }
        
        case OP_LLOAD:
        case OP_DLOAD: {
            u4 high = frame->local_variables[index];
            u4 low  = frame->local_variables[index + 1];
            push(frame->operand_stack, (void*)&high);
            push(frame->operand_stack, (void*)&low);
            return pc + 4;
        }

        case OP_ISTORE:
        case OP_FSTORE:
        case OP_ASTORE: {
            u4 content = *((u4*) getTop(frame->operand_stack)); 
            pop(frame->operand_stack);
            frame->local_variables[index] = content;
            return pc + 4;
        }

        case OP_LSTORE:
        case OP_DSTORE: {
            u4 low = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
            frame->local_variables[index] = high;
            frame->local_variables[index + 1] = low;
            return pc + 4;
        }

        case OP_IINC: {
            u2 const_unsigned = ((u2)code[pc + 4] << 8) | code[pc + 5];
            int32_t const_val = (int32_t)sign_extend_short(const_unsigned);
            u4 current_val = frame->local_variables[index];
            current_val += (u4) const_val; 
            frame->local_variables[index] = current_val;
            return pc + 6; 
        }

        case OP_RET: {
            u4 return_pc = frame->local_variables[index];
            return return_pc; 
        }

        default:
            printf("VerifyError: Opcode 0x%02X não suportado com a instrução WIDE\n", modified_opcode);
            exit(1);
    }
}

// mapea os descriptors da JVM pros ATYPE constants internos
static u1 atype_from_base_descriptor(char base_char) {
    switch (base_char) {
        case 'B': return JVM_ATYPE_BYTE;
        case 'C': return JVM_ATYPE_CHAR;
        case 'D': return JVM_ATYPE_DOUBLE;
        case 'F': return JVM_ATYPE_FLOAT;
        case 'I': return JVM_ATYPE_INT;
        case 'J': return JVM_ATYPE_LONG;
        case 'S': return JVM_ATYPE_SHORT;
        case 'Z': return JVM_ATYPE_BOOLEAN;
        case 'L': 
        case '[': return JVM_ATYPE_OBJECT;
        default:  return 0;
    }
}

// constroi recursivamente a arvore de array multi-dimensional 
// current_depth: 0 = outermost array 
// total_depth - 1 = innermost allocated array.
u4 create_multi_dim_array(RuntimeContext *ctx, u1 leaf_atype, int current_depth, int total_depth, u4 *sizes) {
    ReferenceMap *ref_map = ctx->reference_map;
    u4 count = sizes[current_depth];

    JVMArray *array = (JVMArray *)malloc(sizeof(JVMArray));
    if (!array) {
        printf("OutOfMemoryError: Allocating JVMArray\n");
        exit(1);
    }
    
    array->length = count;

    if (ref_map->size >= MAX_REF_MAP) {
        printf("OutOfMemoryError: ReferenceMap exhausted\n");
        exit(1);
    }

    if (current_depth == total_depth - 1) {
        // caso base
        array->atype = leaf_atype;
        size_t elem_size = (leaf_atype == JVM_ATYPE_OBJECT) ? sizeof(u4) : ArrayTypeSize(leaf_atype);
        array->data = (count > 0) ? calloc(count, elem_size) : NULL;
        
        if (count > 0 && !array->data) {
            printf("OutOfMemoryError: Allocating leaf array data\n");
            exit(1);
        }
    } else {
        // caso recursivo
        array->atype = JVM_ATYPE_OBJECT;
        array->data = (count > 0) ? calloc(count, sizeof(u4)) : NULL;

        if (count > 0 && !array->data) {
            printf("OutOfMemoryError: Allocating sub-array reference block\n");
            exit(1);
        }

        // instacia recursivamente e linka os sub-arrays
        if (count > 0) {
            u4 *sub_keys = (u4 *)array->data;
            for (u4 i = 0; i < count; i++) {
                sub_keys[i] = create_multi_dim_array(ctx, leaf_atype, current_depth + 1, total_depth, sizes);
            }
        }
    }

    // registra a instancia e retorna a key
    u4 ref_key = ref_map->size++;
    ref_map->entries[ref_key] = (void *)array;
    return ref_key;
}

// cria um array multidimensional
u4 handle_multianewarray(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    Cp_info *constant_pool = frame->class_file->constant_pool;

    // 1. resolve o descritor da classe
    u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    Cp_info class_entry = constant_pool[class_index];
    
    if (class_entry.tag != CONSTANT_Class) {
        printf("VerifyError: Invalid CP entry for multianewarray\n");
        exit(1);
    }
    
    u2 name_index = class_entry.info.Class.name_index;
    char *descriptor = (char *)constant_pool[name_index].info.Utf8.bytes;
    u2 desc_len = constant_pool[name_index].info.Utf8.length;

    // 2. parsing das dimensoes do
    int total_dims = 0;
    while (total_dims < desc_len && descriptor[total_dims] == '[') {
        total_dims++;
    }

    u1 dimensions = code[pc + 3];
    if (dimensions == 0 || dimensions > total_dims) {
        printf("VerifyError: Invalid dimensions requested (%u)\n", dimensions);
        exit(1);
    }

    // 3. pop do tamanho das dimensões da operand stack
    u4 sizes[256]; // Safe buffer, JVM restricts dimensions to 255
    for (int i = dimensions - 1; i >= 0; i--) {
        sizes[i] = *((u4 *)getTop(frame->operand_stack));
        pop(frame->operand_stack);
        
        if ((int32_t)sizes[i] < 0) {
            printf("NegativeArraySizeException\n");
            exit(1);
        }
    }

    // 4. determina o tipo de alocacao de memoria pro node folha
    char base_char = descriptor[total_dims];
    u1 actual_leaf_type = atype_from_base_descriptor(base_char);
    
    if (!actual_leaf_type) {
        printf("VerifyError: Invalid base type '%c'\n", base_char);
        exit(1);
    }

    // If partial dimensions are requested (e.g., new int[2][]), the leaf is still a reference
    u1 allocation_type = (dimensions < total_dims) ? JVM_ATYPE_OBJECT : actual_leaf_type;

    // 5. constroi a arvore de array
    u4 ref_key = create_multi_dim_array(ctx, allocation_type, 0, dimensions, sizes);
    push(frame->operand_stack, (void *)&ref_key);

    return pc + 4;
}

u4 handle_if_acmpeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref1 == ref2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_if_acmpne(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref1 != ref2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

u4 handle_goto_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    u1* code = code_attr->code;
    u4 branch_bytes = ((u4)code[pc + 1] << 24) | ((u4)code[pc + 2] << 16)
                    | ((u4)code[pc + 3] << 8)  |  (u4)code[pc + 4];
    return (u4)(pc + compute_branch_wide(branch_bytes));
}

// da push do endereco de retorno e salta para a subrotina (jsr)
u4 handle_jsr(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u4 return_addr = pc + 3; // endereco da instrucao seguinte ao jsr
    u2 branch_bytes = (((u2)code[pc + 1] << 8) | code[pc + 2]);

    push(frame->operand_stack, (void*)&return_addr);

    return (u4)(pc + compute_branch(branch_bytes));
}

// da push do endereco de retorno e salta para a subrotina (jsr_w - wide)
u4 handle_jsr_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;

    u4 return_addr = pc + 5; // endereco da instrucao seguinte ao jsr_w
    u4 branch_bytes = ((u4)code[pc + 1] << 24) |
                      ((u4)code[pc + 2] << 16) |
                      ((u4)code[pc + 3] << 8)  |
                      ((u4)code[pc + 4]);

    push(frame->operand_stack, (void*)&return_addr);

    return (u4)(pc + compute_branch_wide(branch_bytes));
}

u4 handle_aaload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 index = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em aaload\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em aaload\n");
        exit(1);
    }

    u4 value = ((u4*)arrayref->data)[index];
    push(frame->operand_stack, (void*)&value);

    return pc + 1;
}

u4 handle_aastore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 index = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em aastore\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em aastore\n");
        exit(1);
    }

    ((u4*)arrayref->data)[index] = value;

    return pc + 1;
}

// da push de uma variavel local de referencia no indice 1 na operand stack
u4 handle_aload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[1];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}
// da push de uma variavel local de referencia no indice 2 na operand stack
u4 handle_aload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[2];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}
// da push de uma variavel local de referencia no indice 3 na operand stack
u4 handle_aload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[3];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack e armazena referencia em variaveis locais no indice 0
u4 handle_astore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[0] = content;
    return pc + 1;
}
// da pop na operand stack e armazena referencia em variaveis locais no indice 1
u4 handle_astore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[1] = content;
    return pc + 1;
}
// da pop na operand stack e armazena referencia em variaveis locais no indice 2
u4 handle_astore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[2] = content;
    return pc + 1;
}
// da pop na operand stack e armazena referencia em variaveis locais no indice 3
u4 handle_astore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[3] = content;
    return pc + 1;
}

u4 handle_baload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 index = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em baload\n");
        exit(1);
    }
    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em baload\n");
        exit(1);
    }
    if (arrayref->atype != JVM_ATYPE_BYTE &&
        arrayref->atype != JVM_ATYPE_BOOLEAN) {
        printf("ArrayStoreTypeMismatch\n");
        exit(1);
    }

    // Lê apenas 1 byte da memória e realiza "Sign Extension" para int32_t
    int8_t byte_val = ((int8_t*)arrayref->data)[index];
    u4 value = (u4)(int32_t)byte_val;

    push(frame->operand_stack, (void*)&value);

    return pc + 1;
}

u4 handle_bastore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 index = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em bastore\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em bastore\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_BYTE && arrayref->atype != JVM_ATYPE_BOOLEAN) {
        printf("ArrayStoreTypeMismatch em bastore\n");
        exit(1);
    }

    ((int8_t*)arrayref->data)[index] = (int8_t)(value & 0xFF);

    return pc + 1;
}

/* Carrega um char de um array para a operand stack. */
u4 handle_caload(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 index =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 ref_key =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    JVMArray* arrayref =
        (JVMArray*)reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em caload\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em caload\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_CHAR) {
        printf("ArrayStoreTypeMismatch em caload\n");
        exit(1);
    }

    u4 value =
        (u4)((uint16_t*)arrayref->data)[index];

    push(frame->operand_stack, (void*)&value);

    return pc + 1;
}

/* Armazena um char da operand stack em um array. */
u4 handle_castore(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 index =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 ref_key =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    JVMArray* arrayref =
        (JVMArray*)reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em castore\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em castore\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_CHAR) {
        printf("ArrayStoreTypeMismatch em castore\n");
        exit(1);
    }

    ((uint16_t*)arrayref->data)[index] =
        (uint16_t)value;

    return pc + 1;
}

static int check_interface(RuntimeContext *ctx, ClassFile *cf, char *target_name, u2 target_len);

u4 handle_checkcast(RuntimeContext *ctx, Code_attribute *code_attr) {
    // TODO: tratar caso o ref_key seja um JVMArray
    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;

    u4 pc = thread->pc;
    u1* code = code_attr->code;
    u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    u2 name_idx = constant_pool[class_index].info.Class.name_index;
    char* target_class_name = (char*)constant_pool[name_idx].info.Utf8.bytes;
    u2 target_class_len = constant_pool[name_idx].info.Utf8.length;
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); 

    // se for null, o cast sempre passa reto
    if(ref_key == 0) {
        return pc + 3;
    }

    JVMObject* obj = (JVMObject*) ctx->reference_map->entries[ref_key];
    if(obj == NULL) {
        printf("Fatal Error: Objeto inválido no ReferenceMap (checkcast)\n");
        exit(1);
    }

    // percorre a hierarquia de classes e interfaces
    ClassFile* curr = obj->class_ref;
    u1 is_instance = 0; 

    while(curr != NULL) {
        u2 curr_name_idx = curr->constant_pool[curr->this_class].info.Class.name_index;
        u2 curr_len = curr->constant_pool[curr_name_idx].info.Utf8.length;
        char* curr_name = (char*)curr->constant_pool[curr_name_idx].info.Utf8.bytes;

        // valida se eh a própria classe
        if(curr_len == target_class_len && strncmp(curr_name, target_class_name, target_class_len) == 0) {
            is_instance = 1;
            break;
        }

        // valida as interfaces
        if (check_interface(ctx, curr, target_class_name, target_class_len)) {
            is_instance = 1;
            break;
        }

        // sobe para a superclasse
        if(curr->super_class == 0) {
            curr = NULL;
        } 
        else {
            u2 s_name_idx = curr->constant_pool[curr->super_class].info.Class.name_index;
            u2 s_len = curr->constant_pool[s_name_idx].info.Utf8.length;
            char s_name[s_len + 1];
            memcpy(s_name, curr->constant_pool[s_name_idx].info.Utf8.bytes, s_len);
            s_name[s_len] = '\0';
            
            MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
            curr = entry ? entry->class_file : NULL;
        }
    }

    // lança exceção se não for compatível
    if(!is_instance) {
        //TODO: verificar problema de exceção
        printf("ClassCastException: O objeto não pode sofrer cast para a classe alvo\n");
        exit(1);
    }
    return pc + 3;
}

u4 handle_dadd(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t bits1 = ((uint64_t)high1 << 32) | low1;
    uint64_t bits2 = ((uint64_t)high2 << 32) | low2;

    union { uint64_t u; double d; } val1 = { bits1 }, val2 = { bits2 };
    double result = val1.d + val2.d;
    union { double d; uint64_t u; } res = { result };

    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

/* Carrega um double de um array para a operand stack. */
u4 handle_daload(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 index = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 ref_key = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    JVMArray* arrayref =
        (JVMArray*)reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em daload\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em daload\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_DOUBLE) {
        printf("ArrayStoreTypeMismatch em daload\n");
        exit(1);
    }

    uint64_t value =
        ((uint64_t*)arrayref->data)[index];

    u4 high = (u4)(value >> 32);
    u4 low  = (u4)(value & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

/* Armazena um double da operand stack em um array. */
u4 handle_dastore(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 low =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 index =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 ref_key =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    JVMArray* arrayref =
        (JVMArray*)reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em dastore\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em dastore\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_DOUBLE) {
        printf("ArrayStoreTypeMismatch em dastore\n");
        exit(1);
    }

    uint64_t value =
        ((uint64_t)high << 32) | low;

    ((uint64_t*)arrayref->data)[index] =
        value;

    return pc + 1;
}

u4 handle_ddiv(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits1 = ((uint64_t)high1 << 32) | low1;
    uint64_t bits2 = ((uint64_t)high2 << 32) | low2;
    union { uint64_t u; double d; } val1 = { bits1 }, val2 = { bits2 };
    double result = val2.d / val1.d;
    union { double d; uint64_t u; } res = { result };
    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

u4 handle_dmul(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits1 = ((uint64_t)high1 << 32) | low1;
    uint64_t bits2 = ((uint64_t)high2 << 32) | low2;
    union { uint64_t u; double d; } val1 = { bits1 }, val2 = { bits2 };
    double result = val2.d * val1.d;
    union { double d; uint64_t u; } res = { result };
    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}
u4 handle_dneg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits = ((uint64_t)high << 32) | low;
    union { uint64_t u; double d; } val = { bits };
    double result = -val.d;
    union { double d; uint64_t u; } res = { result };
    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}
u4 handle_drem(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits1 = ((uint64_t)high1 << 32) | low1;
    uint64_t bits2 = ((uint64_t)high2 << 32) | low2;
    union { uint64_t u; double d; } val1 = { bits1 }, val2 = { bits2 };
    double result = fmod(val2.d, val1.d);
    union { double d; uint64_t u; } res = { result };
    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

u4 handle_dsub(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 low1  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 low2  = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    uint64_t bits1 = ((uint64_t)high1 << 32) | low1;
    uint64_t bits2 = ((uint64_t)high2 << 32) | low2;
    union { uint64_t u; double d; } val1 = { bits1 }, val2 = { bits2 };
    double result = val2.d - val1.d;
    union { double d; uint64_t u; } res = { result };
    u4 res_high = (u4)(res.u >> 32);
    u4 res_low  = (u4)(res.u & 0xFFFFFFFF);
    push(frame->operand_stack, (void*)&res_high);
    push(frame->operand_stack, (void*)&res_low);
    return pc + 1;
}

// Duplica o valor do topo da pilha e insere ele uma posição abaixo
u4 handle_dup_x1(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value1 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value2 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    push(frame->operand_stack, &value1);
    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);

    return pc + 1;
}

// Duplica o valor do topo da pilha e insere ele duas posições abaixo
u4 handle_dup_x2(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value1 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value2 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value3 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    push(frame->operand_stack, &value1);
    push(frame->operand_stack, &value3);
    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);

    return pc + 1;
}

// Duplica os dois valores do topo da pilha
u4 handle_dup2(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value1 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value2 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);
    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);

    return pc + 1;
}

// Duplica os dois valores do topo da pilha e insere eles uma posição abaixo
u4 handle_dup2_x1(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value1 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value2 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value3 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);
    push(frame->operand_stack, &value3);
    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);

    return pc + 1;
}

// Duplica os dois valores do topo da pilha e insere eles duas posições abaixo
u4 handle_dup2_x2(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value1 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value2 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value3 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value4 = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);
    push(frame->operand_stack, &value4);
    push(frame->operand_stack, &value3);
    push(frame->operand_stack, &value2);
    push(frame->operand_stack, &value1);

    return pc + 1;
}

u4 handle_fadd(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val1 = { raw1 }, val2 = { raw2 };
    float result = val2.f + val1.f;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

u4 handle_faload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 index = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em faload\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em faload\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_FLOAT) {
        printf("ArrayStoreTypeMismatch em faload\n");
        exit(1);
    }

    u4 value = ((u4*)arrayref->data)[index];
    push(frame->operand_stack, (void*)&value);

    return pc + 1;
}

u4 handle_fastore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 index = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em fastore\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em fastore\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_FLOAT) {
        printf("ArrayStoreTypeMismatch em fastore\n");
        exit(1);
    }

    ((u4*)arrayref->data)[index] = value;

    return pc + 1;
}

// da push de 0.0f na operand stack
u4 handle_fconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    union { float f; u4 u; } val = { 0.0f };
    push(frame->operand_stack, (void*)&val.u);
    return pc + 1;
}

// da push de 1.0f na operand stack
u4 handle_fconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    union { float f; u4 u; } val = { 1.0f };
    push(frame->operand_stack, (void*)&val.u);
    return pc + 1;
}

// da push de 2.0f na operand stack
u4 handle_fconst_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    union { float f; u4 u; } val = { 2.0f };
    push(frame->operand_stack, (void*)&val.u);
    return pc + 1;
}

u4 handle_fdiv(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val1 = { raw1 }, val2 = { raw2 };
    float result = val2.f / val1.f;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

// da push de uma variavel local float no indice idx na operand stack
u4 handle_fload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = frame->local_variables[parameter];
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da push de uma variavel local float no indice 0 na operand stack
u4 handle_fload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[0];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de uma variavel local float no indice 1 na operand stack
u4 handle_fload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[1];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de uma variavel local float no indice 2 na operand stack
u4 handle_fload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[2];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de uma variavel local float no indice 3 na operand stack
u4 handle_fload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[3];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

u4 handle_fmul(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val1 = { raw1 }, val2 = { raw2 };
    float result = val2.f * val1.f;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

u4 handle_fneg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val = { raw };
    float result = -val.f;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

u4 handle_frem(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val1 = { raw1 }, val2 = { raw2 };
    float result = fmodf(val2.f, val1.f);
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

// da pop na operand stack e armazena float em variaveis locais no indice idx
u4 handle_fstore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[parameter] = content;
    return pc + 2;
}
// da pop na operand stack e armazena float em variaveis locais no indice 0
u4 handle_fstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[0] = content;
    return pc + 1;
}
// da pop na operand stack e armazena float em variaveis locais no indice 1
u4 handle_fstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[1] = content;
    return pc + 1;
}
// da pop na operand stack e armazena float em variaveis locais no indice 2
u4 handle_fstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[2] = content;
    return pc + 1;
}
// da pop na operand stack e armazena float em variaveis locais no indice 3
u4 handle_fstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[3] = content;
    return pc + 1;
}
u4 handle_fsub(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 raw2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val1 = { raw1 }, val2 = { raw2 };
    float result = val2.f - val1.f;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

// Desloca um inteiro para a esquerda
u4 handle_ishl(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 shift = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 result = value << (shift & 0x1F);

    push(frame->operand_stack, &result);

    return pc + 1;
}

// Faz deslocamento aritmético pra direita preservando o sinal
u4 handle_ishr(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 shift = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    int32_t value =
        *((int32_t*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    int32_t result =
        value >> (shift & 0x1F);

    push(frame->operand_stack, &result);

    return pc + 1;
}

// Faz deslocamento lógico pra direita preenchendo com zeros
u4 handle_iushr(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 shift = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 result =
        value >> (shift & 0x1F);

    push(frame->operand_stack, &result);

    return pc + 1;
}

u4 handle_iaload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    // Pop do índice
    u4 index = (*((u4*) getTop(frame->operand_stack))); pop(frame->operand_stack);
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    // Verificações de segurança
    if (arrayref == NULL) {
        printf("NullPointerException em iaload\n");
        exit(1);
    }
    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em iaload\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_INT) {
        printf("ArrayStoreTypeMismatch em iaload\n");
        exit(1);
    }

    u4 value = (u4)((int32_t*)arrayref->data)[index];
    push(frame->operand_stack, (void*)&value);

    return pc + 1;
}

u4 handle_iastore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 index = (*((u4*) getTop(frame->operand_stack))); pop(frame->operand_stack);
    
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em iastore\n");
        exit(1);
    }
    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em iastore\n");
        exit(1);
    }
    if (arrayref->atype != JVM_ATYPE_INT) {
        printf("ArrayStoreTypeMismatch em iastore\n");
        exit(1);
    }

    // Armazena no buffer de dados como int32_t
    ((int32_t*)arrayref->data)[index] = (int32_t)value;

    return pc + 1;
}

static int check_interface(RuntimeContext *ctx, ClassFile *cf, char *target_name, u2 target_len) {
    if(cf == NULL) return 0;
    for(u2 i = 0; i < cf->interfaces_count; i++) {
        u2 iface_idx = cf->interfaces[i];
        u2 i_name_idx = cf->constant_pool[iface_idx].info.Class.name_index;
        u2 i_len = cf->constant_pool[i_name_idx].info.Utf8.length;
        char* i_name = (char*)cf->constant_pool[i_name_idx].info.Utf8.bytes;

        // interface atual é a desejada
        if(i_len == target_len && strncmp(i_name, target_name, target_len) == 0) {
            return 1;
        }

        // procura nas superinterfaces dela
        char s_name[i_len + 1];
        memcpy(s_name, i_name, i_len);
        s_name[i_len] = '\0';
        
        MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
        ClassFile *iface_cf = entry ? entry->class_file : NULL;
        
        // chamada recursiva para buscar na árvore de interfaces dessa interface
        if(iface_cf != NULL) {
            if(check_interface(ctx, iface_cf, target_name, target_len)) {
                return 1;
            }
        }
    }
    return 0;
}

u4 handle_instanceof(RuntimeContext *ctx, Code_attribute *code_attr) {
    // TODO: tratar caso o ref_key seja um JVMArray
    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;

    u4 pc = thread->pc;
    u1* code = code_attr->code;
    u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // pega o nome da classe alvo
    u2 name_idx = constant_pool[class_index].info.Class.name_index;
    char* target_class_name = (char*)constant_pool[name_idx].info.Utf8.bytes;
    u2 target_class_len = constant_pool[name_idx].info.Utf8.length;
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); 
    pop(frame->operand_stack);

    if(ref_key == 0) {
        u4 false_val = 0;
        push(frame->operand_stack, (void*)&false_val);
        return pc + 3;
    }

    JVMObject* obj = (JVMObject*) ctx->reference_map->entries[ref_key];
    if(obj == NULL) {
        printf("Fatal Error: Objeto inválido no ReferenceMap (instanceof)\n");
        exit(1);
    }

    // percorre a hierarquia de classes
    ClassFile* curr = obj->class_ref;
    u4 is_instance = 0;
    while(curr != NULL) {
        // compara com a classe atual da iteração
        u2 curr_name_idx = curr->constant_pool[curr->this_class].info.Class.name_index;
        u2 curr_len = curr->constant_pool[curr_name_idx].info.Utf8.length;
        char* curr_name = (char*)curr->constant_pool[curr_name_idx].info.Utf8.bytes;
        if(curr_len == target_class_len && strncmp(curr_name, target_class_name, target_class_len) == 0) {
            is_instance = 1;
            break;
        }
        // função auxiliar para verificar interfaces
        if(check_interface(ctx, curr, target_class_name, target_class_len)) {
            is_instance = 1;
            break;
        }

        // sobe para a superclasse
        if(curr->super_class == 0) {
            curr = NULL; // java/lang/Object
        } 
        else {
            u2 s_name_idx = curr->constant_pool[curr->super_class].info.Class.name_index;
            u2 s_len = curr->constant_pool[s_name_idx].info.Utf8.length;
            char s_name[s_len + 1];
            memcpy(s_name, curr->constant_pool[s_name_idx].info.Utf8.bytes, s_len);
            s_name[s_len] = '\0';
            
            MethodAreaEntry *entry = MethodAreaGetEntry(ctx->method_area, s_name);
            curr = entry ? entry->class_file : NULL;
        }
    }
    // empilha o resultado booleano
    push(frame->operand_stack, (void*)&is_instance);
    return pc + 3;
}

u4 handle_laload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    int32_t index = (int32_t)(*((u4*) getTop(frame->operand_stack))); pop(frame->operand_stack);
    
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    if (ref_key >= reference_map->size) {
        printf("Fatal Error: ref_key (%u) excede o tamanho do ReferenceMap (%u) em laload\n", 
               ref_key, reference_map->size);
        exit(1);
    }

    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em laload\n");
        exit(1);
    }
    
    if (index < 0 || index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em laload\n");
        exit(1);
    }
    if (arrayref->atype != JVM_ATYPE_LONG) {
        printf("ArrayStoreTypeMismatch em laload\n");
        exit(1);
    }

    // Acessa o array como um array de int64_t
    int64_t value = ((int64_t*)arrayref->data)[index];
    
    // Divide o long de 64 bits em duas partes de 32 bits para a pilha
    u4 high = (u4)(value >> 32);
    u4 low = (u4)(value & 0xFFFFFFFF);

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

u4 handle_lastore(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 low = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 high = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int64_t value = (((int64_t)high) << 32) | low;

    int32_t index = (int32_t)(*((u4*) getTop(frame->operand_stack))); pop(frame->operand_stack);
    
    u4 ref_key = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    if (ref_key >= reference_map->size) {
        printf("Fatal Error: ref_key (%u) excede o tamanho do ReferenceMap (%u) em lastore\n", 
               ref_key, reference_map->size);
        exit(1);
    }

    JVMArray* arrayref = (JVMArray*) reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em lastore\n");
        exit(1);
    }
    if (index < 0 || index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em lastore\n");
        exit(1);
    }
    if (arrayref->atype != JVM_ATYPE_LONG) {
        printf("ArrayStoreTypeMismatch em lastore\n");
        exit(1);
    }

    ((int64_t*)arrayref->data)[index] = value;

    return pc + 1;
}

/* Carrega um short de um array para a operand stack. */
u4 handle_saload(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 index =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 ref_key =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    JVMArray* arrayref =
        (JVMArray*)reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em saload\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em saload\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_SHORT) {
        printf("ArrayStoreTypeMismatch em saload\n");
        exit(1);
    }

    int16_t short_val =
        ((int16_t*)arrayref->data)[index];

    u4 value =
        (u4)(int32_t)short_val;

    push(frame->operand_stack, &value);

    return pc + 1;
}

/* Armazena um short da operand stack em um array. */
u4 handle_sastore(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 value =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 index =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 ref_key =
        *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    JVMArray* arrayref =
        (JVMArray*)reference_map->entries[ref_key];

    if (arrayref == NULL) {
        printf("NullPointerException em sastore\n");
        exit(1);
    }

    if (index >= arrayref->length) {
        printf("ArrayIndexOutOfBoundsException em sastore\n");
        exit(1);
    }

    if (arrayref->atype != JVM_ATYPE_SHORT) {
        printf("ArrayStoreTypeMismatch em sastore\n");
        exit(1);
    }

    ((int16_t*)arrayref->data)[index] =
        (int16_t)value;

    return pc + 1;
}

// ============================================================
// Execution loop
// ============================================================
void interpret(RuntimeContext *ctx){
    JVMThread* thread = ctx->thread;
    u4 i = 0;
    while (!isEmpty(thread->frame_stack)){
        i++;
        Code_attribute* code_attr = getCodeAttributeFromTopFrame(thread->frame_stack);
        u1* code = code_attr->code;
        u1 opcode = code[thread->pc];
        printf("[INTERPRETER] %d Interpretando opcode 0x%0x (pc = %d)\n", i, opcode, thread->pc);
        InstructionHandler handler = decode(opcode);
        if (handler == NULL){
            printf("[INTERPRETER] erro no opcode 0x%0x\n", opcode);
            FreeCodeAttribute(code_attr);
            break;
        }
        thread->pc = handler(ctx, code_attr);
        FreeCodeAttribute(code_attr);
    }
}
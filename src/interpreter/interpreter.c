#include "interpreter.h"
#include "instruction_handler.h"
#include <math.h>

//TODO: verificar se o tamanho da stack de frames ou de operandos é excedido e tratar

// avanca o pc
static u4 handle_nop(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    return pc + 1;
}

// da push em null na operand stack
static u4 handle_aconst_null(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 0; // NULL
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em -1 na operand stack
static u4 handle_iconst_m1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = -1; // -1
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 0 na operand stack
static u4 handle_iconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 0; // 0
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 1 na operand stack
static u4 handle_iconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 1; // 1
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 2 na operand stack
static u4 handle_iconst_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 2; // 2
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 3 na operand stack
static u4 handle_iconst_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 3; // 3
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 4 na operand stack
static u4 handle_iconst_4(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 4; // 4
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em 5 na operand stack
static u4 handle_iconst_5(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = 5; // 5
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push do long 0 na operand stack
static u4 handle_lconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = 0;
    u4 low  = 0;

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push do long 1 na operand stack
static u4 handle_lconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = 0;
    u4 low  = 1;

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push do double 0.0 na operand stack
static u4 handle_dconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_bipush(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = sign_extend_byte(parameter);
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da push em um short sign extended na operand stack
static u4 handle_sipush(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_ldc(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    // TODO: push constant from constant pool at index
    //return pc + 2;
}

// da push em uma variavel local no indice idx na operand stack
static u4 handle_iload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = frame->local_variables[parameter];
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da push em uma variavel local no indice 0 na operand stack
static u4 handle_iload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[0];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em uma variavel local no indice 1 na operand stack
static u4 handle_iload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[1];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em uma variavel local no indice 2 na operand stack
static u4 handle_iload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[2];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push em uma variavel local no indice 3 na operand stack
static u4 handle_iload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[3];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de um long da variavel local no indice idx na operand stack
static u4 handle_lload(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[0];
    u4 low  = frame->local_variables[1];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um long da variavel local no indice 1 na operand stack
static u4 handle_lload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[1];
    u4 low  = frame->local_variables[2];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um long da variavel local no indice 2 na operand stack
static u4 handle_lload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[2];
    u4 low  = frame->local_variables[3];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um long da variavel local no indice 3 na operand stack
static u4 handle_lload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[3];
    u4 low  = frame->local_variables[4];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice idx na operand stack
static u4 handle_dload(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[0];
    u4 low  = frame->local_variables[1];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice 1 na operand stack
static u4 handle_dload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[1];
    u4 low  = frame->local_variables[2];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice 2 na operand stack
static u4 handle_dload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[2];
    u4 low  = frame->local_variables[3];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de um double da variavel local no indice 3 na operand stack
static u4 handle_dload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 high = frame->local_variables[3];
    u4 low  = frame->local_variables[4];

    push(frame->operand_stack, (void*)&high);
    push(frame->operand_stack, (void*)&low);

    return pc + 1;
}

// da push de uma variavel local de referencia no indice 0 na operand stack
static u4 handle_aload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[0];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice idx
static u4 handle_istore(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_istore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[0] = content;
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice 1
static u4 handle_istore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[1] = content;
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice 2
static u4 handle_istore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[2] = content;
    return pc + 1;
}

// da pop na operand stack e armazena em variaveis locais no indice 3
static u4 handle_istore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[3] = content;
    return pc + 1;
}

// da pop na operand stack e armazena um long em variaveis locais no indice idx
static u4 handle_lstore(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dstore(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_iadd(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_isub(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_imul(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_idiv(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_irem(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_ineg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    u4 content = (-operand);
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// incrementa variavel local no indice idx por um byte sign extended 
static u4 handle_iinc(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_ldiv(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lmul(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lrem(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lshl(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lshr(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lushr(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_ifeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val == 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_ifne(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val != 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_iflt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val < 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_ifge(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val >= 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_ifgt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val > 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_ifle(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (val <= 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_icmpeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 == v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_icmpne(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 != v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_icmplt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 < v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_icmpge(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 >= v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_icmpgt(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 > v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_icmple(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    int32_t v2 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    int32_t v1 = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (v1 <= v2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

// altera o pc de acordo com o short dado como parametro
static u4 handle_goto(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u2 branch_bytes = (((u2)code[pc + 1] << 8) | code[pc + 2]);
    return (u4)(pc + compute_branch(branch_bytes));
}

static u4 handle_i_return(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    u4 return_pc = frame->return_pc;

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&value);
    }

    return return_pc;
}

static u4 handle_lreturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 low = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 return_pc = frame->return_pc;

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&high);
        push(caller->operand_stack, (void*)&low);
    }

    return return_pc;
}

static u4 handle_freturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    u4 return_pc = frame->return_pc;

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&value);
    }

    return return_pc;
}

static u4 handle_dreturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 low = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 high = *((u4*)getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 return_pc = frame->return_pc;

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&high);
        push(caller->operand_stack, (void*)&low);
    }

    return return_pc;
}

static u4 handle_areturn(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;
    Frame *frame = (Frame*)getTop(thread->frame_stack);

    u4 value = *((u4*)getTop(frame->operand_stack));
    u4 return_pc = frame->return_pc;

    pop(thread->frame_stack);

    if (!isEmpty(thread->frame_stack)) {
        Frame *caller = (Frame*)getTop(thread->frame_stack);
        push(caller->operand_stack, (void*)&value);
    }

    return return_pc;
}

static u4 handle_return(RuntimeContext *ctx, Code_attribute *code_attr) {
    (void)code_attr;

    JVMThread *thread = ctx->thread;

    Frame *frame = (Frame*)getTop(thread->frame_stack);
    u4 return_pc = frame->return_pc;

    pop(thread->frame_stack);

    return return_pc;
}

static u4 handle_invokestatic(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve method, create new frame, push to stack
    //return pc + 3;
}

static u4 handle_invokevirtual(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve method ref, dispatch by object type, push new frame
    //return pc + 3;
}

static u4 handle_invokespecial(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: handle <init>, super, private methods
    //return pc + 3;
}

static u4 handle_invokeinterface(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    // TODO: parse method reference and optional count field
    //return pc + 5;
}

static u4 handle_getstatic(RuntimeContext *ctx, Code_attribute *code_attr) {
    JVMThread *thread = ctx->thread;
    
    Frame *frame = (Frame*)getTop(thread->frame_stack);
    ClassFile *class_file = frame->class_file;
    Cp_info *constant_pool = class_file->constant_pool;

    u4 pc = thread->pc;
    u1* code = code_attr->code;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // Resolve field from constant pool, push static field value
    Cp_info field_ref = constant_pool[field_index];
    if (field_ref.tag != CONSTANT_Fieldref) {
        fprintf(stderr, "Invalid constant pool entry for getstatic\n");
        exit(1);
    }
    else {
        MethodArea *method_area = ctx->method_area;
        MethodAreaEntry *entry = get_method_area_entry(method_area, class_file->this_class);
        StaticField *static_fields = entry->static_fields;

        u2 field_idx = field_ref.info.Fieldref.name_and_type_index;
        Cp_info name_and_type = constant_pool[field_idx];
        u2 target_name_index = name_and_type.info.NameAndType.name_index;
        u2 target_descriptor_index = name_and_type.info.NameAndType.descriptor_index;
        char fieldType;

        Field_info *field_info = class_file->fields;
        u2 count = class_file->fields_count;
        u4 mem_offset = 0;

        for (u2 i = 0; i < count; i++) {
            u2 field_name = field_info[i].name_index;
            u2 field_descriptor = field_info[i].descriptor_index;

            if (field_name == target_name_index && field_descriptor == target_descriptor_index) {
                Field_info *field = &field_info[i];
                break;
            }
            
            else {
                fieldType = constant_pool[field_descriptor].info.Utf8.bytes[0];
                if (fieldType == 'J' || fieldType == 'D') {
                    mem_offset += 8;
                } else {
                    mem_offset += 4;
                }
            }
        }

        // Push the value of the static field onto the operand stack based on its type
        StaticField *target_field = (StaticField*)((u1*)static_fields + mem_offset);
        if (constant_pool[target_descriptor_index].info.Utf8.bytes[0] == 'J' || constant_pool[target_descriptor_index].info.Utf8.bytes[0] == 'D') {
            u4 high = target_field->value[0];
            u4 low = target_field->value[1];
            push(frame->operand_stack, (void*)&high);
            push(frame->operand_stack, (void*)&low);
        } else {
            u4 value = target_field->value[0];
            push(frame->operand_stack, (void*)&value);
        }
    }
    return pc + 3;
}

static u4 handle_putstatic(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop value, store in static field
    //return pc + 3;
}

static u4 handle_getfield(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop object ref, resolve field, push field value
    //return pc + 3;
}

static u4 handle_putfield(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop value and object ref, store in object's field
    //return pc + 3;
}

static u4 handle_new(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    ReferenceMap* reference_map = ctx->reference_map;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    // 1. Pega o índice no constant pool
    u2 class_index = (code_attr->code[pc + 1] << 8) | code_attr->code[pc + 2];

    // 2. Resolve a classe!
    ClassFile* resolved_class = get_class_from_constant_pool(ctx->thread, ctx->method_area, frame, class_index);

    // SE RETORNOU NULL, UM NOVO FRAME (<clinit>) FOI EMPILHADO!
    if (resolved_class == NULL) {
        return pc; 
    }

    JVMObject* new_obj = (JVMObject*) malloc(sizeof(JVMObject));
    if (new_obj == NULL) {
        printf("OutOfMemoryError em 'new'\n");
        exit(1);
    }

    new_obj->class_ref = resolved_class;

    u4 ref_key = reference_map->size;
    reference_map->size++;

    if (ref_key > MAX_REF_MAP) {
        printf("OutOfMemoryError: ReferenceMap cheio!\n");
        exit(1);
    }

    reference_map->entries[ref_key] = (void*)new_obj;
    push(frame->operand_stack, (void*)&ref_key);

    return pc + 3;
}

static u4 handle_newarray(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_anewarray(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop size, allocate object array at class_index
    //return pc + 3;
}

static u4 handle_arraylength(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop array ref, push length
    //return pc + 1;
}

// da push de uma variavel local de referencia no indice idx na operand stack
static u4 handle_aload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = frame->local_variables[parameter];
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da pop na operand stack e armazena referencia em variaveis locais no indice idx
static u4 handle_astore(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_pop(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    pop(frame->operand_stack);
    return pc + 1;
}

// Remove os dois valores do topo da stack de operandos
static u4 handle_pop2(RuntimeContext *ctx, Code_attribute *code_attr) {

    u4 pc = ctx->thread->pc;
    Frame *frame = (Frame*)getTop(ctx->thread->frame_stack);

    pop(frame->operand_stack);
    pop(frame->operand_stack);

    return pc + 1;
}

// duplica o topo da stack
static u4 handle_dup(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 operand = *((u4*)getTop(frame->operand_stack));
    push(frame->operand_stack, (void*)&operand);
    return pc + 1;
}

// faz o swap do topo da stack com o valor imediatamente anterior
static u4 handle_swap(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_athrow(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop exception, resolve handler or propagate
    //return pc;
}

static u4 handle_ifnull(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref == 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_ifnonnull(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref != 0) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_lcmp(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_fcmpl(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_fcmpg(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_dcmpl(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_dcmpg(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_ladd(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_lsub(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_iand(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 val1 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 val2 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 res = val2 & val1;
    push(frame->operand_stack, res);
    return pc + 1;
}

static u4 handle_ior(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 val1 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 val2 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 res = val2 | val1;
    push(frame->operand_stack, res);
    return pc + 1;
}

static u4 handle_ixor(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 val1 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 val2 = *((u4*)getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 res = val2 ^ val1;
    push(frame->operand_stack, res);
    return pc + 1;
}

// converte int para long
static u4 handle_i2l(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_i2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    int32_t val = *((int32_t*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    float result = (float)val;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

static u4 handle_i2d(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_l2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 low = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    pop(frame->operand_stack);

    push(frame->operand_stack, (void*)&low);
    
    return pc + 1;
}

static u4 handle_l2f(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_l2d(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_f2i(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_f2l(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_f2d(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_d2i(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_d2l(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_d2f(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_land(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_lor(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);

    u4 l1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 l2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)h1 << 32) | l1;
    uint64_t v2 = ((uint64_t)h2 << 32) | l2;
    uint64_t res = v2 | v1;

    push(frame->operand_stack, (void*)(u4)(res >> 32));
    push(frame->operand_stack, (void*)(u4)(res & 0xFFFFFFFF));
    return pc + 1;
}

static u4 handle_lxor(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 l1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 l2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 h2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);

    uint64_t v1 = ((uint64_t)h1 << 32) | l1;
    uint64_t v2 = ((uint64_t)h2 << 32) | l2;
    uint64_t res = v2 ^ v1;

    push(frame->operand_stack, (void*)(u4)(res >> 32));
    push(frame->operand_stack, (void*)(u4)(res & 0xFFFFFFFF));
    return pc + 1;
}

static u4 handle_i2b(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 val = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    
    int32_t truncated = (int32_t)(int8_t)(val & 0xFF);
    
    u4 content = (u4)truncated;
    push(frame->operand_stack, (void*)&content);
    
    return pc + 1;
}

static u4 handle_i2c(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 val = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    u4 content = val & 0xFFFF;
    
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

static u4 handle_i2s(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    
    u4 val = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);

    int32_t truncated = (int32_t)(int16_t)(val & 0xFFFF);
    
    u4 content = (u4)truncated;
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

static u4 handle_ret(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    // TODO: read local variable, restore pc from subroutine
    //return pc + 1;
}

static u4 handle_tableswitch(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    // TODO: parse padding, low/high/default, compute jump target
    //return pc;
}

static u4 handle_lookupswitch(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    // TODO: parse padding, match pairs, compute jump target
    //return pc;
}

static u4 handle_wide(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    // TODO: next opcode uses 2-byte index instead of 1-byte
    //return pc + 1;
}

static u4 handle_multianewarray(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    //u1 dimensions = code[pc + 3];
    // TODO: pop N dimension sizes, allocate multi-dimensional array
    //return pc + 4;
}

static u4 handle_if_acmpeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref1 == ref2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_if_acmpne(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u4 ref2 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u4 ref1 = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (ref1 != ref2) ? (u4)(pc + compute_branch(branch_bytes)) : pc + 3;
}

static u4 handle_goto_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    u1* code = code_attr->code;
    u4 branch_bytes = ((u4)code[pc + 1] << 24) | ((u4)code[pc + 2] << 16)
                    | ((u4)code[pc + 3] << 8)  |  (u4)code[pc + 4];
    return (u4)(pc + compute_branch_wide(branch_bytes));
}

static u4 handle_jsr(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: push //return address, jump to subroutine
    //return (u4)(pc + compute_branch(branch_bytes));
}

static u4 handle_jsr_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u4 branch_bytes = ((u4)code[pc + 1] << 24) |
    //                  ((u4)code[pc + 2] << 16) |
    //                  ((u4)code[pc + 3] << 8)  |
    //                  ((u4)code[pc + 4]);
    // TODO: push //return address, jump to subroutine
    //return (u4)(pc + compute_branch_wide(branch_bytes));
}

// implementar essas tambem!

static u4 handle_aaload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_aastore(RuntimeContext *ctx, Code_attribute *code_attr) {}
// da push de uma variavel local de referencia no indice 1 na operand stack
static u4 handle_aload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[1];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}
// da push de uma variavel local de referencia no indice 2 na operand stack
static u4 handle_aload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[2];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}
// da push de uma variavel local de referencia no indice 3 na operand stack
static u4 handle_aload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[3];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da pop na operand stack e armazena referencia em variaveis locais no indice 0
static u4 handle_astore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[0] = content;
    return pc + 1;
}
// da pop na operand stack e armazena referencia em variaveis locais no indice 1
static u4 handle_astore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[1] = content;
    return pc + 1;
}
// da pop na operand stack e armazena referencia em variaveis locais no indice 2
static u4 handle_astore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[2] = content;
    return pc + 1;
}
// da pop na operand stack e armazena referencia em variaveis locais no indice 3
static u4 handle_astore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[3] = content;
    return pc + 1;
}

static u4 handle_baload(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_bastore(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_caload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_castore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_checkcast(RuntimeContext *ctx, Code_attribute *code_attr) {}

static u4 handle_dadd(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_daload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dastore(RuntimeContext *ctx, Code_attribute *code_attr) {}

static u4 handle_ddiv(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_dmul(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dneg(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_drem(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_dsub(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_dup_x1(RuntimeContext *ctx, Code_attribute *code_attr) {

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
static u4 handle_dup_x2(RuntimeContext *ctx, Code_attribute *code_attr) {

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
static u4 handle_dup2(RuntimeContext *ctx, Code_attribute *code_attr) {

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
static u4 handle_dup2_x1(RuntimeContext *ctx, Code_attribute *code_attr) {

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
static u4 handle_dup2_x2(RuntimeContext *ctx, Code_attribute *code_attr) {

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

static u4 handle_fadd(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_faload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fastore(RuntimeContext *ctx, Code_attribute *code_attr) {}

// da push de 0.0f na operand stack
static u4 handle_fconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    union { float f; u4 u; } val = { 0.0f };
    push(frame->operand_stack, (void*)&val.u);
    return pc + 1;
}

// da push de 1.0f na operand stack
static u4 handle_fconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    union { float f; u4 u; } val = { 1.0f };
    push(frame->operand_stack, (void*)&val.u);
    return pc + 1;
}

// da push de 2.0f na operand stack
static u4 handle_fconst_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    union { float f; u4 u; } val = { 2.0f };
    push(frame->operand_stack, (void*)&val.u);
    return pc + 1;
}

static u4 handle_fdiv(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_fload(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u1* code = code_attr->code;
    u1 parameter = code[pc+1];
    u4 content = frame->local_variables[parameter];
    push(frame->operand_stack, (void*)&content);
    return pc + 2;
}

// da push de uma variavel local float no indice 0 na operand stack
static u4 handle_fload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[0];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de uma variavel local float no indice 1 na operand stack
static u4 handle_fload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[1];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de uma variavel local float no indice 2 na operand stack
static u4 handle_fload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[2];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

// da push de uma variavel local float no indice 3 na operand stack
static u4 handle_fload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = frame->local_variables[3];
    push(frame->operand_stack, (void*)&content);
    return pc + 1;
}

static u4 handle_fmul(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_fneg(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 raw = *((u4*) getTop(frame->operand_stack)); pop(frame->operand_stack);
    union { u4 u; float f; } val = { raw };
    float result = -val.f;
    union { float f; u4 u; } res = { result };
    push(frame->operand_stack, (void*)&res.u);
    return pc + 1;
}

static u4 handle_frem(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_fstore(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_fstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[0] = content;
    return pc + 1;
}
// da pop na operand stack e armazena float em variaveis locais no indice 1
static u4 handle_fstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[1] = content;
    return pc + 1;
}
// da pop na operand stack e armazena float em variaveis locais no indice 2
static u4 handle_fstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[2] = content;
    return pc + 1;
}
// da pop na operand stack e armazena float em variaveis locais no indice 3
static u4 handle_fstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    u4 pc = ctx->thread->pc;
    Frame* frame = (Frame*)getTop(ctx->thread->frame_stack);
    u4 content = *((u4*) getTop(frame->operand_stack));
    pop(frame->operand_stack);
    frame->local_variables[3] = content;
    return pc + 1;
}
static u4 handle_fsub(RuntimeContext *ctx, Code_attribute *code_attr) {
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
static u4 handle_ishl(RuntimeContext *ctx, Code_attribute *code_attr) {

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
static u4 handle_ishr(RuntimeContext *ctx, Code_attribute *code_attr) {

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
static u4 handle_iushr(RuntimeContext *ctx, Code_attribute *code_attr) {

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

static u4 handle_iaload(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_iastore(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_instanceof(RuntimeContext *ctx, Code_attribute *code_attr) {}

static u4 handle_laload(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_lastore(RuntimeContext *ctx, Code_attribute *code_attr) {
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

static u4 handle_ldc_w(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ldc2_w(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lneg(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_saload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_sastore(RuntimeContext *ctx, Code_attribute *code_attr) {}

// ============================================================
// Execution loop
// ============================================================
void interpret(RuntimeContext *ctx){
    JVMThread* thread = ctx->thread;
    while (!isEmpty(thread->frame_stack)){
        Code_attribute* code_attr = getCodeFromTopFrame(thread->frame_stack);
        u1* code = code_attr->code;
        u1 opcode = code[thread->pc];
        InstructionHandler handler = decode(opcode);
        if (handler == NULL){
            // TODO: handle unimplemented opcode / throw error
            break;
        }
        thread->pc = handler(ctx, code_attr);
    }
}
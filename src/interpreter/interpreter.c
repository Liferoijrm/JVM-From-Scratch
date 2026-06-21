#include "interpreter.h"
#include "instruction_handler.h"

// ============================================================
// Stub implementations for each instruction
// Add your logic here: operand stack access, constant pool resolution, etc.
// ============================================================
static u4 handle_nop(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    ////return pc + 1;
}

static u4 handle_aconst_null(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: push null onto operand stack
    //return pc + 1;
}

static u4 handle_iconst_m1(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: push int -1 onto operand stack
    //return pc + 1;
}

static u4 handle_iconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iconst_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iconst_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iconst_4(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iconst_5(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_bipush(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 byte = code[pc + 1];
    // TODO: sign-extend via sign_extend_byte(byte) and push onto operand stack
    //return pc + 2;
}

static u4 handle_sipush(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 val = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: sign-extend via sign_extend_short(val) and push onto operand stack
    //return pc + 3;
}

static u4 handle_ldc(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    // TODO: push constant from constant pool at index
    //return pc + 2;
}

static u4 handle_iload(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    // TODO: push local variable at index onto operand stack
    //return pc + 2;
}

static u4 handle_iload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iload_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iload_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iload_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_aload_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_istore(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    // TODO: pop int from operand stack into local variable at index
    //return pc + 2;
}

static u4 handle_istore_0(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_istore_1(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_istore_2(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_istore_3(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iadd(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two ints, push sum
    //return pc + 1;
}

static u4 handle_isub(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two ints, push difference
    //return pc + 1;
}

static u4 handle_imul(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two ints, push product
    //return pc + 1;
}

static u4 handle_idiv(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two ints, push quotient (handle division by zero)
    //return pc + 1;
}

static u4 handle_irem(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two ints, push remainder
    //return pc + 1;
}

static u4 handle_ineg(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop int, push negated value
    //return pc + 1;
}

static u4 handle_iinc(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    //u1 const_byte = code[pc + 2];
    // TODO: increment local variable at index by sign_extend_byte(const_byte)
    //return pc + 3;
}

static u4 handle_ifeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if == 0, jump to pc + compute_branch(branch_bytes)
    //return pc + 3;
}

static u4 handle_ifne(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if != 0, jump to pc + compute_branch(branch_bytes)
    //return pc + 3;
}

static u4 handle_iflt(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if < 0, jump
    //return pc + 3;
}

static u4 handle_ifge(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if >= 0, jump
    //return pc + 3;
}

static u4 handle_ifgt(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if > 0, jump
    //return pc + 3;
}

static u4 handle_ifle(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if <= 0, jump
    //return pc + 3;
}

static u4 handle_if_icmpeq(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if equal, jump
    //return pc + 3;
}

static u4 handle_if_icmpne(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if not equal, jump
    //return pc + 3;
}

static u4 handle_if_icmplt(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 < value2, jump
    //return pc + 3;
}

static u4 handle_if_icmpge(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 >= value2, jump
    //return pc + 3;
}

static u4 handle_if_icmpgt(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 > value2, jump
    //return pc + 3;
}

static u4 handle_if_icmple(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 <= value2, jump
    //return pc + 3;
}

static u4 handle_goto(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    //return (u4)(pc + compute_branch(branch_bytes));
}

static u4 handle_i_return(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)code;
    // TODO: pop int from operand stack, pop frame, push result to caller
    //return 0;
}

static u4 handle_return(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop current frame, //return to caller
    //return 0;
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
    //(void)frame;
    //u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve field from constant pool, push static field value
    //return pc + 3;
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
    //(void)frame;
    //u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: allocate object at class_index, push reference
    //return pc + 3;
}

static u4 handle_newarray(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 atype = code[pc + 1];
    // TODO: pop size, allocate primitive array
    //return pc + 2;
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

static u4 handle_aload(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    // TODO: push array reference at local index onto operand stack
    //return pc + 2;
}

static u4 handle_astore(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u1 index = code[pc + 1];
    // TODO: pop reference into local variable at index
    //return pc + 2;
}

static u4 handle_pop(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop top value from operand stack
    //return pc + 1;
}

static u4 handle_dup(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: duplicate top operand stack value
    //return pc + 1;
}

static u4 handle_swap(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: swap top two values on operand stack
    //return pc + 1;
}

static u4 handle_athrow(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop exception, resolve handler or propagate
    //return pc;
}

static u4 handle_ifnull(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop reference; if null, jump
    //return pc + 3;
}

static u4 handle_ifnonnull(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop reference; if not null, jump
    //return pc + 3;
}

static u4 handle_lcmp(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two longs, push -1/0/1
    //return pc + 1;
}

static u4 handle_fcmpl(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two floats, push comparison (-1 if NaN)
    //return pc + 1;
}

static u4 handle_fcmpg(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two floats, push comparison (1 if NaN)
    //return pc + 1;
}

static u4 handle_dcmpl(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two doubles, push comparison (-1 if NaN)
    //return pc + 1;
}

static u4 handle_dcmpg(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop two doubles, push comparison (1 if NaN)
    //return pc + 1;
}

static u4 handle_ladd(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_lsub(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_iand(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_ior(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_ixor(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_i2l(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_i2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_i2d(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_l2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_l2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_l2d(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_f2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_f2l(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_f2d(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_d2i(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_d2l(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_d2f(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_lshl(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_lshr(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_lushr(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_land(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_lor(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_lxor(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    //return pc + 1;
}

static u4 handle_i2b(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop int, truncate to byte, sign-extend, push back
    //return pc + 1;
}

static u4 handle_i2c(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop int, zero-extend to char (ushort), push back
    //return pc + 1;
}

static u4 handle_i2s(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame; (void)code;
    // TODO: pop int, truncate to short, sign-extend, push back
    //return pc + 1;
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
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two references; if equal, jump
    //return pc + 3;
}

static u4 handle_if_acmpne(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two references; if not equal, jump
    //return pc + 3;
}

static u4 handle_goto_w(RuntimeContext *ctx, Code_attribute *code_attr) {
    //(void)frame;
    //u4 branch_bytes = ((u4)code[pc + 1] << 24) |
    //                  ((u4)code[pc + 2] << 16) |
    //                  ((u4)code[pc + 3] << 8)  |
    //                  ((u4)code[pc + 4]);
    //return (u4)(pc + compute_branch_wide(branch_bytes));
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
static u4 handle_aload_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_aload_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_aload_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_areturn(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_astore_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_astore_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_astore_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_astore_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_baload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_bastore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_caload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_castore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_checkcast(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dadd(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_daload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dastore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ddiv(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dload_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dload_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dload_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dload_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dmul(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dneg(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_drem(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dreturn(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dstore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dsub(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dup_x1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dup_x2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dup2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dup2_x1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_dup2_x2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fadd(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_faload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fastore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fconst_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fdiv(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fload_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fload_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fload_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fload_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fmul(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fneg(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_frem(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_freturn(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fstore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_fsub(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_iaload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_iastore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_instanceof(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_invokedynamic(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ishl(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ishr(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_iushr(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_laload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lastore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lconst_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lconst_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ldc_w(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ldc2_w(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_ldiv(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lload(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lload_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lload_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lload_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lload_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lmul(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lneg(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lrem(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lreturn(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lstore(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lstore_0(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lstore_1(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lstore_2(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_lstore_3(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_monitorenter(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_monitorexit(RuntimeContext *ctx, Code_attribute *code_attr) {}
static u4 handle_pop2(RuntimeContext *ctx, Code_attribute *code_attr) {}
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
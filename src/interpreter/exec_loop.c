#include "exec_loop.h"
#include "estrutura.h"

// ============================================================
// Stub implementations for each instruction
// Add your logic here: operand stack access, constant pool resolution, etc.
// ============================================================
static u4 handle_nop(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_aconst_null(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: push null onto operand stack
    return pc + 1;
}

static u4 handle_iconst_m1(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: push int -1 onto operand stack
    return pc + 1;
}

static u4 handle_iconst_0(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iconst_1(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iconst_2(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iconst_3(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iconst_4(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iconst_5(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_bipush(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 byte = code[pc + 1];
    // TODO: sign-extend via sign_extend_byte(byte) and push onto operand stack
    return pc + 2;
}

static u4 handle_sipush(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 val = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: sign-extend via sign_extend_short(val) and push onto operand stack
    return pc + 3;
}

static u4 handle_ldc(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 index = code[pc + 1];
    // TODO: push constant from constant pool at index
    return pc + 2;
}

static u4 handle_iload(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 index = code[pc + 1];
    // TODO: push local variable at index onto operand stack
    return pc + 2;
}

static u4 handle_iload_0(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iload_1(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iload_2(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iload_3(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_aload_0(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_istore(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 index = code[pc + 1];
    // TODO: pop int from operand stack into local variable at index
    return pc + 2;
}

static u4 handle_istore_0(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_istore_1(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_istore_2(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_istore_3(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iadd(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two ints, push sum
    return pc + 1;
}

static u4 handle_isub(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two ints, push difference
    return pc + 1;
}

static u4 handle_imul(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two ints, push product
    return pc + 1;
}

static u4 handle_idiv(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two ints, push quotient (handle division by zero)
    return pc + 1;
}

static u4 handle_irem(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two ints, push remainder
    return pc + 1;
}

static u4 handle_ineg(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop int, push negated value
    return pc + 1;
}

static u4 handle_iinc(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 index = code[pc + 1];
    u1 const_byte = code[pc + 2];
    // TODO: increment local variable at index by sign_extend_byte(const_byte)
    return pc + 3;
}

static u4 handle_ifeq(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if == 0, jump to pc + compute_branch(branch_bytes)
    return pc + 3;
}

static u4 handle_ifne(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if != 0, jump to pc + compute_branch(branch_bytes)
    return pc + 3;
}

static u4 handle_iflt(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if < 0, jump
    return pc + 3;
}

static u4 handle_ifge(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if >= 0, jump
    return pc + 3;
}

static u4 handle_ifgt(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if > 0, jump
    return pc + 3;
}

static u4 handle_ifle(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop int; if <= 0, jump
    return pc + 3;
}

static u4 handle_if_icmpeq(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if equal, jump
    return pc + 3;
}

static u4 handle_if_icmpne(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if not equal, jump
    return pc + 3;
}

static u4 handle_if_icmplt(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 < value2, jump
    return pc + 3;
}

static u4 handle_if_icmpge(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 >= value2, jump
    return pc + 3;
}

static u4 handle_if_icmpgt(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 > value2, jump
    return pc + 3;
}

static u4 handle_if_icmple(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two ints; if value1 <= value2, jump
    return pc + 3;
}

static u4 handle_goto(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    return (u4)(pc + compute_branch(branch_bytes));
}

static u4 handle_ireturn(void *frame, u1 *code, u4 pc) {
    (void)code;
    // TODO: pop int from operand stack, pop frame, push result to caller
    return 0;
}

static u4 handle_return(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop current frame, return to caller
    return 0;
}

static u4 handle_invokestatic(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve method, create new frame, push to stack
    return pc + 3;
}

static u4 handle_invokevirtual(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve method ref, dispatch by object type, push new frame
    return pc + 3;
}

static u4 handle_invokespecial(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 method_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: handle <init>, super, private methods
    return pc + 3;
}

static u4 handle_invokeinterface(void *frame, u1 *code, u4 pc) {
    (void)frame;
    // TODO: parse method reference and optional count field
    return pc + 5;
}

static u4 handle_getstatic(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: resolve field from constant pool, push static field value
    return pc + 3;
}

static u4 handle_putstatic(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop value, store in static field
    return pc + 3;
}

static u4 handle_getfield(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop object ref, resolve field, push field value
    return pc + 3;
}

static u4 handle_putfield(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 field_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop value and object ref, store in object's field
    return pc + 3;
}

static u4 handle_new(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: allocate object at class_index, push reference
    return pc + 3;
}

static u4 handle_newarray(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 atype = code[pc + 1];
    // TODO: pop size, allocate primitive array
    return pc + 2;
}

static u4 handle_anewarray(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop size, allocate object array at class_index
    return pc + 3;
}

static u4 handle_arraylength(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop array ref, push length
    return pc + 1;
}

static u4 handle_aload(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 index = code[pc + 1];
    // TODO: push array reference at local index onto operand stack
    return pc + 2;
}

static u4 handle_astore(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u1 index = code[pc + 1];
    // TODO: pop reference into local variable at index
    return pc + 2;
}

static u4 handle_pop(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop top value from operand stack
    return pc + 1;
}

static u4 handle_dup(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: duplicate top operand stack value
    return pc + 1;
}

static u4 handle_swap(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: swap top two values on operand stack
    return pc + 1;
}

static u4 handle_athrow(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop exception, resolve handler or propagate
    return pc;
}

static u4 handle_ifnull(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop reference; if null, jump
    return pc + 3;
}

static u4 handle_ifnonnull(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop reference; if not null, jump
    return pc + 3;
}

static u4 handle_lcmp(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two longs, push -1/0/1
    return pc + 1;
}

static u4 handle_fcmpl(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two floats, push comparison (-1 if NaN)
    return pc + 1;
}

static u4 handle_fcmpg(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two floats, push comparison (1 if NaN)
    return pc + 1;
}

static u4 handle_dcmpl(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two doubles, push comparison (-1 if NaN)
    return pc + 1;
}

static u4 handle_dcmpg(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop two doubles, push comparison (1 if NaN)
    return pc + 1;
}

static u4 handle_ladd(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_lsub(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_iand(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_ior(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_ixor(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_i2l(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_i2f(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_i2d(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_l2i(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_l2f(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_l2d(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_f2i(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_f2l(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_f2d(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_d2i(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_d2l(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_d2f(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_lshl(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_lshr(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_lushr(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_land(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_lor(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_lxor(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    return pc + 1;
}

static u4 handle_i2b(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop int, truncate to byte, sign-extend, push back
    return pc + 1;
}

static u4 handle_i2c(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop int, zero-extend to char (ushort), push back
    return pc + 1;
}

static u4 handle_i2s(void *frame, u1 *code, u4 pc) {
    (void)frame; (void)code;
    // TODO: pop int, truncate to short, sign-extend, push back
    return pc + 1;
}

static u4 handle_ret(void *frame, u1 *code, u4 pc) {
    (void)frame;
    // TODO: read local variable, restore pc from subroutine
    return pc + 1;
}

static u4 handle_tableswitch(void *frame, u1 *code, u4 pc) {
    (void)frame;
    // TODO: parse padding, low/high/default, compute jump target
    return pc;
}

static u4 handle_lookupswitch(void *frame, u1 *code, u4 pc) {
    (void)frame;
    // TODO: parse padding, match pairs, compute jump target
    return pc;
}

static u4 handle_wide(void *frame, u1 *code, u4 pc) {
    (void)frame;
    // TODO: next opcode uses 2-byte index instead of 1-byte
    return pc + 1;
}

static u4 handle_multianewarray(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 class_index = ((u2)code[pc + 1] << 8) | code[pc + 2];
    u1 dimensions = code[pc + 3];
    // TODO: pop N dimension sizes, allocate multi-dimensional array
    return pc + 4;
}

static u4 handle_if_acmpeq(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two references; if equal, jump
    return pc + 3;
}

static u4 handle_if_acmpne(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: pop two references; if not equal, jump
    return pc + 3;
}

static u4 handle_goto_w(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u4 branch_bytes = ((u4)code[pc + 1] << 24) |
                      ((u4)code[pc + 2] << 16) |
                      ((u4)code[pc + 3] << 8)  |
                      ((u4)code[pc + 4]);
    return (u4)(pc + compute_branch_wide(branch_bytes));
}

static u4 handle_jsr(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u2 branch_bytes = ((u2)code[pc + 1] << 8) | code[pc + 2];
    // TODO: push return address, jump to subroutine
    return (u4)(pc + compute_branch(branch_bytes));
}

static u4 handle_jsr_w(void *frame, u1 *code, u4 pc) {
    (void)frame;
    u4 branch_bytes = ((u4)code[pc + 1] << 24) |
                      ((u4)code[pc + 2] << 16) |
                      ((u4)code[pc + 3] << 8)  |
                      ((u4)code[pc + 4]);
    // TODO: push return address, jump to subroutine
    return (u4)(pc + compute_branch_wide(branch_bytes));
}

// ============================================================
// Execution loop
// ============================================================
void execute_bytecode(u1 *code, u4 code_length) {
    u4 pc = 0;
    while (pc < code_length) {
        u1 opcode = code[pc];
        InstructionHandler handler = decode(opcode);
        if (handler == NULL) {
            // TODO: handle unimplemented opcode / throw error
            break;
        }
        pc = handler(NULL, code, pc);  // NULL frame for now
    }
}
#include "estrutura.h"

// Forward declarations for instruction handlers (implemented in exec_loop.c)
static u4 handle_nop(void *frame, u1 *code, u4 pc);
static u4 handle_aconst_null(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_m1(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_0(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_1(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_2(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_3(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_4(void *frame, u1 *code, u4 pc);
static u4 handle_iconst_5(void *frame, u1 *code, u4 pc);
static u4 handle_bipush(void *frame, u1 *code, u4 pc);
static u4 handle_sipush(void *frame, u1 *code, u4 pc);
static u4 handle_ldc(void *frame, u1 *code, u4 pc);
static u4 handle_iload(void *frame, u1 *code, u4 pc);
static u4 handle_iload_0(void *frame, u1 *code, u4 pc);
static u4 handle_iload_1(void *frame, u1 *code, u4 pc);
static u4 handle_iload_2(void *frame, u1 *code, u4 pc);
static u4 handle_iload_3(void *frame, u1 *code, u4 pc);
static u4 handle_aload_0(void *frame, u1 *code, u4 pc);
static u4 handle_istore(void *frame, u1 *code, u4 pc);
static u4 handle_istore_0(void *frame, u1 *code, u4 pc);
static u4 handle_istore_1(void *frame, u1 *code, u4 pc);
static u4 handle_istore_2(void *frame, u1 *code, u4 pc);
static u4 handle_istore_3(void *frame, u1 *code, u4 pc);
static u4 handle_iadd(void *frame, u1 *code, u4 pc);
static u4 handle_isub(void *frame, u1 *code, u4 pc);
static u4 handle_imul(void *frame, u1 *code, u4 pc);
static u4 handle_idiv(void *frame, u1 *code, u4 pc);
static u4 handle_irem(void *frame, u1 *code, u4 pc);
static u4 handle_ineg(void *frame, u1 *code, u4 pc);
static u4 handle_iinc(void *frame, u1 *code, u4 pc);
static u4 handle_if_icmpeq(void *frame, u1 *code, u4 pc);
static u4 handle_if_icmpne(void *frame, u1 *code, u4 pc);
static u4 handle_if_icmplt(void *frame, u1 *code, u4 pc);
static u4 handle_if_icmpge(void *frame, u1 *code, u4 pc);
static u4 handle_if_icmpgt(void *frame, u1 *code, u4 pc);
static u4 handle_if_icmple(void *frame, u1 *code, u4 pc);
static u4 handle_goto(void *frame, u1 *code, u4 pc);
static u4 handle_ireturn(void *frame, u1 *code, u4 pc);
static u4 handle_return(void *frame, u1 *code, u4 pc);
static u4 handle_invokestatic(void *frame, u1 *code, u4 pc);
static u4 handle_invokevirtual(void *frame, u1 *code, u4 pc);
static u4 handle_invokespecial(void *frame, u1 *code, u4 pc);
static u4 handle_invokeinterface(void *frame, u1 *code, u4 pc);
static u4 handle_getstatic(void *frame, u1 *code, u4 pc);
static u4 handle_putstatic(void *frame, u1 *code, u4 pc);
static u4 handle_getfield(void *frame, u1 *code, u4 pc);
static u4 handle_putfield(void *frame, u1 *code, u4 pc);
static u4 handle_new(void *frame, u1 *code, u4 pc);
static u4 handle_newarray(void *frame, u1 *code, u4 pc);
static u4 handle_anewarray(void *frame, u1 *code, u4 pc);
static u4 handle_arraylength(void *frame, u1 *code, u4 pc);
static u4 handle_aload(void *frame, u1 *code, u4 pc);
static u4 handle_astore(void *frame, u1 *code, u4 pc);
static u4 handle_pop(void *frame, u1 *code, u4 pc);
static u4 handle_dup(void *frame, u1 *code, u4 pc);
static u4 handle_swap(void *frame, u1 *code, u4 pc);
static u4 handle_athrow(void *frame, u1 *code, u4 pc);
static u4 handle_ifeq(void *frame, u1 *code, u4 pc);
static u4 handle_ifne(void *frame, u1 *code, u4 pc);
static u4 handle_iflt(void *frame, u1 *code, u4 pc);
static u4 handle_ifge(void *frame, u1 *code, u4 pc);
static u4 handle_ifgt(void *frame, u1 *code, u4 pc);
static u4 handle_ifle(void *frame, u1 *code, u4 pc);
static u4 handle_ifnull(void *frame, u1 *code, u4 pc);
static u4 handle_ifnonnull(void *frame, u1 *code, u4 pc);
static u4 handle_lcmp(void *frame, u1 *code, u4 pc);
static u4 handle_fcmpl(void *frame, u1 *code, u4 pc);
static u4 handle_fcmpg(void *frame, u1 *code, u4 pc);
static u4 handle_dcmpl(void *frame, u1 *code, u4 pc);
static u4 handle_dcmpg(void *frame, u1 *code, u4 pc);
static u4 handle_ladd(void *frame, u1 *code, u4 pc);
static u4 handle_lsub(void *frame, u1 *code, u4 pc);
static u4 handle_iand(void *frame, u1 *code, u4 pc);
static u4 handle_ior(void *frame, u1 *code, u4 pc);
static u4 handle_ixor(void *frame, u1 *code, u4 pc);
static u4 handle_i2l(void *frame, u1 *code, u4 pc);
static u4 handle_i2f(void *frame, u1 *code, u4 pc);
static u4 handle_i2d(void *frame, u1 *code, u4 pc);
static u4 handle_l2i(void *frame, u1 *code, u4 pc);
static u4 handle_l2f(void *frame, u1 *code, u4 pc);
static u4 handle_l2d(void *frame, u1 *code, u4 pc);
static u4 handle_f2i(void *frame, u1 *code, u4 pc);
static u4 handle_f2l(void *frame, u1 *code, u4 pc);
static u4 handle_f2d(void *frame, u1 *code, u4 pc);
static u4 handle_d2i(void *frame, u1 *code, u4 pc);
static u4 handle_d2l(void *frame, u1 *code, u4 pc);
static u4 handle_d2f(void *frame, u1 *code, u4 pc);
static u4 handle_lshl(void *frame, u1 *code, u4 pc);
static u4 handle_lshr(void *frame, u1 *code, u4 pc);
static u4 handle_lushr(void *frame, u1 *code, u4 pc);
static u4 handle_land(void *frame, u1 *code, u4 pc);
static u4 handle_lor(void *frame, u1 *code, u4 pc);
static u4 handle_lxor(void *frame, u1 *code, u4 pc);
static u4 handle_i2b(void *frame, u1 *code, u4 pc);
static u4 handle_i2c(void *frame, u1 *code, u4 pc);
static u4 handle_i2s(void *frame, u1 *code, u4 pc);
static u4 handle_ret(void *frame, u1 *code, u4 pc);
static u4 handle_tableswitch(void *frame, u1 *code, u4 pc);
static u4 handle_lookupswitch(void *frame, u1 *code, u4 pc);
static u4 handle_wide(void *frame, u1 *code, u4 pc);
static u4 handle_multianewarray(void *frame, u1 *code, u4 pc);
static u4 handle_if_acmpeq(void *frame, u1 *code, u4 pc);
static u4 handle_if_acmpne(void *frame, u1 *code, u4 pc);
static u4 handle_goto_w(void *frame, u1 *code, u4 pc);
static u4 handle_jsr(void *frame, u1 *code, u4 pc);
static u4 handle_jsr_w(void *frame, u1 *code, u4 pc);

// Decoder Table: maps opcode byte -> function pointer
static InstructionHandler decoder_table[256] = {
    [OP_NOP]                = handle_nop,
    [OP_ACONST_NULL]        = handle_aconst_null,
    [OP_ICONST_M1]          = handle_iconst_m1,
    [OP_ICONST_0]           = handle_iconst_0,
    [OP_ICONST_1]           = handle_iconst_1,
    [OP_ICONST_2]           = handle_iconst_2,
    [OP_ICONST_3]           = handle_iconst_3,
    [OP_ICONST_4]           = handle_iconst_4,
    [OP_ICONST_5]           = handle_iconst_5,
    [OP_BIPUSH]             = handle_bipush,
    [OP_SIPUSH]             = handle_sipush,
    [OP_LDC]                = handle_ldc,
    [OP_ILOAD]              = handle_iload,
    [OP_ILOAD_0]            = handle_iload_0,
    [OP_ILOAD_1]            = handle_iload_1,
    [OP_ILOAD_2]            = handle_iload_2,
    [OP_ILOAD_3]            = handle_iload_3,
    [OP_ALOAD_0]            = handle_aload_0,
    [OP_ISTORE]             = handle_istore,
    [OP_ISTORE_0]           = handle_istore_0,
    [OP_ISTORE_1]           = handle_istore_1,
    [OP_ISTORE_2]           = handle_istore_2,
    [OP_ISTORE_3]           = handle_istore_3,
    [OP_IADD]               = handle_iadd,
    [OP_ISUB]               = handle_isub,
    [OP_IMUL]               = handle_imul,
    [OP_IDIV]               = handle_idiv,
    [OP_IREM]               = handle_irem,
    [OP_INEG]               = handle_ineg,
    [OP_IINC]               = handle_iinc,
    [OP_IFEQ]               = handle_ifeq,
    [OP_IFNE]               = handle_ifne,
    [OP_IFLT]               = handle_iflt,
    [OP_IFGE]               = handle_ifge,
    [OP_IFGT]               = handle_ifgt,
    [OP_IFLE]               = handle_ifle,
    [OP_IF_ICMPEQ]          = handle_if_icmpeq,
    [OP_IF_ICMPNE]          = handle_if_icmpne,
    [OP_IF_ICMPLT]          = handle_if_icmplt,
    [OP_IF_ICMPGE]          = handle_if_icmpge,
    [OP_IF_ICMPGT]          = handle_if_icmpgt,
    [OP_IF_ICMPLE]          = handle_if_icmple,
    [OP_GOTO]               = handle_goto,
    [OP_IRETURN]            = handle_ireturn,
    [OP_RETURN]             = handle_return,
    [OP_INVOKESTATIC]       = handle_invokestatic,
    [OP_INVOKEVIRTUAL]      = handle_invokevirtual,
    [OP_INVOKESPECIAL]      = handle_invokespecial,
    [OP_INVOKEINTERFACE]    = handle_invokeinterface,
    [OP_GETSTATIC]          = handle_getstatic,
    [OP_PUTSTATIC]          = handle_putstatic,
    [OP_GETFIELD]           = handle_getfield,
    [OP_PUTFIELD]           = handle_putfield,
    [OP_NEW]                = handle_new,
    [OP_NEWARRAY]           = handle_newarray,
    [OP_ANEWARRAY]          = handle_anewarray,
    [OP_ARRAYLENGTH]        = handle_arraylength,
    [OP_ALOAD]              = handle_aload,
    [OP_ASTORE]             = handle_astore,
    [OP_POP]                = handle_pop,
    [OP_DUP]                = handle_dup,
    [OP_SWAP]               = handle_swap,
    [OP_ATHROW]             = handle_athrow,
    [OP_IFNULL]             = handle_ifnull,
    [OP_IFNONNULL]          = handle_ifnonnull,
    [OP_LCMP]               = handle_lcmp,
    [OP_FCMPL]              = handle_fcmpl,
    [OP_FCMPG]              = handle_fcmpg,
    [OP_DCMPL]              = handle_dcmpl,
    [OP_DCMPG]              = handle_dcmpg,
    [OP_LADD]               = handle_ladd,
    [OP_LSUB]               = handle_lsub,
    [OP_IAND]               = handle_iand,
    [OP_IOR]                = handle_ior,
    [OP_IXOR]               = handle_ixor,
    [OP_I2L]                = handle_i2l,
    [OP_I2F]                = handle_i2f,
    [OP_I2D]                = handle_i2d,
    [OP_L2I]                = handle_l2i,
    [OP_L2F]                = handle_l2f,
    [OP_L2D]                = handle_l2d,
    [OP_F2I]                = handle_f2i,
    [OP_F2L]                = handle_f2l,
    [OP_F2D]                = handle_f2d,
    [OP_D2I]                = handle_d2i,
    [OP_D2L]                = handle_d2l,
    [OP_D2F]                = handle_d2f,
    [OP_LSHL]               = handle_lshl,
    [OP_LSHR]               = handle_lshr,
    [OP_LUSHR]              = handle_lushr,
    [OP_LAND]               = handle_land,
    [OP_LOR]                = handle_lor,
    [OP_LXOR]               = handle_lxor,
    [OP_I2B]                = handle_i2b,
    [OP_I2C]                = handle_i2c,
    [OP_I2S]                = handle_i2s,
    [OP_RET]                = handle_ret,
    [OP_TABLESWITCH]        = handle_tableswitch,
    [OP_LOOKUPSWITCH]       = handle_lookupswitch,
    [OP_WIDE]               = handle_wide,
    [OP_MULTIANEWARRAY]     = handle_multianewarray,
    [OP_IF_ACMPEQ]          = handle_if_acmpeq,
    [OP_IF_ACMPNE]          = handle_if_acmpne,
    [OP_GOTO_W]             = handle_goto_w,
    [OP_JSR]                = handle_jsr,
    [OP_JSR_W]              = handle_jsr_w,
};

// Dispatch: given a bytecode opcode, returns the corresponding handler
static inline InstructionHandler decode(u1 opcode) {
    if (opcode < 256) {
        return decoder_table[opcode];
    }
    return NULL;
}

// Sign extension helpers (pure unsigned arithmetic)
static inline u4 sign_extend_byte(u1 b) {
    return (u4)((b ^ 0x80U) - 0x80U);
}

static inline u4 sign_extend_short(u2 s) {
    return (u4)((s ^ 0x8000U) - 0x8000U);
}

static inline u4 compute_branch(u2 branch_bytes) {
    return sign_extend_short(branch_bytes);
}

static inline u4 compute_branch_wide(u4 v) {
    return (v ^ 0x80000000U) - 0x80000000U;
}
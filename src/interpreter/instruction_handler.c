#include "instruction_handler.h"

// Forward declarations for instruction handlers (implemented in exec_loop.c)
static u4 handle_nop(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_aconst_null(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_m1(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_0(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_1(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_2(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_3(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_4(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iconst_5(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_bipush(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_sipush(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ldc(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iload(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iload_0(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iload_1(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iload_2(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iload_3(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_aload_0(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_istore(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_istore_0(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_istore_1(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_istore_2(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_istore_3(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iadd(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_isub(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_imul(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_idiv(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_irem(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ineg(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iinc(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_icmpeq(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_icmpne(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_icmplt(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_icmpge(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_icmpgt(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_icmple(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_goto(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ireturn(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_return(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_invokestatic(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_invokevirtual(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_invokespecial(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_invokeinterface(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_getstatic(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_putstatic(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_getfield(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_putfield(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_new(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_newarray(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_anewarray(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_arraylength(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_aload(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_astore(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_pop(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_dup(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_swap(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_athrow(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifeq(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifne(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iflt(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifge(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifgt(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifle(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifnull(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ifnonnull(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lcmp(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_fcmpl(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_fcmpg(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_dcmpl(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_dcmpg(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ladd(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lsub(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_iand(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ior(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ixor(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_i2l(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_i2f(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_i2d(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_l2i(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_l2f(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_l2d(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_f2i(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_f2l(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_f2d(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_d2i(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_d2l(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_d2f(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lshl(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lshr(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lushr(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_land(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lor(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lxor(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_i2b(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_i2c(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_i2s(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_ret(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_tableswitch(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_lookupswitch(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_wide(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_multianewarray(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_acmpeq(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_if_acmpne(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_goto_w(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_jsr(RuntimeContext *ctx, Code_attribute *code_attr);
static u4 handle_jsr_w(RuntimeContext *ctx, Code_attribute *code_attr);

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
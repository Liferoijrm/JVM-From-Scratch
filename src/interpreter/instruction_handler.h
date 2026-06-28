/**
 * @file instruction_handler.h
 * @brief Declarações dos manipuladores de instruções bytecode e constantes de opcodes.
 *
 * Define a assinatura dos manipuladores (handlers) para cada instrução
 * bytecode da JVM, a função de dispatch e todas as constantes de opcodes
 * suportadas pelo interpretador.
 */

#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "../utils/utils.h"
#include "interpreter.h"

/* ====================================================================
 * Assinatura dos Manipuladores de Instrução
 * ==================================================================== */

/**
 * @brief Assinatura de função para manipuladores de instruções bytecode.
 * @param ctx Ponteiro para o contexto de execução do interpretador.
 * @param code_attr Ponteiro para o atributo Code contendo o bytecode.
 * @return O valor do próximo PC após a execução da instrução.
 */
typedef u4 (*InstructionHandler)(RuntimeContext *ctx, Code_attribute *code_attr);

/**
 * @brief Retorna o manipulador correspondente a um opcode.
 * @param opcode O código da instrução (ex: 0x60 para iadd).
 * @return Ponteiro para a função manipuladora, ou NULL se não implementado.
 */
InstructionHandler decode(u1 opcode);

/* ====================================================================
 * Constantes de Opcodes (JVM Spec)
 * ==================================================================== */

/** @brief Opcode nop (0x00). */
#define OP_NOP                0x00
/** @brief Opcode aconst_null (0x01). */
#define OP_ACONST_NULL        0x01
/** @brief Opcode iconst_m1 (0x02). */
#define OP_ICONST_M1          0x02
/** @brief Opcode iconst_0 (0x03). */
#define OP_ICONST_0           0x03
/** @brief Opcode iconst_1 (0x04). */
#define OP_ICONST_1           0x04
/** @brief Opcode iconst_2 (0x05). */
#define OP_ICONST_2           0x05
/** @brief Opcode iconst_3 (0x06). */
#define OP_ICONST_3           0x06
/** @brief Opcode iconst_4 (0x07). */
#define OP_ICONST_4           0x07
/** @brief Opcode iconst_5 (0x08). */
#define OP_ICONST_5           0x08
/** @brief Opcode bipush (0x10). */
#define OP_BIPUSH             0x10
/** @brief Opcode sipush (0x11). */
#define OP_SIPUSH             0x11
/** @brief Opcode ldc (0x12). */
#define OP_LDC                0x12
/** @brief Opcode iload (0x15). */
#define OP_ILOAD              0x15
/** @brief Opcode iload_0 (0x1A). */
#define OP_ILOAD_0            0x1A
/** @brief Opcode iload_1 (0x1B). */
#define OP_ILOAD_1            0x1B
/** @brief Opcode iload_2 (0x1C). */
#define OP_ILOAD_2            0x1C
/** @brief Opcode iload_3 (0x1D). */
#define OP_ILOAD_3            0x1D
/** @brief Opcode aload_0 (0x2A). */
#define OP_ALOAD_0            0x2A
/** @brief Opcode istore (0x36). */
#define OP_ISTORE             0x36
/** @brief Opcode istore_0 (0x3B). */
#define OP_ISTORE_0           0x3B
/** @brief Opcode istore_1 (0x3C). */
#define OP_ISTORE_1           0x3C
/** @brief Opcode istore_2 (0x3D). */
#define OP_ISTORE_2           0x3D
/** @brief Opcode istore_3 (0x3E). */
#define OP_ISTORE_3           0x3E
/** @brief Opcode iadd (0x60). */
#define OP_IADD               0x60
/** @brief Opcode isub (0x64). */
#define OP_ISUB               0x64
/** @brief Opcode imul (0x68). */
#define OP_IMUL               0x68
/** @brief Opcode idiv (0x6C). */
#define OP_IDIV               0x6C
/** @brief Opcode irem (0x70). */
#define OP_IREM               0x70
/** @brief Opcode ineg (0x74). */
#define OP_INEG               0x74
/** @brief Opcode iinc (0x84). */
#define OP_IINC               0x84
/** @brief Opcode ifeq (0x99). */
#define OP_IFEQ               0x99
/** @brief Opcode ifne (0x9A). */
#define OP_IFNE               0x9A
/** @brief Opcode iflt (0x9B). */
#define OP_IFLT               0x9B
/** @brief Opcode ifge (0x9C). */
#define OP_IFGE               0x9C
/** @brief Opcode ifgt (0x9D). */
#define OP_IFGT               0x9D
/** @brief Opcode ifle (0x9E). */
#define OP_IFLE               0x9E
/** @brief Opcode if_icmpeq (0x9F). */
#define OP_IF_ICMPEQ          0x9F
/** @brief Opcode if_icmpne (0xA0). */
#define OP_IF_ICMPNE          0xA0
/** @brief Opcode if_icmplt (0xA1). */
#define OP_IF_ICMPLT          0xA1
/** @brief Opcode if_icmpge (0xA2). */
#define OP_IF_ICMPGE          0xA2
/** @brief Opcode if_icmpgt (0xA3). */
#define OP_IF_ICMPGT          0xA3
/** @brief Opcode if_icmple (0xA4). */
#define OP_IF_ICMPLE          0xA4
/** @brief Opcode goto (0xA7). */
#define OP_GOTO               0xA7
/** @brief Opcode ireturn (0xAC). */
#define OP_IRETURN            0xAC
/** @brief Opcode return (0xB1). */
#define OP_RETURN             0xB1
/** @brief Opcode invokestatic (0xB8). */
#define OP_INVOKESTATIC       0xB8
/** @brief Opcode invokevirtual (0xB6). */
#define OP_INVOKEVIRTUAL      0xB6
/** @brief Opcode invokespecial (0xB7). */
#define OP_INVOKESPECIAL      0xB7
/** @brief Opcode invokeinterface (0xB9). */
#define OP_INVOKEINTERFACE    0xB9
/** @brief Opcode getstatic (0xB2). */
#define OP_GETSTATIC          0xB2
/** @brief Opcode putstatic (0xB3). */
#define OP_PUTSTATIC          0xB3
/** @brief Opcode getfield (0xB4). */
#define OP_GETFIELD           0xB4
/** @brief Opcode putfield (0xB5). */
#define OP_PUTFIELD           0xB5
/** @brief Opcode new (0xBB). */
#define OP_NEW                0xBB
/** @brief Opcode newarray (0xBC). */
#define OP_NEWARRAY           0xBC
/** @brief Opcode anewarray (0xBD). */
#define OP_ANEWARRAY          0xBD
/** @brief Opcode arraylength (0xBE). */
#define OP_ARRAYLENGTH        0xBE
/** @brief Opcode aload (0x19). */
#define OP_ALOAD              0x19
/** @brief Opcode astore (0x3A). */
#define OP_ASTORE             0x3A
/** @brief Opcode pop (0x57). */
#define OP_POP                0x57
/** @brief Opcode dup (0x59). */
#define OP_DUP                0x59
/** @brief Opcode swap (0x5F). */
#define OP_SWAP               0x5F
/** @brief Opcode athrow (0xBF). */
#define OP_ATHROW             0xBF
/** @brief Opcode ifnull (0xC6). */
#define OP_IFNULL             0xC6
/** @brief Opcode ifnonnull (0xC7). */
#define OP_IFNONNULL          0xC7
/** @brief Opcode lcmp (0x94). */
#define OP_LCMP               0x94
/** @brief Opcode fcmpl (0x95). */
#define OP_FCMPL              0x95
/** @brief Opcode fcmpg (0x96). */
#define OP_FCMPG              0x96
/** @brief Opcode dcmpl (0x97). */
#define OP_DCMPL              0x97
/** @brief Opcode dcmpg (0x98). */
#define OP_DCMPG              0x98
/** @brief Opcode ladd (0x61). */
#define OP_LADD               0x61
/** @brief Opcode lsub (0x65). */
#define OP_LSUB               0x65
/** @brief Opcode iand (0x7E). */
#define OP_IAND               0x7E
/** @brief Opcode ior (0x80). */
#define OP_IOR                0x80
/** @brief Opcode ixor (0x82). */
#define OP_IXOR               0x82
/** @brief Opcode i2l (0x85). */
#define OP_I2L                0x85
/** @brief Opcode i2f (0x86). */
#define OP_I2F                0x86
/** @brief Opcode i2d (0x87). */
#define OP_I2D                0x87
/** @brief Opcode l2i (0x88). */
#define OP_L2I                0x88
/** @brief Opcode l2f (0x89). */
#define OP_L2F                0x89
/** @brief Opcode l2d (0x8A). */
#define OP_L2D                0x8A
/** @brief Opcode f2i (0x8B). */
#define OP_F2I                0x8B
/** @brief Opcode f2l (0x8C). */
#define OP_F2L                0x8C
/** @brief Opcode f2d (0x8D). */
#define OP_F2D                0x8D
/** @brief Opcode d2i (0x8E). */
#define OP_D2I                0x8E
/** @brief Opcode d2l (0x8F). */
#define OP_D2L                0x8F
/** @brief Opcode d2f (0x90). */
#define OP_D2F                0x90
/** @brief Opcode lshl (0x79). */
#define OP_LSHL               0x79
/** @brief Opcode lshr (0x7B). */
#define OP_LSHR               0x7B
/** @brief Opcode lushr (0x7D). */
#define OP_LUSHR              0x7D
/** @brief Opcode land (0x7F). */
#define OP_LAND               0x7F
/** @brief Opcode lor (0x81). */
#define OP_LOR                0x81
/** @brief Opcode lxor (0x83). */
#define OP_LXOR               0x83
/** @brief Opcode i2b (0x91). */
#define OP_I2B                0x91
/** @brief Opcode i2c (0x92). */
#define OP_I2C                0x92
/** @brief Opcode i2s (0x93). */
#define OP_I2S                0x93
/** @brief Opcode ret (0xA9). */
#define OP_RET                0xA9
/** @brief Opcode tableswitch (0xAA). */
#define OP_TABLESWITCH        0xAA
/** @brief Opcode lookupswitch (0xAB). */
#define OP_LOOKUPSWITCH       0xAB
/** @brief Opcode wide (0xC4). */
#define OP_WIDE               0xC4
/** @brief Opcode multianewarray (0xC5). */
#define OP_MULTIANEWARRAY     0xC5
/** @brief Opcode if_acmpeq (0xA5). */
#define OP_IF_ACMPEQ          0xA5
/** @brief Opcode if_acmpne (0xA6). */
#define OP_IF_ACMPNE          0xA6
/** @brief Opcode goto_w (0xC8). */
#define OP_GOTO_W             0xC8
/** @brief Opcode jsr (0xA8). */
#define OP_JSR                0xA8
/** @brief Opcode jsr_w (0xC9). */
#define OP_JSR_W              0xC9
/** @brief Opcode aaload (0x32). */
#define OP_AALOAD             0x32
/** @brief Opcode aastore (0x53). */
#define OP_AASTORE            0x53
/** @brief Opcode aload_1 (0x2B). */
#define OP_ALOAD_1            0x2B
/** @brief Opcode aload_2 (0x2C). */
#define OP_ALOAD_2            0x2C
/** @brief Opcode aload_3 (0x2D). */
#define OP_ALOAD_3            0x2D
/** @brief Opcode areturn (0xB0). */
#define OP_ARETURN            0xB0
/** @brief Opcode astore_0 (0x4B). */
#define OP_ASTORE_0           0x4B
/** @brief Opcode astore_1 (0x4C). */
#define OP_ASTORE_1           0x4C
/** @brief Opcode astore_2 (0x4D). */
#define OP_ASTORE_2           0x4D
/** @brief Opcode astore_3 (0x4E). */
#define OP_ASTORE_3           0x4E
/** @brief Opcode baload (0x33). */
#define OP_BALOAD             0x33
/** @brief Opcode bastore (0x54). */
#define OP_BASTORE            0x54
/** @brief Opcode caload (0x34). */
#define OP_CALOAD             0x34
/** @brief Opcode castore (0x55). */
#define OP_CASTORE            0x55
/** @brief Opcode checkcast (0xC0). */
#define OP_CHECKCAST          0xC0
/** @brief Opcode dadd (0x63). */
#define OP_DADD               0x63
/** @brief Opcode daload (0x31). */
#define OP_DALOAD             0x31
/** @brief Opcode dastore (0x52). */
#define OP_DASTORE            0x52
/** @brief Opcode dconst_0 (0x0E). */
#define OP_DCONST_0           0x0E
/** @brief Opcode dconst_1 (0x0F). */
#define OP_DCONST_1           0x0F
/** @brief Opcode ddiv (0x6F). */
#define OP_DDIV               0x6F
/** @brief Opcode dload (0x18). */
#define OP_DLOAD              0x18
/** @brief Opcode dload_0 (0x26). */
#define OP_DLOAD_0            0x26
/** @brief Opcode dload_1 (0x27). */
#define OP_DLOAD_1            0x27
/** @brief Opcode dload_2 (0x28). */
#define OP_DLOAD_2            0x28
/** @brief Opcode dload_3 (0x29). */
#define OP_DLOAD_3            0x29
/** @brief Opcode dmul (0x6B). */
#define OP_DMUL               0x6B
/** @brief Opcode dneg (0x77). */
#define OP_DNEG               0x77
/** @brief Opcode drem (0x73). */
#define OP_DREM               0x73
/** @brief Opcode dreturn (0xAF). */
#define OP_DRETURN            0xAF
/** @brief Opcode dstore (0x39). */
#define OP_DSTORE             0x39
/** @brief Opcode dstore_0 (0x47). */
#define OP_DSTORE_0           0x47
/** @brief Opcode dstore_1 (0x48). */
#define OP_DSTORE_1           0x48
/** @brief Opcode dstore_2 (0x49). */
#define OP_DSTORE_2           0x49
/** @brief Opcode dstore_3 (0x4A). */
#define OP_DSTORE_3           0x4A
/** @brief Opcode dsub (0x67). */
#define OP_DSUB               0x67
/** @brief Opcode dup_x1 (0x5A). */
#define OP_DUP_X1             0x5A
/** @brief Opcode dup_x2 (0x5B). */
#define OP_DUP_X2             0x5B
/** @brief Opcode dup2 (0x5C). */
#define OP_DUP2               0x5C
/** @brief Opcode dup2_x1 (0x5D). */
#define OP_DUP2_X1            0x5D
/** @brief Opcode dup2_x2 (0x5E). */
#define OP_DUP2_X2            0x5E
/** @brief Opcode fadd (0x62). */
#define OP_FADD               0x62
/** @brief Opcode faload (0x30). */
#define OP_FALOAD             0x30
/** @brief Opcode fastore (0x51). */
#define OP_FASTORE            0x51
/** @brief Opcode fconst_0 (0x0B). */
#define OP_FCONST_0           0x0B
/** @brief Opcode fconst_1 (0x0C). */
#define OP_FCONST_1           0x0C
/** @brief Opcode fconst_2 (0x0D). */
#define OP_FCONST_2           0x0D
/** @brief Opcode fdiv (0x6E). */
#define OP_FDIV               0x6E
/** @brief Opcode fload (0x17). */
#define OP_FLOAD              0x17
/** @brief Opcode fload_0 (0x22). */
#define OP_FLOAD_0            0x22
/** @brief Opcode fload_1 (0x23). */
#define OP_FLOAD_1            0x23
/** @brief Opcode fload_2 (0x24). */
#define OP_FLOAD_2            0x24
/** @brief Opcode fload_3 (0x25). */
#define OP_FLOAD_3            0x25
/** @brief Opcode fmul (0x6A). */
#define OP_FMUL               0x6A
/** @brief Opcode fneg (0x76). */
#define OP_FNEG               0x76
/** @brief Opcode frem (0x72). */
#define OP_FREM               0x72
/** @brief Opcode freturn (0xAE). */
#define OP_FRETURN            0xAE
/** @brief Opcode fstore (0x38). */
#define OP_FSTORE             0x38
/** @brief Opcode fstore_0 (0x43). */
#define OP_FSTORE_0           0x43
/** @brief Opcode fstore_1 (0x44). */
#define OP_FSTORE_1           0x44
/** @brief Opcode fstore_2 (0x45). */
#define OP_FSTORE_2           0x45
/** @brief Opcode fstore_3 (0x46). */
#define OP_FSTORE_3           0x46
/** @brief Opcode fsub (0x66). */
#define OP_FSUB               0x66
/** @brief Opcode iaload (0x2E). */
#define OP_IALOAD             0x2E
/** @brief Opcode iastore (0x4F). */
#define OP_IASTORE            0x4F
/** @brief Opcode instanceof (0xC1). */
#define OP_INSTANCEOF         0xC1
/** @brief Opcode ishl (0x78). */
#define OP_ISHL               0x78
/** @brief Opcode ishr (0x7A). */
#define OP_ISHR               0x7A
/** @brief Opcode iushr (0x7C). */
#define OP_IUSHR              0x7C
/** @brief Opcode laload (0x2F). */
#define OP_LALOAD             0x2F
/** @brief Opcode lastore (0x50). */
#define OP_LASTORE            0x50
/** @brief Opcode lconst_0 (0x09). */
#define OP_LCONST_0           0x09
/** @brief Opcode lconst_1 (0x0A). */
#define OP_LCONST_1           0x0A
/** @brief Opcode ldc_w (0x13). */
#define OP_LDC_W              0x13
/** @brief Opcode ldc2_w (0x14). */
#define OP_LDC2_W             0x14
/** @brief Opcode ldiv (0x6D). */
#define OP_LDIV               0x6D
/** @brief Opcode lload (0x16). */
#define OP_LLOAD              0x16
/** @brief Opcode lload_0 (0x1E). */
#define OP_LLOAD_0            0x1E
/** @brief Opcode lload_1 (0x1F). */
#define OP_LLOAD_1            0x1F
/** @brief Opcode lload_2 (0x20). */
#define OP_LLOAD_2            0x20
/** @brief Opcode lload_3 (0x21). */
#define OP_LLOAD_3            0x21
/** @brief Opcode lmul (0x69). */
#define OP_LMUL               0x69
/** @brief Opcode lneg (0x75). */
#define OP_LNEG               0x75
/** @brief Opcode lrem (0x71). */
#define OP_LREM               0x71
/** @brief Opcode lreturn (0xAD). */
#define OP_LRETURN            0xAD
/** @brief Opcode lstore (0x37). */
#define OP_LSTORE             0x37
/** @brief Opcode lstore_0 (0x3F). */
#define OP_LSTORE_0           0x3F
/** @brief Opcode lstore_1 (0x40). */
#define OP_LSTORE_1           0x40
/** @brief Opcode lstore_2 (0x41). */
#define OP_LSTORE_2           0x41
/** @brief Opcode lstore_3 (0x42). */
#define OP_LSTORE_3           0x42
/** @brief Opcode monitorenter (0xC2). */
#define OP_MONITORENTER       0xC2
/** @brief Opcode monitorexit (0xC3). */
#define OP_MONITOREXIT        0xC3
/** @brief Opcode pop2 (0x58). */
#define OP_POP2               0x58
/** @brief Opcode saload (0x35). */
#define OP_SALOAD             0x35
/** @brief Opcode sastore (0x56). */
#define OP_SASTORE            0x56

/* ====================================================================
 * Declarações dos Manipuladores de Instrução
 * ==================================================================== */

/**
 * @name Handlers de Instruções Bytecode
 * @{
 */
u4 handle_nop(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aconst_null(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_m1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_4(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iconst_5(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_bipush(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_sipush(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ldc(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iload_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iload_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iload_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iload_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aload_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_istore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_istore_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_istore_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_istore_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_istore_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iadd(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_isub(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_imul(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_idiv(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_irem(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ineg(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iinc(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_icmpeq(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_icmpne(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_icmplt(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_icmpge(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_icmpgt(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_icmple(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_goto(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ireturn(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_return(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_invokestatic(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_invokevirtual(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_invokespecial(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_invokeinterface(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_getstatic(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_putstatic(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_getfield(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_putfield(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_new(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_newarray(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_anewarray(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_arraylength(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_astore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_pop(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dup(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_swap(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_athrow(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifeq(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifne(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iflt(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifge(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifgt(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifle(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifnull(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ifnonnull(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lcmp(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fcmpl(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fcmpg(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dcmpl(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dcmpg(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ladd(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lsub(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iand(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ior(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ixor(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_i2l(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_i2f(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_i2d(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_l2i(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_l2f(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_l2d(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_f2i(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_f2l(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_f2d(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_d2i(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_d2l(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_d2f(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lshl(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lshr(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lushr(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_land(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lor(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lxor(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_i2b(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_i2c(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_i2s(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ret(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_tableswitch(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lookupswitch(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_wide(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_multianewarray(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_acmpeq(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_if_acmpne(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_goto_w(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_jsr(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_jsr_w(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aaload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aastore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aload_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aload_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_aload_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_areturn(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_astore_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_astore_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_astore_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_astore_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_baload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_bastore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_caload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_castore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_checkcast(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dadd(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_daload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dastore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dconst_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dconst_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ddiv(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dload_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dload_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dload_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dload_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dmul(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dneg(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_drem(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dreturn(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dstore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dstore_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dstore_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dstore_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dstore_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dsub(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dup_x1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dup_x2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dup2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dup2_x1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_dup2_x2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fadd(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_faload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fastore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fconst_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fconst_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fconst_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fdiv(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fload_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fload_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fload_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fload_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fmul(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fneg(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_frem(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_freturn(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fstore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fstore_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fstore_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fstore_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fstore_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_fsub(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iaload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iastore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_instanceof(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ishl(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ishr(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_iushr(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_laload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lastore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lconst_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lconst_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ldc_w(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ldc2_w(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_ldiv(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lload_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lload_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lload_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lload_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lmul(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lneg(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lrem(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lreturn(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lstore(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lstore_0(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lstore_1(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lstore_2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_lstore_3(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_pop2(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_saload(RuntimeContext *ctx, Code_attribute *code_attr);
u4 handle_sastore(RuntimeContext *ctx, Code_attribute *code_attr);
/** @} */

/* ====================================================================
 * Funções Utilitárias Inline
 * ==================================================================== */

/**
 * @brief Estende o sinal de um byte para 32 bits.
 * @param b Byte com sinal.
 * @return Valor estendido para u4.
 */
static inline u4 sign_extend_byte(u1 b) {
    return (u4)((b ^ 0x80U) - 0x80U);
}

/**
 * @brief Estende o sinal de um short para 32 bits.
 * @param s Short com sinal.
 * @return Valor estendido para u4.
 */
static inline u4 sign_extend_short(u2 s) {
    return (u4)((s ^ 0x8000U) - 0x8000U);
}

/**
 * @brief Calcula o deslocamento de um branch (goto, if_icmp, etc.).
 * @param branch_bytes Valor de 2 bytes do branch.
 * @return Deslocamento com sinal estendido.
 */
static inline u4 compute_branch(u2 branch_bytes) {
    return sign_extend_short(branch_bytes);
}

/**
 * @brief Calcula o deslocamento de um branch largo (goto_w, jsr_w).
 * @param v Valor de 4 bytes do branch.
 * @return Deslocamento com sinal estendido.
 */
static inline u4 compute_branch_wide(u4 v) {
    return (v ^ 0x80000000U) - 0x80000000U;
}

#endif
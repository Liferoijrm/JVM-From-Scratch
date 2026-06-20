#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "../utils/utils.h"
#include "interpreter.h"

// ============================================================
// Public API for the bytecode decoder
// ============================================================

// Signature for every bytecode instruction handler.
// Returns: next pc value
typedef u4 (*InstructionHandler)(RuntimeContext *ctx, Code_attribute *code_attr);

// Dispatch: given a bytecode opcode, returns the corresponding handler.
// Returns NULL if opcode is not implemented.
InstructionHandler decode(u1 opcode);

// ============================================================
// Opcode constants (JVM spec) — subset, expand as needed
// ============================================================
#define OP_NOP                0x00
#define OP_ACONST_NULL        0x01
#define OP_ICONST_M1          0x02
#define OP_ICONST_0           0x03
#define OP_ICONST_1           0x04
#define OP_ICONST_2           0x05
#define OP_ICONST_3           0x06
#define OP_ICONST_4           0x07
#define OP_ICONST_5           0x08
#define OP_BIPUSH             0x10
#define OP_SIPUSH             0x11
#define OP_LDC                0x12
#define OP_ILOAD              0x15
#define OP_ILOAD_0            0x1A
#define OP_ILOAD_1            0x1B
#define OP_ILOAD_2            0x1C
#define OP_ILOAD_3            0x1D
#define OP_ALOAD_0            0x2A
#define OP_ISTORE             0x36
#define OP_ISTORE_0           0x3B
#define OP_ISTORE_1           0x3C
#define OP_ISTORE_2           0x3D
#define OP_ISTORE_3           0x3E
#define OP_IADD               0x60
#define OP_ISUB               0x64
#define OP_IMUL               0x68
#define OP_IDIV               0x6C
#define OP_IREM               0x70
#define OP_INEG               0x74
#define OP_IINC               0x84
#define OP_IFEQ               0x99
#define OP_IFNE               0x9A
#define OP_IFLT               0x9B
#define OP_IFGE               0x9C
#define OP_IFGT               0x9D
#define OP_IFLE               0x9E
#define OP_IF_ICMPEQ          0x9F
#define OP_IF_ICMPNE          0xA0
#define OP_IF_ICMPLT          0xA1
#define OP_IF_ICMPGE          0xA2
#define OP_IF_ICMPGT          0xA3
#define OP_IF_ICMPLE          0xA4
#define OP_GOTO               0xA7
#define OP_IRETURN            0xAC
#define OP_RETURN             0xB1
#define OP_INVOKESTATIC       0xB8
#define OP_INVOKEVIRTUAL      0xB6
#define OP_INVOKESPECIAL      0xB7
#define OP_INVOKEINTERFACE    0xB9
#define OP_GETSTATIC          0xB2
#define OP_PUTSTATIC          0xB3
#define OP_GETFIELD           0xB4
#define OP_PUTFIELD           0xB5
#define OP_NEW                0xBB
#define OP_NEWARRAY           0xBC
#define OP_ANEWARRAY          0xBD
#define OP_ARRAYLENGTH        0xBE
#define OP_ALOAD              0x32
#define OP_ASTORE             0x53
#define OP_POP                0x57
#define OP_DUP                0x59
#define OP_SWAP               0x5F
#define OP_ATHROW             0xBF
#define OP_IFNULL             0xC6
#define OP_IFNONNULL          0xC7
#define OP_LCMP               0x94
#define OP_FCMPL              0x95
#define OP_FCMPG              0x96
#define OP_DCMPL              0x97
#define OP_DCMPG              0x98
#define OP_LADD               0x61
#define OP_LSUB               0x65
#define OP_IAND               0x7E
#define OP_IOR                0x80
#define OP_IXOR               0x82
#define OP_I2L                0x85
#define OP_I2F                0x86
#define OP_I2D                0x87
#define OP_L2I                0x88
#define OP_L2F                0x89
#define OP_L2D                0x8A
#define OP_F2I                0x8B
#define OP_F2L                0x8C
#define OP_F2D                0x8E
#define OP_D2I                0x8F
#define OP_D2L                0x90
#define OP_D2F                0x90
#define OP_LSHL               0x79
#define OP_LSHR               0x7B
#define OP_LUSHR              0x7D
#define OP_LAND               0x7F
#define OP_LOR                0x81
#define OP_LXOR               0x83
#define OP_I2B                0x91
#define OP_I2C                0x92
#define OP_I2S                0x93
#define OP_RET                0xA9
#define OP_TABLESWITCH        0xAA
#define OP_LOOKUPSWITCH       0xAB
#define OP_WIDE               0xC4
#define OP_MULTIANEWARRAY     0xC5
#define OP_IF_ACMPEQ          0xC6
#define OP_IF_ACMPNE          0xC7
#define OP_GOTO_W             0xC8
#define OP_JSR                0xA8
#define OP_JSR_W              0xC9

#endif
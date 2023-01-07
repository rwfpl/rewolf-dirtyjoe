/*
* dirtyJOE - Java Overall Editor
* Copyright 2016 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/
#pragma once
#include "../java_types.h"

enum _opcode_flag
{
	OP_NO_FLAGS = 0x00000000,
	OP_BYTE_TO_INT = 0x00000001,
	OP_WORD_TO_INT = 0x00000002,
	OP_BYTE_CP = 0x00000004,
	OP_WORD_CP = 0x00000008,
	OP_WORD_JUMP = 0x00000010,
	OP_BYTE_LOCAL = 0x00000020,
	OP_BYTE_TO_UINT = 0x00000040,
	OP_ATYPE = 0x00000080,
	OP_DWORD_JUMP = 0x00000100,
	OP_WORD_LOCAL = 0x00000200
};

enum _atype
{
	T_BOOLEAN = 4,
	T_CHAR,
	T_FLOAT,
	T_DOUBLE,
	T_BYTE,
	T_SHORT,
	T_INT,
	T_LONG
};

enum _local_var_type
{
	local_var_object,
	local_var_double,
	local_var_float,
	local_var_int,
	local_var_long,
	local_var_undefined
};

enum _jdisasm_opcodes
{
	j_opcode_nop = 0x00,
	j_opcode_aconst_null = 0x01,
	j_opcode_iconst_ml = 0x02,
	j_opcode_iconst_0 = 0x03,
	j_opcode_iconst_1 = 0x04,
	j_opcode_iconst_2 = 0x05,
	j_opcode_iconst_3 = 0x06,
	j_opcode_iconst_4 = 0x07,
	j_opcode_iconst_5 = 0x08,
	j_opcode_lconst_0 = 0x09,
	j_opcode_lconst_1 = 0x0A,
	j_opcode_fconst_0 = 0x0B,
	j_opcode_fconst_1 = 0x0C,
	j_opcode_fconst_2 = 0x0D,
	j_opcode_dconst_0 = 0x0E,
	j_opcode_dconst_1 = 0x0F,
	j_opcode_bipush = 0x10,
	j_opcode_sipush = 0x11,
	j_opcode_ldc = 0x12,
	j_opcode_ldc_w = 0x13,
	j_opcode_ldc2_w = 0x14,
	j_opcode_iload = 0x15,
	j_opcode_lload = 0x16,
	j_opcode_fload = 0x17,
	j_opcode_dload = 0x18,
	j_opcode_aload = 0x19,
	j_opcode_iload_0 = 0x1A,
	j_opcode_iload_1 = 0x1B,
	j_opcode_iload_2 = 0x1C,
	j_opcode_iload_3 = 0x1D,
	j_opcode_lload_0 = 0x1E,
	j_opcode_lload_1 = 0x1F,
	j_opcode_lload_2 = 0x20,
	j_opcode_lload_3 = 0x21,
	j_opcode_fload_0 = 0x22,
	j_opcode_fload_1 = 0x23,
	j_opcode_fload_2 = 0x24,
	j_opcode_fload_3 = 0x25,
	j_opcode_dload_0 = 0x26,
	j_opcode_dload_1 = 0x27,
	j_opcode_dload_2 = 0x28,
	j_opcode_dload_3 = 0x29,
	j_opcode_aload_0 = 0x2A,
	j_opcode_aload_1 = 0x2B,
	j_opcode_aload_2 = 0x2C,
	j_opcode_aload_3 = 0x2D,
	j_opcode_iaload = 0x2E,
	j_opcode_laload = 0x2F,
	j_opcode_faload = 0x30,
	j_opcode_daload = 0x31,
	j_opcode_aaload = 0x32,
	j_opcode_baload = 0x33,
	j_opcode_caload = 0x34,
	j_opcode_saload = 0x35,
	j_opcode_istore = 0x36,
	j_opcode_lstore = 0x37,
	j_opcode_fstore = 0x38,
	j_opcode_dstore = 0x39,
	j_opcode_astore = 0x3A,
	j_opcode_istore_0 = 0x3B,
	j_opcode_istore_1 = 0x3C,
	j_opcode_istore_2 = 0x3D,
	j_opcode_istore_3 = 0x3E,
	j_opcode_lstore_0 = 0x3F,
	j_opcode_lstore_1 = 0x40,
	j_opcode_lstore_2 = 0x41,
	j_opcode_lstore_3 = 0x42,
	j_opcode_fstore_0 = 0x43,
	j_opcode_fstore_1 = 0x44,
	j_opcode_fstore_2 = 0x45,
	j_opcode_fstore_3 = 0x46,
	j_opcode_dstore_0 = 0x47,
	j_opcode_dstore_1 = 0x48,
	j_opcode_dstore_2 = 0x49,
	j_opcode_dstore_3 = 0x4A,
	j_opcode_astore_0 = 0x4B,
	j_opcode_astore_1 = 0x4C,
	j_opcode_astore_2 = 0x4D,
	j_opcode_astore_3 = 0x4E,
	j_opcode_iastore = 0x4F,
	j_opcode_lastore = 0x50,
	j_opcode_fastore = 0x51,
	j_opcode_dastore = 0x52,
	j_opcode_aastore = 0x53,
	j_opcode_bastore = 0x54,
	j_opcode_castore = 0x55,
	j_opcode_sastore = 0x56,
	j_opcode_pop = 0x57,
	j_opcode_pop2 = 0x58,
	j_opcode_dup = 0x59,
	j_opcode_dup_x1 = 0x5A,
	j_opcode_dup_x2 = 0x5B,
	j_opcode_dup2 = 0x5C,
	j_opcode_dup2_x1 = 0x5D,
	j_opcode_dup2_x2 = 0x5E,
	j_opcode_swap = 0x5F,
	j_opcode_iadd = 0x60,
	j_opcode_ladd = 0x61,
	j_opcode_fadd = 0x62,
	j_opcode_dadd = 0x63,
	j_opcode_isub = 0x64,
	j_opcode_lsub = 0x65,
	j_opcode_fsub = 0x66,
	j_opcode_dsub = 0x67,
	j_opcode_imul = 0x68,
	j_opcode_lmul = 0x69,
	j_opcode_fmul = 0x6A,
	j_opcode_dmul = 0x6B,
	j_opcode_idiv = 0x6C,
	j_opcode_ldiv = 0x6D,
	j_opcode_fdiv = 0x6E,
	j_opcode_ddiv = 0x6F,
	j_opcode_irem = 0x70,
	j_opcode_lrem = 0x71,
	j_opcode_frem = 0x72,
	j_opcode_drem = 0x73,
	j_opcode_ineg = 0x74,
	j_opcode_lneg = 0x75,
	j_opcode_fneg = 0x76,
	j_opcode_dneg = 0x77,
	j_opcode_ishl = 0x78,
	j_opcode_lshl = 0x79,
	j_opcode_ishr = 0x7A,
	j_opcode_lshr = 0x7B,
	j_opcode_iushr = 0x7C,
	j_opcode_lushr = 0x7D,
	j_opcode_iand = 0x7E,
	j_opcode_land = 0x7F,
	j_opcode_ior = 0x80,
	j_opcode_lor = 0x81,
	j_opcode_ixor = 0x82,
	j_opcode_lxor = 0x83,
	j_opcode_iinc = 0x84,
	j_opcode_i2l = 0x85,
	j_opcode_i2f = 0x86,
	j_opcode_i2d = 0x87,
	j_opcode_l2i = 0x88,
	j_opcode_l2f = 0x89,
	j_opcode_l2d = 0x8A,
	j_opcode_f2i = 0x8B,
	j_opcode_f2l = 0x8C,
	j_opcode_f2d = 0x8D,
	j_opcode_d2i = 0x8E,
	j_opcode_d2l = 0x8F,
	j_opcode_d2f = 0x90,
	j_opcode_i2b = 0x91,
	j_opcode_i2c = 0x92,
	j_opcode_i2s = 0x93,
	j_opcode_lcmp = 0x94,
	j_opcode_fcmpl = 0x95,
	j_opcode_fcmpg = 0x96,
	j_opcode_dcmpl = 0x97,
	j_opcode_dcmpg = 0x98,
	j_opcode_ifeq = 0x99,
	j_opcode_ifne = 0x9A,
	j_opcode_iflt = 0x9B,
	j_opcode_ifge = 0x9C,
	j_opcode_ifgt = 0x9D,
	j_opcode_ifle = 0x9E,
	j_opcode_if_icmpeq = 0x9F,
	j_opcode_if_icmpne = 0xA0,
	j_opcode_if_icmplt = 0xA1,
	j_opcode_if_icmpge = 0xA2,
	j_opcode_if_icmpgt = 0xA3,
	j_opcode_if_icmple = 0xA4,
	j_opcode_if_acmpeq = 0xA5,
	j_opcode_if_acmpne = 0xA6,
	j_opcode_goto = 0xA7,
	j_opcode_jsr = 0xA8,
	j_opcode_ret = 0xA9,
	j_opcode_tableswitch = 0xAA,
	j_opcode_lookupswitch = 0xAB,
	j_opcode_ireturn = 0xAC,
	j_opcode_lreturn = 0xAD,
	j_opcode_freturn = 0xAE,
	j_opcode_dreturn = 0xAF,
	j_opcode_areturn = 0xB0,
	j_opcode_return = 0xB1,
	j_opcode_getstatic = 0xB2,
	j_opcode_putstatic = 0xB3,
	j_opcode_getfield = 0xB4,
	j_opcode_putfield = 0xB5,
	j_opcode_invokevirtual = 0xB6,
	j_opcode_invokespecial = 0xB7,
	j_opcode_invokestatic = 0xB8,
	j_opcode_invokeinterface = 0xB9,
	j_opcode_invokedynamic = 0xBA,
	j_opcode_new = 0xBB,
	j_opcode_newarray = 0xBC,
	j_opcode_anewarray = 0xBD,
	j_opcode_arraylength = 0xBE,
	j_opcode_athrow = 0xBF,
	j_opcode_checkcast = 0xC0,
	j_opcode_instanceof = 0xC1,
	j_opcode_monitorenter = 0xC2,
	j_opcode_monitorexit = 0xC3,
	j_opcode_wide = 0xC4,
	j_opcode_multianewarray = 0xC5,
	j_opcode_ifnull = 0xC6,
	j_opcode_ifnonnull = 0xC7,
	j_opcode_goto_w = 0xC8,
	j_opcode_jsr_w = 0xC9,
	j_opcode_breakpoint = 0xCA,
	j_opcode_undefined = 0xCB /* - 0xFD */,
	j_opcode_impdep1 = 0xFE,
	j_opcode_impdep2 = 0xFF,
	j_opcode_error = 0xDEAD
};

enum class JvmOpTypeClass
{
	JvmOpAtype,
	JvmOpByteLocal,
	JvmOpFlow,
	JvmOpInt,
	JvmOpLookupSwitch,
	JvmOpOneByte,
	JvmOpTableSwitch,
	JvmOpWordCP
};

struct _opcode_struct
{
	_jdisasm_opcodes op_type;
	_local_var_type local_var_type;
	JvmOpTypeClass op_type_class;
	const char* const specName;
	const wchar_t* const name;
	bool is_flow;
	bool is_terminator;
	int size;		//-1 means variable size; -2 means no opcode
	u1 opcode;
	int stackPop;	//-1 means variable number of pop's
	int stackPush;	//-1 means variable number of push's
	u4 flag01;
#pragma warning (suppress: 4510 4512 4610)
};

/*
* dirtyJOE - Java Overall Editor
* Copyright 2010 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/

#include <string.h>
#include <cstdlib>
#include "jdisasm.h"
#include "../../common/common.h"
#include "../helpers.h"

const _opcode_struct opcodes[256] =
{
	{ j_opcode_nop             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "nop"            , L"nop"            , false, false,  1, 0x00, 0, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_aconst_null     , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "aconst_null"    , L"aconst_null"    , false, false,  1, 0x01, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_ml       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_ml"      , false, false,  1, 0x02, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_0        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_0"       , false, false,  1, 0x03, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_1        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_1"       , false, false,  1, 0x04, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_2        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_2"       , false, false,  1, 0x05, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_3        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_3"       , false, false,  1, 0x06, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_4        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_4"       , false, false,  1, 0x07, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iconst_5        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iconst_<i>"     , L"iconst_5"       , false, false,  1, 0x08, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lconst_0        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lconst_<l>"     , L"lconst_0"       , false, false,  1, 0x09, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_lconst_1        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lconst_<l>"     , L"lconst_1"       , false, false,  1, 0x0A, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fconst_0        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fconst_<f>"     , L"fconst_0"       , false, false,  1, 0x0B, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fconst_1        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fconst_<f>"     , L"fconst_1"       , false, false,  1, 0x0C, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fconst_2        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fconst_<f>"     , L"fconst_2"       , false, false,  1, 0x0D, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dconst_0        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dconst_<d>"     , L"dconst_0"       , false, false,  1, 0x0E, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_dconst_1        , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dconst_<d>"     , L"dconst_1"       , false, false,  1, 0x0F, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_bipush          , local_var_undefined, JvmOpTypeClass::JvmOpInt         , "bipush"         , L"bipush"         , false, false,  2, 0x10, 0, 1, OP_BYTE_TO_INT}, /**/			// [1] sign extend to int, and push
	{ j_opcode_sipush          , local_var_undefined, JvmOpTypeClass::JvmOpInt         , "sipush"         , L"sipush"         , false, false,  3, 0x11, 0, 1, OP_WORD_TO_INT}, /**/			// ([1] << 8) | [2] sign extend to int, and push
	{ j_opcode_ldc             , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "ldc"            , L"ldc"            , false, false,  2, 0x12, 0, 1, OP_BYTE_CP},	  /**/			// [1] index to constant pool table
	{ j_opcode_ldc_w           , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "ldc_w"          , L"ldc_w"          , false, false,  3, 0x13, 0, 1, OP_WORD_CP},	  /**/			// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_ldc2_w          , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "ldc2_w"         , L"ldc2_w"         , false, false,  3, 0x14, 0, 2, OP_WORD_CP},	  /**/			// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_iload           , local_var_int      , JvmOpTypeClass::JvmOpByteLocal   , "iload"          , L"iload"          , false, false,  2, 0x15, 0, 1, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_lload           , local_var_long     , JvmOpTypeClass::JvmOpByteLocal   , "lload"          , L"lload"          , false, false,  2, 0x16, 0, 2, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_fload           , local_var_float    , JvmOpTypeClass::JvmOpByteLocal   , "fload"          , L"fload"          , false, false,  2, 0x17, 0, 1, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_dload           , local_var_double   , JvmOpTypeClass::JvmOpByteLocal   , "dload"          , L"dload"          , false, false,  2, 0x18, 0, 2, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_aload           , local_var_object   , JvmOpTypeClass::JvmOpByteLocal   , "aload"          , L"aload"          , false, false,  2, 0x19, 0, 1, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_iload_0         , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "iload_<n>"      , L"iload_0"        , false, false,  1, 0x1A, 0, 1, OP_NO_FLAGS},    /**/
	{ j_opcode_iload_1         , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "iload_<n>"      , L"iload_1"        , false, false,  1, 0x1B, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iload_2         , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "iload_<n>"      , L"iload_2"        , false, false,  1, 0x1C, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iload_3         , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "iload_<n>"      , L"iload_3"        , false, false,  1, 0x1D, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lload_0         , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lload_<n>"      , L"lload_0"        , false, false,  1, 0x1E, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_lload_1         , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lload_<n>"      , L"lload_1"        , false, false,  1, 0x1F, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_lload_2         , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lload_<n>"      , L"lload_2"        , false, false,  1, 0x20, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_lload_3         , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lload_<n>"      , L"lload_3"        , false, false,  1, 0x21, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fload_0         , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fload_<n>"      , L"fload_0"        , false, false,  1, 0x22, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fload_1         , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fload_<n>"      , L"fload_1"        , false, false,  1, 0x23, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fload_2         , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fload_<n>"      , L"fload_2"        , false, false,  1, 0x24, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fload_3         , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fload_<n>"      , L"fload_3"        , false, false,  1, 0x25, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dload_0         , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dload_<n>"      , L"dload_0"        , false, false,  1, 0x26, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_dload_1         , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dload_<n>"      , L"dload_1"        , false, false,  1, 0x27, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_dload_2         , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dload_<n>"      , L"dload_2"        , false, false,  1, 0x28, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_dload_3         , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dload_<n>"      , L"dload_3"        , false, false,  1, 0x29, 0, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_aload_0         , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "aload_<n>"      , L"aload_0"        , false, false,  1, 0x2A, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_aload_1         , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "aload_<n>"      , L"aload_1"        , false, false,  1, 0x2B, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_aload_2         , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "aload_<n>"      , L"aload_2"        , false, false,  1, 0x2C, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_aload_3         , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "aload_<n>"      , L"aload_3"        , false, false,  1, 0x2D, 0, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_iaload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iaload"         , L"iaload"         , false, false,  1, 0x2E, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_laload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "laload"         , L"laload"         , false, false,  1, 0x2F, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_faload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "faload"         , L"faload"         , false, false,  1, 0x30, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_daload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "daload"         , L"daload"         , false, false,  1, 0x31, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_aaload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "aaload"         , L"aaload"         , false, false,  1, 0x32, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_baload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "baload"         , L"baload"         , false, false,  1, 0x33, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_caload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "caload"         , L"caload"         , false, false,  1, 0x34, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_saload          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "saload"         , L"saload"         , false, false,  1, 0x35, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_istore          , local_var_int      , JvmOpTypeClass::JvmOpByteLocal   , "istore"         , L"istore"         , false, false,  2, 0x36, 1, 0, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_lstore          , local_var_long     , JvmOpTypeClass::JvmOpByteLocal   , "lstore"         , L"lstore"         , false, false,  2, 0x37, 2, 0, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_fstore          , local_var_float    , JvmOpTypeClass::JvmOpByteLocal   , "fstore"         , L"fstore"         , false, false,  2, 0x38, 1, 0, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_dstore          , local_var_double   , JvmOpTypeClass::JvmOpByteLocal   , "dstore"         , L"dstore"         , false, false,  2, 0x39, 2, 0, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_astore          , local_var_object   , JvmOpTypeClass::JvmOpByteLocal   , "astore"         , L"astore"         , false, false,  2, 0x3A, 1, 0, OP_BYTE_LOCAL},  /**/			// [1] index to local variables table
	{ j_opcode_istore_0        , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "istore_<n>"     , L"istore_0"       , false, false,  1, 0x3B, 1, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_istore_1        , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "istore_<n>"     , L"istore_1"       , false, false,  1, 0x3C, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_istore_2        , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "istore_<n>"     , L"istore_2"       , false, false,  1, 0x3D, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_istore_3        , local_var_int      , JvmOpTypeClass::JvmOpOneByte     , "istore_<n>"     , L"istore_3"       , false, false,  1, 0x3E, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_lstore_0        , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lstore_<n>"     , L"lstore_0"       , false, false,  1, 0x3F, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_lstore_1        , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lstore_<n>"     , L"lstore_1"       , false, false,  1, 0x40, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_lstore_2        , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lstore_<n>"     , L"lstore_2"       , false, false,  1, 0x41, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_lstore_3        , local_var_long     , JvmOpTypeClass::JvmOpOneByte     , "lstore_<n>"     , L"lstore_3"       , false, false,  1, 0x42, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_fstore_0        , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fstore_<n>"     , L"fstore_0"       , false, false,  1, 0x43, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_fstore_1        , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fstore_<n>"     , L"fstore_1"       , false, false,  1, 0x44, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_fstore_2        , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fstore_<n>"     , L"fstore_2"       , false, false,  1, 0x45, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_fstore_3        , local_var_float    , JvmOpTypeClass::JvmOpOneByte     , "fstore_<n>"     , L"fstore_3"       , false, false,  1, 0x46, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dstore_0        , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dstore_<n>"     , L"dstore_0"       , false, false,  1, 0x47, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dstore_1        , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dstore_<n>"     , L"dstore_1"       , false, false,  1, 0x48, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dstore_2        , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dstore_<n>"     , L"dstore_2"       , false, false,  1, 0x49, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dstore_3        , local_var_double   , JvmOpTypeClass::JvmOpOneByte     , "dstore_<n>"     , L"dstore_3"       , false, false,  1, 0x4A, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_astore_0        , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "astore_<n>"     , L"astore_0"       , false, false,  1, 0x4B, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_astore_1        , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "astore_<n>"     , L"astore_1"       , false, false,  1, 0x4C, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_astore_2        , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "astore_<n>"     , L"astore_2"       , false, false,  1, 0x4D, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_astore_3        , local_var_object   , JvmOpTypeClass::JvmOpOneByte     , "astore_<n>"     , L"astore_3"       , false, false,  1, 0x4E, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_iastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iastore"        , L"iastore"        , false, false,  1, 0x4F, 3, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_lastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lastore"        , L"lastore"        , false, false,  1, 0x50, 4, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_fastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fastore"        , L"fastore"        , false, false,  1, 0x51, 3, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dastore"        , L"dastore"        , false, false,  1, 0x52, 4, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_aastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "aastore"        , L"aastore"        , false, false,  1, 0x53, 3, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_bastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "bastore"        , L"bastore"        , false, false,  1, 0x54, 3, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_castore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "castore"        , L"castore"        , false, false,  1, 0x55, 3, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_sastore         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "sastore"        , L"sastore"        , false, false,  1, 0x56, 3, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_pop             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "pop"            , L"pop"            , false, false,  1, 0x57, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_pop2            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "pop2"           , L"pop2"           , false, false,  1, 0x58, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dup             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dup"            , L"dup"            , false, false,  1, 0x59, 1, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_dup_x1          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dup_x1"         , L"dup_x1"         , false, false,  1, 0x5A, 2, 3, OP_NO_FLAGS},	  /**/
	{ j_opcode_dup_x2          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dup_x2"         , L"dup_x2"         , false, false,  1, 0x5B, 3, 4, OP_NO_FLAGS},	  /**/
	{ j_opcode_dup2            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dup2"           , L"dup2"           , false, false,  1, 0x5C, 2, 4, OP_NO_FLAGS},	  /**/
	{ j_opcode_dup2_x1         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dup2_x1"        , L"dup2_x1"        , false, false,  1, 0x5D, 3, 5, OP_NO_FLAGS},	  /**/
	{ j_opcode_dup2_x2         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dup2_x2"        , L"dup2_x2"        , false, false,  1, 0x5E, 4, 6, OP_NO_FLAGS},	  /**/
	{ j_opcode_swap            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "swap"           , L"swap"           , false, false,  1, 0x5F, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_iadd            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iadd"           , L"iadd"           , false, false,  1, 0x60, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_ladd            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ladd"           , L"ladd"           , false, false,  1, 0x61, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fadd            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fadd"           , L"fadd"           , false, false,  1, 0x62, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dadd            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dadd"           , L"dadd"           , false, false,  1, 0x63, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_isub            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "isub"           , L"isub"           , false, false,  1, 0x64, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lsub            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lsub"           , L"lsub"           , false, false,  1, 0x65, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fsub            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fsub"           , L"fsub"           , false, false,  1, 0x66, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dsub            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dsub"           , L"dsub"           , false, false,  1, 0x67, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_imul            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "imul"           , L"imul"           , false, false,  1, 0x68, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lmul            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lmul"           , L"lmul"           , false, false,  1, 0x69, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fmul            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fmul"           , L"fmul"           , false, false,  1, 0x6A, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dmul            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dmul"           , L"dmul"           , false, false,  1, 0x6B, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_idiv            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "idiv"           , L"idiv"           , false, false,  1, 0x6C, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_ldiv            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ldiv"           , L"ldiv"           , false, false,  1, 0x6D, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fdiv            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fdiv"           , L"fdiv"           , false, false,  1, 0x6E, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_ddiv            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ddiv"           , L"ddiv"           , false, false,  1, 0x6F, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_irem            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "irem"           , L"irem"           , false, false,  1, 0x70, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lrem            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lrem"           , L"lrem"           , false, false,  1, 0x71, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_frem            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "frem"           , L"frem"           , false, false,  1, 0x72, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_drem            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "drem"           , L"drem"           , false, false,  1, 0x73, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_ineg            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ineg"           , L"ineg"           , false, false,  1, 0x74, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lneg            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lneg"           , L"lneg"           , false, false,  1, 0x75, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_fneg            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fneg"           , L"fneg"           , false, false,  1, 0x76, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dneg            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dneg"           , L"dneg"           , false, false,  1, 0x77, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_ishl            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ishl"           , L"ishl"           , false, false,  1, 0x78, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lshl            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lshl"           , L"lshl"           , false, false,  1, 0x79, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_ishr            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ishr"           , L"ishr"           , false, false,  1, 0x7A, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lshr            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lshr"           , L"lshr"           , false, false,  1, 0x7B, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_iushr           , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iushr"          , L"iushr"          , false, false,  1, 0x7C, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lushr           , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lushr"          , L"lushr"          , false, false,  1, 0x7D, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_iand            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "iand"           , L"iand"           , false, false,  1, 0x7E, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_land            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "land"           , L"land"           , false, false,  1, 0x7F, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_ior             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ior"            , L"ior"            , false, false,  1, 0x80, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lor             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lor"            , L"lor"            , false, false,  1, 0x81, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_ixor            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ixor"           , L"ixor"           , false, false,  1, 0x82, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lxor            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lxor"           , L"lxor"           , false, false,  1, 0x83, 4, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_iinc            , local_var_int      , JvmOpTypeClass::JvmOpByteLocal   , "iinc"           , L"iinc"           , false, false,  3, 0x84, 0, 0, OP_BYTE_LOCAL | (OP_BYTE_TO_INT << 16)},	/**/		// [1] index to local variables table, [2] immediate signed byte extended to int
	{ j_opcode_i2l             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "i2l"            , L"i2l"            , false, false,  1, 0x85, 1, 2, OP_NO_FLAGS},    /**/
	{ j_opcode_i2f             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "i2f"            , L"i2f"            , false, false,  1, 0x86, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_i2d             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "i2d"            , L"i2d"            , false, false,  1, 0x87, 1, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_l2i             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "l2i"            , L"l2i"            , false, false,  1, 0x88, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_l2f             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "l2f"            , L"l2f"            , false, false,  1, 0x89, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_l2d             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "l2d"            , L"l2d"            , false, false,  1, 0x8A, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_f2i             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "f2i"            , L"f2i"            , false, false,  1, 0x8B, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_f2l             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "f2l"            , L"f2l"            , false, false,  1, 0x8C, 1, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_f2d             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "f2d"            , L"f2d"            , false, false,  1, 0x8D, 1, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_d2i             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "d2i"            , L"d2i"            , false, false,  1, 0x8E, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_d2l             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "d2l"            , L"d2l"            , false, false,  1, 0x8F, 2, 2, OP_NO_FLAGS},	  /**/
	{ j_opcode_d2f             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "d2f"            , L"d2f"            , false, false,  1, 0x90, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_i2b             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "i2b"            , L"i2b"            , false, false,  1, 0x91, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_i2c             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "i2c"            , L"i2c"            , false, false,  1, 0x92, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_i2s             , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "i2s"            , L"i2s"            , false, false,  1, 0x93, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_lcmp            , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lcmp"           , L"lcmp"           , false, false,  1, 0x94, 4, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fcmpl           , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fcmp<op>"       , L"fcmpl"          , false, false,  1, 0x95, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_fcmpg           , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "fcmp<op>"       , L"fcmpg"          , false, false,  1, 0x96, 2, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dcmpl           , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dcmp<op>"       , L"dcmpl"          , false, false,  1, 0x97, 4, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_dcmpg           , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dcmp<op>"       , L"dcmpg"          , false, false,  1, 0x98, 4, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_ifeq            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if<cond>"       , L"ifeq"           , true , false,  3, 0x99, 1, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_ifne            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if<cond>"       , L"ifne"           , true , false,  3, 0x9A, 1, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_iflt            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if<cond>"       , L"iflt"           , true , false,  3, 0x9B, 1, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_ifge            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if<cond>"       , L"ifge"           , true , false,  3, 0x9C, 1, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_ifgt            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if<cond>"       , L"ifgt"           , true , false,  3, 0x9D, 1, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_ifle            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if<cond>"       , L"ifle"           , true , false,  3, 0x9E, 1, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_icmpeq       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_icmp<cond>"  , L"if_icmpeq"      , true , false,  3, 0x9F, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_icmpne       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_icmp<cond>"  , L"if_icmpne"      , true , false,  3, 0xA0, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_icmplt       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_icmp<cond>"  , L"if_icmplt"      , true , false,  3, 0xA1, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_icmpge       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_icmp<cond>"  , L"if_icmpge"      , true , false,  3, 0xA2, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_icmpgt       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_icmp<cond>"  , L"if_icmpgt"      , true , false,  3, 0xA3, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_icmple       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_icmp<cond>"  , L"if_icmple"      , true , false,  3, 0xA4, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_acmpeq       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_acmp<cond>"  , L"if_acmpeq"      , true , false,  3, 0xA5, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_if_acmpne       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "if_acmp<cond>"  , L"if_acmpne"      , true , false,  3, 0xA6, 2, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_goto            , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "goto"           , L"goto"           , true , true ,  3, 0xA7, 0, 0, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_jsr             , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "jsr"            , L"jsr"            , true , false,  3, 0xA8, 0, 1, OP_WORD_JUMP},	  /**/			// ([1] << 8) | [2] sign extended to int, relative jump destination
	{ j_opcode_ret             , local_var_undefined, JvmOpTypeClass::JvmOpByteLocal   , "ret"            , L"ret"            , true , true ,  2, 0xA9, 0, 0, OP_BYTE_LOCAL},  /**/		// [1] index to local variables table
	{ j_opcode_tableswitch     , local_var_undefined, JvmOpTypeClass::JvmOpTableSwitch , "tableswitch"    , L"tableswitch"    , true , true , -1, 0xAA, 1, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_lookupswitch    , local_var_undefined, JvmOpTypeClass::JvmOpLookupSwitch, "lookupswitch"   , L"lookupswitch"   , true , true , -1, 0xAB, 1, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_ireturn         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "ireturn"        , L"ireturn"        , true , true ,  1, 0xAC, 1, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_lreturn         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "lreturn"        , L"lreturn"        , true , true ,  1, 0xAD, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_freturn         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "freturn"        , L"freturn"        , true , true ,  1, 0xAE, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_dreturn         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "dreturn"        , L"dreturn"        , true , true ,  1, 0xAF, 2, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_areturn         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "areturn"        , L"areturn"        , true , true ,  1, 0xB0, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_return          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "return"         , L"return"         , true , true ,  1, 0xB1, 0, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_getstatic       , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "getstatic"      , L"getstatic"      , false, false,  3, 0xB2, 0, 1, OP_WORD_CP},     /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_putstatic       , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "putstatic"      , L"putstatic"      , false, false,  3, 0xB3, 1, 0, OP_WORD_CP},     /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_getfield        , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "getfield"       , L"getfield"       , false, false,  3, 0xB4, 1, 1, OP_WORD_CP},     /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_putfield        , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "putfield"       , L"putfield"       , false, false,  3, 0xB5, 2, 0, OP_WORD_CP},     /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_invokevirtual   , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "invokevirtual"  , L"invokevirtual"  , false, false,  3, 0xB6, -1, -1, OP_WORD_CP},	  /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_invokespecial   , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "invokespecial"  , L"invokespecial"  , false, false,  3, 0xB7, -1, -1, OP_WORD_CP},	  /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_invokestatic    , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "invokestatic"   , L"invokestatic"   , false, false,  3, 0xB8, -1, -1, OP_WORD_CP},	  /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_invokeinterface , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "invokeinterface", L"invokeinterface", false, false,  5, 0xB9, -1, -1, OP_WORD_CP | (OP_BYTE_TO_UINT << 16)}, /**/	// ([1] << 8) | [2] index to constant pool table, [3] byte extended to uint, [4] = 0
	{ j_opcode_invokedynamic   , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "invokedynamic"  , L"invokedynamic"  , false, false,  5, 0xBA, -1, -1, OP_WORD_CP},	  /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_new             , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "new"            , L"new"            , false, false,  3, 0xBB, 0, 1, OP_WORD_CP},	  /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_newarray        , local_var_undefined, JvmOpTypeClass::JvmOpAtype       , "newarray"       , L"newarray"       , false, false,  2, 0xBC, 1, 1, OP_ATYPE},		  /**/	// [1] OP_ATYPE
	{ j_opcode_anewarray       , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "anewarray"      , L"anewarray"      , false, false,  3, 0xBD, 1, 1, OP_WORD_CP},	  /**/	// ([1] << 8) | [2] index to constant pool table
	{ j_opcode_arraylength     , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "arraylength"    , L"arraylength"    , false, false,  1, 0xBE, 1, 1, OP_NO_FLAGS},    /**/
	{ j_opcode_athrow          , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "athrow"         , L"athrow"         , true , true ,  1, 0xBF, 1, 1, OP_NO_FLAGS},	  /**/
	{ j_opcode_checkcast       , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "checkcast"      , L"checkcast"      , false, false,  3, 0xC0, 1, 1, OP_WORD_CP},	  /**/
	{ j_opcode_instanceof      , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "instanceof"     , L"instanceof"     , false, false,  3, 0xC1, 1, 1, OP_WORD_CP},	  /**/
	{ j_opcode_monitorenter    , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "monitorenter"   , L"monitorenter"   , false, false,  1, 0xC2, 1, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_monitorexit     , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , "monitorexit"    , L"monitorexit"    , false, false,  1, 0xC3, 1, 0, OP_NO_FLAGS},	  /**/
	{ j_opcode_wide            , local_var_undefined, JvmOpTypeClass::JvmOpByteLocal   , "wide"           , L"wide"           , false, false, -1, 0xC4, -1, -1, OP_NO_FLAGS},  /**/
	{ j_opcode_multianewarray  , local_var_undefined, JvmOpTypeClass::JvmOpWordCP      , "multianewarray" , L"multianewarray" , false, false,  4, 0xC5, -1, 1, OP_WORD_CP | (OP_BYTE_TO_UINT << 16)}, /**/
	{ j_opcode_ifnull          , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "ifnull"         , L"ifnull"         , true , false,  3, 0xC6, 1, 0, OP_WORD_JUMP},	  /**/
	{ j_opcode_ifnonnull       , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "ifnonnull"      , L"ifnonnull"      , true , false,  3, 0xC7, 1, 0, OP_WORD_JUMP},	  /**/
	{ j_opcode_goto_w          , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "goto_w"         , L"goto_w"         , true , true ,  5, 0xC8, 0, 0, OP_DWORD_JUMP},  /**/
	{ j_opcode_jsr_w           , local_var_undefined, JvmOpTypeClass::JvmOpFlow        , "jsr_w"          , L"jsr_w"          , true , false,  5, 0xC9, 0, 1, OP_DWORD_JUMP},  /**/
	{ j_opcode_breakpoint      , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L"breakpoint"     , false, false,  1, 0xCA, 0, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xCB, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xCC, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xCD, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xCE, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xCF, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD0, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD1, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD2, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD3, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD4, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD5, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD6, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD7, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD8, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xD9, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xDA, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xDB, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xDC, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xDD, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xDE, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xDF, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE0, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE1, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE2, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE3, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE4, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE5, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE6, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE7, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE8, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xE9, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xEA, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xEB, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xEC, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xED, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xEE, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xEF, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF0, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF1, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF2, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF3, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF4, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF5, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF6, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF7, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF8, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xF9, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xFA, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xFB, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xFC, 0, 0, OP_NO_FLAGS},
	{ j_opcode_undefined       , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L""               , false, false, -2, 0xFD, 0, 0, OP_NO_FLAGS},
	{ j_opcode_impdep1         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L"impdep1"        , false, false,  1, 0xFE, 0, 0, OP_NO_FLAGS},    /**/
	{ j_opcode_impdep2         , local_var_undefined, JvmOpTypeClass::JvmOpOneByte     , 0                , L"impdep2"        , false, false,  1, 0xFF, 0, 0, OP_NO_FLAGS}	  /**/
};

const _opcode_struct jvm_opcode_error =
{
	j_opcode_error,
	local_var_undefined,
	JvmOpTypeClass::JvmOpOneByte,
	"error",
	L"error",
	0,
	0,
	0,
	0,
	OP_NO_FLAGS
};

int jdisasm_get_ref(const u1* buf, u4 size)
{
	if (size <= 1)
		return -1;
	if (opcodes[buf[0]].size < 0)
		return -1;
	
	if (opcodes[buf[0]].flag01 & OP_WORD_CP)
	{
		if (size < 3)
			return -1;
		return BE2LEU2(*(u2*)(buf + 1));
	}
	else if (opcodes[buf[0]].flag01 & OP_BYTE_CP)
	{
		if (size < 2)
			return -1;
		return buf[1];
	}
	else
		return -1;
}

int jdisasm_get_len(const u1* buf, u4 size, u4 buf_pos)
{
	//-1 -> buffer to small
	//-2 -> invalid opcode
	//-3 -> unknown variable size opcode
	//-4 -> other error

	if (buf_pos >= size)
		return -1;

	if (opcodes[buf[buf_pos]].size == -2)
		return -2;
	else if (opcodes[buf[buf_pos]].size == -1)
	{
		//variable size

		//calculate padd size
		int padd_size = (4 - ((buf_pos + 1) % 4)) % 4;

		switch (opcodes[buf[buf_pos]].op_type)
		{
			case j_opcode_tableswitch:
				{
					if (buf_pos + 1 + padd_size + 3*sizeof(u4) > size)
						return -1;

					int low = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)));
					int high = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + 2*sizeof(u4)));
					if (low > high)
						return -4;
					return 1 + padd_size + 3*sizeof(u4) + (high - low + 1)*sizeof(u4);
				}
				break;
			case j_opcode_lookupswitch:
				{
					if (buf_pos + 1 + padd_size + 2*sizeof(u4) > size)
						return -1;

					int npairs = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)));
					return 1 + padd_size + 2*sizeof(u4) + npairs*2*sizeof(u4);
				}
				break;
			case j_opcode_wide:	//wide
				{
					if (buf_pos + 1 >= size)
						return -1;
					switch (opcodes[buf[buf_pos + 1]].op_type)
					{
						case j_opcode_iload:
						case j_opcode_lload:
						case j_opcode_fload:
						case j_opcode_dload:
						case j_opcode_aload:
						case j_opcode_istore:
						case j_opcode_lstore:
						case j_opcode_fstore:
						case j_opcode_dstore:
						case j_opcode_astore:
						case j_opcode_ret:
							return 4;
						case j_opcode_iinc:
							return 6;
						default:
							return -4;
					}
				}
				break;
			default:
				return -3;
		}
	}
	else
		return opcodes[buf[buf_pos]].size;
}

#define JDISASM_SIMPLE_01(size, str, val) \
	static const int len = size; \
	ret = malloc<wchar_t>(len); \
	if (nullptr == ret) \
		break; \
	if (swprintf(ret.get(), len, str, val) == -1) \
	{ \
		ret = nullptr; \
		break; \
	}

malloc_ptr<wchar_t> jdisasm_hlp_decode_u1(u4 flags, u1 _u1, const JavaConstPool* cpInfo)
{
	static const u1 atsize = 12;
	const wchar_t* const _atype_str[atsize] =
	{
		L"UNKNOWN_ATYPE_0",
		L"UNKNOWN_ATYPE_1",
		L"UNKNOWN_ATYPE_2",
		L"UNKNOWN_ATYPE_3",
		L"boolean",
		L"char",
		L"float",
		L"double",
		L"byte",
		L"short",
		L"int",
		L"long"
	};

	malloc_ptr<wchar_t> ret = nullptr;
	switch (flags)
	{
	case OP_BYTE_TO_INT:
		{
			wchar_t tmp[16];
			int ll = swprintf_s(tmp, L"%d", (su1)_u1);
			if (ll == -1)
				break;
			ret = malloc<wchar_t>(ll + 1);
			if (nullptr == ret)
				break;
			memmove(ret.get(), tmp, 2*(ll + 1));
		}
		break;
	case OP_BYTE_TO_UINT:
		{
			wchar_t tmp[16];
			int ll = swprintf_s(tmp, L"%d", _u1);
			if (ll == -1)
				break;
			ret = malloc<wchar_t>(ll + 1);
			if (nullptr == ret)
				break;
			memmove(ret.get(), tmp, 2*(ll + 1));
		}
		break;
	case OP_BYTE_CP:
		{
			if (cpInfo)
			{
				if ((ret = malloc_ptr<wchar_t>(genDisAsmStringFromCP(*cpInfo, _u1))) == nullptr)
				{
					JDISASM_SIMPLE_01(14, L"invalid_cp.%02X", _u1);
				}
			}
			else
			{
				JDISASM_SIMPLE_01(6, L"cp.%02X", _u1);
			}
		}
		break;
	case OP_BYTE_LOCAL:
		{
			JDISASM_SIMPLE_01(9, L"local.%02X", _u1);
		}
		break;
	case OP_ATYPE:
		{
			const wchar_t* tp = nullptr;
			if (_u1 < atsize)
				tp = _atype_str[_u1];
			else
				tp = L"unknown";
			size_t len = wcslen(tp) + 6 + 1;
			ret = malloc<wchar_t>(len);
			if (nullptr == ret)
				break;
			memmove(ret.get(), L"atype:", 2*6);
			memmove(ret.get() + 6, tp, 2*(len - 6));
		}
		break;
	default:
		{
			JDISASM_SIMPLE_01(11, L"unknown %02X", _u1);
		}
		break;
	}
	return ret;
}

malloc_ptr<wchar_t> jdisasm_hlp_decode_u2(u4 flags, u2 _u2, const JavaConstPool* cpInfo, u4 buf_pos)
{
	malloc_ptr<wchar_t> ret = nullptr;
	switch (flags)
	{
	case OP_WORD_JUMP:
		{
			u4 pos = buf_pos + (su2)_u2;
			JDISASM_SIMPLE_01(13, L"pos.%08X", pos);
		}
		break;
	case OP_WORD_TO_INT:
		{
			wchar_t tmp[16];
			int ll = swprintf_s(tmp, L"%d", (su2)_u2);
			if (ll == -1)
				break;
			ret = malloc<wchar_t>(ll + 1);
			if (nullptr == ret)
				break;
			memmove(ret.get(), tmp, 2*(ll + 1));
		}
		break;
	case OP_WORD_CP:
		{
			if (cpInfo)
			{
				if ((ret = genDisAsmStringFromCP(*cpInfo, _u2)) == nullptr)
				{
					JDISASM_SIMPLE_01(16, L"invalid_cp.%04X", _u2);
				}
			}
			else
			{
				JDISASM_SIMPLE_01(8, L"cp.%04X", _u2);
			}
		}
		break;
	case OP_WORD_LOCAL:
		{
			JDISASM_SIMPLE_01(11, L"local.%04X", _u2);
		}
		break;
	default:
		{
			JDISASM_SIMPLE_01(13, L"unknown %04X", _u2)
		}
		break;
	}
	return ret;
}

malloc_ptr<wchar_t> jdisasm_hlp_4bytes(const u1* buf, u4 buf_pos, const JavaConstPool* cpInfo)
{
	malloc_ptr<wchar_t> tstr1 = jdisasm_hlp_decode_u2(OP_WORD_CP, BE2LEU2(*(u2*)(buf + buf_pos + 1)), cpInfo, buf_pos);
	if (nullptr == tstr1)
		return nullptr;
	
	malloc_ptr<wchar_t> tstr2 = jdisasm_hlp_decode_u1(OP_BYTE_TO_UINT, buf[buf_pos + 3], cpInfo);
	if (nullptr == tstr2)
		return nullptr;

	size_t tl = wcslen(tstr1.get()) + wcslen(tstr2.get()) + 2 + 1;
	malloc_ptr<wchar_t> ret = malloc<wchar_t>(tl);
	if (nullptr == ret)
		return nullptr;

	ret.get()[0] = 0;
	wcscat_s(ret.get(), tl, tstr1.get());
	wcscat_s(ret.get(), tl, L", ");
	wcscat_s(ret.get(), tl, tstr2.get());
	return ret;
}

#define JDISASM_MAX_PAD 21

void jdisasm_get_pad(size_t oplen, wchar_t* sfill)
{
	static const wchar_t space_pad[JDISASM_MAX_PAD] = L"                    ";	//20 spaces
	static const size_t max_op_len = 20;
	memmove(sfill, space_pad, 2*(max_op_len - oplen));
	sfill[max_op_len - oplen] = 0;
}

const wchar_t* const jdisasm_get_pad2(const wchar_t* const opName, size_t tabSize)
{
	if (nullptr == opName)
		return nullptr;

	static const wchar_t space_pad[] = L"                                                            ";	//60 spaces
	static const size_t space_pad_size = _countof(space_pad) - 1;
	const size_t opLen = wcslen(opName);
	if ((opLen > space_pad_size) ||
		(opLen > tabSize))
		return space_pad + space_pad_size - 1;

	tabSize -= opLen;
	if (tabSize > space_pad_size)
		return space_pad;

	return space_pad + space_pad_size - tabSize;
}

malloc_ptr<wchar_t> jdisasm_get_str(const u1* buf, u4 size, u4 buf_pos, const JavaConstPool* cpInfo)
{
	wchar_t space_fill[JDISASM_MAX_PAD] = { 0 };

	if (size - buf_pos < 1)
		return nullptr;

	//support for 'wide' prefix
	bool wide = false;
	u4 buf_pos_b = buf_pos;
	if (j_opcode_wide == opcodes[buf[buf_pos]].op_type)
	{
		wide = true;
		if (size - buf_pos < 2)
			return nullptr;
		switch (opcodes[buf[buf_pos + 1]].op_type)
		{						
		case j_opcode_iload:
		case j_opcode_lload:
		case j_opcode_fload:
		case j_opcode_dload:
		case j_opcode_aload:
		case j_opcode_istore:
		case j_opcode_lstore:
		case j_opcode_fstore:
		case j_opcode_dstore:
		case j_opcode_astore:
		case j_opcode_ret:
		case j_opcode_iinc:
			break;
		default:
			return nullptr;
		}
		buf_pos_b++;
	}
	
	const _opcode_struct* current = &opcodes[buf[buf_pos_b]];

	if (current->size == 1)
	{
		//one byte opcodes
		size_t len = wcslen(current->name);
		malloc_ptr<wchar_t> ret = malloc<wchar_t>(len+1);
		if (nullptr == ret)
			return nullptr;
		memmove(ret.get(), current->name, 2*(len+1));
		return ret;
	}
	else if (current->size == -2)
	{
		static const wchar_t und_op[] = L"undefined opcode";
		malloc_ptr<wchar_t> ret = malloc<wchar_t>(_countof(und_op));
		if (nullptr == ret)
			return nullptr;
		memmove(ret.get(), und_op, sizeof(und_op));
		return ret;
	}
	else if ((size - buf_pos >= 2) && (current->size == 2))
	{
		//OP_BYTE_TO_INT, OP_BYTE_CP, OP_BYTE_LOCAL, OP_ATYPE
		wchar_t tmpw[0x80];
		const wchar_t* cname = nullptr;
		malloc_ptr<wchar_t> tstr = nullptr;
		if (wide)
		{
			if (size - buf_pos < 4)
				return nullptr;
			if (current->flag01 != OP_BYTE_LOCAL)
				return nullptr;
			tmpw[0] = 0;
			wcscpy_s(tmpw, 0x80, L"wide ");
			wcscat_s(tmpw, 0x80, current->name);
			cname = tmpw;
			tstr = jdisasm_hlp_decode_u2(OP_WORD_LOCAL, BE2LEU2(*(u2*)(buf + buf_pos + 2)), cpInfo, buf_pos_b);
			if (nullptr == tstr)
				return nullptr;
		}
		else
		{
			cname = current->name;
			tstr = jdisasm_hlp_decode_u1(current->flag01, buf[buf_pos + 1], cpInfo);
			if (nullptr == tstr)
				return nullptr;
		}
		jdisasm_get_pad(wcslen(cname), space_fill);
		return malloc_ptr<wchar_t>(wcscat_var(cname, space_fill, tstr.get(), 0));
	}
	else if ((size - buf_pos >= 3) && (current->size == 3))
	{
		//OP_BYTE_LOCAL | (OP_BYTE_TO_INT << 16)
		wchar_t tmpw[0x80];
		malloc_ptr<wchar_t> tstr = nullptr;
		const wchar_t* cname = current->name;
		if (current->flag01 == (OP_BYTE_LOCAL | (OP_BYTE_TO_INT << 16)))
		{
			malloc_ptr<wchar_t> tstr1 = nullptr;
			malloc_ptr<wchar_t> tstr2 = nullptr;
			if (wide)
			{
				if (size - buf_pos < 6)
					return 0;
				wcscpy_s(tmpw, 0x80, L"wide ");
				wcscat_s(tmpw, 0x80, current->name);
				cname = tmpw;
				
				tstr1 = jdisasm_hlp_decode_u2(OP_WORD_LOCAL, BE2LEU2(*(u2*)(buf + buf_pos + 2)), cpInfo, buf_pos_b);
				if (nullptr == tstr1)
					return nullptr;

				tstr2 = jdisasm_hlp_decode_u2(OP_WORD_TO_INT, BE2LEU2(*(u2*)(buf + buf_pos + 4)), cpInfo, buf_pos_b);
				if (nullptr == tstr2)
					return nullptr;
			}
			else
			{
				//special case
				tstr1 = jdisasm_hlp_decode_u1(OP_BYTE_LOCAL, buf[buf_pos + 1], cpInfo);
				if (nullptr == tstr1)
					return nullptr;

				tstr2 = jdisasm_hlp_decode_u1(OP_BYTE_TO_INT, buf[buf_pos + 2], cpInfo);
				if (nullptr == tstr2)
					return nullptr;
			}
			size_t tl = wcslen(tstr1.get()) + wcslen(tstr2.get()) + 2 + 1;
			tstr = malloc<wchar_t>(tl);
			if (nullptr == tstr)
				return nullptr;
			
			tstr.get()[0] = 0;
			wcscat_s(tstr.get(), tl, tstr1.get());
			wcscat_s(tstr.get(), tl, L", ");
			wcscat_s(tstr.get(), tl, tstr2.get());
		}
		else
		{
			//OP_WORD_TO_INT, OP_WORD_CP, OP_WORD_JUMP
			tstr = jdisasm_hlp_decode_u2(current->flag01, BE2LEU2(*(u2*)(buf + buf_pos + 1)), cpInfo, buf_pos);
			if (nullptr == tstr)
				return nullptr;
		}
		jdisasm_get_pad(wcslen(cname), space_fill);
		return malloc_ptr<wchar_t>(wcscat_var(cname, space_fill, tstr.get(), 0));
	}
	else if ((size - buf_pos >= 4) && (current->size == 4))
	{
		malloc_ptr<wchar_t> tstr = nullptr;
		//OP_WORD_CP | (OP_BYTE_TO_UINT << 16)
		if (current->flag01 == (OP_WORD_CP | (OP_BYTE_TO_UINT << 16)))
		{
			tstr = jdisasm_hlp_4bytes(buf, buf_pos, cpInfo);
			if (nullptr == tstr)
				return nullptr;
		}
		else
		{
			tstr = malloc<wchar_t>(18);
			if (nullptr == tstr)
				return nullptr;
			tstr.get()[0] = 0;
			if (swprintf(tstr.get(), 18, L"unknown(%02X %02X %02X)", buf[buf_pos + 1], buf[buf_pos + 2], buf[buf_pos + 3]) == -1)
				return nullptr;
		}
		jdisasm_get_pad(wcslen(current->name), space_fill);
		return malloc_ptr<wchar_t>(wcscat_var(current->name, space_fill, tstr.get(), 0));
	}
	else if ((size - buf_pos >= 5) && (current->size == 5))
	{
		malloc_ptr<wchar_t> tstr = nullptr; 
		//OP_WORD_CP | (OP_BYTE_TO_UINT << 16) | 0
		if (current->flag01 == (OP_WORD_CP | (OP_BYTE_TO_UINT << 16)))
		{
			tstr = jdisasm_hlp_4bytes(buf, buf_pos, cpInfo);
			if (nullptr == tstr)
				return nullptr;
		}
		else if (current->flag01 == OP_WORD_CP)
		{
			//OP_WORD_CP
			tstr = jdisasm_hlp_decode_u2(current->flag01, BE2LEU2(*(u2*)(buf + buf_pos + 1)), cpInfo, buf_pos);
			if (nullptr == tstr)
				return nullptr;
		}
		else
		{
			//OP_DWORD_JUMP
			u4 pos = buf_pos + (su4)BE2LEU4(*(u4*)(buf + buf_pos + 1));
			static const int len = 13;
			tstr = malloc<wchar_t>(len);
			if (nullptr == tstr)
				return nullptr;
			if (swprintf(tstr.get(), len, L"pos.%08X", pos) == -1)
				return nullptr;
		}
		jdisasm_get_pad(wcslen(current->name), space_fill);
		return malloc_ptr<wchar_t>(wcscat_var(current->name, space_fill, tstr.get(), 0));
	}
	else if (current->size == -1)
	{
		//calculate padd size
		int padd_size = (4 - ((buf_pos + 1) % 4)) % 4;
		malloc_ptr<wchar_t> tstr = nullptr;

		switch (opcodes[buf[buf_pos]].op_type)
		{
			case j_opcode_tableswitch:
			{
				if (buf_pos + 1 + padd_size + 3*sizeof(u4) > size)
					return nullptr;

				su4 def = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size));
				su4 low = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)));
				su4 high = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + 2*sizeof(u4)));
				if (low > high)
					return nullptr;
				const int max_size = 1 + padd_size + 3*sizeof(u4) + (high - low + 1)*sizeof(u4);
				if (buf_pos + max_size > size)
					return nullptr;
				const int msize = (80 + 10*(high - low + 1));
				tstr = malloc<wchar_t>(msize);
				if (nullptr == tstr)
					return nullptr;
				if (swprintf(tstr.get(), msize, L"l: %d, h: %d, def: pos.%08X, pos.(", low, high, buf_pos + def) == -1)
					return nullptr;

				for (int i = 0; i < high - low + 1; i++)
				{
					wchar_t t[0x20];
					if (swprintf_s(t, L"%08X", buf_pos + (su4)BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)*(i + 3)))) == -1)
						return nullptr;

					wcscat_s(tstr.get(), msize, t);
					if (i != high - low)
						wcscat_s(tstr.get(), msize, L", ");
				}
				wcscat_s(tstr.get(), msize, L")");
			}
			break;
			
			case j_opcode_lookupswitch:
			{
				if (buf_pos + 1 + padd_size + 2*sizeof(u4) > size)
					return nullptr;

				su4 def = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size));
				su4 npairs = BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)));

				const int max_size = 1 + padd_size + 2*sizeof(u4) + npairs*2*sizeof(u4);
				if (buf_pos + max_size > size)
					return nullptr;
				const int msize = (64 + 32*(npairs));
				tstr = malloc<wchar_t>(msize);
				if (nullptr == tstr)
					return nullptr;
				if (swprintf(tstr.get(), msize, L"def: pos.%08X, (key:pos).(", buf_pos + def) == -1)
					return nullptr;

				for (int i = 0; i < npairs; i++)
				{
					wchar_t t[0x20];
					if (swprintf_s(t, L"%d:%08X", (su4)BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)*(i*2 + 2))), buf_pos + (su4)BE2LEU4(*(u4*)(buf + buf_pos + 1 + padd_size + sizeof(u4)*(i*2 + 3)))) == -1)
						return nullptr;

					wcscat_s(tstr.get(), msize, t);
					if (i != npairs - 1)
						wcscat_s(tstr.get(), msize, L", ");
				}
				wcscat_s(tstr.get(), msize, L")");
			}
			break;
			default:
				return nullptr;
		}
		jdisasm_get_pad(wcslen(current->name), space_fill);
		return malloc_ptr<wchar_t>(wcscat_var(current->name, space_fill, tstr.get(), 0));
	}
	else
	{
		return nullptr;
	}
}

_jdisasm_opcodes jdisasm_get_type(const u1* buf, u4 size, u4 buf_pos)
{
	if (buf_pos >= size)
		return j_opcode_undefined;

	return opcodes[buf[buf_pos]].op_type;
}

_local_var_type jdisasm_get_local_type(const u1* buf, u4 size, u4 buf_pos)
{
	if (buf_pos >= size)
		return local_var_undefined;

	if (j_opcode_wide == opcodes[buf[buf_pos]].op_type)
	{
		buf_pos++;
		if (buf_pos >= size)
			return local_var_undefined;
	}

	return opcodes[buf[buf_pos]].local_var_type;
}

const char* jdisasm_get_specName(u1 op)
{
	return opcodes[op].specName;
}

bool jdisasm_is_block_end_opcode(_jdisasm_opcodes op)
{
	if ((j_opcode_goto == op) || 
		(j_opcode_goto_w == op) ||
		(j_opcode_ret == op) ||
		(j_opcode_return == op) ||
		(j_opcode_ireturn == op) ||
		(j_opcode_lreturn == op) ||
		(j_opcode_freturn == op) ||
		(j_opcode_dreturn == op) ||
		(j_opcode_areturn == op))
	{
		return true;
	}
	return false;
}

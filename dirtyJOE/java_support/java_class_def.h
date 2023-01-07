/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2008 ReWolf
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

#include <memory.h>
#include <cstring>
#include <cstdlib>

#include "cp_info.h"
#include "attribute_info.h"
#include "field_info.h"
#include "CONSTANT_uX_info.h"
#include "CONSTANT_uXuX_info.h"
#include "CONSTANT_Utf8_info.h"
#include "CAttrTableTemplate.h"
#include "CAttrUxTemplate.h"
#include "CAttrUxUxTemplate.h"
#include "exception_entry.h"
#include "inner_class.h"
#include "local_vars.h"
#include "line_number_tab.h"
#include "CAttrCode.h"
#include "uX_dynStruct.h"
#include "CAttrUnknown.h"
#include "stack_map_frame.h"
#include "uXuX_dynStruct.h"
#include "stack_map_frame.h"
#include "stack_map_frame_cldc.h"
#include "bootstrap_methods.h"

typedef CONSTANT_uXuX_info<u2, u2> CONSTANT_Fieldref_info;
typedef CONSTANT_uXuX_info<u2, u2> CONSTANT_Methodref_info;
typedef CONSTANT_uXuX_info<u2, u2> CONSTANT_InterfaceMethodref_info;
typedef CONSTANT_uXuX_info<u2, u2> CONSTANT_NameAndType_info;
typedef CONSTANT_uXuX_info<u1, u2> CONSTANT_MethodHandle_info;
typedef CONSTANT_uXuX_info<u2, u2> CONSTANT_InvokeDynamic_info;

typedef CONSTANT_uX_info<u2> CONSTANT_Class_info;
typedef CONSTANT_uX_info<u2> CONSTANT_String_info;
typedef CONSTANT_uX_info<u4> CONSTANT_Integer_info;
typedef CONSTANT_uX_info<u4> CONSTANT_Float_info;
typedef CONSTANT_uX_info<u2> CONSTANT_MethodType_info;

typedef CONSTANT_uXuX_info<u4, u4> CONSTANT_Long_info;
typedef CONSTANT_uXuX_info<u4, u4> CONSTANT_Double_info;

typedef field_info method_info;

typedef CAttrTable<uX_dynStruct<u2>*> CAttrExceptions;
typedef CAttrTable<inner_class*> CAttrInnerClasses;
typedef CAttrTable<line_number_tab*> CAttrLineNumberTable;
typedef CAttrTable<local_vars*> CAttrLocalVariableTable;
typedef CAttrTable<local_vars*> CAttrLocalVariableTypeTable;
typedef CAttrTable<stack_map_frame*> CAttrStackMapTable;
typedef CAttrTable<stack_map_frame_cldc*> CAttrStackMap;
typedef CAttrTable<bootstrap_methods*> CAttrBootstrapMethods;

typedef CAttrUxTemplate<u2> CAttrSourceFile;
typedef CAttrUxTemplate<u2> CAttrConstantValue;
typedef CAttrUxTemplate<u2> CAttrSignature;
typedef CAttrUxUxTemplate<u2, u2> CAttrEnclosingMethod;

typedef uX_dynStruct<u1> same_frame;
typedef uXuX_dynStruct<u1, u2> chop_frame;
typedef uXuX_dynStruct<u1, u2> same_frame_extended;

static const u1 CONSTANT_Undefined          =  0;
static const u1 CONSTANT_Utf8               =  1;
static const u1 CONSTANT_Integer            =  3;
static const u1 CONSTANT_Float              =  4;
static const u1 CONSTANT_Long               =  5;
static const u1 CONSTANT_Double             =  6;
static const u1 CONSTANT_Class              =  7;
static const u1 CONSTANT_String             =  8;
static const u1 CONSTANT_Fieldref           =  9;
static const u1 CONSTANT_Methodref          = 10;
static const u1 CONSTANT_InterfaceMethodref = 11;
static const u1 CONSTANT_NameAndType        = 12;
static const u1 CONSTANT_MethodHandle       = 15;
static const u1 CONSTANT_MethodType         = 16;
static const u1 CONSTANT_InvokeDynamic      = 18;

static const u2 ACC_PUBLIC       = 0x0001;
static const u2 ACC_PRIVATE      = 0x0002;
static const u2 ACC_PROTECTED    = 0x0004;
static const u2 ACC_STATIC       = 0x0008;
static const u2 ACC_FINAL        = 0x0010;
static const u2 ACC_SUPER        = 0x0020; //both ACC have the same value
static const u2 ACC_SYNCHRONIZED = 0x0020; //(method only)
static const u2 ACC_VOLATILE     = 0x0040;
static const u2 ACC_BRIDGE       = 0x0040; //(method only)
static const u2 ACC_TRANSIENT    = 0x0080;
static const u2 ACC_VARARGS      = 0x0080; //(method onyl)
static const u2 ACC_NATIVE       = 0x0100;
static const u2 ACC_INTERFACE    = 0x0200;
static const u2 ACC_ABSTRACT     = 0x0400;
static const u2 ACC_STRICT       = 0x0800;
static const u2 ACC_SYNTHETIC    = 0x1000;
static const u2 ACC_ANNOTATION   = 0x2000;
static const u2 ACC_ENUM         = 0x4000;

enum reference_index_type : u1
#pragma warning (suppress : 4480)
{
	REF_getField         = 1,
	REF_getStatic        = 2,
	REF_putField         = 3,
	REF_putStatic        = 4,
	REF_invokeVirtual    = 5,
	REF_invokeStatic     = 6,
	REF_invokeSpecial    = 7,
	REF_newInvokeSpecial = 8,
	REF_invokeInterface  = 9
};

static const wchar_t ATTR_UNKNOWN_STR[]                                 = L"Unknown Attribute";
static const wchar_t ATTR_CONSTANT_VALUE_STR[]                          = L"ConstantValue";
static const wchar_t ATTR_CODE_STR[]                                    = L"Code";
static const wchar_t ATTR_EXCEPTIONS_STR[]                              = L"Exceptions";
static const wchar_t ATTR_INNER_CLASSES_STR[]                           = L"InnerClasses";
static const wchar_t ATTR_SYNTHETIC_STR[]                               = L"Synthetic";
static const wchar_t ATTR_SOURCE_FILE_STR[]                             = L"SourceFile";
static const wchar_t ATTR_LINE_NUMBER_TABLE_STR[]                       = L"LineNumberTable";
static const wchar_t ATTR_LOCAL_VARIABLE_TABLE_STR[]                    = L"LocalVariableTable";
static const wchar_t ATTR_DEPRECATED_STR[]                              = L"Deprecated";
static const wchar_t ATTR_ENCLOSING_METHOD_STR[]                        = L"EnclosingMethod";
static const wchar_t ATTR_SIGNATURE_STR[]                               = L"Signature";
static const wchar_t ATTR_SOURCE_DEBUG_EXTENSION_STR[]                  = L"SourceDebugExtension";
static const wchar_t ATTR_LOCAL_VARIABLE_TYPE_TABLE_STR[]               = L"LocalVariableTypeTable";
static const wchar_t ATTR_RUNTIME_VISIBLE_ANNOTATIONS_STR[]             = L"RuntimeVisibleAnnotations";
static const wchar_t ATTR_RUNTIME_INVISIBLE_ANNOTATIONS_STR[]           = L"RuntimeInvisibleAnnotations";
static const wchar_t ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS_STR[]   = L"RuntimeVisibleParameterAnnotations";
static const wchar_t ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS_STR[] = L"RuntimeInvisibleParameterAnnotations";
static const wchar_t ATTR_ANNOTATION_DEFAULT_STR[]                      = L"AnnotationDefault";
static const wchar_t ATTR_STACK_MAP_TABLE_STR[]                         = L"StackMapTable";
static const wchar_t ATTR_STACK_MAP_STR[]                               = L"StackMap";
static const wchar_t ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS_STR[]        = L"RuntimeVisibleTypeAnnotations";
static const wchar_t ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS_STR[]      = L"RuntimeInvisibleTypeAnnotations";
static const wchar_t ATTR_BOOTSTRAP_METHODS_STR[]                       = L"BootstrapMethods";
static const wchar_t ATTR_METHOD_PARAMETERS_STR[]                       = L"MethodParameters";

static const wchar_t CONSTANT_Class_STR[]               = L"CONSTANT_Class";
static const wchar_t CONSTANT_Utf8_STR[]                = L"CONSTANT_Utf8";
static const wchar_t CONSTANT_Fieldref_STR[]            = L"CONSTANT_Fieldref";
static const wchar_t CONSTANT_Methodref_STR[]           = L"CONSTANT_Methodref";
static const wchar_t CONSTANT_InterfaceMethodref_STR[]  = L"CONSTANT_InterfaceMethodref";
static const wchar_t CONSTANT_String_STR[]              = L"CONSTANT_String";
static const wchar_t CONSTANT_Integer_STR[]             = L"CONSTANT_Integer";
static const wchar_t CONSTANT_Float_STR[]               = L"CONSTANT_Float";
static const wchar_t CONSTANT_Long_STR[]                = L"CONSTANT_Long";
static const wchar_t CONSTANT_Double_STR[]              = L"CONSTANT_Double";
static const wchar_t CONSTANT_NameAndType_STR[]         = L"CONSTANT_NameAndType";
static const wchar_t CONSTANT_MethodHandle_STR[]        = L"CONSTANT_MethodHandle";
static const wchar_t CONSTANT_MethodType_STR[]          = L"CONSTANT_MethodType";
static const wchar_t CONSTANT_InvokeDynamic_STR[]       = L"CONSTANT_InvokeDynamic";

static const wchar_t ACC_PUBLIC_STR[]       = L"public";
static const wchar_t ACC_PRIVATE_STR[]      = L"private";
static const wchar_t ACC_PROTECTED_STR[]    = L"protected";
static const wchar_t ACC_STATIC_STR[]       = L"static";
static const wchar_t ACC_FINAL_STR[]        = L"final";
static const wchar_t ACC_SYNCHRONIZED_STR[] = L"synchronized";
static const wchar_t ACC_BRIDGE_STR[]       = L"bridge";
static const wchar_t ACC_VARARGS_STR[]      = L"varargs";
static const wchar_t ACC_SUPER_STR[]        = L"super";
static const wchar_t ACC_VOLATILE_STR[]     = L"volatile";
static const wchar_t ACC_TRANSIENT_STR[]    = L"transient";
static const wchar_t ACC_NATIVE_STR[]       = L"native";
static const wchar_t ACC_INTERFACE_STR[]    = L"interface";
static const wchar_t ACC_ABSTRACT_STR[]     = L"abstract";
static const wchar_t ACC_STRICT_STR[]       = L"strict";
static const wchar_t ACC_SYNTHETIC_STR[]    = L"synthetic";
static const wchar_t ACC_ANNOTATION_STR[]   = L"annotation";
static const wchar_t ACC_ENUM_STR[]         = L"enum";

static const u1 _U1_ = 0;
static const u2 _U2_ = 0;
static const u4 _U4_ = 0;

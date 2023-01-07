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

#include <windows.h>
#include "java_class_def.h"
#include "CClassFile.h"

static const struct
{
	JavaAttribute attr_id;
	const wchar_t* attr_name;
} 
attr_tab[] =
{
	{ ATTR_UNKNOWN, ATTR_UNKNOWN_STR },
	{ ATTR_CONSTANT_VALUE, ATTR_CONSTANT_VALUE_STR },
	{ ATTR_CODE, ATTR_CODE_STR },
	{ ATTR_EXCEPTIONS, ATTR_EXCEPTIONS_STR },
	{ ATTR_INNER_CLASSES, ATTR_INNER_CLASSES_STR },
	{ ATTR_SYNTHETIC, ATTR_SYNTHETIC_STR },
	{ ATTR_SOURCE_FILE, ATTR_SOURCE_FILE_STR },
	{ ATTR_LINE_NUMBER_TABLE, ATTR_LINE_NUMBER_TABLE_STR },
	{ ATTR_LOCAL_VARIABLE_TABLE, ATTR_LOCAL_VARIABLE_TABLE_STR },
	{ ATTR_DEPRECATED, ATTR_DEPRECATED_STR },
	{ ATTR_ENCLOSING_METHOD, ATTR_ENCLOSING_METHOD_STR },
	{ ATTR_SIGNATURE, ATTR_SIGNATURE_STR },
	{ ATTR_SOURCE_DEBUG_EXTENSION, ATTR_SOURCE_DEBUG_EXTENSION_STR },
	{ ATTR_LOCAL_VARIABLE_TYPE_TABLE, ATTR_LOCAL_VARIABLE_TYPE_TABLE_STR },
	{ ATTR_RUNTIME_VISIBLE_ANNOTATIONS, ATTR_RUNTIME_VISIBLE_ANNOTATIONS_STR },
	{ ATTR_RUNTIME_INVISIBLE_ANNOTATIONS, ATTR_RUNTIME_INVISIBLE_ANNOTATIONS_STR },
	{ ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS, ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS_STR },
	{ ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS, ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS_STR },
	{ ATTR_ANNOTATION_DEFAULT, ATTR_ANNOTATION_DEFAULT_STR },
	{ ATTR_STACK_MAP_TABLE, ATTR_STACK_MAP_TABLE_STR },
	{ ATTR_STACK_MAP, ATTR_STACK_MAP_STR },
	{ ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS, ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS_STR },
	{ ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS, ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS_STR },
	{ ATTR_BOOTSTRAP_METHODS, ATTR_BOOTSTRAP_METHODS_STR },
	{ ATTR_METHOD_PARAMETERS, ATTR_METHOD_PARAMETERS_STR }
};

static const struct
{
	const u1 tag_id;
	const wchar_t* const tag_name;
#pragma warning (suppress: 4510 4512 4610)
} 
cp_type_tab[] =
{
	{ CONSTANT_Utf8, CONSTANT_Utf8_STR },
	{ CONSTANT_Integer, CONSTANT_Integer_STR },
	{ CONSTANT_Float, CONSTANT_Float_STR },
	{ CONSTANT_Long, CONSTANT_Long_STR },
	{ CONSTANT_Double, CONSTANT_Double_STR },
	{ CONSTANT_Class, CONSTANT_Class_STR },
	{ CONSTANT_String, CONSTANT_String_STR },
	{ CONSTANT_Fieldref, CONSTANT_Fieldref_STR },
	{ CONSTANT_Methodref, CONSTANT_Methodref_STR },
	{ CONSTANT_InterfaceMethodref, CONSTANT_InterfaceMethodref_STR },
	{ CONSTANT_NameAndType, CONSTANT_NameAndType_STR },
	{ CONSTANT_MethodType, CONSTANT_MethodType_STR },
	{ CONSTANT_MethodHandle, CONSTANT_MethodHandle_STR },
	{ CONSTANT_InvokeDynamic, CONSTANT_InvokeDynamic_STR }
};

const wchar_t* const getWCharFromCPTag(u1 tag);
CONSTANT_Utf8_info* getUtf8FromV1(CONSTANT_uX_info<u2>* classS, const JavaConstPool& cpInfo);
CONSTANT_Utf8_info* getUtf8FromV1(u2 classNum, const JavaConstPool& cpInfo);
CONSTANT_Utf8_info* getUtf8FromCP(u2 index, const JavaConstPool& cpInfo);
malloc_ptr<wchar_t> getNameAndTypeString(CONSTANT_NameAndType_info* nat, const JavaConstPool& cpInfo);
malloc_ptr<wchar_t> getRefString(CONSTANT_uXuX_info<u2, u2>* ref, const JavaConstPool& cpInfo);
malloc_ptr<wchar_t> genStringFromCP(const JavaConstPool& cpInfo, u2 index, bool typeSpec = true);
malloc_ptr<wchar_t> genStringFromCP(CClassFile* clf, u4 index, cp_info* ccp);
malloc_ptr<wchar_t> genStringFromCP(const JavaConstPool& cpInfo, u4 index, cp_info* ccp, bool typeSpec = true, bool hexIdx = false);
malloc_ptr<wchar_t> genDisAsmStringFromCP(const JavaConstPool& cpInfo, u4 index);
malloc_ptr<wchar_t> genWStringFromAccessFlags(u2 flags, bool method);
malloc_ptr<wchar_t> preprocessUtf8ToWChar(CONSTANT_Utf8_info* utf8, int* outSize = nullptr);
malloc_ptr<u1> preprocessWCharToUtf8(const wchar_t* wstr, size_t& outSize);
int decodeDescriptorLen(const wchar_t* descriptor);
malloc_ptr<wchar_t> decodeDescriptor(const wchar_t* descriptor, const wchar_t* name, const wchar_t* accflags);
CAttrCode* getCodeAttribute(method_info& method);
JavaAttribute getAttributeType(CONSTANT_Utf8_info* utf8);
JavaAttribute getAttributeType(const JavaConstPool& cp, u2 attrIdx);
bool containsAttribute(const JavaConstPool& cp, std::vector<attribute_info*>& aiv, JavaAttribute attrType);
void eraseAttributeByType(const JavaConstPool& cp, std::vector<attribute_info*>& aiv, JavaAttribute attrType);
attribute_info* getAttributeByType(const JavaConstPool& cp, std::vector<attribute_info*>& aiv, JavaAttribute attrType);
CDynamicStructWithCP* frameSwitch(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
verification_type_info* verificationSwitch(u1* mem, u4 maxLen, const JavaConstPool& constant_pool, CLogger& logger);
bool writeStruct(FILE* file, CDynamicStruct& str);
u2 getIdxOfFirstObjectByType(const JavaConstPool& cp, u1 type);
u2 checkStringInCP(JavaConstPool& cp, const wchar_t* cpStr, CLogger& lgr);
u2 getStringIdxFromCP(JavaConstPool& cp, const wchar_t* cpStr, CLogger& lgr);
malloc_ptr<wchar_t> genWStringFromVerificationTypeInfo(const JavaConstPool& constant_pool, verification_type_info* vti);
const wchar_t* const genWCharFromReferenceIndexType(reference_index_type rit);

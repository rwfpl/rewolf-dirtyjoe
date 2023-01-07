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

#include "attribute_info.h"
#include <cstring>
#include <cstdlib>

#include "helpers.h"
#include "CAttrCode.h"
#include "java_class_def.h"

bool attribute_info::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		u4 attribute_length = attr->getSize();
		*(u2*)(mem) = BE2LEU2(attribute_name_index);
		*(u4*)(mem + 2) = BE2LEU4(attribute_length);
		if (attribute_length)
			attr->storeData(mem + 6, maxSize - 6);
	}
	LOGFEND();
	return ret;
}

u4 attribute_info::getSize() const
{
	LOGFSTART();
	LOGFEND();
	return attr->getSize() + 6;
}

attribute_info::attribute_info(JavaAttribute attrType, JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	//CAttrUxTemplate<T> : T
	//CAttrUxUxTemplate<T, U> : T, U
	//CAttrTable<T*> : u2, counter = 0
	//CAttrCode : u2, u2, u4, u2, u2
#pragma pack(push)
#pragma pack(1)
	struct
	{
		u2 nmI;
		u4 atL;
		u1 dummy[0x10];	//0x10 dummy bytes should be sufficient to create any supported empty attribute
	} ds = { 0 };
#pragma pack(pop)

	ds.nmI = BE2LEU2(getStringIdxFromCP(cp, attr_tab[attrType].attr_name, log));
	if (0 == ds.nmI)
	{
		error = true;
		LOGFEND();
		return;
	}

	switch (attrType)
	{
		case ATTR_SIGNATURE:
		case ATTR_INNER_CLASSES:
		case ATTR_LINE_NUMBER_TABLE:
		case ATTR_LOCAL_VARIABLE_TABLE:
		case ATTR_STACK_MAP_TABLE:
		case ATTR_STACK_MAP:
		case ATTR_EXCEPTIONS:
		case ATTR_SOURCE_FILE:
		case ATTR_CONSTANT_VALUE: 
			ds.atL = BE2LEU4(sizeof(u2)); 
			break;
		case ATTR_CODE: 
			ds.atL = BE2LEU4(4*sizeof(u2) + sizeof(u4)); 
			break;
		case ATTR_ENCLOSING_METHOD: 
			ds.atL = BE2LEU4(2*sizeof(u2)); 
			break;
		case ATTR_SYNTHETIC:
		case ATTR_DEPRECATED: 
			ds.atL = BE2LEU4(0); 
			break;
		default: 
			error = true; 
			LOGFEND();
			return;
	}

	new (this) attribute_info((u1*)&ds, sizeof(ds), cp, log);
	LOGFEND();
}

attribute_info::attribute_info(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	attr = 0;
	if (maxLen < 6)
	{
		error = true;
		LOGFEND();
		return;
	}
	attribute_name_index = BE2LEU2(*(u2*)(mem));
	u4 attribute_length = BE2LEU4(*(u4*)(mem + 2));
	if (maxLen < attribute_length + 6)
	{
		error = true;
		LOGFEND();
		return;
	}

	CONSTANT_Utf8_info* utf8 = getUtf8FromCP(attribute_name_index, constant_pool);
	if (!utf8) 
	{
		error = true;
		LOGFEND();
		return;
	}
	JavaAttribute attrType = getAttributeType(utf8);
	//TODO:
	//ATTR_RUNTIME_VISIBLE_ANNOTATIONS
	switch (attrType)
	{
		case ATTR_SOURCE_FILE:
			attr = new (std::nothrow) CAttrSourceFile(mem + 6, maxLen - 6, log);
			break;
		case ATTR_CONSTANT_VALUE:
			attr = new (std::nothrow) CAttrConstantValue(mem + 6, maxLen - 6, log);
			break;
		case ATTR_CODE:
			attr = new (std::nothrow) CAttrCode(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_EXCEPTIONS:
			attr = new (std::nothrow) CAttrExceptions(mem + 6, maxLen - 6, constant_pool, log);
			break;	
		case ATTR_INNER_CLASSES:
			attr = new (std::nothrow) CAttrInnerClasses(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_LINE_NUMBER_TABLE:
			attr = new (std::nothrow) CAttrLineNumberTable(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_LOCAL_VARIABLE_TABLE:
			attr = new (std::nothrow) CAttrLocalVariableTable(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_LOCAL_VARIABLE_TYPE_TABLE:
			attr = new (std::nothrow) CAttrLocalVariableTypeTable(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_STACK_MAP_TABLE:
			attr = new (std::nothrow) CAttrStackMapTable(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_STACK_MAP:
			attr = new (std::nothrow) CAttrStackMap(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_SIGNATURE:
			attr = new (std::nothrow) CAttrSignature(mem + 6, maxLen - 6, log);
			break;
		case ATTR_ENCLOSING_METHOD:
			attr = new (std::nothrow) CAttrEnclosingMethod(mem + 6, maxLen - 6, log);
			break;
		case ATTR_BOOTSTRAP_METHODS:
			attr = new (std::nothrow) CAttrBootstrapMethods(mem + 6, maxLen - 6, constant_pool, log);
			break;
		case ATTR_SYNTHETIC:
		case ATTR_DEPRECATED:
			attr = new (std::nothrow) CAttrUnknown(mem + 6, 0, log);
			break;
		default:
			log.log(1, "Unsupported attribute: %s\n", &utf8->get_bytes()[0]);
			attr = new (std::nothrow) CAttrUnknown(mem + 6, attribute_length, log);
			break;
	}
	if (attr == 0)
	{
		error = true;
		LOGFEND();
		return;
	}
	if (attr->getError())
	{
		delete attr;
		attr = 0;
		error = true;
		LOGFEND();
		return;
	}
	error = false;
	LOGFEND();
}

attribute_info::~attribute_info()
{
	LOGFSTART();
	if (attr)
		delete attr;
	LOGFEND();
}

CDynamicStruct& attribute_info::get_attr()
{
	LOGFSTART();
	LOGFEND();
	return *attr;
}

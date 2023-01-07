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

#include "java_types.h"
#include "java_class_def.h"
#include "CDynamicStructWithCP.h"
#include "../common/common_macros.h"
#include "exception_entry.h"
#include "jdisasm/JvmFunction.h"

class CAttrCode : public CDynamicStructWithCP
{
private:
	GETSET(u2, max_stack);
	GETSET(u2, max_locals);
	//GETTER(u4, code_length);
	GETTER_PTR(std::vector<u1>, code);
	//GETTER(u2, exception_table_length);
	GETTER_PTR(std::vector<exception_entry*>, exception_table);
	//GETTER(u2, attributes_count);
	GETTER_PTR(std::vector<attribute_info*>, attributes);

	GETTER_PTR(CJvmFunction*, funcCode);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	CAttrCode(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~CAttrCode();
};

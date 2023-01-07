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
#include "CDynamicStructWithCP.h"
#include "../common/common_macros.h"
#include "cp_info.h"
#include <vector>

class attribute_info : public CDynamicStructWithCP
{
private:
	GETTER(u2, attribute_name_index);
	//u4 attribute_length;

	CDynamicStruct* attr;
	
public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	attribute_info(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	attribute_info(JavaAttribute attrType, JavaConstPool& cp, CLogger& log);
	virtual ~attribute_info();

	CDynamicStruct& get_attr();
};

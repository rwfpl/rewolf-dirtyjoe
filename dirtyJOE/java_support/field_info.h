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

#include "CDynamicStructWithCP.h"
#include "../common/common_macros.h"
#include "attribute_info.h"
#include "cp_info.h"
#include <vector>

class field_info : public CDynamicStructWithCP
{
private:
	GETSET(u2, access_flags);
	GETSET(u2, name_index);
	GETSET(u2, descriptor_index);
	//u2 attributes_count;
	GETTER_PTR(std::vector<attribute_info*>, attributes);	//[attributes_count];

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	field_info(u2 access_flags, u2 name_index, u2 descriptor_index, const JavaConstPool& cp, CLogger& log);
	field_info(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~field_info();
};

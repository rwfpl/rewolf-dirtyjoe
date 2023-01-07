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

#include "field_info.h"
#include <cstring>
#include <cstdlib>
#include "helpers.h"

bool field_info::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(access_flags);
		*(u2*)(mem + 2) = BE2LEU2(name_index);
		*(u2*)(mem + 4) = BE2LEU2(descriptor_index);
		//attributes_count = (u2)attributes.size();
		u4 currentPos = 6;
		if (!storeObjects(_U2_, attributes, mem, currentPos, maxSize))
		{
			LOGFEND();
			return false;
		}
	}
	LOGFEND();
	return ret;
}

u4 field_info::getSize() const
{
	LOGFSTART();
	u4 ret = 8;
	if (error) 
		ret = (u4)-1;
	else
	{
		ret += getSizeOfObjects(attributes);
	}
	LOGFEND();
	return ret;
}

field_info::field_info(u2 access_flags, u2 name_index, u2 descriptor_index, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	access_flags(access_flags),
	name_index(name_index),
	descriptor_index(descriptor_index)
{}

field_info::field_info(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < 8)
	{
		error = true;
		LOGFEND();
		return;
	}
	access_flags = BE2LEU2(*(u2*)(mem));
	name_index = BE2LEU2(*(u2*)(mem + 2));
	descriptor_index = BE2LEU2(*(u2*)(mem + 4));
	u4 currentPos = 6;
	if (!readObjects(_U2_, attributes, mem, currentPos, maxLen, constant_pool, logger))
	{
		error = true;
		LOGFEND();
		return;
	}
	LOGFEND();
}

field_info::~field_info()
{
	LOGFSTART();
	cleanUpObjects(attributes);
	LOGFEND();
}

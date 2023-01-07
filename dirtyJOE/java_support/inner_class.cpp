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

#include "inner_class.h"

bool inner_class::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(inner_class_info_index);
		*(u2*)(mem + 2) = BE2LEU2(outer_class_info_index);
		*(u2*)(mem + 4) = BE2LEU2(inner_name_index);
		*(u2*)(mem + 6) = BE2LEU2(inner_class_access_flags);
	}
	LOGFEND();
	return ret;
}

u4 inner_class::getSize() const
{
	LOGFSTART();
	LOGFEND();
	return 4*sizeof(u2); 
}

inner_class::inner_class(u2 inner_class_info_index, u2 outer_class_info_index, u2 inner_name_index, u2 inner_class_access_flags, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	inner_class_info_index(inner_class_info_index),
	outer_class_info_index(outer_class_info_index),
	inner_name_index(inner_name_index),
	inner_class_access_flags(inner_class_access_flags) {}

inner_class::inner_class(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < getSize()) 
		error = true;
	else
	{
		inner_class_info_index = BE2LEU2(*(u2*)(mem));
		outer_class_info_index = BE2LEU2(*(u2*)(mem + 2));
		inner_name_index = BE2LEU2(*(u2*)(mem + 4));
		inner_class_access_flags = BE2LEU2(*(u2*)(mem + 6));
	}
	LOGFEND();		
}

inner_class::~inner_class()
{
	LOGFSTART();
	LOGFEND();
}

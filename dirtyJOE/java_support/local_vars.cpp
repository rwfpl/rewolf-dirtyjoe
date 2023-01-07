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

#include "local_vars.h"
#include "helpers.h"

bool local_vars::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(start_pc);
		*(u2*)(mem + 2) = BE2LEU2(length);
		*(u2*)(mem + 4) = BE2LEU2(name_index);
		*(u2*)(mem + 6) = BE2LEU2(descriptor_index);
		*(u2*)(mem + 8) = BE2LEU2(index);
	}
	LOGFEND();
	return ret;
}

u4 local_vars::getSize() const
{ 
	LOGFSTART();
	LOGFEND();
	return 5*sizeof(u2); 
}

local_vars::local_vars(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : 
CDynamicStructWithCP(cp, log), signature_index(descriptor_index)
{
	LOGFSTART();
	if (maxLen < getSize()) 
		error = true;
	else
	{
		start_pc = BE2LEU2(*(u2*)(mem));
		length = BE2LEU2(*(u2*)(mem + 2));
		name_index = BE2LEU2(*(u2*)(mem + 4));
		descriptor_index = BE2LEU2(*(u2*)(mem + 6));
		index = BE2LEU2(*(u2*)(mem + 8));
		error = false;
	}
	LOGFEND();		
}

local_vars::local_vars(JavaConstPool& cp, CLogger& log, u2 _start_pc, u2 _len, wchar_t* name, wchar_t* descriptor, u2 local_index) :
CDynamicStructWithCP(cp, log), signature_index(descriptor_index), start_pc(_start_pc), length(_len), index(local_index)
{
	LOGFSTART();
	name_index = getStringIdxFromCP(cp, name, log);
	descriptor_index = getStringIdxFromCP(cp, descriptor, log);
	error = (0 == name_index) || (0 == descriptor_index);
	LOGFEND();
}

local_vars::~local_vars()
{
	LOGFSTART();
	LOGFEND();
}

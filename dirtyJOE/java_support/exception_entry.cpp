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

#include "exception_entry.h"

bool exception_entry::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(start_pc);
		*(u2*)(mem + 2) = BE2LEU2(end_pc);
		*(u2*)(mem + 4) = BE2LEU2(handler_pc);
		*(u2*)(mem + 6) = BE2LEU2(catch_type);
	}
	LOGFEND();
	return ret;
}

u4 exception_entry::getSize() const
{ 
	LOGFSTART();
	LOGFEND();
	return 4*sizeof(u2); 
}

exception_entry::exception_entry(u2 _start_pc, u2 _end_pc, u2 _handler_pc, u2 _catch_type, const JavaConstPool& cp, CLogger& log) :
CDynamicStructWithCP(cp, log), start_pc(_start_pc), end_pc(_end_pc), handler_pc(_handler_pc), catch_type(_catch_type)
{
	LOGFSTART();
	LOGFEND();
}

exception_entry::exception_entry(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < getSize()) 
		error = true;
	else
	{
		start_pc = BE2LEU2(*(u2*)(mem));
		end_pc = BE2LEU2(*(u2*)(mem + 2));
		handler_pc = BE2LEU2(*(u2*)(mem + 4));
		catch_type = BE2LEU2(*(u2*)(mem + 6));
		error = false;
	}
	LOGFEND();		
}

exception_entry::~exception_entry()
{
	LOGFSTART();
	LOGFEND();
}

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

#include "line_number_tab.h"

bool line_number_tab::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(start_pc);
		*(u2*)(mem + 2) = BE2LEU2(line_number);
	}
	LOGFEND();
	return ret;
}

u4 line_number_tab::getSize() const
{ 
	LOGFSTART();
	LOGFEND();
	return 2*sizeof(u2); 
}

line_number_tab::line_number_tab(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < getSize()) 
		error = true;
	else
	{
		start_pc = BE2LEU2(*(u2*)(mem));
		line_number = BE2LEU2(*(u2*)(mem + 2));
		error = false;
	}
	LOGFEND();		
}

line_number_tab::line_number_tab(u2 spc, u2 ln, const JavaConstPool& cp, CLogger& log) : 
CDynamicStructWithCP(cp, log), start_pc(spc), line_number(ln)
{
	LOGFSTART();
	LOGFEND();
}

line_number_tab::~line_number_tab()
{
	LOGFSTART();
	LOGFEND();
}

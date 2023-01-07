/*
* dirtyJOE - Java Overall Editor
* Copyright 2010 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/

#include "CAttrUnknown.h"

bool CAttrUnknown::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	if (data.size() > maxSize)
	{
		LOGFEND();
		return false;
	}
	memmove(mem, &data[0], data.size());
	LOGFEND();
	return true;
}

u4 CAttrUnknown::getSize() const
{
	LOGFSTART();
	LOGFEND();
	return (u4)data.size();
}

CAttrUnknown::CAttrUnknown(u1* mem, u4 maxLen, CLogger& log) : CDynamicStruct(log)
{
	LOGFSTART();
	data.resize(maxLen);
	if (maxLen) 
		memmove(&data[0], mem, maxLen);
	error = false;
	LOGFEND();
}

CAttrUnknown::~CAttrUnknown()
{
	LOGFSTART();
	LOGFEND();
}

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
#include "../common/CDynamicStruct.h"
#include "../common/common_macros.h"

template <class T>
class CAttrUxTemplate : public CDynamicStruct
{
private:
	GETSET(T, value);
public:
	bool storeData(unsigned char* mem, unsigned int maxSize)
	{
		LOGFSTART();

		bool ret = true;
		if (maxSize < getSize()) 
			ret = false;
		else
		{
			#pragma warning (suppress : 4333)
			*(T*)(mem) = BE2LE(value);
		}

		LOGFEND();
		return ret;
	}

	u4 getSize() const
	{
		LOGFSTART();
		LOGFEND();
		return sizeof(T);
	}

	CAttrUxTemplate(u1* mem, u4 maxLen, CLogger& log) : CDynamicStruct(log)
	{
		LOGFSTART();
		if (maxLen < getSize()) 
			error = true;
		else 
		{
			#pragma warning (suppress : 4333)
			value = BE2LE(*(T*)mem);
		}
		LOGFEND();
	}

	virtual ~CAttrUxTemplate()
	{
		LOGFSTART();
		LOGFEND();
	}
};

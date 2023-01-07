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

template <class T, class U>
class CAttrUxUxTemplate : public CDynamicStruct
{
private:
	GETSET(T, v1);
	GETSET(U, v2);
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
			*(T*)(mem) = BE2LE(v1);
			#pragma warning (suppress : 4333)
			*(U*)(mem + sizeof(T)) = BE2LE(v2);
		}

		LOGFEND();
		return ret;
	}

	u4 getSize() const
	{
		LOGFSTART();
		LOGFEND();
		return sizeof(T) + sizeof(U);
	}

	CAttrUxUxTemplate(u1* mem, u4 maxLen, CLogger& log) : CDynamicStruct(log)
	{
		LOGFSTART();
		if (maxLen < getSize()) 
			error = true;
		else 
		{
			#pragma warning (suppress : 4333)
			v1 = BE2LE(*(T*)mem);
			#pragma warning (suppress : 4333)
			v2 = BE2LE(*(U*)(mem + sizeof(T)));
		}
		LOGFEND();
	}

	virtual ~CAttrUxUxTemplate()
	{
		LOGFSTART();
		LOGFEND();
	}
};

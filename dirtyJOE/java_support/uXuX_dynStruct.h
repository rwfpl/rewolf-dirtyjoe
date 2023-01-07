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
#pragma once

#include "CDynamicStructWithCP.h"
#include "uX_dynStruct.h"
#include "../common/common_macros.h"

template<class T, class U>
class uXuX_dynStruct : public uX_dynStruct<T>
{
private:
	GETTER(U, value2);

public:
	virtual bool storeData(unsigned char* mem, unsigned int maxSize)
	{
		LOGFSTART();
		bool ret = true;
		if (maxSize < getSize()) 
			ret = false;
		else	
		{
#			pragma warning (suppress : 4127)
			if (sizeof(T) == 1)
				*(T*)(mem) = value;
			else
			{
#				pragma warning (suppress : 4333)
				*(T*)(mem) = BE2LE(value);
			}

#			pragma warning (suppress : 4127)
			if (sizeof(U) == 1)
				*(U*)(mem + sizeof(T)) = value2;
			else
			{
#				pragma warning (suppress : 4333)
				*(U*)(mem + sizeof(T)) = BE2LE(value2);
			}
		}
		LOGFEND();
		return ret;
	}

	virtual u4 getSize() const
	{
		LOGFSTART();
		LOGFEND();
		return sizeof(T) + sizeof(U);
	}

	uXuX_dynStruct(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : uX_dynStruct(cp, log)
	{
		LOGFSTART();
		if (maxLen < getSize()) 
			error = true;
		else
		{
#			pragma warning (suppress : 4127)
			if (sizeof(T) == 1)
				value = *(T*)mem;
			else
			{
#				pragma warning (suppress : 4333)
				value = BE2LE(*(T*)(mem));
			}

#			pragma warning (suppress : 4127)
			if (sizeof(U) == 1)
				value2 = *(U*)(mem + sizeof(T));
			else
			{
#				pragma warning (suppress : 4333)
				value2 = BE2LE(*(U*)(mem + sizeof(T)));
			}

			error = false;
		}
		LOGFEND();		
	}

	virtual ~uXuX_dynStruct()
	{
		LOGFSTART();
		LOGFEND();
	}
};

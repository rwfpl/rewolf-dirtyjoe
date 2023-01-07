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

template<class T>
class uX_dynStruct : public CDynamicStructWithCP
{
private:
	GETSET_PROT(T, value);

public:
	virtual bool storeData(unsigned char* mem, unsigned int maxSize)
	{
		LOGFSTART();
		bool ret = true;
		if (maxSize < getSize()) 
			ret = false;
		else	
		{
			#pragma warning (suppress : 4127)
			if (sizeof(T) == 1)
				*(T*)(mem) = value;
			else
			{
				#pragma warning (suppress : 4333)
				*(T*)(mem) = BE2LE(value);
			}
		}
		LOGFEND();
		return ret;
	}

	virtual u4 getSize() const
	{
		LOGFSTART();
		LOGFEND();
		return sizeof(T);
	}

	uX_dynStruct(const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log) {}

	uX_dynStruct(const JavaConstPool& cp, T v, CLogger& log) : CDynamicStructWithCP(cp, log), value(v) {}

	uX_dynStruct(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
	{
		LOGFSTART();
		if (maxLen < getSize()) 
			error = true;
		else
		{
			#pragma warning (suppress : 4127)
			if (sizeof(T) == 1)
				value = *(T*)mem;
			else
			{
				#pragma warning (suppress : 4333)
				value = BE2LE(*(T*)(mem));
			}
			error = false;
		}
		LOGFEND();		
	}

	virtual ~uX_dynStruct()
	{
		LOGFSTART();
		LOGFEND();
	}
};

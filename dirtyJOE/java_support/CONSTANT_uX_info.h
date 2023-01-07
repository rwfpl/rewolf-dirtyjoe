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

#include "cp_info.h"

template<class T>
class CONSTANT_uX_info : public cp_info
{
public:
	T v1;

	bool storeData(u1* mem, u4 maxSize)
	{
		LOGFSTART();
		bool ret = true;
		if (maxSize < getSize()) 
			ret = false;
		else
		{	
			mem[0] = tag;
			#pragma warning (suppress : 4333)
			*(T*)(mem + 1) = BE2LE(v1);
		}
		LOGFEND();
		return ret;
	}

	u4 getSize() const
	{
		LOGFSTART();
		LOGFEND();
		return 1 + sizeof(T);
	}

	CONSTANT_uX_info(u1 tag, T v1, CLogger& log) : cp_info(log, tag), v1(v1) {}

	CONSTANT_uX_info(u1* mem, u4 maxLen, CLogger& log) : cp_info(log)
	{
		LOGFSTART();
		if (maxLen < getSize())
			error = true;
		else
		{
			tag = mem[0];
			#pragma warning (suppress : 4333)
			v1 = BE2LE(*(T*)(mem + 1));
		}
		LOGFEND();
	}

	virtual ~CONSTANT_uX_info()
	{
		LOGFSTART();
		LOGFEND();
	}
};

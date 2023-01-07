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

template <class T, class U>
class CONSTANT_uXuX_info : public cp_info
{
public:
	T v1;
	U v2;

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
			#pragma warning (suppress : 4333)
			*(U*)(mem + 1 + sizeof(T)) = BE2LE(v2);
		}
		LOGFEND();
		return ret;
	}

	u4 getSize() const
	{
		LOGFSTART();
		LOGFEND();
		return 1 + sizeof(T) + sizeof(U);
	}

	CONSTANT_uXuX_info(u1 tag, T v1, U v2, CLogger& log) : cp_info(log, tag), v1(v1), v2(v2) {}

	CONSTANT_uXuX_info(u1* mem, u4 maxLen, CLogger& log) : cp_info(log)
	{
		LOGFSTART();
		if (maxLen < getSize())
			error = true;
		else
		{
			tag = mem[0];
			#pragma warning (suppress : 4333)
			v1 = BE2LE(*(T*)(mem + 1));
			#pragma warning (suppress : 4333)
			v2 = BE2LE(*(U*)(mem + 1 + sizeof(T)));
		}
		LOGFEND();
	}

	virtual ~CONSTANT_uXuX_info()
	{
		LOGFSTART();
		LOGFEND();
	}
};

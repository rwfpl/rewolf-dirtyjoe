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

#include "CONSTANT_Utf8_info.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "helpers.h"

u4 CONSTANT_Utf8_info::getSize() const
{
	LOGFSTART();
	LOGFEND();
	return (u4)(bytes.size() - 1 + 3);
}

bool CONSTANT_Utf8_info::storeData(u1* mem, u4 maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		u2 length = (u2)bytes.size() - 1;
		mem[0] = tag;
		*(u2*)(mem + 1) = BE2LEU2(length);
		memmove(mem + 3, &bytes[0], length);
	}
	LOGFEND();
	return ret;
}

CONSTANT_Utf8_info::CONSTANT_Utf8_info(u1* mem, u4 maxLen, CLogger& log) : cp_info(log)
{
	LOGFSTART();
	if (maxLen < 3)
	{
		error = true;
		LOGFEND();
		return;
	}
	tag = mem[0];
	u2 length = BE2LEU2(*(u2*)(mem + 1));
	if ((u4)length + 3 > maxLen) 
	{
		error = true;
		LOGFEND();
		return;
	}
	bytes.resize(length + 1);
	memmove(&bytes[0], mem + 3, length);
	bytes[length] = 0;
	error = false;
	char tmp[64];
	sprintf_s(tmp, 64, "%%.%ds\n", length);
	logger.log(5, tmp, &bytes[0]);
	LOGFEND();
}

CONSTANT_Utf8_info::CONSTANT_Utf8_info(CLogger& log) : cp_info(log)
{
	LOGFSTART();
	tag = CONSTANT_Utf8;
	error = false;
	LOGFEND();
}

CONSTANT_Utf8_info::CONSTANT_Utf8_info(const wchar_t* wstr, CLogger& log) : cp_info(log)
{
	LOGFSTART();
	
	size_t osize = 0;
	malloc_ptr<u1> utf8 = preprocessWCharToUtf8(wstr, osize);
	if (nullptr == utf8)
	{
		error = true;
		return;
	}
	bytes.resize(osize);
	memmove(&bytes[0], utf8.get(), osize);
	tag = CONSTANT_Utf8;
	error = false;
	
	LOGFEND();
}

CONSTANT_Utf8_info::~CONSTANT_Utf8_info()
{
	LOGFSTART();
	LOGFEND();
}

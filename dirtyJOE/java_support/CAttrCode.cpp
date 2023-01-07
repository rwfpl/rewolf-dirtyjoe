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

#include "CAttrCode.h"
#include <cstring>
#include <cstdlib>
#include "helpers.h"

bool CAttrCode::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();

	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(max_stack);
		*(u2*)(mem + 2) = BE2LEU2(max_locals);
		u4 code_length = (u4)code.size();
		*(u4*)(mem + 4) = BE2LEU4(code_length);
		u4 cPos = 8;
		if (code_length) 
			memmove(mem + cPos, &code[0], code_length);
		cPos += code_length;

		if (!storeObjects(_U2_, exception_table, mem, cPos, maxSize))
		{
			LOGFEND();
			return false;
		}
		if (!storeObjects(_U2_, attributes, mem, cPos, maxSize))
		{
			LOGFEND();
			return false;
		}
	}

	LOGFEND();
	return ret;
}

u4 CAttrCode::getSize() const
{
	LOGFSTART();
	u4 ret = 0;
	if (error)
		ret = (u4)-1;
	else
	{
		ret = (u4)(4*sizeof(u2) + sizeof(u4) + code.size());
		ret += getSizeOfObjects(attributes);
		ret += getSizeOfObjects(exception_table);
	}

	LOGFEND();
	return ret;
}

CAttrCode::CAttrCode(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < 10)
	{
		error = true;
		LOGFEND();
		return;
	}
	max_stack = BE2LEU2(*(u2*)(mem));
	max_locals = BE2LEU2(*(u2*)(mem + 2));
	u4 code_length = BE2LEU4(*(u4*)(mem + 4));
	u4 cPos = 8;
	if (maxLen < cPos + code_length)
	{
		error = true;
		LOGFEND();
		return;
	}
	code.resize(code_length);
	if (code_length)
	{
		memmove(&code[0], mem + cPos, code_length);
		funcCode = new CJvmFunction(mem + cPos, code_length, cp, log);
	}
	cPos += code_length;

	if (!readObjects(_U2_, exception_table, mem, cPos, maxLen, cp, logger))
	{
		error = true;
		LOGFEND();
		return;
	}
	
	if (!readObjects(_U2_, attributes, mem, cPos, maxLen, cp, logger))
	{
		error = true;
		LOGFEND();
		return;
	}

	error = false;
	LOGFEND();
}

CAttrCode::~CAttrCode()
{
	LOGFSTART();
	if (nullptr != funcCode)
		delete funcCode;
	cleanUpObjects(attributes);
	cleanUpObjects(exception_table);
	LOGFEND();
}

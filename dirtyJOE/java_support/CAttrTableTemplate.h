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
#include "CDynamicStructWithCP.h"
#include "helpers_templates.h"
#include "../common/common_macros.h"
#include <vector>

template <class T>
class CAttrTable : public CDynamicStructWithCP
{
private:
	//u2 number_of_elements;
	GETTER_PTR(std::vector<T>, table);
public:
	bool storeData(unsigned char* mem, unsigned int maxSize)
	{
		LOGFSTART();
		bool ret = true;
		u4 cPos = 0;
		if (!storeObjects(_U2_, table, mem, cPos, maxSize)) 
			ret = false;
		LOGFEND();
		return ret;
	}

	u4 getSize() const
	{
		LOGFSTART();
		u4 ret = 0;
		if (error)
			ret = (u4)-1;
		else
		{
			ret = 2;
			ret += getSizeOfObjects(table);
		}
		LOGFEND();
		return ret;
	}

	CAttrTable(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
	{
		LOGFSTART();
		u4 cPos = 0;
		if (!readObjects(_U2_, table, mem, cPos, maxLen, constant_pool, logger)) 
			error = true;
		LOGFEND();
	}

	virtual ~CAttrTable()
	{
		LOGFSTART();
		cleanUpObjects(table);
		LOGFEND();
	}
};

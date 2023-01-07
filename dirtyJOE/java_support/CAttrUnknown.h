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

#include "java_types.h"
#include "java_class_def.h"
#include "../common/CDynamicStruct.h"
#include "../common/common_macros.h"

class CAttrUnknown : public CDynamicStruct
{
	private:
		GETTER_PTR(std::vector<u1>, data);

	public:
		bool storeData(unsigned char* mem, unsigned int maxSize);
		u4 getSize() const;
		CAttrUnknown(u1* mem, u4 maxLen, CLogger& log);
		virtual ~CAttrUnknown();
};

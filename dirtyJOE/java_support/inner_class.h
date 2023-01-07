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

class inner_class : public CDynamicStructWithCP
{
private:
	GETSET(u2, inner_class_info_index);
	GETSET(u2, outer_class_info_index);
	GETSET(u2, inner_name_index);
	GETSET(u2, inner_class_access_flags);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	inner_class(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	inner_class(u2 inner_class_info_index, u2 outer_class_info_index, u2 inner_name_index, u2 inner_class_access_flags, const JavaConstPool& cp, CLogger& log);
	virtual ~inner_class();
};

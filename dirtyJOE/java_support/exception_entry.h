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

class exception_entry : public CDynamicStructWithCP
{
private:
	GETSET(u2, start_pc);
	GETSET(u2, end_pc);
	GETSET(u2, handler_pc);
	GETSET(u2, catch_type);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	exception_entry(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	exception_entry(u2 _start_pc, u2 _end_pc, u2 _handler_pc, u2 _catch_type, const JavaConstPool& cp, CLogger& log);
	virtual ~exception_entry();
};

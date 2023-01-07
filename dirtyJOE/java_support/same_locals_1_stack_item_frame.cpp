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

#include "same_locals_1_stack_item_frame.h"
#include "helpers.h"

same_locals_1_stack_item_frame::same_locals_1_stack_item_frame(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < 1)
		error = true;
	else
	{
		frame_type = mem[0];
		error = false;
		stack = verificationSwitch(mem + 1, maxLen - 1, cp, logger);
		if (nullptr == stack)
			error = true;
	}
	LOGFEND();
}

bool same_locals_1_stack_item_frame::storeData(u1* mem, u4 maxSize)
{
	LOGFSTART();

	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		mem[0] = frame_type;
		if (!stack->storeData(mem + 1, maxSize - 1))
			ret = false;
	}

	LOGFEND();
	return ret;
}

u4 same_locals_1_stack_item_frame::getSize() const
{
	LOGFSTART();
	LOGFEND();
	if (error)
		return (u4)-1;
	else
		return 1 + stack->getSize();
}

same_locals_1_stack_item_frame::~same_locals_1_stack_item_frame()
{
	LOGFSTART();
	if (nullptr != stack)
		delete stack;
	LOGFEND();
}

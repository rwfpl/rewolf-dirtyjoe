/*
* dirtyJOE - Java Overall Editor
* Copyright 2014 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/

#include "same_locals_1_stack_item_frame_extended.h"
#include "helpers.h"

same_locals_1_stack_item_frame_extended::same_locals_1_stack_item_frame_extended(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < sizeof(u1) + sizeof(u2))
		error = true;
	else
	{
		frame_type = mem[0];
		offset_delta = BE2LEU2(*(u2*)(mem + sizeof(u1)));
		error = false;
		stack = verificationSwitch(mem + sizeof(u1) + sizeof(u2), maxLen - (sizeof(u1) + sizeof(u2)), cp, logger);
		if (nullptr == stack)
			error = true;
	}
	LOGFEND();
}

bool same_locals_1_stack_item_frame_extended::storeData(u1* mem, u4 maxSize)
{
	LOGFSTART();

	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		mem[0] = frame_type;
		*(u2*)(mem + sizeof(u1)) = BE2LEU2(offset_delta);
		if (!stack->storeData(mem + sizeof(u1) + sizeof(u2), maxSize - (sizeof(u1) + sizeof(u2))))
			ret = false;
	}

	LOGFEND();
	return ret;
}

u4 same_locals_1_stack_item_frame_extended::getSize() const
{
	LOGFSTART();
	LOGFEND();
	if (error)
		return (u4)-1;
	else
		return sizeof(u1) + sizeof(u2) + stack->getSize();
}

same_locals_1_stack_item_frame_extended::~same_locals_1_stack_item_frame_extended()
{
	LOGFSTART();
	if (nullptr != stack)
		delete stack;
	LOGFEND();
}

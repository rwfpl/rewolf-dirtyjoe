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

#include "stack_map_frame.h"
#include "helpers.h"

bool stack_map_frame::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = false;
	if (frame)
	{
		if (frame->storeData(mem, maxSize))
			ret = true;
	}
	LOGFEND();
	return ret;
}

u4 stack_map_frame::getSize() const
{
	LOGFSTART();
	LOGFEND();
	if (frame)
		return frame->getSize();
	else
		return (u4)-1;
}

stack_map_frame::stack_map_frame(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	frame = frameSwitch(mem, maxLen, cp, log);
	if (frame == 0)
		error = true;
	else
		error = false;
	LOGFEND();
}

stack_map_frame::~stack_map_frame()
{
	LOGFSTART();
	if (frame)
		delete frame;
	LOGFEND();
}

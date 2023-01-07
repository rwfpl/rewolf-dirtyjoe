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

#include "append_frame.h"
#include "helpers.h"

append_frame::append_frame(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
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
		u4 cpos = sizeof(u1) + sizeof(u2);
		for (u1 i = 0; i < frame_type - 251; i++)
		{
			verification_type_info* t = verificationSwitch(mem + cpos, maxLen - cpos, cp, logger);
			if (nullptr == t)
			{
				error = true;
				LOGFEND();
				return;
			}
			if (t->getError() || (cpos + t->getSize() > maxLen))
			{
				error = true;
				delete t;
				LOGFEND();
				return;
			}
			cpos += t->getSize();
			locals.push_back(t);
		}
	}
	LOGFEND();
}

bool append_frame::storeData(u1* mem, u4 maxSize)
{
	LOGFSTART();

	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		mem[0] = frame_type;
		*(u2*)(mem + sizeof(u1)) = BE2LEU2(offset_delta);
		u4 cpos = sizeof(u1) + sizeof(u2);
		for (u4 i = 0; i < locals.size(); i++)
		{
			if (!locals[i]->storeData(mem + cpos, (unsigned int)(maxSize - cpos)))
			{
				LOGFEND();
				return false;
			}
			cpos += locals[i]->getSize();
		}
	}

	LOGFEND();
	return ret;
}

u4 append_frame::getSize() const
{
	LOGFSTART();
	LOGFEND();
	if (error)
		return (u4)-1;
	else
		return sizeof(u1) + sizeof(u2) + getSizeOfObjects(locals);
}

append_frame::~append_frame()
{
	LOGFSTART();
	cleanUpObjects(locals);
	LOGFEND();
}

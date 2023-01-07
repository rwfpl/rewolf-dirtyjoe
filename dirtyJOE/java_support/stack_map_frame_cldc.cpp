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

#include "stack_map_frame_cldc.h"
#include "helpers.h"

bool stack_map_frame_cldc::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = false;
	if (maxSize >= getSize())
	{
		*(u2*)mem = BE2LEU2(offset);
		u4 cpos = 2;
		if (!storeObjects(_U2_, locals, mem, cpos, maxSize - cpos))
		{
			LOGFEND();
			return false;
		}
		if (!storeObjects(_U2_, stack_items, mem, cpos, maxSize - cpos))
		{
			LOGFEND();
			return false;
		}
		ret = true;
	}
	LOGFEND();
	return ret;
}

u4 stack_map_frame_cldc::getSize() const
{
	LOGFSTART();
	LOGFEND();
	if (error)
		return (u4)-1;
	else
		return 6 + getSizeOfObjects(locals) + getSizeOfObjects(stack_items);
}

stack_map_frame_cldc::stack_map_frame_cldc(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
CDynamicStructWithCP(cp, log)
{
	LOGFSTART();

	if (maxLen < 4)
		error = true;
	else
	{
		offset = BE2LEU2(*(u2*)mem);
		u2 number_of_locals = BE2LEU2(*(u2*)(mem + 2));
		u4 cpos = 4;
		for (u4 i = 0; i < number_of_locals; i++)
		{
			verification_type_info* t = verificationSwitch(mem + cpos, maxLen - cpos, cp, logger);
			if (t == 0)
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
		if (cpos + 2 > maxLen)
			error = true;
		else
		{
			u2 number_of_stack_items = BE2LEU2(*(u2*)(mem + cpos));
			cpos += 2;
			for (u4 i = 0; i < number_of_stack_items; i++)
			{
				verification_type_info* t = verificationSwitch(mem + cpos, maxLen - cpos, cp, logger);
				if (t == 0)
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
				stack_items.push_back(t);
			}
		}
		
	}

	LOGFEND();
}

stack_map_frame_cldc::~stack_map_frame_cldc()
{
	LOGFSTART();
	cleanUpObjects(locals);
	cleanUpObjects(stack_items);
	LOGFEND();
}

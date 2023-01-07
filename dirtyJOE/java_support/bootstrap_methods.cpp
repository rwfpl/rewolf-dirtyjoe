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
#include "bootstrap_methods.h"

bool bootstrap_methods::storeData(unsigned char* mem, unsigned int maxSize)
{
	LOGFSTART();
	bool ret = true;
	if (maxSize < getSize()) 
		ret = false;
	else
	{
		*(u2*)(mem) = BE2LEU2(bootstrap_method_ref);
		*(u2*)(mem + sizeof(u2)) = BE2LEU2((u2)bootstrap_arguments.size());
		u4 cpos = 2*sizeof(u2);
		auto bait = bootstrap_arguments.begin();
		while (bait != bootstrap_arguments.end())
		{
			*(u2*)(mem + cpos) = BE2LEU2(*bait);
			++bait;
			cpos += sizeof(u2);
		}
	}
	LOGFEND();
	return ret;
}

u4 bootstrap_methods::getSize() const
{
	LOGFSTART();
	LOGFEND();
	return (u4)((2 + bootstrap_arguments.size())*sizeof(u2));
}

bootstrap_methods::bootstrap_methods(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	LOGFSTART();
	if (maxLen < getSize()) 
		error = true;
	else
	{
		bootstrap_method_ref = BE2LEU2(*(u2*)(mem));
		u2 numOfItems = BE2LEU2(*(u2*)(mem + sizeof(u2)));
		u4 cpos = 2*sizeof(u2);
		for (u4 i = 0; i < numOfItems; i++)
		{
			bootstrap_arguments.push_back(BE2LEU2(*(u2*)(mem + cpos)));
			cpos += sizeof(u2);
		}
		error = false;
	}
	LOGFEND();		
}

bootstrap_methods::~bootstrap_methods()
{
	LOGFSTART();
	LOGFEND();
}

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
#include "../common/common_macros.h"
#include "CDynamicStructWithCP.h"
#include "stack_map_frame.h"

class stack_map_frame_cldc : public CDynamicStructWithCP
{
private:
	GETTER(u2, offset);
	//GETTER(u2, number_of_locals);
	//GETTER(u2, number_of_stack_items);
	GETTER_PTR(std::vector<verification_type_info*>, locals);
	GETTER_PTR(std::vector<verification_type_info*>, stack_items);

public:
	virtual bool storeData(unsigned char* mem, unsigned int maxSize);
	virtual u4 getSize() const;
	stack_map_frame_cldc(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~stack_map_frame_cldc();
};

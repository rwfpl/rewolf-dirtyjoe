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
#pragma once

#include "java_types.h"
#include "../common/common_macros.h"
#include "CDynamicStructWithCP.h"
#include "stack_map_frame.h"

class full_frame : public CDynamicStructWithCP
{
private:
	GETTER(u1, frame_type);
	GETTER(u2, offset_delta);
	//GETTER(u2, number_of_locals);
	//GETTER(u2, number_of_stack_items);
	GETTER_PTR(std::vector<verification_type_info*>, locals);
	GETTER_PTR(std::vector<verification_type_info*>, stack_items);

public:
	virtual bool storeData(unsigned char* mem, unsigned int maxSize);
	virtual u4 getSize() const;
	full_frame(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~full_frame();
};

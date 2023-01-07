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

#include "../common/common_macros.h"
#include "java_types.h"
#include "CDynamicStructWithCP.h"
#include "stack_map_frame.h"

class same_locals_1_stack_item_frame : public CDynamicStructWithCP
{
private:
	GETTER(u1, frame_type);
	GETTER(verification_type_info*, stack);

public:
	same_locals_1_stack_item_frame(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual bool storeData(u1* mem, u4 maxSize);
	virtual u4 getSize() const;
	virtual ~same_locals_1_stack_item_frame();
};

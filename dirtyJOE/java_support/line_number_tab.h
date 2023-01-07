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

class line_number_tab : public CDynamicStructWithCP
{
private:
	GETTER(u2, start_pc);
	GETTER(u2, line_number);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	line_number_tab(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	line_number_tab(u2 spc, u2 ln, const JavaConstPool& cp, CLogger& log);
	virtual ~line_number_tab();
};

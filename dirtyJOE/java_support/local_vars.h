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


class local_vars : public CDynamicStructWithCP
{
private:
	GETSET(u2, start_pc);
	GETSET(u2, length);
	GETSET(u2, name_index);
	GETSET(u2, descriptor_index);
	GETSET(u2, index);

	// small hack for LocalVariableTypeTable attribute, as it uses the same structure but
	// descriptor_index is called signature_index, so to keep it as a one class, and have
	// proper getter I'll add this small piece of code:
	GETTER(u2&, signature_index);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	local_vars(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	local_vars(JavaConstPool& cp, CLogger& log, u2 _start_pc, u2 _len, wchar_t* name, wchar_t* descriptor, u2 local_index);
	virtual ~local_vars();
};

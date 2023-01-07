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

#include "CDynamicStructWithCP.h"
#include "../common/common_macros.h"

class bootstrap_methods : public CDynamicStructWithCP
{
private:
	GETSET(u2, bootstrap_method_ref);
	//u2 num_bootstrap_arguments;
	GETTER_PTR(std::vector<u2>, bootstrap_arguments);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	bootstrap_methods(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~bootstrap_methods();
};

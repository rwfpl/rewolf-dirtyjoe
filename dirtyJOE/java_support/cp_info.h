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

#include "../common/CDynamicStruct.h"

class cp_info : public CDynamicStruct
{
	private:
		cp_info();

	public:
		u1 tag;
		cp_info(CLogger& log) : CDynamicStruct(log) {}
		cp_info(CLogger& log, u1 tag) : CDynamicStruct(log), tag(tag) {}
		virtual ~cp_info() {}
};

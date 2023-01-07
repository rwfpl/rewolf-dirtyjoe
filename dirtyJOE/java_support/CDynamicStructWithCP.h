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

#include "../common/CDynamicStruct.h"
#include "cp_info.h"
#include "JavaConstPool_type.h"

class CDynamicStructWithCP : public CDynamicStruct
{
	protected:
		const JavaConstPool& constant_pool;

	public:
		CDynamicStructWithCP(const JavaConstPool& cp, CLogger& log) : CDynamicStruct(log), constant_pool(cp) {}
		virtual ~CDynamicStructWithCP() {}

	private:
		CDynamicStructWithCP();
		CDynamicStructWithCP& operator=(const CDynamicStructWithCP&);
};

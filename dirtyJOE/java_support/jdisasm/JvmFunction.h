/*
* dirtyJOE - Java Overall Editor
* Copyright 2016 ReWolf
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
#include "../CDynamicStructWithCP.h"
#include "JvmBasicBlock.h"
#include "../../common/common_macros.h"

class CJvmFunction : public CDynamicStructWithCP
{
private:
	GETTER_PTR(std::vector<CJvmBasicBlock*>, m_basicBlocks);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	CJvmFunction(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~CJvmFunction();
};

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
#include "../../common/common_macros.h"
#include "JvmOpcode.h"

class CJvmBasicBlock : public CDynamicStructWithCP
{
private:
	GETTER_PTR(std::vector<CJvmBasicBlock*>, m_inbound);
	GETTER_PTR(std::vector<CJvmBasicBlock*>, m_outbound);
	GETTER_PTR(std::set<u4>, m_outboundUnresolved)
	GETTER_PTR(std::vector<IJvmOpcode*>, m_opcodes);
	GETSET(bool, m_hasTerminator);
	GETSET(u4, m_pos);

public:
	bool storeData(unsigned char* mem, unsigned int maxSize);
	u4 getSize() const;
	CJvmBasicBlock(u1* mem, u4 maxLen, u4 pos, const JavaConstPool& cp, CLogger& log);
	CJvmBasicBlock(u4 pos, const JavaConstPool& cp, CLogger& log);
	virtual ~CJvmBasicBlock();


};

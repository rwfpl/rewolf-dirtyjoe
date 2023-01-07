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
#include "JvmBasicBlock.h"
#include "JvmOpcodeFactory.h"

CJvmBasicBlock::CJvmBasicBlock(u1* mem, u4 maxLen, u4 pos, const JavaConstPool& cp, CLogger& log) : 
	CDynamicStructWithCP(cp, log),
	m_hasTerminator(false),
	m_pos(pos)
{
	u1* curPtr = mem;
	while (curPtr < mem + maxLen)
	{
		IJvmOpcode* curOp = CJvmOpcodeFactory::makeOpcode(curPtr, maxLen - (u4)(curPtr - mem), pos, cp, log);
		if (nullptr == curOp)
		{
			error = true;
			return;
		}
		
		m_opcodes.push_back(curOp);
		curPtr += curOp->getLen();
		pos += (u4)curOp->getLen();
		for (auto t : curOp->getTargets())
		{
			m_outboundUnresolved.insert(t);
		}

		if (curOp->isFlow())
		{
			m_hasTerminator = curOp->isTerminator();
			break;
		}
	}
}

CJvmBasicBlock::CJvmBasicBlock(u4 pos, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	m_hasTerminator(false),
	m_pos(pos)
{}

CJvmBasicBlock::~CJvmBasicBlock()
{
	for (auto op : m_opcodes)
	{
		delete op;
	}
}

bool CJvmBasicBlock::storeData(unsigned char* mem, unsigned int maxSize)
{
	return false;
}

u4 CJvmBasicBlock::getSize() const
{
	u4 ret = 0;
	for (auto op : m_opcodes)
	{
		ret += (u4)op->getLen();
	}
	return ret;
}

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
#include "JvmFunction.h"
#include <map>

CJvmFunction::CJvmFunction(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) : CDynamicStructWithCP(cp, log)
{
	u4 curPos = 0;
	std::multimap<u4, CJvmBasicBlock*> futureTargets;
	while (curPos < maxLen)
	{
		CJvmBasicBlock* bb = new (std::nothrow) CJvmBasicBlock(mem + curPos, maxLen - curPos, curPos, cp, log);
		if (nullptr == bb)
		{
			error = true;
			return;
		}
		if (bb->getError())
		{
			delete bb;
			error = true;
			return;
		}

		auto er = futureTargets.equal_range(curPos);
		for (auto it = er.first; it != er.second; ++it)
		{
			bb->get_m_inbound().push_back(it->second);
			it->second->get_m_outbound().push_back(bb);
		}
		futureTargets.erase(er.first, er.second);

		curPos += bb->getSize();
		m_basicBlocks.push_back(bb);

		// iterate over all outbound targets
		for (auto t : bb->get_m_outboundUnresolved())
		{
			if (t < curPos)
			{
				// if target is below curPos, it means that it should be already in some basic block
				//for (auto b : m_basicBlocks)
				for (auto bit = m_basicBlocks.begin(); bit != m_basicBlocks.end(); ++bit)
				{
					auto b = *bit;
					if (t == b->get_m_pos())
					{
						// if target equals to the pos of existing basic block, then just adjust inbound/outbound lists
						b->get_m_inbound().push_back(bb);
						bb->get_m_outbound().push_back(b);
						break;
					}
					else if ((t > b->get_m_pos()) && (t < b->get_m_pos() + b->getSize()))
					{
						// else if target is in the middle of existing basic block, we need to split bb
						b->get_m_inbound().push_back(bb);
						u4 cpos = b->get_m_pos();
						for (unsigned int i = 0; i < b->get_m_opcodes().size(); i++)
						{
							if (t == cpos)
							{
								// split
								CJvmBasicBlock* newBB = new (std::nothrow) CJvmBasicBlock(cpos, cp, log);
								if (nullptr == newBB)
								{
									error = true;
									return;
								}
								newBB->get_m_opcodes().insert(newBB->get_m_opcodes().begin(), b->get_m_opcodes().begin() + i, b->get_m_opcodes().end());
								b->get_m_opcodes().erase(b->get_m_opcodes().begin() + i, b->get_m_opcodes().end());

								newBB->set_m_hasTerminator(b->get_m_hasTerminator());
								b->set_m_hasTerminator(false);

								newBB->get_m_inbound().push_back(b);
								newBB->get_m_outbound() = b->get_m_outbound();
								b->get_m_outbound().clear();
								b->get_m_outbound().push_back(newBB);

								m_basicBlocks.insert(bit, newBB);

								break;
							}
							cpos += (u4)b->get_m_opcodes()[i]->getLen();
						}
						break;
					}
				}
			}
			else
			{
				//keep target for future processing
				futureTargets.emplace(std::pair<u4, CJvmBasicBlock*>(t, bb));
			}
		}
		bb->get_m_outboundUnresolved().clear();
	}
}

CJvmFunction::~CJvmFunction()
{
	for (auto b : m_basicBlocks)
	{
		delete b;
	}
}

bool CJvmFunction::storeData(unsigned char* mem, unsigned int maxSize)
{
	return false;
}

u4 CJvmFunction::getSize() const
{
	return 0;
}
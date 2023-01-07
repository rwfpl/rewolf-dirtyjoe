/*
* dirtyJOE - Java Overall Editor
* Copyright 2015 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/
#include "JvmOpLookupSwitch.h"
#include "../../common/common_templates.h"
#include "jdisasm.h"

CJvmOpLookupSwitch::CJvmOpLookupSwitch(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log, u4 pos) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error),
	paddSize((4 - ((pos + 1) % 4)) % 4),
	rawPos(pos)
{
	if ((j_opcode_lookupswitch != opcode.op_type) ||
		(maxLen < 1 + paddSize + 2 * sizeof(su4)))
	{
		error = true;
		return;
	}

	def = BE2LEU4(*(u4*)(mem + 1 + paddSize + 0 * sizeof(u4)));
	su4 npairs = BE2LEU4(*(u4*)(mem + 1 + paddSize + 1 * sizeof(u4)));


	if ((npairs * 2 * sizeof(u4) < npairs) ||	// overflow
		(npairs * 2 * sizeof(u4) > maxLen - (1 + paddSize + 2 * sizeof(u4))))
	{
		error = true;
		return;
	}

	for (int i = 0; i < npairs; i++)
	{
		u4* tmp = (u4*)(mem + 1 + paddSize + (2 + 2 * i) * sizeof(u4));
		m_npairs.emplace_back(std::pair<su4, su4>((su4)BE2LEU4(tmp[0]), (su4)BE2LEU4(tmp[1])));
		m_targets.insert(rawPos + (su4)BE2LEU4(tmp[1]));
	}
}

CJvmOpLookupSwitch::~CJvmOpLookupSwitch() {}

bool CJvmOpLookupSwitch::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < getSize()) || error)
		return false;

	// update paddSize
	paddSize = (4 - ((rawPos + 1) % 4)) % 4;

	*mem = opcode.opcode;
	memset(mem + 1, 0, paddSize);
	*(u4*)(mem + 1 + paddSize + 0 * sizeof(u4)) = BE2LEU4(def);
	*(u4*)(mem + 1 + paddSize + 1 * sizeof(u4)) = BE2LEU4((su4)m_npairs.size());

	u4* ptr = (u4*)(mem + 1 + paddSize + 3 * sizeof(u4));
	for (auto v : m_npairs)
	{
		*ptr++ = BE2LEU4(v.first);
		*ptr++ = BE2LEU4(v.second);
	}

	return true;
}

u4 CJvmOpLookupSwitch::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpLookupSwitch::getLen() const
{
	return 1 + paddSize + 2*sizeof(u4) + m_npairs.size()*2*sizeof(u4);
}

_jdisasm_opcodes CJvmOpLookupSwitch::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpLookupSwitch::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpLookupSwitch::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpLookupSwitch::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpLookupSwitch::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpLookupSwitch::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpLookupSwitch::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpLookupSwitch::getTargets() const
{
	return m_targets;
}

malloc_ptr<wchar_t> CJvmOpLookupSwitch::getDisasmString(bool decodeCP, size_t tabSize) const
{
	//swprintf_s(tmpStr, L"pos.%08X", rawPos + rawDelta);
	//return wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), tmpStr, 0);
	wchar_t tmpStr[0x80];
	swprintf_s(tmpStr, L"def: pos.%08X, (key:pos).(", rawPos + def);
	malloc_ptr<wchar_t> disStr = wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), tmpStr, 0);
	for (auto v : m_npairs)
	{
		wchar_t t[0x20];
		swprintf_s(t, L"%d:%08X, ", v.first, rawPos + v.second);
		disStr = wcscat_var(disStr.get(), t, 0);
	}
	//cut last ", "
	disStr.get()[wcslen(disStr.get()) - 2] = 0;
	return wcscat_var(disStr.get(), L")");
}

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
#include "JvmOpTableSwitch.h"
#include "../../common/common_templates.h"
#include "jdisasm.h"

CJvmOpTableSwitch::CJvmOpTableSwitch(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log, u4 pos) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error),
	paddSize((4 - ((pos + 1) % 4)) % 4),
	rawPos(pos)
{
	if ((j_opcode_tableswitch != opcode.op_type) ||
		(maxLen < 1 + paddSize + 4*sizeof(su4)))
	{
		error = true;
		return;
	}

	def = BE2LEU4(*(u4*)(mem + 1 + paddSize + 0*sizeof(u4)));
	low = BE2LEU4(*(u4*)(mem + 1 + paddSize + 1*sizeof(u4)));
	hi  = BE2LEU4(*(u4*)(mem + 1 + paddSize + 2*sizeof(u4)));

	if ((low > hi) ||
		(maxLen < 1 + paddSize + 4*sizeof(su4) + (hi - low + 1)*sizeof(su4)))
	{
		error = true;
		return;
	}

	for (int i = 0; i < hi - low + 1; i++)
	{
		su4 v = (su4)BE2LEU4(*(u4*)(mem + 1 + paddSize + sizeof(u4)*(i + 3)));
		labels.push_back(v);
		m_targets.insert(rawPos + v);
	}
}

CJvmOpTableSwitch::~CJvmOpTableSwitch() {}

bool CJvmOpTableSwitch::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < getSize()) || error)
		return false;

	// update paddSize
	paddSize = (4 - ((rawPos + 1) % 4)) % 4;

	*mem = opcode.opcode;
	memset(mem + 1, 0, paddSize);
	*(u4*)(mem + 1 + paddSize + 0*sizeof(u4)) = BE2LEU4(def);
	*(u4*)(mem + 1 + paddSize + 1*sizeof(u4)) = BE2LEU4(low);
	*(u4*)(mem + 1 + paddSize + 2*sizeof(u4)) = BE2LEU4(hi);

	u4* ptr = (u4*)(mem + 1 + paddSize + 3*sizeof(u4));
	for (auto v : labels)
	{
		*ptr++ = BE2LEU4(v);
	}

	return true;
}

u4 CJvmOpTableSwitch::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpTableSwitch::getLen() const
{
	return 1 + paddSize + 3*sizeof(u4) + (hi - low + 1)*sizeof(u4);
}

_jdisasm_opcodes CJvmOpTableSwitch::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpTableSwitch::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpTableSwitch::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpTableSwitch::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpTableSwitch::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpTableSwitch::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpTableSwitch::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpTableSwitch::getTargets() const
{
	return m_targets;
}

malloc_ptr<wchar_t> CJvmOpTableSwitch::getDisasmString(bool decodeCP, size_t tabSize) const
{
	wchar_t tmpStr[0x80];
	swprintf_s(tmpStr, L"l: %d, h: %d, def: pos.%08X, pos.(", low, hi, rawPos + def);
	malloc_ptr<wchar_t> disStr = wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), tmpStr, 0);
	for (auto v : labels)
	{
		disStr = wcscat_var(disStr.get(), to_hex(rawPos + v).get(), L", ", 0);
	}
	//cut last ", "
	disStr.get()[wcslen(disStr.get()) - 2] = 0;
	return wcscat_var(disStr.get(), L")");
}

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
#include "JvmOpFlow.h"
#include "../../common/common_templates.h"
#include "jdisasm.h"

CJvmOpFlow::CJvmOpFlow(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log, u4 pos) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error),
	rawPos(pos)
{
	if ((j_opcode_error == opcode.op_type) ||
		(maxLen < getSize()) ||
		((OP_WORD_JUMP != opcode.flag01) && (OP_DWORD_JUMP != opcode.flag01)) ||
		((3 != getSize()) && (5 != getSize())))
	{
		error = true;
		return;
	}

	if (OP_WORD_JUMP == opcode.flag01)
		rawDelta = (su4)BE2LEU2(*(u2*)(mem + 1));
	else
		rawDelta = (su4)BE2LEU4(*(u4*)(mem + 1));

	m_targets.insert(rawPos + rawDelta);
}

CJvmOpFlow::~CJvmOpFlow() {}

bool CJvmOpFlow::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < getSize()) || error)
		return false;

	*mem = opcode.opcode;
	if (OP_WORD_JUMP == opcode.flag01)
		*(u2*)(mem + 1) = BE2LEU2(rawDelta);
	else
		*(u4*)(mem + 1) = BE2LEU4(rawDelta);;

	return true;
}

u4 CJvmOpFlow::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpFlow::getLen() const
{
	return opcode.size;
}

_jdisasm_opcodes CJvmOpFlow::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpFlow::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpFlow::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpFlow::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpFlow::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpFlow::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpFlow::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpFlow::getTargets() const
{
	return m_targets;
}

malloc_ptr<wchar_t> CJvmOpFlow::getDisasmString(bool decodeCP, size_t tabSize) const
{
	wchar_t tmpStr[0x40];
	swprintf_s(tmpStr, L"pos.%08X", rawPos + rawDelta);
	return wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), tmpStr, 0);
}

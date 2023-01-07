
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
#include "JvmOpInt.h"
#include "../../common/common_templates.h"
#include "jdisasm.h"

CJvmOpInt::CJvmOpInt(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error)
{
	if (maxLen < getSize())
	{
		error = true;
		return;
	}
	if (OP_BYTE_TO_INT == opcode.flag01)
	{
		value = (su1)mem[1];
	}
	else if (OP_WORD_TO_INT == opcode.flag01)
	{
		value = (su2)BE2LEU2(*(u2*)(mem + 1));
	}
	else
	{
		error = true;
	}
}

CJvmOpInt::~CJvmOpInt() {}

bool CJvmOpInt::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < getSize()) || error)
		return false;

	mem[0] = opcode.opcode;
	if (OP_BYTE_TO_INT == opcode.flag01)
	{
		mem[1] = (u1)value;
	}
	else if (OP_WORD_TO_INT == opcode.flag01)
	{
		*(u2*)(mem + 1) = BE2LEU2((u2)value);
	}
	return true;
}

u4 CJvmOpInt::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpInt::getLen() const
{
	return opcode.size;
}

_jdisasm_opcodes CJvmOpInt::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpInt::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpInt::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpInt::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpInt::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpInt::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpInt::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpInt::getTargets() const
{
	static std::set<u4> dummy;
	return dummy;
}

malloc_ptr<wchar_t> CJvmOpInt::getDisasmString(bool decodeCP, size_t tabSize) const
{
	wchar_t tmp[0x10];
	swprintf_s(tmp, L"%d", value);
	return wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), tmp, 0);
}

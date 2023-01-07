
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
#include "JvmOpOneByte.h"
#include "jdisasm.h"

CJvmOpOneByte::CJvmOpOneByte(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error)
{
	if ((j_opcode_error == opcode.op_type) ||
		(1 != opcode.size))
	{
		error = true;
		return;
	}
}

CJvmOpOneByte::~CJvmOpOneByte() {}

bool CJvmOpOneByte::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < 1) || error)
		return false;

	*mem = opcode.opcode;
	return true;
}

u4 CJvmOpOneByte::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpOneByte::getLen() const
{
	return opcode.size;
}

_jdisasm_opcodes CJvmOpOneByte::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpOneByte::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpOneByte::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpOneByte::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpOneByte::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpOneByte::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpOneByte::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpOneByte::getTargets() const
{
	static std::set<u4> dummy;
	return dummy;
}

malloc_ptr<wchar_t> CJvmOpOneByte::getDisasmString(bool decodeCP, size_t tabSize) const
{
	return malloc_ptr<wchar_t>(_wcsdup(opcode.name));
}

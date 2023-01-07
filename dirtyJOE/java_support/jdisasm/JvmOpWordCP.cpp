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
#include "JvmOpWordCP.h"
#include "../../common/common_templates.h"
#include "../helpers.h"
#include "jdisasm.h"

CJvmOpWordCP::CJvmOpWordCP(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error)
{
	if ((j_opcode_error == opcode.op_type) ||
		(maxLen < getSize()) ||
		((opcode.flag01 & (OP_WORD_CP | OP_BYTE_CP)) == 0))
	{
		error = true;
		return;
	}

	if (OP_WORD_CP == opcode.flag01)
	{
		cpIndex = BE2LEU2(*(u2*)(mem + 1));
		if ((j_opcode_invokeinterface == opcode.op_type) ||
			(j_opcode_multianewarray == opcode.op_type))
		{
			value = mem[3];
		}
	}
	else
		cpIndex = mem[1];
}

CJvmOpWordCP::~CJvmOpWordCP() {}

bool CJvmOpWordCP::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < getSize()) || error)
		return false;

	mem[0] = opcode.opcode;
	if (OP_WORD_CP == opcode.flag01)
	{
		*(u2*)(mem + 1) = BE2LEU2(cpIndex);
		switch (opcode.op_type)
		{
		case j_opcode_invokedynamic:
			*(u2*)(mem + 3) = 0;
			break;
		case j_opcode_invokeinterface:
			mem[3] = value;
			mem[4] = 0;
			break;
		case j_opcode_multianewarray:
			mem[3] = value;
			break;
		}

	}
	else
		mem[1] = (u1)cpIndex;

	return true;
}

u4 CJvmOpWordCP::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpWordCP::getLen() const
{
	return opcode.size;
}

_jdisasm_opcodes CJvmOpWordCP::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpWordCP::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpWordCP::getCPReference() const
{
	return cpIndex;
}

const wchar_t* const CJvmOpWordCP::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpWordCP::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpWordCP::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpWordCP::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpWordCP::getTargets() const
{
	static std::set<u4> dummy;
	return dummy;
}

malloc_ptr<wchar_t> CJvmOpWordCP::getDisasmString(bool decodeCP, size_t tabSize) const
{
	wchar_t* argStr;
	malloc_ptr<wchar_t> argStrSmart = genDisAsmStringFromCP(constant_pool, cpIndex);
	wchar_t tmpStr[0x40];
	if (nullptr == argStrSmart)
	{
		swprintf_s(tmpStr, L"cp.%04X", cpIndex);
		argStr = tmpStr;
	}
	else
		argStr = argStrSmart.get();

	switch (opcode.op_type)
	{
	case j_opcode_invokeinterface:
	case j_opcode_multianewarray:
	{
		wchar_t tmpStr2[0x10];
		swprintf_s(tmpStr2, L"%d", value);
		return wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), argStr, L", ", tmpStr2, 0);
	}
	default:
		return wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), argStr, 0);
	}
}

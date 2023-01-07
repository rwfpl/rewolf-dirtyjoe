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
#include "JvmOpAtype.h"
#include "../../common/common_templates.h"
#include "jdisasm.h"

CJvmOpAtype::CJvmOpAtype(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	opcode(maxLen != 0 ? opcodes[*mem] : jvm_opcode_error)
{
	if ((OP_ATYPE != opcode.flag01) ||
		(2 != opcode.size) ||
		(maxLen < getSize()))
	{
		error = true;
		return;
	}
	atype = mem[1];
}

CJvmOpAtype::~CJvmOpAtype() {}

bool CJvmOpAtype::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < getSize()) || error)
		return false;

	mem[0] = opcode.opcode;
	mem[1] = atype;
	return true;
}

u4 CJvmOpAtype::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpAtype::getLen() const
{
	return opcode.size;
}

_jdisasm_opcodes CJvmOpAtype::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpAtype::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpAtype::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpAtype::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpAtype::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpAtype::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpAtype::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpAtype::getTargets() const
{
	static std::set<u4> dummy;
	return dummy;
}

malloc_ptr<wchar_t> CJvmOpAtype::getDisasmString(bool decodeCP, size_t tabSize) const
{
	const wchar_t* const _atype_str[] =
	{
		L"boolean",
		L"char",
		L"float",
		L"double",
		L"byte",
		L"short",
		L"int",
		L"long"
	};
	wchar_t atp[0x20];
	if (atype >= 4 && atype <= 11)
		wcscpy_s(atp, _atype_str[atype - 4]);
	else
		swprintf_s(atp, L"unknown_atype_%d", atype);

	return wcscat_var(opcode.name, jdisasm_get_pad2(opcode.name, tabSize), atp, 0);
}

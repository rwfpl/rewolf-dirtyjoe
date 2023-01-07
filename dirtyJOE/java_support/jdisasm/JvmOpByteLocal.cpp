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
#include "JvmOpByteLocal.h"
#include "../../common/common_templates.h"
#include <string>
#include "jdisasm.h"

CJvmOpByteLocal::CJvmOpByteLocal(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log) :
	CDynamicStructWithCP(cp, log),
	wide(maxLen != 0 ? opcodes[*mem].op_type == j_opcode_wide : false),
	opcode(maxLen > 1 ? (wide ? opcodes[mem[1]] : opcodes[mem[0]]) : jvm_opcode_error),
	size(wide ? (j_opcode_iinc == opcode.op_type ? 6 : 3) : (j_opcode_iinc == opcode.op_type ? 4 : 2))
{
	if ((j_opcode_error == opcode.op_type) ||
		(maxLen < size) ||
		((opcode.flag01 & OP_BYTE_LOCAL) == 0))
	{
		error = true;
		return;
	}

	if (wide)
	{
		localIndex.u2 = BE2LEU2(*(u2*)(mem + 2));
		if (opcode.op_type == j_opcode_iinc)
		{
			iincValue = (su2)BE2LEU2(*(u2*)(mem + 2 + sizeof(u2)));
		}
	}
	else
	{
		localIndex.u1 = mem[1];
		if (opcode.op_type == j_opcode_iinc)
		{
			iincValue = (su1)mem[2];
		}
	}
}

CJvmOpByteLocal::~CJvmOpByteLocal() {}

bool CJvmOpByteLocal::storeData(unsigned char* mem, unsigned int maxSize)
{
	if ((maxSize < size) || error)
		return false;

	if (wide)
	{
		mem[0] = j_opcode_wide;
		mem[1] = opcode.opcode;
		*(u2*)(mem + 2) = BE2LEU2(localIndex.u2);
		if (opcode.op_type == j_opcode_iinc)
		{
			*(u2*)(mem + 2 + sizeof(u2)) = BE2LEU2((u2)iincValue);
		}
	}
	else
	{
		mem[0] = opcode.opcode;
		mem[1] = localIndex.u1;
		if (opcode.op_type == j_opcode_iinc)
		{
			mem[2] = (u1)iincValue;
		}
	}

	return true;
}

u4 CJvmOpByteLocal::getSize() const
{
	return (u4)getLen();
}

size_t CJvmOpByteLocal::getLen() const
{
	return size;
}

_jdisasm_opcodes CJvmOpByteLocal::getType() const
{
	return opcode.op_type;
}

_local_var_type CJvmOpByteLocal::getLocalVarType() const
{
	return opcode.local_var_type;
}

int CJvmOpByteLocal::getCPReference() const
{
	return -1;
}

const wchar_t* const CJvmOpByteLocal::getOpcodeName() const
{
	return opcode.name;
}

const char* const CJvmOpByteLocal::getSpecificationName() const
{
	return opcode.specName;
}

bool CJvmOpByteLocal::isFlow() const
{
	return opcode.is_flow;
}

bool CJvmOpByteLocal::isTerminator() const
{
	return opcode.is_terminator;
}

const std::set<u4>& CJvmOpByteLocal::getTargets() const
{
	static std::set<u4> dummy;
	return dummy;
}

malloc_ptr<wchar_t> CJvmOpByteLocal::getDisasmString(bool decodeCP, size_t tabSize) const
{
	std::wstring opName = L"";
	wchar_t tmpStr[0x40];
	if (wide)
	{
		opName = L"wide ";
		swprintf_s(tmpStr, L"local.%04X", localIndex.u2);
	}
	else
		swprintf_s(tmpStr, L"local.%02X", localIndex.u1);

	opName += opcode.name;
	if (opcode.op_type == j_opcode_iinc)
	{
		wchar_t tmpStr2[0x10];
		swprintf_s(tmpStr2, L"%d", iincValue);
		return wcscat_var(opName.c_str(), jdisasm_get_pad2(opName.c_str(), tabSize), tmpStr, L", ", tmpStr2, 0);
	}
	else
	{
		return wcscat_var(opName.c_str(), jdisasm_get_pad2(opName.c_str(), tabSize), tmpStr, 0);
	}
}

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
#pragma once
#include "JvmOpcode.h"
#include <vector>

class CJvmOpTableSwitch : public IJvmOpcode, public CDynamicStructWithCP
{
private:
	const _opcode_struct& opcode;
	u4 rawPos;
	su4 paddSize;
	su4 def;
	su4 low;
	su4 hi;
	std::vector<su4> labels;
	std::set<u4> m_targets;

public:
	CJvmOpTableSwitch(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log, u4 pos);
	virtual ~CJvmOpTableSwitch();
	virtual bool storeData(unsigned char* mem, unsigned int maxSize);
	virtual u4 getSize() const;

	//IJvmOpcode
	virtual size_t getLen() const;
	virtual _jdisasm_opcodes getType() const;
	virtual _local_var_type getLocalVarType() const;
	virtual int getCPReference() const;
	virtual const wchar_t* const getOpcodeName() const;
	virtual const char* const getSpecificationName() const;
	virtual malloc_ptr<wchar_t> getDisasmString(bool decodeCP, size_t tabSize) const;
	virtual bool isFlow() const;
	virtual bool isTerminator() const;
	virtual const std::set<u4>& getTargets() const;
};

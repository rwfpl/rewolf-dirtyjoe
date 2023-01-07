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
#include <set>
#include "jdisasm_enums.h"
#include "../CDynamicStructWithCP.h"
#include "../../common/common_macros.h"

class IJvmOpcode
{
	public:
		virtual size_t getLen() const = 0;
		virtual _jdisasm_opcodes getType() const = 0;
		virtual _local_var_type getLocalVarType() const = 0;
		virtual int getCPReference() const = 0;
		virtual const wchar_t* const getOpcodeName() const = 0;
		virtual const char* const getSpecificationName() const = 0;
		virtual bool isFlow() const = 0;
		virtual bool isTerminator() const = 0;
		virtual const std::set<u4>& getTargets() const = 0;
		virtual malloc_ptr<wchar_t> getDisasmString(bool decodeCP, size_t tabSize = 20) const = 0;
		virtual ~IJvmOpcode() {};
};

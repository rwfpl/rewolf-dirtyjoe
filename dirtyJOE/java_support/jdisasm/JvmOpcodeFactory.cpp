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
#include "JvmOpcodeFactory.h"
#include "JvmOpAtype.h"
#include "JvmOpByteLocal.h"
#include "JvmOpFlow.h"
#include "JvmOpInt.h"
#include "JvmOpLookupSwitch.h"
#include "JvmOpOneByte.h"
#include "JvmOpTableSwitch.h"
#include "JvmOpWordCP.h"
#include "jdisasm.h"

IJvmOpcode* CJvmOpcodeFactory::makeOpcode(u1* mem, u4 maxLen, u4 pos, const JavaConstPool& cp, CLogger& log)
{
	if (0 == maxLen)
		return nullptr;
	
	IJvmOpcode* ret = nullptr;
	switch (opcodes[mem[0]].op_type_class)
	{
		case JvmOpTypeClass::JvmOpAtype: ret = new (std::nothrow) CJvmOpAtype(mem, maxLen, cp, log); break;
		case JvmOpTypeClass::JvmOpByteLocal: ret = new (std::nothrow) CJvmOpByteLocal(mem, maxLen, cp, log); break;
		case JvmOpTypeClass::JvmOpFlow: ret = new (std::nothrow) CJvmOpFlow(mem, maxLen, cp, log, pos); break;
		case JvmOpTypeClass::JvmOpInt: ret = new (std::nothrow) CJvmOpInt(mem, maxLen, cp, log); break;
		case JvmOpTypeClass::JvmOpLookupSwitch: ret = new (std::nothrow) CJvmOpLookupSwitch(mem, maxLen, cp, log, pos); break;
		case JvmOpTypeClass::JvmOpOneByte: ret = new (std::nothrow) CJvmOpOneByte(mem, maxLen, cp, log); break;
		case JvmOpTypeClass::JvmOpTableSwitch: ret = new (std::nothrow) CJvmOpTableSwitch(mem, maxLen, cp, log, pos); break;
		case JvmOpTypeClass::JvmOpWordCP: ret = new (std::nothrow) CJvmOpWordCP(mem, maxLen, cp, log); break;
	}

	if ((nullptr != ret) && dynamic_cast<CDynamicStructWithCP*>(ret)->getError())
	{
		delete ret;
		ret = nullptr;
	}
	return ret;
}

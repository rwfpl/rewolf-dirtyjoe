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

class CJvmOpcodeFactory
{
public:
	static IJvmOpcode* makeOpcode(u1* mem, u4 maxLen, u4 pos, const JavaConstPool& cp, CLogger& log);
};
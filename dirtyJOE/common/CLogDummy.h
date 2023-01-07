/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2008 ReWolf
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

#include "CLogger.h"
#include <cstdio>

class CLogDummy : public CLogger
{
public:
	void log(int level, ...) {}
	CLogDummy() { showLevel = 0; }
};

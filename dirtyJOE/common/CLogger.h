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

#include <stdarg.h>

class CLogger
{
protected:
	/*
	0 - no logs at all ;>
	1 - highest priority logs (errors)
	...
	*/
	int showLevel;

public:
	void setLevel(int lvl) { showLevel = lvl; }
	void logFunctionStart(char* funcName) { log(10, "function %s start\n", funcName); }
	void logFunctionEnd(char* funcName) { log(10, "function %s end\n", funcName); }
	virtual void log(int level, ...) = 0;
	virtual ~CLogger() {}
};

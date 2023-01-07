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

class CDefClass
{
protected:
	CLogger& logger;
public:
	CDefClass(CLogger& log) : logger(log) {}
	virtual ~CDefClass() {}

private:
	CDefClass();
	CDefClass& operator=(const CDefClass&);
};

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

#include "../java_support/java_types.h"
#include "CDefClass.h"

class CDynamicStruct : public CDefClass
{
protected:
	bool error;
public:
	CDynamicStruct(CLogger& log) : CDefClass(log), error(false) {}
	bool getError() const { return error; }
	virtual bool storeData(u1* mem, u4 maxSize) = 0;
	virtual u4 getSize() const = 0;
	virtual ~CDynamicStruct() {}
private:
	CDynamicStruct();
};

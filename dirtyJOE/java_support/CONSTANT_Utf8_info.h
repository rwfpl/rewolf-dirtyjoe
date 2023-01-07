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

#include "cp_info.h"
#include "../common/common_macros.h"
#include <vector>

class CONSTANT_Utf8_info : public cp_info
{
private:
	//u2 length;
	GETTER_PTR(std::vector<u1>, bytes);

public:
	u4 getSize() const;
	bool storeData(u1* mem, u4 maxSize);
	CONSTANT_Utf8_info(CLogger& log);
	CONSTANT_Utf8_info(u1* mem, u4 maxLen, CLogger& log);
	CONSTANT_Utf8_info(const wchar_t* wstr, CLogger& log);
	virtual ~CONSTANT_Utf8_info();
};

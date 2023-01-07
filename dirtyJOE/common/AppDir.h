/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
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
#include "common_macros.h"

class CAppDir
{
	private:
		malloc_ptr<wchar_t> cd;
		bool error;

	public:
		CAppDir();
		~CAppDir();
		bool getError() const;
};

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
#include "../common/common_macros.h"

namespace shellExt
{
	enum checkShellExtRet
	{
		Check_ShellExt_Error = 0,
		Check_ShellExt_Ok,
		Check_ShellExt_Keep
	};
	malloc_ptr<wchar_t> getClassFileSubKey(bool command = true);
	checkShellExtRet checkShellExtension(HWND hWnd = 0, bool askIfExist = true);
	void delShellExtension(HWND hWnd, bool silent = false);
	void addShellExtension(HWND hWnd, bool silent = false);
	malloc_ptr<wchar_t> getAppNameString();
};

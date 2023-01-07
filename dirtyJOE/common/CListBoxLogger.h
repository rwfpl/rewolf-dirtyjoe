/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2013 ReWolf
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
#include <windows.h>

class CListBoxLogger : public CLogger
{
private:
	const HWND listBox;

public:
	void log(int level, ...)
	{
		if (level <= showLevel)
		{
			char tempBuf[0x200];
			tempBuf[0] = 0;
			va_list argptr;
			va_start(argptr, level);
			vsprintf_s(tempBuf, va_arg(argptr, char*), argptr);
			SendMessageA(listBox, WM_SETREDRAW, FALSE, 0);
			if ('\r' == tempBuf[0])
			{
				LRESULT last = SendMessageA(listBox, LB_GETCOUNT, 0, 0) - 1;
				SendMessageA(listBox, LB_DELETESTRING, last, 0);
				SendMessageA(listBox, LB_ADDSTRING, 0, (LPARAM)tempBuf);
			}
			else
			{
				LRESULT idx = SendMessageA(listBox, LB_ADDSTRING, 0, (LPARAM)tempBuf);
				SendMessageA(listBox, LB_SETTOPINDEX, (WPARAM)idx, 0);
			}
			SendMessageA(listBox, WM_SETREDRAW, TRUE, 0);
			RedrawWindow(listBox, 0, 0, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
		}
	}

	CListBoxLogger(HWND hListBox) : listBox(hListBox) { showLevel = 1; }
};

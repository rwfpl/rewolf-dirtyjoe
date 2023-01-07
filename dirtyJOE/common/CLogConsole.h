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
#include <windows.h>

class CLogConsole : public CLogger
{
private:
	HANDLE hCon;
	malloc_ptr<char> spaces;
	size_t maxLen;

public:
	void log(int level, ...)
	{
		if ((INVALID_HANDLE_VALUE == hCon) || (0 == hCon))
			return;

		if (level <= showLevel)
		{
			char tempBuf[512];
			va_list argptr;
			va_start(argptr, level);
			vsprintf_s(tempBuf, va_arg(argptr, char*), argptr);
			DWORD dummy;
			size_t tl = strlen(tempBuf);
			if (tl > maxLen)
				tempBuf[maxLen - 1] = tempBuf[tl - 1];
			WriteConsoleA(hCon, tempBuf, (DWORD)((tl > maxLen) ? maxLen : tl), &dummy, 0);
			if (('\r' == tempBuf[0]) && (nullptr != spaces))
			{
				CONSOLE_SCREEN_BUFFER_INFO conInfo;
				GetConsoleScreenBufferInfo(hCon, &conInfo);
				if (conInfo.dwCursorPosition.X < conInfo.dwSize.X)
					WriteConsoleA(hCon, spaces.get(), conInfo.dwSize.X - conInfo.dwCursorPosition.X - 1, &dummy, 0);
			}
		}
	}

	CLogConsole() 
	{ 
		showLevel = 100;
		AttachConsole((DWORD)-1);
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		if ((INVALID_HANDLE_VALUE == hCon) || (0 == hCon))
			return;

		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		GetConsoleScreenBufferInfo(hCon, &conInfo);
		maxLen = conInfo.dwSize.X - 1;
		spaces = malloc<char>(conInfo.dwSize.X);
		if (nullptr != spaces)
			memset(spaces.get(), ' ', conInfo.dwSize.X);
	}

	virtual ~CLogConsole()
	{
		FreeConsole();
	}
};

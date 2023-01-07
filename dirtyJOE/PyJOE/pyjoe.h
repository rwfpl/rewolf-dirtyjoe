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
#include <Windows.h>

typedef bool (WINAPI *__pyjoe_Init)(HMODULE hPythonDll);
typedef void (WINAPI *__pyjoe_Deinit)();
typedef void (WINAPI *__pyjoe_FreeBuffer)(BYTE* buf);
typedef bool (WINAPI *__pyjoe_GetError)(char** outBuf);
typedef BYTE* (WINAPI *__pyjoe_DecryptBuffer)(char* script, char* funcName, BYTE* inBuf, DWORD inBufSize, DWORD* retSize, wchar_t** errStr);

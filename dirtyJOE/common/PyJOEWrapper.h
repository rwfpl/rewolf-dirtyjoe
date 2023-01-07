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
#include "../PyJOE/pyjoe.h"

class PyJOEWrapper
{
	private:
		__pyjoe_Init pyjoe_Init;
		__pyjoe_Deinit pyjoe_Deinit;
		__pyjoe_DecryptBuffer pyjoe_DecryptBuffer;
		__pyjoe_FreeBuffer pyjoe_FreeBuffer;
		__pyjoe_GetError pyjoe_GetError;
		bool error;

	public:
		PyJOEWrapper(HMODULE hModule);
		bool Init(HMODULE hPythonDll);
		void Deinit();
		bool SetNewModule(HMODULE hModule);
		bool GetError(char** outBuf);
		void FreeBuffer(BYTE* buf);
		BYTE* DecryptBuffer(char* script, char* funcName, BYTE* inBuf, DWORD inBufSize, DWORD* retSize, wchar_t** errStr);
		bool isError() const;
};

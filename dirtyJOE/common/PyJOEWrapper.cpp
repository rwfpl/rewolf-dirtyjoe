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
#include "PyJOEWrapper.h"

PyJOEWrapper::PyJOEWrapper(HMODULE hModule)
{
	SetNewModule(hModule);
}

bool PyJOEWrapper::SetNewModule(HMODULE hModule)
{
#define RESOLVE_FUNC(f) f = (__##f)GetProcAddress(hModule, #f)
	RESOLVE_FUNC(pyjoe_FreeBuffer);
	RESOLVE_FUNC(pyjoe_DecryptBuffer);
	RESOLVE_FUNC(pyjoe_GetError);
	RESOLVE_FUNC(pyjoe_Init);
	RESOLVE_FUNC(pyjoe_Deinit);
#undef RESOLVE_FUNC

	if ((nullptr == pyjoe_GetError) ||
		(nullptr == pyjoe_FreeBuffer) ||
		(nullptr == pyjoe_DecryptBuffer) ||
		(nullptr == pyjoe_Init) ||
		(nullptr == pyjoe_Deinit))
	{

		pyjoe_GetError = nullptr;
		pyjoe_FreeBuffer = nullptr;
		pyjoe_DecryptBuffer = nullptr;
		pyjoe_Init = nullptr;
		pyjoe_Deinit = nullptr;
		error = true;
		return false;
	}

	error = false;
	return true;
}

bool PyJOEWrapper::Init(HMODULE hPythonDll)
{
	if (nullptr != pyjoe_Init)
		return pyjoe_Init(hPythonDll);
	return false;
}

void PyJOEWrapper::Deinit()
{
	if (nullptr != pyjoe_Deinit)
		pyjoe_Deinit();
}

void PyJOEWrapper::FreeBuffer(BYTE* buf)
{
	if (nullptr != pyjoe_FreeBuffer)
		pyjoe_FreeBuffer(buf);
}

bool PyJOEWrapper::GetError(char** outBuf)
{
	if (nullptr != pyjoe_GetError)
		return pyjoe_GetError(outBuf);
	return false;
}

BYTE* PyJOEWrapper::DecryptBuffer(char* script, char* funcName, BYTE* inBuf, DWORD inBufSize, DWORD* retSize, wchar_t** errStr)
{
	if (nullptr != pyjoe_DecryptBuffer)
		return pyjoe_DecryptBuffer(script, funcName, inBuf, inBufSize, retSize, errStr);
	return 0;
}

bool PyJOEWrapper::isError() const
{
	return error;
}

/*
* dirtyJOE - Java Overall Editor
* Copyright 2010 ReWolf
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

#include <map>
#include "../java_support/CClassFile.h"
#include "../common/CLogger.h"
#include "../common/JVMSpec.h"
#include "resource.h"
#include "pre_wtl.h"
#include "CDJSettings.h"

class CDlgMain;

class CDirtyJOE
{
	public:
	CDirtyJOE() : clf(nullptr), logptr(nullptr), opHelper(0), funcName(nullptr)
	{
		#ifdef DJ_TOTALCMD
		hInstance = GetModuleHandle(L"TotalJOE.wlx");
		#else
		hInstance = GetModuleHandle(0);
		#endif
	}

	CClassFile* clf;
	CDJSettings settings;
	CJVMSpec jvmspec;
	CLogger* logptr;
	HINSTANCE hInstance;

	CDlgMain* dlgMain;

	HWND opHelper;

	// hack for dlgCode, so it can show current function name
	wchar_t* funcName;
};

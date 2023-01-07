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
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <map>
#include "../ui/pre_wtl.h"
#include "../ui/dlgMain.h"
#include "../ui/CDirtyJOE.h"
#include "../common/CLogDummy.h"
#include "../common/common_macros.h"

struct djTotalCtx
{
	CDirtyJOE* dj;
	CDlgMain* dlg;
};

CAppModule _Module;
std::map<HWND, djTotalCtx> wnds;

void totalCmdUpdateCheck(CDirtyJOE& dj)
{
	static bool plugin_initialized = false;
	if (!plugin_initialized)
	{
		if (dj.settings.get_StartupUpdateCheck())
			checkForUpdate(0, true);

		plugin_initialized = true;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (DLL_PROCESS_ATTACH == fdwReason)
	{
		DefWindowProc(NULL, 0, 0, 0);
		AtlInitCommonControls(ICC_LINK_CLASS | ICC_PAGESCROLLER_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES);

		if (FAILED(_Module.Init(NULL, hinstDLL)))
			return FALSE;

		appendScriptsPath();
	}
	else if (DLL_PROCESS_DETACH == fdwReason)
	{
		_Module.Term();
	}
	return TRUE;
}

#pragma comment(linker, "/export:ListLoadW@12=ListLoadW")
extern "C" HWND __stdcall ListLoadW(HWND ParentWin, wchar_t* FileToLoad, int ShowFlags)
{
	djTotalCtx ctx;
	ctx.dj = new (std::nothrow) CDirtyJOE;
	if (ctx.dj == 0)
		return 0;

	if (ctx.dj->settings.getError())
	{
		delete ctx.dj;
		return 0;
	}

	ctx.dj->logptr = new (std::nothrow) CLogDummy();
	if (ctx.dj->logptr == 0)
	{
		delete ctx.dj;
		return 0;
	}

	ctx.dj->logptr->setLevel(1);

	ctx.dj->clf = new (std::nothrow) CClassFile(*ctx.dj->logptr);
	if (ctx.dj->clf == 0)
	{
		delete ctx.dj->logptr;
		delete ctx.dj;
		return 0;
	}

	malloc_ptr<wchar_t> fCanonName = CanonicalizePath(FileToLoad);
	if (!ctx.dj->clf->LoadFile(fCanonName.get()))
	{
		delete ctx.dj->clf;
		delete ctx.dj->logptr;
		delete ctx.dj;
		return 0;
	}

	totalCmdUpdateCheck(*ctx.dj);

	ctx.dlg = new (std::nothrow) CDlgMain(*ctx.dj);
	if (ctx.dlg == 0)
	{
		delete ctx.dj->clf;
		delete ctx.dj->logptr;
		delete ctx.dj;
		return 0;
	}
	
	ctx.dlg->Create(ParentWin);
	ctx.dlg->ShowWindow(SW_SHOW);

	HWND rt = *ctx.dlg;
	wnds[rt] = ctx;
	return rt;
}

#pragma comment(linker, "/export:ListLoad@12=ListLoad")
extern "C" HWND __stdcall ListLoad(HWND ParentWin, char* FileToLoad, int ShowFlags)
{
	int ws = MultiByteToWideChar(CP_ACP, 0, FileToLoad, -1, 0, 0);
	if (ws == 0)
		return 0;
	malloc_ptr<wchar_t> wsFileToLoad = malloc<wchar_t>(ws);
	if (nullptr == wsFileToLoad)
		return 0;

	if (MultiByteToWideChar(CP_ACP, 0, FileToLoad, -1, wsFileToLoad.get(), ws) == 0)
		return 0;

	return ListLoadW(ParentWin, wsFileToLoad.get(), ShowFlags);
}

#pragma comment(linker, "/export:ListCloseWindow@4=ListCloseWindow")
extern "C" void __stdcall ListCloseWindow(HWND ListWin)
{
	auto crw = wnds.find(ListWin);
	if (crw == wnds.end())
	{
		//this probably shouldn't happen but I'll handle it anyway
		DestroyWindow(ListWin);
	}
	else
	{
		EndDialog(crw->second.dlg->opHelp, 0);
		DestroyWindow(crw->second.dlg->opHelp);
		crw->second.dlg->SendMessage(WM_CLOSE, 0, 0);
		crw->second.dlg->DestroyWindow();
		crw->second.dj->settings.saveSettings();
		delete crw->second.dlg;
		delete crw->second.dj->clf;
		delete crw->second.dj->logptr;
		delete crw->second.dj;
		wnds.erase(crw);
	}
}

#pragma comment(linker, "/export:ListGetDetectString@8=ListGetDetectString")
extern "C" void __stdcall ListGetDetectString(char* DetectString, int maxlen)
{
	strcpy_s(DetectString, maxlen, "EXT=\"CLASS\"");
}

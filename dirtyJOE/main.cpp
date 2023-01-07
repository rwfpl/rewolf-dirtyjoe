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

#ifdef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#endif

#include <cstdio>
#include <windows.h>
#include <CommCtrl.h>

#include "ui/pre_wtl.h"
#include "ui/dlgMain.h"
#include "ui/dlgOpcodesHelp.h"
#include "ui/CDirtyJOE.h"
#include "ui/ui_helpers.h"
#include "ui/CDJSettings.h"
#include "ui/shellext.h"

#include "common/CLogDummy.h"
#include "common/CLogConsole.h"
#include "common/common_templates.h"

#include "java_support/CClassFile.h"
#include "java_support/RestoreDebugInfo.h"

/*#if 0
#include "java_support/hlp_references.h"
#include "java_support/helpers.h"
#endif*/

CAppModule _Module;

void batchRestoreDebugInfo(wchar_t* classFileName)
{
	CLogDummy logger;
	CLogConsole logCon;
	CClassFile classFile(logger);

	logCon.log(1, "Restoring Debug Info for: %ws...\n", classFileName ? classFileName : L"unknown file");

	malloc_ptr<wchar_t> fName = CanonicalizePath(classFileName);
	if ((nullptr == fName) || (!classFile.LoadFile(fName.get())))
	{
		logCon.log(1, "Can't load requested file: %ws\n", classFileName ? classFileName : L"unknown file");
		return;
	}

	wchar_t fileNameNoExt[_MAX_FNAME];
	wchar_t fileNameDrive[_MAX_DRIVE];
	wchar_t fileNameDir[_MAX_DIR];
	wchar_t fileNameExt[_MAX_EXT];
	malloc_ptr<wchar_t> linesFileName = nullptr;
	if ((0 != _wsplitpath_s(classFile.GetFileName(), fileNameDrive, fileNameDir, fileNameNoExt, fileNameExt)) ||
		(nullptr == (linesFileName = wcscat_var(fileNameDrive, fileNameDir, fileNameNoExt, L".joe", 0))))
	{
		badAllocMsg(logCon);
		return;
	}

	if (!RestoreLineNumbers(classFile, linesFileName.get(), logger, logCon))
	{
		logCon.log(1, "Failed to restore Line Numbers. Aborting.\n");
		return;
	}
	if (!RestoreLocalVariables(classFile, logger, logCon))
	{
		logCon.log(1, "Failed to restore Local Variables. Aborting.\n");
		return;
	}

	if (!classFile.SaveFile(classFile.GetFileName()))
	{
		logCon.log(1, "Can't save output class file.\n");
		return;
	}

	logCon.log(1, "Debug Info restored successfully.\n");
}

void showOnlyOpcodesHelp()
{
	CDirtyJOE dj;

	if (dj.jvmspec.getError())
		MessageBox(0, L"jvmspec.dat file is either missing or corrupted.", L"Error", MB_ICONERROR);

	DefWindowProc(NULL, 0, 0, 0);
	AtlInitCommonControls(ICC_LINK_CLASS | ICC_PAGESCROLLER_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES);

	if (FAILED(_Module.Init(NULL, dj.hInstance)))
	{
		MessageBox(0, L"Initialization failed.", L"Error", MB_ICONERROR);
		return;
	}
	else
	{
		CDlgOpcodesHelp(dj, true).DoModal();
		_Module.Term();
	}
}

int wmain(int argc, wchar_t* argv[])
{
	if ((argc >= 2) && (0 == wcscmp(L"/rdi", argv[1])))
	{
		batchRestoreDebugInfo(argc != 2 ? argv[2] : L"");
		return 0;
	}
	else if ((argc >= 2) && (0 == wcscmp(L"/opcodes", argv[1])))
	{
		showOnlyOpcodesHelp();
		return 0;
	}

	appendScriptsPath();
	
	CDirtyJOE dj;
	if (dj.settings.getError())
	{
		MessageBox(0, L"Settings initialization failed.", L"Error", MB_ICONERROR);
		return 0;
	}

	if (dj.jvmspec.getError())
		MessageBox(0, L"jvmspec.dat file is either missing or corrupted.", L"Error", MB_ICONERROR);

	shellExt::checkShellExtRet shExist = shellExt::checkShellExtension(0, !dj.settings.shellExtAsked);
	if (dj.settings.get_ShellExtension())
	{
		if (shellExt::Check_ShellExt_Error == shExist)
			shellExt::addShellExtension(0, true);
	}
	else
	{
		if (shellExt::Check_ShellExt_Ok == shExist)
			shellExt::delShellExtension(0, true);
	}

	#ifdef _DEBUG
	dj.logptr = new (std::nothrow) CLogConsole();
	#else
	dj.logptr = new (std::nothrow) CLogDummy();
	#endif
	if (dj.logptr == 0)
	{
		MessageBox(0, L"Unable to create logging interface.", L"Error", MB_ICONERROR);
		return 0;
	}
	dj.logptr->setLevel(1);

	if (argc >= 2)
	{
		dj.clf = new (std::nothrow) CClassFile(*dj.logptr);
		if (dj.clf != 0)
		{
			malloc_ptr<wchar_t> fName = CanonicalizePath(argv[1]);
			if ((nullptr == fName) || (!dj.clf->LoadFile(fName.get())))
			{
				const size_t errSz = (fName) ? wcslen(fName.get()) + 0x40 : 0;
				malloc_ptr<wchar_t> errMsg = malloc<wchar_t>(errSz);
				if (errMsg == 0)
				{
					MessageBox(0, L"Can't load given .class file.", L"Error", MB_ICONERROR);
				}
				else
				{
					errMsg.get()[0] = 0;
					swprintf(errMsg.get(), errSz, L"Can't load %ws file.", fName.get());
					MessageBox(0, errMsg.get(), L"Error", MB_ICONERROR);
				}
				delete dj.clf;
				dj.clf = 0;
			}
			else
				dj.settings.addRecentFile(dj.clf->GetFileName());
		}
	}

	/*#if 0
	if (dj.clf)
	{
		for (u4 i = 1; i < dj.clf->get_constant_pool().size(); i++)
		{
			std::vector<wchar_t*>* test = getAllRefs(*dj.clf, (u2)i);
			if (test)
			{
				//if (test->size() == 0)
				//{
				//	cp_info* ccp = dj.clf->get_constant_pool()[i];
				//	cp_info* pcp = dj.clf->get_constant_pool()[i - 1];
				//	if (!(((ccp->tag == CONSTANT_Double) && (pcp->tag == CONSTANT_Double)) || 
				//		((ccp->tag == CONSTANT_Long) && (pcp->tag == CONSTANT_Long))))
				//	{
				//		dj.logptr->log(1, "no refs: %d\n", i);
				//	}
				//}
				for (u4 j = 0; j < test->size(); j++)
				{
					wchar_t* ot = parseReference(*dj.clf, (*test)[j]);
					if (ot)
					{
						dj.logptr->log(1, "%ws\n", ot);
						free(ot);
					}
				}
			}
			cleanUpObjects(*test);
			delete test;
		}
	}
	#endif*/

	static const wchar_t strInitFailed[] = L"Initialization failed.";

	//Need to call CoInitialize() to support URLDownloadToFile() on windows xp sp2
	if (FAILED(CoInitialize(0)))
	{
		if (dj.clf != 0)
			delete dj.clf;
		if (dj.logptr != 0)
			delete dj.logptr;
		MessageBox(0, strInitFailed, L"Error", MB_ICONERROR);
		return 0;
	}

	DefWindowProc(NULL, 0, 0, 0);
	AtlInitCommonControls(ICC_LINK_CLASS | ICC_PAGESCROLLER_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES);
	
	if (FAILED(_Module.Init(NULL, dj.hInstance)))
	{
		if (dj.clf != 0)
			delete dj.clf;
		if (dj.logptr != 0)
			delete dj.logptr;
		CoUninitialize();
		MessageBox(0, strInitFailed, L"Error", MB_ICONERROR);
		return 0;
	}

	//everything should be initialized now, so we can check for update
	if (dj.settings.get_StartupUpdateCheck())
		checkForUpdate(0, true);

	{
		//dummy block to prevent using object after _Module.Term()
		CDlgMain(dj).DoModal();
	}

	dj.settings.saveSettings();

	if (dj.clf != 0)
		delete dj.clf;
	if (dj.logptr != 0)
		delete dj.logptr;

	_Module.Term();
	CoUninitialize();

	return 0;
}

#ifndef _DEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = 0;
	int ret = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (argv)
	{
		ret = wmain(argc, argv);
		LocalFree(argv);
	}
	return ret;
}
#endif

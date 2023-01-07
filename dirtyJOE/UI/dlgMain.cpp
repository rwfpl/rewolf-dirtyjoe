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

#include <Windows.h>
#include <CommCtrl.h>
#include "dlgMain.h"
#include "../common/common.h"
#include "../common/AppDir.h"
#include "version_info.h"
#include "shellext.h"
#include "ui_helpers.h"
#include "dlgAbout.h"
#include "dlgSettings.h"
#include "dlgRestoreDebugInfo.h"
#include "../java_support/helpers.h"
#include "../java_support/jdisasm/jdisasm.h"

HWND CDlgMain::m_hWnd_static;
HHOOK CDlgMain::m_MainMsgHook_static;
HACCEL CDlgMain::m_hAccel_static;

CDlgMain::CDlgMain(CDirtyJOE& _dj) : 
	dj(_dj), 
	tabOverView(_dj, *this), 
	tabConstantPool(_dj, *this), 
	tabFields(_dj, *this, FIELDS), 
	tabMethods(_dj, *this, METHODS),
	opHelp(_dj),
	lastFileName(0), lastIsModified(false),
	stickyWnd(m_hWnd),
	dlgCurrentCodeWindow(0)
{
	tabsTab[0] = TABSTRUCT(L"Overview", 0);
	tabsTab[1] = TABSTRUCT(L"Constant Pool", 0);
	tabsTab[2] = TABSTRUCT(L"Fields", 0);
	tabsTab[3] = TABSTRUCT(L"Methods", 0);
}

void CDlgMain::updateTitle()
{
	bool b_setTitle = false;
	if (dj.clf)
	{
		const wchar_t* currentFileName = dj.clf->GetFileName();
		const bool currentIsModified = dj.clf->isModified();
		if ((lastFileName == currentFileName) && (lastIsModified == currentIsModified))
			return;

		malloc_ptr<wchar_t> title = wcscat_var(L"dirtyJOE - " DJ_VERSION_SHORT_W L" - ", currentFileName, currentIsModified ? L" - *modified" : 0, 0);
		if (nullptr != title)
		{
			SetWindowText(title.get());
			b_setTitle = true;
		}
	}
	if (!b_setTitle)
		SetWindowText(L"dirtyJOE - " DJ_VERSION_SHORT_W);
}

void CDlgMain::shellExtMenuUpdate(HMENU hMenu)
{
	if (shellExt::Check_ShellExt_Ok == shellExt::checkShellExtension(m_hWnd, false))
	{
		EnableMenuItem(hMenu, MENU_OPTIONS_REGISTERSHELLEXTENSION, MF_GRAYED);
		EnableMenuItem(hMenu, MENU_OPTIONS_UNREGISTERSHELLEXTENSION, MF_ENABLED);
	}
	else
	{
		EnableMenuItem(hMenu, MENU_OPTIONS_UNREGISTERSHELLEXTENSION, MF_GRAYED);
		EnableMenuItem(hMenu, MENU_OPTIONS_REGISTERSHELLEXTENSION, MF_ENABLED);
	}
}

void CDlgMain::addTabControl(DWORD num, wchar_t* name)
{
	TCITEM itm;
	memset(&itm, 0, sizeof(TCITEM));
	itm.mask = TCIF_TEXT;
	itm.pszText = name;
	TabCtrl_InsertItem(GetDlgItem(DJ_MAIN_TAB), num, &itm);
}

void CDlgMain::updateRecentMenu()
{
#ifndef DJ_TOTALCMD
	HMENU recentMenu = GetSubMenu(GetSubMenu(GetMenu(), 0), 3);

	// delete existing items
	int itemCnt = GetMenuItemCount(recentMenu);
	if (itemCnt > 1)
	{
		for (int i = 1; i < itemCnt; i++)
			DeleteMenu(recentMenu, 1, MF_BYPOSITION);
	}

	// add recent files
	for (int i = 0; i < dj.settings.getMaxRecentFiles(); i++)
	{
		UINT_PTR ttt = 0xC000 + i;
		if (nullptr != dj.settings.getRecentFile(i))
			AppendMenu(recentMenu, MF_ENABLED | MF_STRING, ttt, dj.settings.getRecentFile(i));
	}
#endif
}

LRESULT CDlgMain::OnMenu_RecentFiles(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	savePrompt();
	loadFile(dj.settings.getRecentFile(wID & 0xF));
	return 0;
}

LRESULT CALLBACK CDlgMain::GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	if ((HC_ACTION == code) &&
		(PM_REMOVE == wParam) &&
		(0 != lParam))
	{
		if (TranslateAccelerator(m_hWnd_static, m_hAccel_static, (MSG*)lParam))
		{
			((MSG*)lParam)->message = WM_NULL;
		}
	}

	return CallNextHookEx(m_MainMsgHook_static, code, wParam, lParam);
}

LRESULT CDlgMain::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#ifndef DJ_TOTALCMD

	updateTitle();

	HICON ico = AtlLoadIcon(MAKEINTRESOURCE(IDI_ICON1));
	if (ico)
	{
		SetIcon(ico, TRUE);
		SetIcon(ico, FALSE);
	}

	HMENU mainMenu = GetMenu();
	if (dj.clf == 0)
	{
		EnableMenuItem(mainMenu, MENU_FILE_SAVE, MF_GRAYED);
	}

	shellExtMenuUpdate(mainMenu);
	DragAcceptFiles();
	updateRecentMenu();

	m_hAccel_static = LoadAccelerators(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(ACCEL_DLGMAIN));
	m_hWnd_static = m_hWnd;
	m_MainMsgHook_static = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, 0, GetCurrentThreadId());

#endif

	HWND tabHandle = GetDlgItem(DJ_MAIN_TAB);
	
	tabsTab[0].wnd = &tabOverView;
	tabOverView.Create(tabHandle);
	
	tabsTab[1].wnd = &tabConstantPool;
	tabConstantPool.Create(tabHandle);

	tabsTab[2].wnd = &tabFields;
	tabFields.Create(tabHandle);

	tabsTab[3].wnd = &tabMethods;
	tabMethods.Create(tabHandle);

	for (int i = 0; i < TABCNT; i++)
	{
		addTabControl(i, tabsTab[i].name);
		::SendMessage(*tabsTab[i].wnd, WM_UPDATE_TAB, 0, 0);
	}

	dj.dlgMain = this;
	opHelp.Create(m_hWnd);

	::ShowWindow(tabOverView, SW_SHOW);

	return TRUE;
}

LRESULT CDlgMain::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DWORD nameSize = DragQueryFile((HDROP)wParam, 0, 0, 0);
	if (0 == nameSize)
		return 0;

	malloc_ptr<wchar_t> fileName = malloc<wchar_t>(nameSize + 1);
	if (0 == fileName)
		return 0;

	if (0 == DragQueryFile((HDROP)wParam, 0, fileName.get(), nameSize + 1))
		return 0;

	savePrompt();
	loadFile(fileName.get());
	DragFinish((HDROP)wParam);
	return 0;
}

void CDlgMain::savePrompt()
{
	if (dj.clf && dj.clf->isModified())
	{
		if (IDYES == MessageBox(L"File has been modified, do you want to save changes ?", L"dirtyJOE - Save ?", MB_ICONQUESTION | MB_YESNO))
			saveFile();
	}
}

LRESULT CDlgMain::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	updateTitle();
	savePrompt();

	for (int i = 0; i < TABCNT; i++) 
		::EndDialog(*tabsTab[i].wnd, 0);

	::EndDialog(opHelp, 0);

	if (0 != m_MainMsgHook_static)
		UnhookWindowsHookEx(m_MainMsgHook_static);

	if (0 != m_hAccel_static)
		DestroyAcceleratorTable(m_hAccel_static);

	EndDialog(0);
	return TRUE;
}

LRESULT CDlgMain::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	#ifdef DJ_TOTALCMD
	int _top = 25;
	::MoveWindow(GetDlgItem(BTN_TCMD_OPTIONS), 0, 0, LOWORD(lParam), _top, TRUE);
	#else
	int _top = 0;
	#endif
	HWND tabHandle = GetDlgItem(DJ_MAIN_TAB);
	::MoveWindow(tabHandle, 0, _top, LOWORD(lParam), HIWORD(lParam) - _top, TRUE);
	RECT crect;
	::GetClientRect(tabHandle, &crect);
	TabCtrl_AdjustRect(tabHandle, FALSE, &crect);
	for (int i = 0; i < TABCNT; i++)
	{
		::MoveWindow(*tabsTab[i].wnd, crect.left + 4, crect.top + 4, crect.right - crect.left - 8, crect.bottom - crect.top - 8, 0);
	}
	return FALSE;
}

LRESULT CDlgMain::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 750) 
		rptr->right = rptr->left + 750;
	if (rptr->bottom - rptr->top < 520) 
		rptr->bottom = rptr->top + 520;
	return TRUE;
}

LRESULT CDlgMain::OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	stickyWnd.stickyUpdate(*(RECT*)lParam);
	return TRUE;
}

LRESULT CDlgMain::OnWindowMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (0 != dlgCurrentCodeWindow)
		::SendMessage(dlgCurrentCodeWindow->m_hWnd, uMsg, wParam, lParam);

	stickyWnd.stickTry((HWND)wParam, *(RECT*)lParam);
	bHandled = TRUE;
	return TRUE;
}

LRESULT CDlgMain::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	int crt = TabCtrl_GetCurSel(GetDlgItem(DJ_MAIN_TAB));
	if (crt != -1)
	{
		::InvalidateRect(*tabsTab[crt].wnd, 0, TRUE);
	}
	bHandled = FALSE;
	return TRUE;
}

LRESULT CDlgMain::OnBtn_TCmdOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), tcmd_menu);
	return 0;
}

LRESULT CDlgMain::OnMenu_File_Exit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(WM_CLOSE, 0, 0);
	return TRUE;
}

void CDlgMain::loadFile(wchar_t* fileName)
{
	if (dj.clf != 0)
		delete dj.clf;

	malloc_ptr<wchar_t> fCanonName = CanonicalizePath(fileName);

	dj.clf = new (std::nothrow) CClassFile(*dj.logptr);
	if (dj.clf == 0)
	{
		badAllocMsg(m_hWnd);
	}
	else if (dj.clf->LoadFile(fCanonName.get()))
	{
		EnableMenuItem(GetMenu(), MENU_FILE_SAVE, MF_ENABLED);
		dj.settings.addRecentFile(dj.clf->GetFileName());
		updateRecentMenu();
	}
	else
	{
		dj.settings.removeRecentFile(fileName);
		updateRecentMenu();
		EnableMenuItem(GetMenu(), MENU_FILE_SAVE, MF_GRAYED);
		MessageBox(L"Error during file loading.", L"Error", MB_ICONERROR);
		delete dj.clf;
		dj.clf = 0;
	}

	for (int i = 0; i < TABCNT; i++)
	{
		::SendMessage(*tabsTab[i].wnd, WM_UPDATE_TAB, 0, 0);
	}
	updateTitle();
}

LRESULT CDlgMain::OnMenu_File_Open(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	savePrompt();
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"Java .class files\0*.class\0All files\0*.*\0\0";
	wchar_t fNameBuf[MAX_PATH];
	fNameBuf[0] = 0;
	ofn.lpstrFile = fNameBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
		loadFile(fNameBuf);
	return 0;
}

LRESULT CDlgMain::OnMenu_File_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	saveFile();
	return 0;
}

void CDlgMain::saveFile()
{
	if (dj.clf)
	{
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = L"Java .class files\0*.class\0All files\0*.*\0\0";
		wchar_t fNameBuf[MAX_PATH];
		fNameBuf[0] = 0;
		ofn.lpstrFile = fNameBuf;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			bool saveError = true;
			malloc_ptr<wchar_t> fName = CanonicalizePath(fNameBuf);
			if (nullptr != fName)
			{
				wchar_t fext[_MAX_EXT];
				if (0 == _wsplitpath_s(fName.get(), 0, 0, 0, 0, 0, 0, fext, _MAX_EXT))
				{
					malloc_ptr<wchar_t> properName = nullptr;
					if (0 == wcslen(fext))
						properName = wcscat_var(fName.get(), L".class", 0);

					if (dj.clf->SaveFile((nullptr != properName) ? properName.get() : fName.get()))
					{
						updateTitle();
						saveError = false;
					}
				}
			}
			
			if (saveError)
				MessageBox(L"Error during file saving.", L"Error", MB_ICONERROR);
		}
	}
}

LRESULT CDlgMain::OnMenu_Options_RegisterShellExtension(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	dj.settings.set_ShellExtension(true);
	shellExt::addShellExtension(m_hWnd);
	shellExtMenuUpdate(GetMenu());
	return 0;
}

LRESULT CDlgMain::OnMenu_Options_UnRegisterShellExtension(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	dj.settings.set_ShellExtension(false);
	shellExt::delShellExtension(m_hWnd);
	shellExtMenuUpdate(GetMenu());
	return 0;
}

LRESULT CDlgMain::OnMenu_Options_Settings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (1 == CDlgSettings(dj).DoModal(m_hWnd))
	{
		tabFields.setLBFont(dj.settings.getFontHandle());
		tabMethods.setLBFont(dj.settings.getFontHandle());
		shellExtMenuUpdate(GetMenu());
	}
	return 0;
}

LRESULT CDlgMain::OnMenu_Help_HelpTopics(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAppDir appdir;
	if (appdir.getError())
		return 0;

	ShellExecute(m_hWnd, L"open", L"dirtyjoe.chm", 0, 0, SW_SHOWNORMAL);
	return 0;
}

LRESULT CDlgMain::OnMenu_Help_OpcodesHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	opHelp.ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CDlgMain::OnMenu_Help_CheckForUpdates(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	checkForUpdate(m_hWnd);
	return 0;
}

LRESULT CDlgMain::OnMenu_Help_About(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgAbout().DoModal(m_hWnd);
	return 0;
}

LRESULT CDlgMain::OnMenu_Tools_AddLineNumbers(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgRestoreDebugInfo(dj).DoModal(m_hWnd);

	updateTitle();
	SendMessage(tabConstantPool, WM_UPDATE_TAB, 0, 0);
	SendMessage(tabOverView, WM_UPDATE_TAB, 0, 0);
	return 0;
}

LRESULT CDlgMain::OnMenu_Help_Donate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd, L"open", L"http://dirty-joe.com/donate.php", 0, 0, SW_SHOW);
	return 0;
}

LRESULT CDlgMain::OnTab_SelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	DWORD ct = TabCtrl_GetCurSel(pnmh->hwndFrom);
	::SendMessage(*tabsTab[ct].wnd, WM_UPDATE_TAB, 0, 0);
	::ShowWindow(*tabsTab[ct].wnd, SW_SHOW);
	SendMessage(WM_SETREDRAW, TRUE, 0);
	RedrawWindow();
	return TRUE;
}

LRESULT CDlgMain::OnTab_SelChanging(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	SendMessage(WM_SETREDRAW, FALSE, 0);
	DWORD ct = TabCtrl_GetCurSel(pnmh->hwndFrom);
	::ShowWindow(*tabsTab[ct].wnd, SW_HIDE);
	return FALSE;
}

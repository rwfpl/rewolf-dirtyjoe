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

#include "pre_wtl.h"
#include "resource.h"
#include "CDirtyJOE.h"
#include "dlgTabOverview.h"
#include "dlgTabConstantPool.h"
#include "dlgTabFM.h"
#include "dlgOpcodesHelp.h"
#include "dlgCode.h"
#include "StickyWindow.h"

#define MENU_RECENT_RANGE_START 0xC000
#define MENU_RECENT_RANGE_END 0xC009

class CDlgMain : public CDialogImpl<CDlgMain>
{
	public:

	#ifdef DJ_TOTALCMD
		enum { IDD = DJ_MAIN_TOTALCMD };
	#else
		enum { IDD = DJ_MAIN_DLG };
	#endif

	BEGIN_MSG_MAP(CDlgMain)
// 		{
// 			wchar_t tmp[0x1000];
// 			swprintf(tmp, L"msg: %04X\n", uMsg);
// 			OutputDebugString(tmp);
// 		}
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_WINDOW_MOVING, OnWindowMoving)
		#ifndef DJ_TOTALCMD
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
		COMMAND_ID_HANDLER(MENU_FILE_EXIT, OnMenu_File_Exit)
		COMMAND_ID_HANDLER(MENU_FILE_OPEN, OnMenu_File_Open)
		COMMAND_ID_HANDLER(MENU_FILE_SAVE, OnMenu_File_Save)
		COMMAND_ID_HANDLER(MENU_OPTIONS_REGISTERSHELLEXTENSION, OnMenu_Options_RegisterShellExtension)
		COMMAND_ID_HANDLER(MENU_OPTIONS_UNREGISTERSHELLEXTENSION, OnMenu_Options_UnRegisterShellExtension)
		COMMAND_ID_HANDLER(MENU_OPTIONS_SETTINGS, OnMenu_Options_Settings)
		COMMAND_ID_HANDLER(MENU_HELP_HELPTOPICS, OnMenu_Help_HelpTopics)
		COMMAND_ID_HANDLER(MENU_HELP_OPCODESHELP, OnMenu_Help_OpcodesHelp)
		COMMAND_ID_HANDLER(MENU_HELP_CHECKFORUPDATES, OnMenu_Help_CheckForUpdates)
		COMMAND_ID_HANDLER(MENU_HELP_DONATE, OnMenu_Help_Donate)
		COMMAND_ID_HANDLER(MENU_HELP_ABOUT, OnMenu_Help_About)
		COMMAND_ID_HANDLER(MENU_TOOLS_ADDLINENUMS, OnMenu_Tools_AddLineNumbers)
		COMMAND_RANGE_HANDLER(MENU_RECENT_RANGE_START, MENU_RECENT_RANGE_END, OnMenu_RecentFiles)
		#else
		COMMAND_ID_HANDLER(BTN_TCMD_OPTIONS, OnBtn_TCmdOptions)
		COMMAND_ID_HANDLER(ID_TCMD_RESTOREDEBUGINFO, OnMenu_Tools_AddLineNumbers)
		COMMAND_ID_HANDLER(ID_TCMD_ABOUT, OnMenu_Help_About)
		COMMAND_ID_HANDLER(ID_TCMD_DONATE, OnMenu_Help_Donate)
		COMMAND_ID_HANDLER(ID_TCMD_CHECKFORUPDATES, OnMenu_Help_CheckForUpdates)
		COMMAND_ID_HANDLER(ID_TCMD_HELPTOPICS, OnMenu_Help_HelpTopics)
		COMMAND_ID_HANDLER(ID_TCMD_OPCODESHELP, OnMenu_Help_OpcodesHelp)
		COMMAND_ID_HANDLER(ID_TCMD_SAVEFILE, OnMenu_File_Save)
		COMMAND_ID_HANDLER(ID_TCMD_SETTINGS, OnMenu_Options_Settings)
		#endif
		NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnTab_SelChange)
		NOTIFY_CODE_HANDLER(TCN_SELCHANGING, OnTab_SelChanging)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWindowMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnMenu_File_Exit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_File_Open(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_File_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Options_RegisterShellExtension(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Options_UnRegisterShellExtension(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Options_Settings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Help_HelpTopics(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Help_OpcodesHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Help_CheckForUpdates(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Help_About(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Help_Donate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Tools_AddLineNumbers(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_RecentFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_TCmdOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnTab_SelChange(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnTab_SelChanging(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

	CDlgMain(CDirtyJOE& _dj);

	CDlgTabOverview tabOverView;
	CDlgTabConstantPool tabConstantPool;
	CDlgTabFM tabFields;
	CDlgTabFM tabMethods;

	CDlgOpcodesHelp opHelp;

	CDlgCode* dlgCurrentCodeWindow;

	private:
	
	CDirtyJOE& dj;

	static const int TABCNT = 4;
	struct TABSTRUCT
	{
		TABSTRUCT(wchar_t* n, CWindow* w) : name(n), wnd(w) {}
		TABSTRUCT() : name(0), wnd(0) {}
		wchar_t* name;
		CWindow* wnd;
	} tabsTab[TABCNT];

	const wchar_t* lastFileName;
	bool lastIsModified;
	StickyWindow stickyWnd;
	
	static HWND m_hWnd_static;
	static HHOOK m_MainMsgHook_static;
	static HACCEL m_hAccel_static;

	void addTabControl(DWORD num, wchar_t* name);
	void shellExtMenuUpdate(HMENU hMenu);
	void loadFile(wchar_t* fileName);
	void saveFile();
	void savePrompt();
	void updateRecentMenu();
	static LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);

	public:
	void updateTitle();
};

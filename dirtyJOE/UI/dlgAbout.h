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
#include "ui_helpers.h"

class CDlgAbout : public CDialogImpl<CDlgAbout>
{
public:

	enum { IDD = DJ_ABOUT };

	BEGIN_MSG_MAP(CDlgAbout)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		COMMAND_HANDLER(TXT_WWW, STN_CLICKED, OnTxt_WWWClicked)
		COMMAND_HANDLER(TXT_MAIL, STN_CLICKED, OnTxt_MailClicked)
		COMMAND_HANDLER(BMP_LOGO, STN_CLICKED, OnBmp_Clicked)
		COMMAND_HANDLER(BMP_DONATE, STN_CLICKED, OnBmp_Donate)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnTxt_WWWClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTxt_MailClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBmp_Clicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBmp_Donate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgAbout() : threadHandle(0) { }

	private:

	bool thRun;
	HWND thWnd;
	HANDLE threadHandle;

	static DWORD WINAPI hiddenThread(LPVOID prm);
};

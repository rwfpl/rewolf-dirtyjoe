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
#include "ui_helpers.h"

class CDlgEditException : public CDialogImpl<CDlgEditException>
{
public:

	enum { IDD = DJ_EDIT_EXCEPTION };

	BEGIN_MSG_MAP(CDlgEditException)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtnCancel)
		COMMAND_ID_HANDLER(BTN_OK, OnBtnOK)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtnCancel)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditException(CDirtyJOE& _dj, exception_entry& _exc) : dj(_dj), exc(_exc) {} 

private:

	CDirtyJOE& dj;
	exception_entry& exc;
	std::map<u2, u2> editMap_1;
};

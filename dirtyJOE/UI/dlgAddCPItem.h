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

class CDlgAddCPItem : public CDialogImpl<CDlgAddCPItem>
{
public:

	enum { IDD = DJ_ADD_CP };

	BEGIN_MSG_MAP(CDlgAddCPItem)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_ADD_CP, OnBtn_Add)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Add(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgAddCPItem(CDirtyJOE& _dj) : dj(_dj) {} 

	private:

	CDirtyJOE& dj;
};

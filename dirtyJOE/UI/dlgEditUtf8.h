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

class CDlgEditUtf8 : public CDialogImpl<CDlgEditUtf8>
{
public:

	enum { IDD = DJ_EDIT_UTF8 };

	BEGIN_MSG_MAP(CDlgEditUtf8)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_SAVE, OnBtn_Save)
		COMMAND_ID_HANDLER(BTN_SHOW_REF, OnBtn_ShowRef)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_ShowRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditUtf8(CDirtyJOE& _dj, u2 _cpIdx) : dj(_dj), cpIdx(_cpIdx) {}

private:

	CDirtyJOE& dj;
	u2 cpIdx;

	bool init();
};

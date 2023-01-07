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

class CDlgEditAttrExceptions : public CDialogImpl<CDlgEditAttrExceptions>
{
public:

	enum { IDD = DJ_EDIT_ATTR_EXCEPTIONS };

	BEGIN_MSG_MAP(CDlgEditAttrExceptions)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtnClose)
		COMMAND_ID_HANDLER(BTN_ADD, OnBtnAdd)
		COMMAND_ID_HANDLER(BTN_DELETE, OnBtnDelete)
		COMMAND_ID_HANDLER(BTN_CLOSE, OnBtnClose)
		COMMAND_ID_HANDLER(BTN_EDIT, OnBtnEdit)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditAttrExceptions(CDirtyJOE& _dj, CAttrExceptions& _exc) : dj(_dj), exc(_exc) {} 

private:

	CDirtyJOE& dj;
	CAttrExceptions& exc;

	void refresh();
};

/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
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

class CDlgSelectColumn : public CDialogImpl<CDlgSelectColumn>
{
public:

	enum { IDD = DJ_SELECT_COLUMN };

	enum CopyMode
	{
		Copy_None = 0, 
		Copy_Address = 1, 
		Copy_Hex = 2, 
		Copy_Disasm = 4
	};

	BEGIN_MSG_MAP(CDlgSelectColumn)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_OK, OnBtn_Ok)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Ok(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgSelectColumn(CDirtyJOE& _dj) : dj(_dj) {} 

private:
	CDirtyJOE& dj;
};

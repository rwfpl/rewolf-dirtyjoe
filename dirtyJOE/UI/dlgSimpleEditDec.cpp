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
#include "CDirtyJOE.h"
#include "ui_helpers.h"

#include "dlgSimpleEditDec.h"

LRESULT CDlgSimpleEditDec::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetDlgItemInt(EDIT_DEC, val, FALSE);
	return TRUE;
}

LRESULT CDlgSimpleEditDec::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgSimpleEditDec::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgSimpleEditDec::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL trans = FALSE;
	UINT _val = GetDlgItemInt(EDIT_DEC, &trans, FALSE);
	if (trans)
	{
		val = _val;
		EndDialog(1);
	}
	return FALSE;
}

UINT CDlgSimpleEditDec::getVal() const
{
	return val;
}

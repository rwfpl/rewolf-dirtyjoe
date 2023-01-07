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
#include "dlgSelectColumn.h"

LRESULT CDlgSelectColumn::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CheckDlgButton(CHK_ADDR, dj.settings.get_CopyAddr() ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(CHK_HEX, dj.settings.get_CopyHex() ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(CHK_DISASM, dj.settings.get_CopyDisasm() ? BST_CHECKED : BST_UNCHECKED);
	return TRUE;
}

LRESULT CDlgSelectColumn::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgSelectColumn::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return 0;
}

LRESULT CDlgSelectColumn::OnBtn_Ok(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DWORD ret = Copy_None;
	
	if (BST_CHECKED == IsDlgButtonChecked(CHK_ADDR))
	{
		ret |= Copy_Address;
		dj.settings.set_CopyAddr(true);
	}
	else
		dj.settings.set_CopyAddr(false);
	
	if (BST_CHECKED == IsDlgButtonChecked(CHK_HEX))
	{
		ret |= Copy_Hex;
		dj.settings.set_CopyHex(true);
	}
	else
		dj.settings.set_CopyHex(false);
	
	if (BST_CHECKED == IsDlgButtonChecked(CHK_DISASM))
	{
		ret |= Copy_Disasm;
		dj.settings.set_CopyDisasm(true);
	}
	else
		dj.settings.set_CopyDisasm(false);

	EndDialog(ret);
	return 0;
}

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

#include "dlgTabSubFM.h"

LRESULT CDlgTabSubFM::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_ATTRIBS), lbSubClass, 0x2a0, (DWORD_PTR)&sc);
	SetWindowSubclass(GetDlgItem(LB_ATTRIBS_PROPS), lbSubClass, 0x2a1, (DWORD_PTR)&sc);
	SendDlgItemMessage(LB_ATTRIBS_PROPS, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);
	return TRUE;
}

LRESULT CDlgTabSubFM::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgTabSubFM::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	::MoveWindow(GetDlgItem(IDC_FIELD_GRP), 0, 0, LOWORD(lParam), HIWORD(lParam), 0);
	::MoveWindow(GetDlgItem(LB_ATTRIBS_PROPS), 6, 160, LOWORD(lParam) - 12, HIWORD(lParam) - 166, 0);
	return FALSE;
}

LRESULT CDlgTabSubFM::OnBtn_EditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(GetParent(), WM_EDIT_FM, EDIT_FM_NAME, 0);
	return FALSE;
}

LRESULT CDlgTabSubFM::OnBtn_EditDescriptor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(GetParent(), WM_EDIT_FM, EDIT_FM_DESCRIPTOR, 0);
	return FALSE;
}

LRESULT CDlgTabSubFM::OnBtn_EditFlags(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(GetParent(), WM_EDIT_FM, EDIT_FM_FLAGS, 0);
	return FALSE;
}

LRESULT CDlgTabSubFM::OnAttr_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(GetParent(), WM_SHOW_ATTRIB, SendDlgItemMessage(LB_ATTRIBS, LB_GETCURSEL, 0, 0), 0);
	return FALSE;
}

LRESULT CDlgTabSubFM::OnAttr_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(GetParent(), WM_SHOW_ATTR_WND, SendDlgItemMessage(LB_ATTRIBS, LB_GETCURSEL, 0, 0), 0);
	return FALSE;
}

LRESULT CDlgTabSubFM::OnAttr_RButtonUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(wID, wNotifyCode), (LPARAM)hWndCtl);
	return FALSE;
}

LRESULT CDlgTabSubFM::ForwardToParent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return ::SendMessage(GetParent(), uMsg, wParam, lParam);
}

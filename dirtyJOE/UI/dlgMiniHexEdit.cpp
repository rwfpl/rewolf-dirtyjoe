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

#include "dlgMiniHexEdit.h"
#include "ui_helpers.h"
#include "../java_support/jdisasm/jdisasm.h"
#include "../common/common.h"

LRESULT CDlgMiniHexEdit::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(EDT_HEX), edtHexSubClass, 0x29d, 0);

	RECT deskrt;
	::GetWindowRect(GetDesktopWindow(), &deskrt);
	RECT wndrt;
	GetWindowRect(&wndrt);

	if (pt.y + wndrt.bottom - wndrt.top > deskrt.bottom)
		pt.y = deskrt.bottom - (wndrt.bottom - wndrt.top);

	if (pt.x + wndrt.right - wndrt.left > deskrt.right)
		pt.x = deskrt.right - (wndrt.right - wndrt.left);

	SetWindowPos(HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SendDlgItemMessage(EDT_HEX, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);

	if (ac.get_code().size() != 0)
	{
		int len = jdisasm_get_len(&ac.get_code()[0], (u4)ac.get_code().size(), offs);
		if (len <= 0)
			len = 1;
		if ((size_t)(offs + len) > ac.get_code().size())
			len = (int)(ac.get_code().size() - offs);
		malloc_ptr<wchar_t> str_hex = data2hex(&ac.get_code()[offs], len);
		SendDlgItemMessage(EDT_HEX, WM_SETTEXT, 0, (LPARAM)str_hex.get());
	}
	refreshInfo(ac.get_code());
	return TRUE;
}

LRESULT CDlgMiniHexEdit::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgMiniHexEdit::OnHex_Change(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	std::vector<u1>* hxv = GetWindowHexBufV(hWndCtl);
	if (hxv)
	{
		//temporary behaviour, jdisasm_* functions should be modified :(
		hxv->insert(hxv->begin(), offs, 0);
		refreshInfo(*hxv);
		delete hxv;
	}
	return FALSE;
}

LRESULT CDlgMiniHexEdit::OnHex_Enter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	::EndDialog(m_hWnd, (INT_PTR)GetWindowHexBufV(hWndCtl));
	return FALSE;
}

void CDlgMiniHexEdit::refreshInfo(std::vector<u1>& vec)
{
	if (vec.empty())
		return;

	int len = jdisasm_get_len(&vec[0], (u4)vec.size(), offs);
	wchar_t tbuf[0x20];
	wchar_t* tbufr = tbuf;
	switch (len)
	{
	case -1: tbufr = L"buffer to small"; break;
	case -2: tbufr = L"undefined opcode"; break;
	case -3: tbufr = L"unknown variable size opcode"; break;
	case -4: tbufr = L"error"; break;
	default: swprintf_s(tbuf, L"%d", len); break;
	}
	SetDlgItemText(TXT_ESTLEN, tbufr);

	malloc_ptr<wchar_t> istr = jdisasm_get_str(&vec[0], (u4)vec.size(), offs);
	if (nullptr == istr)
		tbufr = L"error";
	else
		tbufr = istr.get();
	SetDlgItemText(TXT_INSTR, tbufr);
}

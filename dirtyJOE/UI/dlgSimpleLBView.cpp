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

#include "dlgSimpleLBView.h"
#include "../java_support/helpers.h"
#include "../java_support/hlp_references.h"

void CDlgSimpleLBView::initAttributes()
{
	if (ai == 0)
	{
		EndDialog(0);
		return;
	}

	SendDlgItemMessage(LB_DJ_LB, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);
	CONSTANT_Utf8_info* attName = getUtf8FromCP(ai->get_attribute_name_index(), dj.clf->get_constant_pool());
	SetWindowText(preprocessUtf8ToWChar(attName).get());
	showAttributeWindow(ai, dj.clf->get_constant_pool(), m_hWnd, LB_DJ_LB, LB_ADDSTRING);
}

void CDlgSimpleLBView::initAsciiBuffer()
{
	SetWindowText(L"");
	SendDlgItemMessage(LB_DJ_LB, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);
	if (asciiBuffer)
	{
		malloc_ptr<char> tmpStr(_strdup(asciiBuffer));
		if (nullptr == tmpStr)
			return;

		char* crStr = tmpStr.get();
		int i = 0;
		bool _end = false;
		LONG maxX = 0;
		HDC lbHDC = ::GetWindowDC(GetDlgItem(LB_DJ_LB));
		do
		{
			if (0 == crStr[i])
				_end = true;
			else if (0xA == crStr[i])
				crStr[i] = 0;
			else if ((i > 75) && (0x20 == crStr[i]))
				crStr[i] = 0;

			if (0 == crStr[i])
			{
				//compute width in pixels to set proper horizontal scrollbar
				HGDIOBJ hGdiTmp = SelectObject(lbHDC, dj.settings.getFontHandle());
				SIZE sz = { 0 };
				GetTextExtentPoint32A(lbHDC, crStr, i, &sz);
				SelectObject(lbHDC, hGdiTmp);
				if (sz.cx > maxX)
					maxX = sz.cx;
				SendDlgItemMessageA(m_hWnd, LB_DJ_LB, LB_ADDSTRING, 0, (LPARAM)crStr);
				crStr = crStr + i + 1;
				i = 0;
			}
			else
				i++;
		}
		while(!_end);
		SendDlgItemMessage(LB_DJ_LB, LB_SETHORIZONTALEXTENT, maxX + 10, 0);
	}
}

void CDlgSimpleLBView::initReferences()
{
	std::vector<wchar_t*>* test = getAllRefs(*dj.clf, idx);
	if (test)
	{
		wchar_t dn[0x40];
		swprintf_s(dn, L"References (%zd)", test->size());
		SetWindowText(dn);
		for (DWORD i = 0; i < test->size(); i++)
		{
			wchar_t* ot = parseReference(*dj.clf, (*test)[i]);
			if (ot)
			{
				SendDlgItemMessage(LB_DJ_LB, LB_ADDSTRING, 0, (LPARAM)ot);
				free(ot);
			}
			else
				SendDlgItemMessage(LB_DJ_LB, LB_ADDSTRING, 0, (LPARAM)(*test)[i]);
		}
		cleanUpObjects(*test);
	}
	else
		SetWindowText(L"References (error)");
	delete test;
}

LRESULT CDlgSimpleLBView::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_DJ_LB), lbSubClass, 0x29a, 0);

	if (t == Attributes)
		initAttributes();
	else if (t == References)
		initReferences();
	else if (t == AsciiBuffer)
		initAsciiBuffer();

	CheckDlgButton(CHK_CASE, dj.settings.get_LBCaseSensitive() ? BST_CHECKED : BST_UNCHECKED);

	RECT rt;
	BOOL dummy;
	GetClientRect(&rt);
	OnSize(0, 0, MAKEWPARAM(rt.right - rt.left, rt.bottom - rt.top), dummy);

	return TRUE;
}

LRESULT CDlgSimpleLBView::OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;
	toolWindowHandleEscape(m_hWnd, wParam, lParam, bHandled);
	return TRUE;
}

LRESULT CDlgSimpleLBView::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgSimpleLBView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	HWND edtHwnd = GetDlgItem(EDT_SEARCH);
	HWND btnPrev = GetDlgItem(BTN_PREV);
	HWND btnNext = GetDlgItem(BTN_NEXT);
	HWND chkCase = GetDlgItem(CHK_CASE);

	RECT edtRt;
	::GetWindowRect(edtHwnd, &edtRt);

	RECT btnRect;
	::GetWindowRect(btnPrev, &btnRect);

	RECT chkRect;
	::GetWindowRect(chkCase, &chkRect);

	POINT edtPt = { edtRt.right, edtRt.bottom };
	ScreenToClient(&edtPt);

	POINT edtPtLT = { edtRt.left, edtRt.top };
	ScreenToClient(&edtPtLT);

	const int chkWdth = chkRect.right - chkRect.left;
	const int chkHght = chkRect.bottom - chkRect.top;
	const int btnWdth = btnRect.right - btnRect.left;
	const int btnHght = btnRect.bottom - btnRect.top;
	const int edtWdth = LOWORD(lParam) - 4*grid_space - 2*btnWdth - chkWdth;
	::MoveWindow(chkCase, 3*grid_space + edtWdth + 2*btnWdth, edtPtLT.y + grid_space/2, chkWdth, chkHght, FALSE);
	::MoveWindow(btnPrev, 2*grid_space + edtWdth, edtPtLT.y, btnWdth, btnHght, FALSE);
	::MoveWindow(btnNext, 2*grid_space + edtWdth + btnWdth, edtPtLT.y, btnWdth, btnHght, FALSE);
	::MoveWindow(edtHwnd, grid_space, edtPtLT.y, edtWdth, edtRt.bottom - edtRt.top, FALSE);
	::MoveWindow(GetDlgItem(LB_DJ_LB), grid_space, grid_space + edtPt.y, LOWORD(lParam) - 2*grid_space, HIWORD(lParam) - 2*grid_space - edtPt.y, FALSE);
	InvalidateRect(NULL, TRUE);
	return FALSE;
}

LRESULT CDlgSimpleLBView::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 400) 
		rptr->right = rptr->left + 400;
	if (rptr->bottom - rptr->top < 200) 
		rptr->bottom = rptr->top + 200;
	return TRUE;
}

LRESULT CDlgSimpleLBView::OnLB_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), attrib_props_menu);
	return 0;
}

void CDlgSimpleLBView::copyHelper(bool selOnly)
{
	malloc_ptr<wchar_t> tmpStr = getLBContent(GetDlgItem(LB_DJ_LB), selOnly);
	if (nullptr != tmpStr)
		CopyWCharToClipBoard(m_hWnd, tmpStr.get());
}

LRESULT CDlgSimpleLBView::OnMenu_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyHelper(false);
	return 0;
}

LRESULT CDlgSimpleLBView::OnMenu_CopySelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyHelper(true);
	return 0;
}

LRESULT CDlgSimpleLBView::OnBtn_Next(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	search(false);
	return 0;
}

LRESULT CDlgSimpleLBView::OnBtn_Prev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	search(true);
	return 0;
}

LRESULT CDlgSimpleLBView::OnBtn_CheckCase(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	dj.settings.set_LBCaseSensitive(IsDlgButtonChecked(CHK_CASE) == BST_CHECKED);
	SendDlgItemMessage(LB_DJ_LB, LB_SETSEL, FALSE, -1);
	search(false);
	return 0;
}

LRESULT CDlgSimpleLBView::OnSearch_Change(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendDlgItemMessage(LB_DJ_LB, LB_SETSEL, FALSE, -1);
	search(false);
	return 0;
}

void CDlgSimpleLBView::search(bool prev)
{
	malloc_ptr<wchar_t> schText = getWindowText(GetDlgItem(EDT_SEARCH));
	int ret = listBoxSearch(GetDlgItem(LB_DJ_LB), schText.get(), dj.settings.get_LBCaseSensitive(), prev);
	if (-1 != ret)
		SendDlgItemMessage(LB_DJ_LB, LB_SETSEL, TRUE, ret);
}

LRESULT CDlgSimpleLBView::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (skipNextUserChar)
	{
		skipNextUserChar = false;
		return 1;
	}
	return 0;
}

LRESULT CDlgSimpleLBView::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		BOOL dummy;
		skipNextUserChar = true;
		switch (wParam)
		{
		case 'C': OnMenu_CopySelection(0, 0, 0, dummy); break;
		case 'A': OnMenu_CopyAll(0, 0, 0, dummy); break;
		default:
			skipNextUserChar = false;
			return 0;
		}
		return 1;
	}
	skipNextUserChar = false;
	return 0;
}

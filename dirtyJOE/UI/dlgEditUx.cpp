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

#include "dlgEditUx.h"
#include "dlgSimpleLBView.h"
#include "../java_support/helpers.h"

LRESULT CDlgEditUx::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!init())
	{
		EndDialog(0);
		return FALSE;
	}
	return TRUE;
}

bool CDlgEditUx::init()
{
	u1 cmptag = 0;
	if (t == CPEDIT)
	{
		u1 tag = dj.clf->get_constant_pool()[cpIdx]->tag;
		if (tag == CONSTANT_String)
			SetWindowText(L"Edit CONSTANT_String");
		else if (tag == CONSTANT_Class)
			SetWindowText(L"Edit CONSTANT_Class");
		else if (tag == CONSTANT_MethodType)
			SetWindowText(L"Edit CONSTANT_MethodType");
		else
			return false;
		cmptag = CONSTANT_Utf8;
	}
	else
	{
		SetWindowText(L"Edit");
		::ShowWindow(GetDlgItem(BTN_SHOW_REF), SW_HIDE);

		if (t == OVREDIT)
		{
			cmptag = CONSTANT_Class;
		}
		else if (t == UTF8)
		{
			cmptag = CONSTANT_Utf8;
		}
		else
			return false;
	}

	//NULL value support
	SendDlgItemMessage( CB_EDIT, CB_ADDSTRING, 0, (LPARAM)L"NULL");
	editMap_1[0] = 0;
	//

	u2 curSel = 0;
	for (size_t i = 1; i < dj.clf->get_constant_pool().size(); i++)
	{
		if (dj.clf->get_constant_pool()[i] && (cmptag == dj.clf->get_constant_pool()[i]->tag))
		{
			malloc_ptr<wchar_t> tmpstr = genStringFromCP(dj.clf->get_constant_pool(), (u4)i, dj.clf->get_constant_pool()[i], true, dj.settings.get_CPHexIdx());
			const wchar_t* const newstr = (nullptr != tmpstr) ? tmpstr.get() : strError;

			u2 idx = (u2)SendDlgItemMessage(CB_EDIT, CB_ADDSTRING, 0, (LPARAM)newstr);
			editMap_1[idx] = (u2)i;
			if ((t == CPEDIT) && (dj.clf->get_constant_pool()[cpIdx]))
			{
				if (((CONSTANT_uX_info<u2>*)dj.clf->get_constant_pool()[cpIdx])->v1 == i)
					curSel = idx;
			}
			else if ((t == OVREDIT) || (t == UTF8))
			{
				if (cpIdx == i)
					curSel = idx;
			}
		}
	}
	SendDlgItemMessage(CB_EDIT, CB_SETCURSEL, curSel, 0);

	return true;
}

LRESULT CDlgEditUx::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditUx::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditUx::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT idx = SendDlgItemMessage(CB_EDIT, CB_GETCURSEL, 0, 0);
	if (idx != CB_ERR)
	{
		EndDialog(1);
		cpIdx = editMap_1[(u2)idx];
	}
	else
	{
		MessageBox(L"Cannot get proper selection, value will not be changed.", L"Error", MB_ICONERROR);
	}
	return FALSE;
}

LRESULT CDlgEditUx::OnBtn_ShowRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleLBView(dj, CDlgSimpleLBView::References, cpIdx).DoModal(m_hWnd);
	return FALSE;
}

u2 CDlgEditUx::getCPIdx() const
{
	return cpIdx;
}

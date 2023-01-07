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

#include "dlgEditAttrConstantValue.h"
#include "../java_support/helpers.h"

LRESULT CDlgEditAttrConstantValue::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!init())
	{
		EndDialog(0);
		return FALSE;
	}
	return TRUE;
}

bool CDlgEditAttrConstantValue::init()
{
	SetWindowText(L"Edit ConstantValue attribute");
	::ShowWindow(GetDlgItem(BTN_SHOW_REF), SW_HIDE);

	//NULL value support
	SendDlgItemMessage(CB_EDIT, CB_ADDSTRING, 0, (LPARAM)L"NULL");
	editMap_1[0] = 0;
	//

	u2 curSel = 0;
	for (size_t i = 1; i < dj.clf->get_constant_pool().size(); i++)
	{
		if (dj.clf->get_constant_pool()[i])
		{
			u1 tTag = dj.clf->get_constant_pool()[i]->tag;
			if ((tTag != CONSTANT_Integer) && (tTag != CONSTANT_Long) && (tTag != CONSTANT_Double) && (tTag != CONSTANT_Float) && (tTag != CONSTANT_String))
				continue;

			malloc_ptr<wchar_t> tmpstr = genStringFromCP(dj.clf->get_constant_pool(), (u4)i, dj.clf->get_constant_pool()[i], true, dj.settings.get_CPHexIdx());
			const wchar_t* const newstr = (nullptr != tmpstr) ? tmpstr.get() : strError;
			u2 idx = (u2)SendDlgItemMessage(CB_EDIT, CB_ADDSTRING, 0, (LPARAM)newstr);
			editMap_1[idx] = (u2)i;

			if (cpIdx == i)
				curSel = idx;
		}
	}
	SendDlgItemMessage(CB_EDIT, CB_SETCURSEL, curSel, 0);

	return true;
}

LRESULT CDlgEditAttrConstantValue::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditAttrConstantValue::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditAttrConstantValue::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT idx = SendDlgItemMessage(CB_EDIT, CB_GETCURSEL, 0, 0);
	if (idx != CB_ERR)
	{
		cpIdx = editMap_1[(u2)idx];
		EndDialog(1);
	}
	else
	{
		MessageBox(L"Cannot get proper selection, value will not be changed.", L"Error", MB_ICONERROR);
	}
	return FALSE;
}

u2 CDlgEditAttrConstantValue::getCPIdx() const
{
	return cpIdx;
}

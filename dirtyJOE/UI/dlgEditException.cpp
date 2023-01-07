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

#include "dlgEditException.h"
#include "../java_support/helpers.h"

LRESULT CDlgEditException::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	wchar_t tmp[0x100];
	swprintf_s(tmp, L"0x%04X", exc.get_start_pc());
	SetDlgItemText(EDT_START_PC, tmp);
	swprintf_s(tmp, L"0x%04X", exc.get_end_pc());
	SetDlgItemText(EDT_END_PC, tmp);
	swprintf_s(tmp, L"0x%04X", exc.get_handler_pc());
	SetDlgItemText(EDT_HANDLER, tmp);

	//'any' value support
	SendDlgItemMessage(CB_CATCH_TYPE, CB_ADDSTRING, 0, (LPARAM)L"any");
	editMap_1[0] = 0;
	//
	
	u2 curSel = 0;
	for (size_t i = 1; i < dj.clf->get_constant_pool().size(); i++)
	{
		if (dj.clf->get_constant_pool()[i] && (CONSTANT_Class == dj.clf->get_constant_pool()[i]->tag))
		{
			malloc_ptr<wchar_t> cpstr = genStringFromCP(dj.clf->get_constant_pool(), (u2)i);
			if (nullptr == cpstr)
				return FALSE;

			u2 idx = (u2)SendDlgItemMessage(CB_CATCH_TYPE, CB_ADDSTRING, 0, (LPARAM)cpstr.get());
			editMap_1[idx] = (u2)i;
			if (i == exc.get_catch_type())
			{
				curSel = idx;
			}
		}
	}
	SendDlgItemMessage(CB_CATCH_TYPE, CB_SETCURSEL, curSel, 0);

	//cbToolTip.Create(m_hWnd, 0, 0, TTS_ALWAYSTIP | TTS_NOPREFIX);
	//cbToolTip.AddTool(CToolInfo(TTF_SUBCLASS | TTF_IDISHWND, GetDlgItem(CB_CATCH_TYPE), 0, 0, L"test tooltip"));

	return TRUE;
}

LRESULT CDlgEditException::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditException::OnBtnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int v = 0;
	if (dlgGetHexValue(m_hWnd, EDT_START_PC, v))
	{
		exc.set_start_pc((u2)v);
		dj.clf->setModified();
	}
	if (dlgGetHexValue(m_hWnd, EDT_END_PC, v))
	{
		exc.set_end_pc((u2)v);
		dj.clf->setModified();
	}
	if (dlgGetHexValue(m_hWnd, EDT_HANDLER, v))
	{
		exc.set_handler_pc((u2)v);
		dj.clf->setModified();
	}

	LRESULT idx = SendDlgItemMessage(CB_CATCH_TYPE, CB_GETCURSEL, 0, 0);
	if (idx != LB_ERR)
	{
		exc.set_catch_type(editMap_1[(u2)idx]);
		dj.clf->setModified();
	}

	EndDialog(1);
	return FALSE;
}

LRESULT CDlgEditException::OnBtnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

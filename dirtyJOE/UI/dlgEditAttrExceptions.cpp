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

#include "dlgEditAttrExceptions.h"
#include "../java_support/helpers.h"
#include "dlgEditUx.h"

void CDlgEditAttrExceptions::refresh()
{
	SendDlgItemMessage(LB_EXCEPTIONS, LB_RESETCONTENT, 0, 0);
	for (u4 i = 0; i < exc.get_table().size(); i++)
		setDlgItemTextWChar(m_hWnd, LB_EXCEPTIONS, LB_ADDSTRING, genStringFromCP(dj.clf->get_constant_pool(), exc.get_table()[i]->get_value()).get());
}

LRESULT CDlgEditAttrExceptions::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	refresh();
	return TRUE;
}

LRESULT CDlgEditAttrExceptions::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditAttrExceptions::OnBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgEditUx dlg(dj, CDlgEditUx::OVREDIT, 0);
	if (dlg.DoModal(m_hWnd))
	{
		uX_dynStruct<u2>* tptr = new (std::nothrow) uX_dynStruct<u2>(dj.clf->get_constant_pool(), dlg.getCPIdx(), *dj.logptr);
		if (tptr == 0)
			badAllocMsg(m_hWnd);
		else
		{
			exc.get_table().push_back(tptr);
			dj.clf->setModified();
			refresh();
		}
	}
	return FALSE;
}

LRESULT CDlgEditAttrExceptions::OnBtnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT crs = SendDlgItemMessage(LB_EXCEPTIONS, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return FALSE;

	auto& tbl = exc.get_table();
	if ((size_t)crs >= tbl.size())
		return FALSE;

	if (tbl[crs])
		delete tbl[crs];

	tbl.erase(tbl.begin() + crs);
	dj.clf->setModified();
	refresh();
	return FALSE;
}

LRESULT CDlgEditAttrExceptions::OnBtnClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditAttrExceptions::OnBtnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT crs = SendDlgItemMessage(LB_EXCEPTIONS, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return FALSE;

	auto& tbl = exc.get_table();
	if ((size_t)crs >= tbl.size())
		return FALSE;

	CDlgEditUx dlg(dj, CDlgEditUx::OVREDIT, tbl[crs]->get_value());
	if (dlg.DoModal(m_hWnd))
	{
		tbl[crs]->set_value(dlg.getCPIdx());
		dj.clf->setModified();
		refresh();
	}

	return FALSE;
}

/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2013 ReWolf
 * Contact:
 * rewolf@dirty-joe.com
 * http://dirty-joe.com
 *
 * --
 *
 * see license.txt for more info
 *
 */
#include "dlgEditLocalVariableTable.h"
#include "../java_support/helpers.h"
#include "dlgEditUx.h"

LRESULT CDlgEditLocalVariableTable::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_LOCALVARS), lbSubClass, 0x29f, 0);
	updateList();
	SendDlgItemMessage(LB_LOCALVARS, LB_SETCURSEL, 0, 0);
	updateInfo();
	return TRUE;
}

LRESULT CDlgEditLocalVariableTable::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	saveCurrent();
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditLocalVariableTable::OnLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	updateInfo();
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnLB_UserRButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), localvars_menu);
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SendMessage(WM_CLOSE, 0, 0);
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnBtn_Descriptor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	local_vars* lv = getLV();
	CDlgEditUx dlg(dj, CDlgEditUx::UTF8, lv->get_descriptor_index());
	if (0 != dlg.DoModal(m_hWnd))
	{
		u2 dlgRes2 = dlg.getCPIdx();
		lv->set_descriptor_index(dlgRes2);
		setItemText(dlgRes2, EDT_DESCRIPTOR);
		dj.clf->setModified();
	}
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnBtn_Name(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	local_vars* lv = getLV();
	CDlgEditUx dlg(dj, CDlgEditUx::UTF8, lv->get_name_index());
	if (0 != dlg.DoModal(m_hWnd))
	{
		u2 dlgRes2 = dlg.getCPIdx();
		lv->set_name_index(dlgRes2);
		setItemText(dlgRes2, EDT_NAME);
		dj.clf->setModified();
		updateList();
	}
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnMenu_AddLocalVar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	local_vars* lvn = new (std::nothrow) local_vars(dj.clf->get_constant_pool(), *dj.logptr, 0, 0, 0, 0, 0);
	if (lvn != 0)
	{
		lvt.get_table().push_back(lvn);
		dj.clf->setModified();
		updateList();
		LRESULT lr = SendDlgItemMessage(LB_LOCALVARS, LB_GETCOUNT, 0, 0);
		SendDlgItemMessage(LB_LOCALVARS, LB_SETCURSEL, lr - 1, 0);
		updateInfo();
	}
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnMenu_DeleteLocalVar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DWORD sel = getLVTableSelection();
	if (-1 != sel)
	{
		lvt.get_table().erase(lvt.get_table().begin() + sel);
		dj.clf->setModified();
		updateList();
		updateInfo();
	}
	return FALSE;
}

void CDlgEditLocalVariableTable::updateList()
{
	SendDlgItemMessage(LB_LOCALVARS, WM_SETREDRAW, FALSE, 0);
	LRESULT csel = SendDlgItemMessage(LB_LOCALVARS, LB_GETCURSEL, 0, 0);
	SendDlgItemMessage(LB_LOCALVARS, LB_RESETCONTENT, 0, 0);

	for (u4 i = 0; i < lvt.get_table().size(); i++)
	{
		malloc_ptr<wchar_t> cnm = genStringFromCP(dj.clf->get_constant_pool(), lvt.get_table()[i]->get_name_index(), false);
		if (nullptr != cnm)
		{
			SendDlgItemMessage(LB_LOCALVARS, LB_ADDSTRING, 0, (LPARAM)cnm.get());
		}
		else
			SendDlgItemMessage(LB_LOCALVARS, LB_ADDSTRING, 0, (LPARAM)L"NULL");
	}
	SendDlgItemMessage(LB_LOCALVARS, LB_SETCURSEL, csel, 0);
	SendDlgItemMessage(LB_LOCALVARS, WM_SETREDRAW, TRUE, 0);
}

void CDlgEditLocalVariableTable::setItemText(u2 cpid, int ctrl)
{
	if (cpid)
		setDlgItemTextWChar(m_hWnd, ctrl, WM_SETTEXT, genStringFromCP(dj.clf->get_constant_pool(), cpid, false).get());
	else
		SetDlgItemText(ctrl, L"NULL");
}

LRESULT CDlgEditLocalVariableTable::OnBtn_SaveEntry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	lockGUI();
	saveCurrent();
	updateList();
	return FALSE;
}

LRESULT CDlgEditLocalVariableTable::OnBtn_EditEntry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	unlockGUI();
	return FALSE;
}

void CDlgEditLocalVariableTable::lockGUI()
{
	::EnableWindow(GetDlgItem(EDT_DESCRIPTOR), FALSE);
	::EnableWindow(GetDlgItem(EDT_NAME), FALSE);
	::EnableWindow(GetDlgItem(EDT_INDEX), FALSE);
	::EnableWindow(GetDlgItem(EDT_LENGTH), FALSE);
	::EnableWindow(GetDlgItem(EDT_START_PC), FALSE);
	::EnableWindow(GetDlgItem(BTN_LV_NAME), FALSE);
	::EnableWindow(GetDlgItem(BTN_LV_DESCRIPTOR), FALSE);
	::EnableWindow(GetDlgItem(BTN_LV_SAVEENTRY), FALSE);
	::EnableWindow(GetDlgItem(BTN_LV_EDITENTRY), TRUE);
}

void CDlgEditLocalVariableTable::unlockGUI()
{
	::EnableWindow(GetDlgItem(EDT_DESCRIPTOR), TRUE);
	::EnableWindow(GetDlgItem(EDT_NAME), TRUE);
	::EnableWindow(GetDlgItem(EDT_INDEX), TRUE);
	::EnableWindow(GetDlgItem(EDT_LENGTH), TRUE);
	::EnableWindow(GetDlgItem(EDT_START_PC), TRUE);
	::EnableWindow(GetDlgItem(BTN_LV_NAME), TRUE);
	::EnableWindow(GetDlgItem(BTN_LV_DESCRIPTOR), TRUE);
	::EnableWindow(GetDlgItem(BTN_LV_SAVEENTRY), TRUE);
	::EnableWindow(GetDlgItem(BTN_LV_EDITENTRY), FALSE);
}

void CDlgEditLocalVariableTable::updateInfo()
{
	local_vars* _lv = getLV();
	if (_lv == 0)
	{
		SetDlgItemText(EDT_START_PC, L"");
		SetDlgItemText(EDT_LENGTH, L"");
		SetDlgItemText(EDT_INDEX, L"");
		SetDlgItemText(EDT_NAME, L"");
		SetDlgItemText(EDT_DESCRIPTOR, L"");
		return;
	}
	setItemText(_lv->get_name_index(), EDT_NAME);
	setItemText(_lv->get_descriptor_index(), EDT_DESCRIPTOR);
	dlgPrintf(m_hWnd, EDT_START_PC, L"0x%04X", _lv->get_start_pc());
	dlgPrintf(m_hWnd, EDT_LENGTH, L"0x%04X", _lv->get_length());
	dlgPrintf(m_hWnd, EDT_INDEX, L"%d", _lv->get_index());

	lockGUI();
}

DWORD CDlgEditLocalVariableTable::getLVTableSelection()
{
	LRESULT csel = ::SendMessage(GetDlgItem(LB_LOCALVARS), LB_GETCURSEL, 0, 0);
	if (csel == LB_ERR)
		return (DWORD)-1;

	if ((size_t)csel >= lvt.get_table().size())
		return (DWORD)-1;

	return (DWORD)csel;
}

local_vars* CDlgEditLocalVariableTable::getLV()
{
	DWORD csel = getLVTableSelection();
	if (-1 == csel)
		return 0;
	return lvt.get_table()[csel];
}

void CDlgEditLocalVariableTable::saveCurrent()
{
	local_vars* lv = getLV();
	if (0 == lv)
		return;

	int tval = 0;
	bool modified = false;
	if (dlgGetHexValue(m_hWnd, EDT_START_PC, tval) && ((u2)tval != lv->get_start_pc()))
	{
		lv->set_start_pc((u2)tval);
		modified = true;
	}
	
	if (dlgGetHexValue(m_hWnd, EDT_LENGTH, tval) && ((u2)tval != lv->get_length()))
	{
		lv->set_length((u2)tval);
		modified = true;
	}

	tval = (int)GetDlgItemInt(EDT_INDEX, 0, FALSE);
	if ((u2)tval != lv->get_index())
	{
		lv->set_index((u2)tval);
		modified = true;
	}

	wchar_t tstr[0x100] = { 0 };
	GetDlgItemText(EDT_NAME, tstr, 0x100);
	u2 tu2 = getStringIdxFromCP(dj.clf->get_constant_pool(), tstr, *dj.logptr);
	if ((0 != tu2) && (tu2 != lv->get_name_index()))
	{
		lv->set_name_index(tu2);
		modified = true;
	}

	GetDlgItemText(EDT_DESCRIPTOR, tstr, 0x100);
	tu2 = getStringIdxFromCP(dj.clf->get_constant_pool(), tstr, *dj.logptr);
	if ((0 != tu2) && (tu2 != lv->get_descriptor_index()))
	{
		lv->set_descriptor_index(tu2);
		modified = true;
	}

	if (modified)
		dj.clf->setModified();
}

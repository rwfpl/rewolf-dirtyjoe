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

#include "dlgEditInnerClasses.h"
#include "ui_helpers.h"
#include "../java_support/helpers.h"
#include "dlgEditUx.h"
#include "dlgEditFlags.h"

LRESULT CDlgEditInnerClasses::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_INNERCLS), lbSubClass, 0x29f, 0);
	updateList();
	SendDlgItemMessage(LB_INNERCLS, LB_SETCURSEL, 0, 0);
	updateInfo();
	return TRUE;
}

LRESULT CDlgEditInnerClasses::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditInnerClasses::OnLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	updateInfo();
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnLB_UserRButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), innerclasses_menu);
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnBtn_ICAccFlags(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editICField(id_inner_class_access_flags);
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnBtn_ICNameIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editICField(id_inner_name_index);
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnBtn_ICInfoIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editICField(id_inner_class_info_index);
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnBtn_OCInfoIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editICField(id_outer_class_info_index);
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnMenu_AddInnerClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	std::vector<inner_class*>* icv = getICTable();
	if (icv)
	{
		inner_class* nic = new (std::nothrow) inner_class(0, 0, 0, 0, dj.clf->get_constant_pool(), *dj.logptr);
		if (nic != 0)
		{
			icv->push_back(nic);
			dj.clf->setModified();
			updateList();
			LRESULT lr = SendDlgItemMessage(LB_INNERCLS, LB_GETCOUNT, 0, 0);
			SendDlgItemMessage(LB_INNERCLS, LB_SETCURSEL, lr - 1, 0);
			updateInfo();
		}
	}
	return FALSE;
}

LRESULT CDlgEditInnerClasses::OnMenu_DeleteInnerClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DWORD sel = 0;
	std::vector<inner_class*>* icv = getICTable(&sel);
	if (icv)
	{
		icv->erase(icv->begin() + sel);
		dj.clf->setModified();
		updateList();
		updateInfo();
	}
	return FALSE;
}

void CDlgEditInnerClasses::updateList()
{
	SendDlgItemMessage(LB_INNERCLS, WM_SETREDRAW, FALSE, 0);
	LRESULT csel = SendDlgItemMessage(LB_INNERCLS, LB_GETCURSEL, 0, 0);
	SendDlgItemMessage(LB_INNERCLS, LB_RESETCONTENT, 0, 0);

	for (u4 i = 0; i < ic.get_table().size(); i++)
	{
		malloc_ptr<wchar_t> cnm = genStringFromCP(dj.clf->get_constant_pool(), ic.get_table()[i]->get_inner_name_index(), false);
		if (nullptr != cnm)
			SendDlgItemMessage(LB_INNERCLS, LB_ADDSTRING, 0, (LPARAM)cnm.get());
		else
			SendDlgItemMessage(LB_INNERCLS, LB_ADDSTRING, 0, (LPARAM)L"NULL");
	}
	SendDlgItemMessage(LB_INNERCLS, LB_SETCURSEL, csel, 0);
	SendDlgItemMessage(LB_INNERCLS, WM_SETREDRAW, TRUE, 0);
}

void CDlgEditInnerClasses::setItemText(u2 cpid, int ctrl)
{
	if (cpid)
		setDlgItemTextWChar(m_hWnd, ctrl, WM_SETTEXT, genStringFromCP(dj.clf->get_constant_pool(), cpid, false).get());
	else
		SetDlgItemText(ctrl, L"NULL");
}

std::vector<inner_class*>* CDlgEditInnerClasses::getICTable(DWORD* sel)
{
	if (sel)
	{
		LRESULT csel = ::SendMessage(GetDlgItem(LB_INNERCLS), LB_GETCURSEL, 0, 0);
		if (csel == LB_ERR)
			return 0;

		if ((size_t)csel >= ic.get_table().size())
			return 0;

		*sel = (DWORD)csel;
	}

	return &ic.get_table();
}

inner_class* CDlgEditInnerClasses::getIC()
{
	DWORD csel = 0;
	std::vector<inner_class*>* rt = getICTable(&csel);
	if (rt == 0)
		return 0;
	return (*rt)[csel];
}

void CDlgEditInnerClasses::editICField(CDlgEditInnerClasses::editIC_ID id)
{
	inner_class* _ic = getIC();
	if (_ic == 0)
		return;

	int dlgID;
	CDlgEditUx::TYPE x = CDlgEditUx::OVREDIT;
	u2 cpVal = 0;
	switch (id)
	{
	case id_inner_class_info_index:
		dlgID = EDT_IC_INFO_INDEX;
		cpVal = _ic->get_inner_class_info_index();
		break;
	case id_outer_class_info_index:
		dlgID = EDT_OC_INFO_INDEX;
		cpVal = _ic->get_outer_class_info_index();
		break;
	case id_inner_name_index:
		dlgID = EDT_IC_NAME_INDEX;
		cpVal = _ic->get_inner_name_index();
		x = CDlgEditUx::UTF8;
		break;
	case id_inner_class_access_flags:
		dlgID = EDT_IC_ACC_FLAGS;
		break;
	default:
		return;
	}

	INT_PTR dlgRes = 0;
	u2 dlgRes2 = 0;
	if (id == id_inner_class_access_flags)
	{
		CDlgEditFlags dlg(dj, CDlgEditFlags::INNER_CLASS, _ic->get_inner_class_access_flags());
		dlgRes = dlg.DoModal(m_hWnd);
		dlgRes2 = dlg.getRFlags();
	}
	else
	{
		CDlgEditUx dlg(dj, x, cpVal);
		dlgRes = dlg.DoModal(m_hWnd);
		dlgRes2 = dlg.getCPIdx();
	}

	if (dlgRes)
	{
		switch (id)
		{
		case id_inner_class_info_index: 
			{
				_ic->set_inner_class_info_index(dlgRes2); 
				dj.clf->setModified();
				break;
			}
		case id_outer_class_info_index: 
			{
				_ic->set_outer_class_info_index(dlgRes2); 
				dj.clf->setModified();
				break;
			}
		case id_inner_name_index: 
			{
				_ic->set_inner_name_index(dlgRes2); 
				dj.clf->setModified();
				updateList();
			}
			break;
		case id_inner_class_access_flags: 
			{
				_ic->set_inner_class_access_flags(dlgRes2); 
				dj.clf->setModified();
				break;
			}
		}
		if (id == id_inner_class_access_flags)
			setDlgItemTextWChar(m_hWnd, EDT_IC_ACC_FLAGS, WM_SETTEXT, genWStringFromAccessFlags(dlgRes2, false).get());
		else
			setItemText(dlgRes2, dlgID);
	}
}

void CDlgEditInnerClasses::updateInfo()
{
	inner_class* _ic = getIC();
	if (_ic == 0)
	{
		SetDlgItemText(EDT_IC_ACC_FLAGS, L"");
		SetDlgItemText(EDT_IC_INFO_INDEX, L"");
		SetDlgItemText(EDT_IC_NAME_INDEX, L"");
		SetDlgItemText(EDT_OC_INFO_INDEX, L"");
		return;
	}
	setItemText(_ic->get_inner_class_info_index(), EDT_IC_INFO_INDEX);
	setItemText(_ic->get_outer_class_info_index(), EDT_OC_INFO_INDEX);
	setItemText(_ic->get_inner_name_index(), EDT_IC_NAME_INDEX);
	setDlgItemTextWChar(m_hWnd, EDT_IC_ACC_FLAGS, WM_SETTEXT, genWStringFromAccessFlags(_ic->get_inner_class_access_flags(), false).get());
}

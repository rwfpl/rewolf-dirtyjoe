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

#include "dlgTabFM.h"
#include "../java_support/helpers.h"
#include "dlgEditUx.h"
#include "dlgEditFlags.h"
#include "dlgEditRef.h"
#include "attributesWnd.h"
#include "dlgMain.h"
#include "../common/common_macros.h"

bool CDlgTabFM::updateXInfo()
{
	if (dj.clf != 0)
	{
		if (tp == FIELDS)
			x_info = &dj.clf->get_fields();
		else if (tp == METHODS)
			x_info = &dj.clf->get_methods();
		return true;
	}
	else
		x_info = 0;

	return false;
}

field_info* CDlgTabFM::getCurrentFI()
{
	if (nullptr == x_info)
		return nullptr;

	LRESULT sel = SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0, 0);
	if (LB_ERR == sel)
		return nullptr;
	
	if ((size_t)sel >= x_info->size())
		return nullptr;
	
	return (*x_info)[sel];
}

void CDlgTabFM::updateInfo(field_info& fi, bool method)
{
	CONSTANT_Utf8_info* nameUtf8 = getUtf8FromCP(fi.get_name_index(), dj.clf->get_constant_pool());
	CONSTANT_Utf8_info* descUtf8 = getUtf8FromCP(fi.get_descriptor_index(), dj.clf->get_constant_pool());
	malloc_ptr<wchar_t> acc_wstr = genWStringFromAccessFlags(fi.get_access_flags(), method);
	malloc_ptr<wchar_t> name_wstr = preprocessUtf8ToWChar(nameUtf8);
	malloc_ptr<wchar_t> desc_wstr = preprocessUtf8ToWChar(descUtf8);
	setDlgItemTextWChar(subTab, IDC_EDIT_NAME, WM_SETTEXT, name_wstr.get());
	setDlgItemTextWChar(subTab, IDC_EDIT_DESCRIPTOR, WM_SETTEXT, desc_wstr.get());
	setDlgItemTextWChar(subTab, IDC_EDIT_FLAGS, WM_SETTEXT, acc_wstr.get());
	setDlgItemTextWChar(subTab, IDC_EDIT_DEMANGLED, WM_SETTEXT, decodeDescriptor(desc_wstr.get(), name_wstr.get(), acc_wstr.get()).get());
}

bool CDlgTabFM::isMethod()
{
	return (tp == METHODS) ? true : false;
}

void CDlgTabFM::updateList(bool setLastSel)
{
	LRESULT sel = SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0, 0);
	if (sel == LB_ERR)
		sel = 0;

	LRESULT tpi = SendDlgItemMessage(CPTAB_LB, LB_GETTOPINDEX, 0, 0);
	SendDlgItemMessage(CPTAB_LB, WM_SETREDRAW, FALSE, 0);
	SendDlgItemMessage(CPTAB_LB, LB_RESETCONTENT, 0, 0);

	for (size_t i = 0; i < x_info->size(); i++)
	{
		CONSTANT_Utf8_info* fname = getUtf8FromCP((*x_info)[i]->get_name_index(), dj.clf->get_constant_pool());
		setDlgItemTextUtf8(m_hWnd, CPTAB_LB, LB_ADDSTRING, fname);
	}

	SendDlgItemMessage(CPTAB_LB, LB_SETTOPINDEX, tpi, 0);
	SendDlgItemMessage(CPTAB_LB, LB_SETCURSEL, (setLastSel) ? x_info->size() - 1 : sel, 0);
	SendDlgItemMessage(CPTAB_LB, WM_SETREDRAW, TRUE, 0);
}

LRESULT CDlgTabFM::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	subTab.Create(m_hWnd);
	::ShowWindow(subTab, SW_SHOW);
	SetWindowSubclass(GetDlgItem(CPTAB_LB), lbSubClass, 0x2a1, (DWORD_PTR)&subTab.sc);
	return TRUE;
}

LRESULT CDlgTabFM::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	::EndDialog(subTab, 0);
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgTabFM::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	::MoveWindow(GetDlgItem(CPTAB_LB), 0, 0, LOWORD(lParam), HIWORD(lParam) / 3, 0);
	::MoveWindow(subTab, 0, HIWORD(lParam) / 3 + 4, LOWORD(lParam), 2 * HIWORD(lParam) / 3 - 4, 0);
	return FALSE;
}

LRESULT CDlgTabFM::OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (updateXInfo())
	{
		updateList();
		SendMessage(WM_COMMAND, MAKEWPARAM(CPTAB_LB, LBN_SELCHANGE), (LPARAM)(HWND)GetDlgItem(CPTAB_LB));
	}
	else
		clearData();

	return TRUE;
}

LRESULT CDlgTabFM::OnShowAttrib(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!updateXInfo())
		return FALSE;

	::SendDlgItemMessage(subTab, LB_ATTRIBS_PROPS, WM_SETREDRAW, FALSE, 0);
	::SendDlgItemMessage(subTab, LB_ATTRIBS_PROPS, LB_RESETCONTENT, 0, 0);
	LRESULT curSel = SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0 , 0);
	if ((curSel != LB_ERR) && (wParam != LB_ERR) && (wParam < (*x_info)[curSel]->get_attributes().size()))
	{
		showAttributeWindow((*x_info)[curSel]->get_attributes()[wParam], dj.clf->get_constant_pool(), subTab, LB_ATTRIBS_PROPS, LB_ADDSTRING);
	}
	::SendDlgItemMessage(subTab, LB_ATTRIBS_PROPS, WM_SETREDRAW, TRUE, 0);
	return FALSE;
}

LRESULT CDlgTabFM::OnShowAttrWnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	editAttribute();
	return FALSE;
}

LRESULT CDlgTabFM::OnEditFM(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!updateXInfo())
		return FALSE;

	DWORD curSel = (DWORD)SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0 , 0);
	if ((curSel == LB_ERR) || (curSel >= x_info->size()))
		return FALSE;

	if (wParam == EDIT_FM_DESCRIPTOR)
	{
		CDlgEditUx dlg(dj, CDlgEditUx::UTF8, (*x_info)[curSel]->get_descriptor_index());
		if (dlg.DoModal(m_hWnd))
		{
			(*x_info)[curSel]->set_descriptor_index(dlg.getCPIdx());
			dj.clf->setModified();
			updateInfo(*(*x_info)[curSel], isMethod());
		}
	}
	else if (wParam == EDIT_FM_FLAGS)
	{
		CDlgEditFlags::TYPE x = CDlgEditFlags::METHOD;
		if (tp == FIELDS)
			x = CDlgEditFlags::FIELD;
		
		CDlgEditFlags dlg(dj, x, (*x_info)[curSel]->get_access_flags());
		if (dlg.DoModal(m_hWnd))
		{
			(*x_info)[curSel]->set_access_flags(dlg.getRFlags());
			dj.clf->setModified();
			updateInfo(*(*x_info)[curSel], isMethod());
		}
	}
	else if (wParam == EDIT_FM_NAME)
	{
		CDlgEditUx dlg(dj, CDlgEditUx::UTF8, (*x_info)[curSel]->get_name_index());
		if (dlg.DoModal(m_hWnd))
		{
			(*x_info)[curSel]->set_name_index(dlg.getCPIdx());
			dj.clf->setModified();
			updateList();
			updateInfo(*(*x_info)[curSel], isMethod());
		}
	}
	dlgMain.updateTitle();
	return FALSE;
}

void CDlgTabFM::clearData(bool clearMainList)
{
	if (clearMainList)
		SendDlgItemMessage(CPTAB_LB, LB_RESETCONTENT, 0, 0);

	::SendDlgItemMessage(subTab, LB_ATTRIBS, LB_RESETCONTENT, 0, 0);
	::SendDlgItemMessage(subTab, LB_ATTRIBS_PROPS, LB_RESETCONTENT, 0, 0);
	::SendDlgItemMessage(subTab, IDC_EDIT_NAME, WM_SETTEXT, 0, (LPARAM)L"");
	::SendDlgItemMessage(subTab, IDC_EDIT_DESCRIPTOR, WM_SETTEXT, 0, (LPARAM)L"");
	::SendDlgItemMessage(subTab, IDC_EDIT_FLAGS, WM_SETTEXT, 0, (LPARAM)L"");
	::SendDlgItemMessage(subTab, IDC_EDIT_DEMANGLED, WM_SETTEXT, 0, (LPARAM)L"");
}

LRESULT CDlgTabFM::OnFMLB_DblClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	editAttribute(true);
	return 0;
}

LRESULT CDlgTabFM::OnFMLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	if (updateXInfo() && x_info->size())
	{
		LRESULT curSel = ::SendMessage(hWndCtl, LB_GETCURSEL, 0, 0);
		if (curSel == LB_ERR)
		{
			clearData(false);
		}
		else if ((size_t)curSel < x_info->size()) 
		{
			updateInfo(*(*x_info)[curSel], isMethod());
			attributesWnd::updateAttributes(::GetDlgItem(subTab, LB_ATTRIBS), (*x_info)[curSel]->get_attributes(), dj.clf->get_constant_pool());
			::SendDlgItemMessage(subTab, LB_ATTRIBS_PROPS, LB_RESETCONTENT, 0, 0);
			if ((*x_info)[curSel]->get_attributes().size())
			{
				::SendDlgItemMessage(subTab, LB_ATTRIBS, LB_SETCURSEL, 0, 0);
				showAttributeWindow((*x_info)[curSel]->get_attributes()[0], dj.clf->get_constant_pool(), subTab, LB_ATTRIBS_PROPS, LB_ADDSTRING);
			}
		}
	}
	else
		clearData();

	return 0;
}

LRESULT CDlgTabFM::OnFMLB_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU),(isMethod()) ? methods_menu : fields_menu);
	return 0;
}

LRESULT CDlgTabFM::OnMenu_MethodAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!updateXInfo())
		return 0;

	field_info* tmp = new (std::nothrow) field_info(0, 0, 0, dj.clf->get_constant_pool(), *dj.logptr);
	if (tmp == 0)
	{
		badAllocMsg(m_hWnd);
		return 0;
	}
	if (tmp->getError())
	{
		delete tmp;
		MessageBox(L"Object creation failure.", L"Error", MB_ICONERROR);
		return 0;
	}
	x_info->push_back(tmp);
	if (CDlgEditRef(dj, (isMethod()) ? CDlgEditRef::Method : CDlgEditRef::Field, (u2)(x_info->size() - 1)).DoModal() != 1)
	{
		x_info->pop_back();
		delete tmp;
		return 0;
	}
	setModified();
	updateList(true);
	SendMessage(WM_COMMAND, MAKEWPARAM(CPTAB_LB, LBN_SELCHANGE), (LPARAM)::GetDlgItem(m_hWnd, CPTAB_LB));
	return 0;
}

LRESULT CDlgTabFM::OnMenu_MethodDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT crIdx = SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0, 0);
	if (crIdx == LB_ERR)
		return 0;

	if (!updateXInfo())
		return 0;

	if ((size_t)crIdx >= x_info->size())
		return 0;

	x_info->erase(x_info->begin() + crIdx);
	setModified();
	updateList();
	SendMessage(WM_COMMAND, MAKEWPARAM(CPTAB_LB, LBN_SELCHANGE), (LPARAM)::GetDlgItem(m_hWnd, CPTAB_LB));
	return 0;
}

LRESULT CDlgTabFM::OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), attributes_menu);
	return 0;
}

LRESULT CDlgTabFM::OnAttrProps_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), attrib_props_menu);
	return 0;
}

LRESULT CDlgTabFM::OnMenu_Attributes_DeleteAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	field_info* fi = getCurrentFI();
	if (0 != fi)
	{
		attributesWnd::deleteCurrentAttribute(::GetDlgItem(subTab, LB_ATTRIBS), fi->get_attributes());
		setModified();
		attributesWnd::updateAttributes(::GetDlgItem(subTab, LB_ATTRIBS), fi->get_attributes(), dj.clf->get_constant_pool());
	}
	return 0;
}

LRESULT CDlgTabFM::OnMenu_Attributes_AddAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	field_info* fi = getCurrentFI();
	if (0 != fi)
	{
		attributesWnd::addAttribute(m_hWnd, fi->get_attributes(), dj);
		attributesWnd::updateAttributes(::GetDlgItem(subTab, LB_ATTRIBS), fi->get_attributes(), dj.clf->get_constant_pool());
		dlgMain.updateTitle();
	}
	return 0;
}

LRESULT CDlgTabFM::OnMenu_Attributes_ViewAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	field_info* fi = getCurrentFI();
	if (0 != fi)
		attributesWnd::viewAttribute(m_hWnd, ::GetDlgItem(subTab, LB_ATTRIBS), fi->get_attributes(), dj);

	return 0;
}

LRESULT CDlgTabFM::OnMenu_Attributes_EditAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editAttribute();
	return 0;
}

void CDlgTabFM::editAttribute(bool firstAttr/* = false*/)
{
	field_info* fi = getCurrentFI();
	if (nullptr != fi)
	{
		// doesn't need to check for nullptr here
		malloc_ptr<wchar_t> dummyPtr = genStringFromCP(dj.clf->get_constant_pool(), fi->get_name_index(), false);
		dj.funcName = dummyPtr.get();

		if (!firstAttr)
			attributesWnd::editAttribute(m_hWnd, ::GetDlgItem(subTab, LB_ATTRIBS), fi->get_attributes(), dj);
		else
		{
			if (0 != fi->get_attributes().size())
				attributesWnd::editAttribute(m_hWnd, fi->get_attributes()[0], dj);
		}
		dj.funcName = nullptr;
		SendMessage(WM_SHOW_ATTRIB, ::SendDlgItemMessage(subTab, LB_ATTRIBS, LB_GETCURSEL, 0, 0), 0);
		dlgMain.updateTitle();
	}
}

void CDlgTabFM::setLBFont(HFONT hFont)
{
	::SendDlgItemMessage(subTab, LB_ATTRIBS_PROPS, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);
}

void CDlgTabFM::copyHelper(bool selOnly)
{
	malloc_ptr<wchar_t> tmpStr = getLBContent(::GetDlgItem(subTab, LB_ATTRIBS_PROPS), selOnly);
	if (nullptr != tmpStr)
		CopyWCharToClipBoard(m_hWnd, tmpStr.get());
}

LRESULT CDlgTabFM::OnMenu_AttribProps_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyHelper(false);
	return 0;
}

LRESULT CDlgTabFM::OnMenu_AttribProps_CopySelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyHelper(true);
	return 0;
}

void CDlgTabFM::setModified()
{
	dj.clf->setModified();
	dlgMain.updateTitle();
}

LRESULT CDlgTabFM::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (skipNextUserChar)
	{
		skipNextUserChar = false;
		return 1;
	}
	return 0;
}

LRESULT CDlgTabFM::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (subTab.sc.lastWndKeyDown == ::GetDlgItem(subTab, LB_ATTRIBS_PROPS))
		{
			BOOL dummy;
			skipNextUserChar = true;
			switch (wParam)
			{
			case 'C': OnMenu_AttribProps_CopySelection(0, 0, 0, dummy); break;
			case 'A': OnMenu_AttribProps_CopyAll(0, 0, 0, dummy); break;
			default:
				skipNextUserChar = false;
				return 0;
			}
			return 1;
		}
	}
	skipNextUserChar = false;
	return 0;
}

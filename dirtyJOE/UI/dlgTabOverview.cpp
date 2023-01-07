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

#include "dlgTabOverview.h"
#include "../java_support/helpers.h"
#include "dlgMain.h"
#include "dlgAddAttribute.h"
#include "dlgEditUx.h"
#include "dlgEditFlags.h"
#include "dlgEditInnerClasses.h"
#include "dlgEditRef.h"
#include "dlgSimpleLBView.h"
#include "dlgSimpleEditDec.h"
#include "attributesWnd.h"

LRESULT CDlgTabOverview::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_INTERFACES), lbSubClass, 0x29f, 0);
	SetWindowSubclass(GetDlgItem(LB_ATTRIBUTES), lbSubClass, 0x2a0, 0);
	return TRUE;
}

LRESULT CDlgTabOverview::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgTabOverview::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	::MoveWindow(GetDlgItem(IDC_MN_GRP), 0, 0, LOWORD(lParam), HIWORD(lParam), 0);
	return FALSE;
}

LRESULT CDlgTabOverview::OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	#define SETSPECVAL(_id, _fmt, _name) \
		wsprintf(strbuf, _fmt, dj.clf->get_##_name()); \
		SetDlgItemText(_id, strbuf);

	#define SETSPECVAL2(_id, _fmt, _name) \
		wsprintf(strbuf, _fmt, dj.clf->get_##_name().size()); \
		SetDlgItemText(_id, strbuf);

	SendDlgItemMessage(LB_INTERFACES, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(LB_ATTRIBUTES, LB_RESETCONTENT, 0, 0);
	if (dj.clf != 0)
	{
		wchar_t strbuf[0x40];

		SETSPECVAL(IDC_EDIT_MAGIC, L"%08X", magic);
		SETSPECVAL(IDC_EDIT_MINOR, L"%d", minor_version);
		SETSPECVAL(IDC_EDIT_MAJOR, L"%d", major_version);
		SETSPECVAL2(IDC_EDIT_CPC, L"%d", constant_pool);
		SETSPECVAL2(IDC_EDIT_FILC, L"%d", fields);
		SETSPECVAL2(IDC_EDIT_METHC, L"%d", methods);

		updateThisClass();
		updateSuperClass();
		updateAccessFlags();
		updateInterfaces();
		attributesWnd::updateAttributes(GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj.clf->get_constant_pool());
		SetDlgItemInt(IDC_EDIT_ATTC, (UINT)dj.clf->get_attributes().size(), FALSE);
	}
	else
	{
		static const int zeroFields[] =
		{
			IDC_EDIT_MAGIC, IDC_EDIT_MINOR, IDC_EDIT_MAJOR, IDC_EDIT_CPC,
			IDC_EDIT_INTC, IDC_EDIT_FILC, IDC_EDIT_METHC, IDC_EDIT_ATTC,
			IDC_EDIT_THIS, IDC_EDIT_SUPER, IDC_EDIT_ACCF
		};
		for (unsigned int i = 0; i < sizeof(zeroFields)/sizeof(int); i++)
			SetDlgItemText(zeroFields[i], L"");
	}
	dlgMain.updateTitle();

	#undef SETSPECVAL
	#undef SETSPECVAL2
	return TRUE;
}

LRESULT CDlgTabOverview::OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (dj.clf == 0)
		return 0;

	bHandled = FALSE;
	return TRUE;
}

LRESULT CDlgTabOverview::OnMenu_Interfaces_CopyText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT crs = SendDlgItemMessage(LB_INTERFACES, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return 0;

	LRESULT tlen = SendDlgItemMessage(LB_INTERFACES, LB_GETTEXTLEN, crs, 0);
	if (tlen == LB_ERR)
		return 0;

	malloc_ptr<wchar_t> tstr = malloc<wchar_t>(tlen + 1);
	if (nullptr == tstr)
		return 0;

	SendDlgItemMessage(LB_INTERFACES, LB_GETTEXT, crs, (LPARAM)tstr.get());
	CopyWCharToClipBoard(m_hWnd, tstr.get());
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Interfaces_AddInterface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgEditUx dlg(dj, CDlgEditUx::OVREDIT, 0);
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->get_interfaces().push_back(dlg.getCPIdx());
		setModified();
		updateInterfaces();
	}
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Interfaces_EditInterface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editInterface();
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Interfaces_DeleteInterface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT crs = SendDlgItemMessage(LB_INTERFACES, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return 0;

	if (crs >= (LRESULT)dj.clf->get_interfaces().size())
		return 0;

	dj.clf->get_interfaces().erase(dj.clf->get_interfaces().begin() + crs);
	setModified();
	updateInterfaces();
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Attributes_DeleteAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::deleteCurrentAttribute(GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes());
	setModified();
	attributesWnd::updateAttributes(GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj.clf->get_constant_pool());
	SetDlgItemInt(IDC_EDIT_ATTC, (UINT)dj.clf->get_attributes().size(), FALSE);
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Attributes_AddAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::addAttribute(m_hWnd, dj.clf->get_attributes(), dj);
	attributesWnd::updateAttributes(GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj.clf->get_constant_pool());
	SetDlgItemInt(IDC_EDIT_ATTC, (UINT)dj.clf->get_attributes().size(), FALSE);
	::SendMessage(dlgMain.tabConstantPool, WM_UPDATE_TAB, 0, 0);
	dlgMain.updateTitle();
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Attributes_ViewAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::viewAttribute(m_hWnd, GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj);
	return 0;
}

LRESULT CDlgTabOverview::OnMenu_Attributes_EditAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::editAttribute(m_hWnd, GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj);
	dlgMain.updateTitle();
	return 0;
}

LRESULT CDlgTabOverview::OnBtn_EditMinVer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleEditDec dlg(dj.clf->get_minor_version());
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->set_minor_version((u2)dlg.getVal());
		setModified();
		SetDlgItemInt(IDC_EDIT_MINOR, dj.clf->get_minor_version(), FALSE);
	}
	return 0;
}

LRESULT CDlgTabOverview::OnBtn_EditMajVer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleEditDec dlg(dj.clf->get_major_version());
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->set_major_version((u2)dlg.getVal());
		setModified();
		SetDlgItemInt(IDC_EDIT_MAJOR, dj.clf->get_major_version(), FALSE);
	}
	return 0;
}

LRESULT CDlgTabOverview::OnBtn_EditAccessFlags(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgEditFlags dlg(dj, CDlgEditFlags::CLASS, dj.clf->get_access_flags());
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->set_access_flags(dlg.getRFlags());
		setModified();
		updateAccessFlags();
	}
	return 0;
}

LRESULT CDlgTabOverview::OnBtn_EditSuperClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgEditUx dlg(dj, CDlgEditUx::OVREDIT, dj.clf->get_super_class());
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->set_super_class(dlg.getCPIdx());
		setModified();
		updateSuperClass();
	}
	return 0;
}

LRESULT CDlgTabOverview::OnBtn_EditThisClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgEditUx dlg(dj, CDlgEditUx::OVREDIT, dj.clf->get_this_class());
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->set_this_class(dlg.getCPIdx());
		setModified();
		updateThisClass();
	}
	return 0;
}

LRESULT CDlgTabOverview::OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), attributes_menu);
	return 0;
}

LRESULT CDlgTabOverview::OnAttr_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (attributesWnd::isEditableAttribute(GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj))
		attributesWnd::editAttribute(m_hWnd, GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj);
	else
		attributesWnd::viewAttribute(m_hWnd, GetDlgItem(LB_ATTRIBUTES), dj.clf->get_attributes(), dj);
	dlgMain.updateTitle();
	return 0;
}

LRESULT CDlgTabOverview::OnInterfaces_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), interfaces_menu);
	return 0;
}

LRESULT CDlgTabOverview::OnInterfaces_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editInterface();
	return 0;
}

void CDlgTabOverview::updateThisClass()
{
	CONSTANT_Utf8_info* utfstr = getUtf8FromV1(dj.clf->get_this_class(), dj.clf->get_constant_pool());
	setDlgItemTextUtf8(m_hWnd, IDC_EDIT_THIS, WM_SETTEXT, utfstr);
}

void CDlgTabOverview::updateSuperClass()
{
	if (dj.clf->get_super_class())
	{
		CONSTANT_Utf8_info* utfstr = getUtf8FromV1(dj.clf->get_super_class(), dj.clf->get_constant_pool());
		setDlgItemTextUtf8(m_hWnd, IDC_EDIT_SUPER, WM_SETTEXT, utfstr);
	}
	else
		SetDlgItemText(IDC_EDIT_SUPER, L"no super class");
}

void CDlgTabOverview::updateAccessFlags()
{
	setDlgItemTextWChar(m_hWnd, IDC_EDIT_ACCF, WM_SETTEXT, genWStringFromAccessFlags(dj.clf->get_access_flags(), false).get());
}

void CDlgTabOverview::updateInterfaces()
{
	SetDlgItemInt(IDC_EDIT_INTC, (UINT)dj.clf->get_interfaces().size(), FALSE);
	SendDlgItemMessage(LB_INTERFACES, LB_RESETCONTENT, 0, 0);
	for (DWORD i = 0; i < dj.clf->get_interfaces().size(); i++)
	{
		CONSTANT_Utf8_info* clsName = getUtf8FromV1(dj.clf->get_interfaces()[i], dj.clf->get_constant_pool());
		setDlgItemTextUtf8(m_hWnd, LB_INTERFACES, LB_ADDSTRING, clsName);
	}
}

void CDlgTabOverview::editInterface()
{
	LRESULT crs = SendDlgItemMessage(LB_INTERFACES, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return;
	if (crs >= (LRESULT)dj.clf->get_interfaces().size())
		return;

	CDlgEditUx dlg(dj, CDlgEditUx::OVREDIT, dj.clf->get_interfaces()[crs]);
	if (dlg.DoModal(m_hWnd))
	{
		dj.clf->get_interfaces()[crs] = dlg.getCPIdx();
		setModified();
		updateInterfaces();
	}
}

void CDlgTabOverview::setModified()
{
	dj.clf->setModified();
	dlgMain.updateTitle();
}

LRESULT CDlgTabOverview::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (skipNextUserChar)
	{
		skipNextUserChar = false;
		return 1;
	}
	return 0;
}

LRESULT CDlgTabOverview::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		BOOL dummy;
		skipNextUserChar = true;
		switch (wParam)
		{
		case 'C': OnMenu_Interfaces_CopyText(0, 0, 0, dummy); break;
		case 'E': OnMenu_Interfaces_EditInterface(0, 0, 0, dummy); break;
		case 'A': OnMenu_Interfaces_AddInterface(0, 0, 0, dummy); break;
		case 'D': OnMenu_Interfaces_DeleteInterface(0, 0, 0, dummy); break;
		default:
			skipNextUserChar = false;
			return 0;
		}
		return 1;
	}
	skipNextUserChar = false;
	return 0;
}

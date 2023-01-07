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

#include "dlgCode.h"
#include "../java_support/helpers.h"
#include "dlgCodeEditor.h"
#include "dlgSimpleEditDec.h"
#include "attributesWnd.h"
#include "dlgEditException.h"
#include "dlgMain.h"
#include "../common/common_templates.h"

CDlgCode::CDlgCode(CDirtyJOE& _dj, CAttrCode& _ac) : 
	dj(_dj), 
	ac(_ac), 
	codeEd(_dj, *this, _ac),
	stickyWnd(m_hWnd),
	skipNextUserChar(false) {} 

void CDlgCode::refreshExceptions(bool selectLast)
{
	SendDlgItemMessage(LB_EXCS, WM_SETREDRAW, FALSE, 0);
	SendDlgItemMessage(LB_EXCS, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(LB_EXCS, LB_ADDSTRING, 0, (LPARAM)L"Start | End  | Handler | Type");
	size_t maxlen = 0;
	wchar_t tstr[0x200];
	LRESULT lastSel = 0;
	for (DWORD i = 0; i < ac.get_exception_table().size(); i++)
	{
		swprintf_s(tstr, L"%04X  | %04X | %04X    | ", ac.get_exception_table()[i]->get_start_pc(), ac.get_exception_table()[i]->get_end_pc(), ac.get_exception_table()[i]->get_handler_pc());

		if (ac.get_exception_table()[i]->get_catch_type() == 0)
		{
			wcscat_s(tstr, L"any (finally statement)");
		}
		else
		{
			malloc_ptr<wchar_t> catchstr = genStringFromCP(dj.clf->get_constant_pool(), ac.get_exception_table()[i]->get_catch_type());
			if (nullptr == catchstr)
				wcscat_s(tstr, L"error");
			else
			{
				wcscat_s(tstr, catchstr.get());
			}
		}
		if (wcslen(tstr) > maxlen)
			maxlen = wcslen(tstr);
		lastSel = SendDlgItemMessage(LB_EXCS, LB_ADDSTRING, 0, (WPARAM)tstr);
	}
	for (u4 i = 0; i < maxlen; i++)
		tstr[i] = '-';
	tstr[maxlen] = 0;
	SendDlgItemMessage(LB_EXCS, LB_INSERTSTRING, 1, (LPARAM)tstr);

	if (selectLast)
		SendDlgItemMessage(LB_EXCS, LB_SETCURSEL, lastSel + 1, 0);

	SendDlgItemMessage(LB_EXCS, WM_SETREDRAW, TRUE, 0);
}

LRESULT CDlgCode::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_ATTRS), lbSubClass, 0x2a1, (DWORD_PTR)&sc);
	SetWindowSubclass(GetDlgItem(LB_EXCS), lbSubClass, 0x2a2, (DWORD_PTR)&sc);

	SendDlgItemMessage(LB_EXCS, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);

	SetDlgItemInt(IDC_EDIT_STACK, ac.get_max_stack(), FALSE);
	SetDlgItemInt(IDC_EDIT_LOCALS, ac.get_max_locals(), FALSE);
	SetDlgItemInt(IDC_EDIT_CODE_SIZE, (UINT)ac.get_code().size(), FALSE);
	SetDlgItemInt(IDC_EDIT_ATTRS, (UINT)ac.get_attributes().size(), FALSE);
	SetDlgItemInt(IDC_EDIT_EXCS, (UINT)ac.get_exception_table().size(), FALSE);

	dj.dlgMain->dlgCurrentCodeWindow = this;
	codeEd.Create(m_hWnd);
	::ShowWindow(codeEd, SW_SHOW);

	refreshExceptions();
	attributesWnd::updateAttributes(GetDlgItem(LB_ATTRS), ac.get_attributes(), dj.clf->get_constant_pool());

	RECT rt = { 0 };
	GetClientRect(&rt);
	::MoveWindow(codeEd, 10, 144, rt.right - rt.left - 20, rt.bottom - rt.top - 154, TRUE);

	if (nullptr != dj.funcName)
	{
		malloc_ptr<wchar_t> wndTitle = wcscat_var(L"Code editor - ", dj.funcName, 0);
		if (nullptr != wndTitle)
			SetWindowText(wndTitle.get());
	}

	return TRUE;
}

LRESULT CDlgCode::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	dj.dlgMain->dlgCurrentCodeWindow = 0;
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgCode::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT lbrt;
	::GetWindowRect(GetDlgItem(LB_EXCS), &lbrt);
	::MoveWindow(GetDlgItem(LB_EXCS), 360, 26, LOWORD(lParam) - 370, lbrt.bottom - lbrt.top, TRUE);
	::MoveWindow(codeEd, 10, 144, LOWORD(lParam) - 20, HIWORD(lParam) - 148, TRUE);
	return FALSE;
}

LRESULT CDlgCode::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 600) 
		rptr->right = rptr->left + 600;
	if (rptr->bottom - rptr->top < 300) 
		rptr->bottom = rptr->top + 300;

	return TRUE;
}

LRESULT CDlgCode::OnRefresh(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	refresh();
	return TRUE;
}

void CDlgCode::refresh()
{
	wchar_t tstr[0x20];
	swprintf_s(tstr, L"%zd", ac.get_code().size());
	SetDlgItemText(IDC_EDIT_CODE_SIZE, tstr);
}

LRESULT CDlgCode::OnBtn_MaxStack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleEditDec dlg(ac.get_max_stack());
	if (dlg.DoModal(m_hWnd))
	{
		ac.set_max_stack((u2)dlg.getVal());
		dj.clf->setModified();
		SetDlgItemInt(IDC_EDIT_STACK, ac.get_max_stack(), FALSE);
	}
	return 0;
}

LRESULT CDlgCode::OnBtn_MaxLocals(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleEditDec dlg(ac.get_max_locals());
	if (dlg.DoModal(m_hWnd))
	{
		ac.set_max_locals((u2)dlg.getVal());
		dj.clf->setModified();
		SetDlgItemInt(IDC_EDIT_LOCALS, ac.get_max_locals(), FALSE);
	}
	return 0;
}

LRESULT CDlgCode::OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), attributes_menu);
	return 0;
}

LRESULT CDlgCode::OnAttr_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (attributesWnd::isEditableAttribute(GetDlgItem(LB_ATTRS), ac.get_attributes(), dj))
		attributesWnd::editAttribute(m_hWnd, GetDlgItem(LB_ATTRS), ac.get_attributes(), dj);
	else
		attributesWnd::viewAttribute(m_hWnd, GetDlgItem(LB_ATTRS), ac.get_attributes(), dj);

	return 0;
}

LRESULT CDlgCode::OnMenu_Attributes_DeleteAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::deleteCurrentAttribute(GetDlgItem(LB_ATTRS), ac.get_attributes());
	dj.clf->setModified();
	attributesWnd::updateAttributes(GetDlgItem(LB_ATTRS), ac.get_attributes(), dj.clf->get_constant_pool());
	SetDlgItemInt(IDC_EDIT_ATTRS, (UINT)ac.get_attributes().size(), FALSE);
	return 0;
}

LRESULT CDlgCode::OnMenu_Attributes_AddAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::addAttribute(m_hWnd, ac.get_attributes(), dj);
	attributesWnd::updateAttributes(GetDlgItem(LB_ATTRS), ac.get_attributes(), dj.clf->get_constant_pool());
	SetDlgItemInt(IDC_EDIT_ATTRS, (UINT)ac.get_attributes().size(), FALSE);
	return 0;
}

LRESULT CDlgCode::OnMenu_Attributes_ViewAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::viewAttribute(m_hWnd, GetDlgItem(LB_ATTRS), ac.get_attributes(), dj);
	return 0;
}

LRESULT CDlgCode::OnMenu_Attributes_EditAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	attributesWnd::editAttribute(m_hWnd, GetDlgItem(LB_ATTRS), ac.get_attributes(), dj);
	return 0;
}

LRESULT CDlgCode::OnExcs_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), code_exc_menu);
	return 0;
}

void CDlgCode::addException(u2 start_pc, u2 end_pc, u2 handler_pc, u2 catch_type)
{
	exception_entry* newex = new (std::nothrow) exception_entry(start_pc, end_pc, handler_pc, catch_type, dj.clf->get_constant_pool(), *dj.logptr);
	if (newex == 0)
	{
		badAllocMsg(m_hWnd);
		return;
	}

	if (CDlgEditException(dj, *newex).DoModal(m_hWnd))
	{
		ac.get_exception_table().push_back(newex);
		SetDlgItemInt(IDC_EDIT_EXCS, (UINT)ac.get_exception_table().size(), FALSE);
		refreshExceptions(true);
	}
	else
		delete newex;
}

LRESULT CDlgCode::OnMenu_Exc_AddException(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	addException(0, 0, 0, 0);
	return 0;
}

LRESULT CDlgCode::OnMenu_Exc_DeleteException(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT sel = SendDlgItemMessage(LB_EXCS, LB_GETCURSEL, 0, 0);
	if ((sel == LB_ERR) || (sel < 2))
		return 0;

	sel -= 2;
	auto& ext = ac.get_exception_table();
	if ((size_t)sel >= ext.size())
		return 0;

	if (ext[sel])
		delete ext[sel];

	ext.erase(ext.begin() + sel);
	SetDlgItemInt(IDC_EDIT_EXCS, (UINT)ext.size(), FALSE);
	refreshExceptions();

	return 0;
}

LRESULT CDlgCode::OnMenu_Exc_EditException(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT sel = SendDlgItemMessage(LB_EXCS, LB_GETCURSEL, 0, 0);
	if ((sel == LB_ERR) || (sel < 2))
		return 0;

	sel -= 2;
	if ((size_t)sel >= ac.get_exception_table().size())
		return 0;

	if (CDlgEditException(dj, *ac.get_exception_table()[sel]).DoModal(m_hWnd))
	{
		refreshExceptions();
		SendDlgItemMessage(LB_EXCS, LB_SETCURSEL, sel + 2, 0);
	}

	return 0;
}

LRESULT CDlgCode::OnMenu_Exc_MoveDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT sel = SendDlgItemMessage(LB_EXCS, LB_GETCURSEL, 0, 0);
	if ((sel == LB_ERR) || (sel < 2))
		return 0;
	
	sel -= 2;
	auto& ext = ac.get_exception_table();
	if ((size_t)sel + 1 == ext.size())
		return 0;

	exception_entry* tex = ext[sel + 1];
	ext[sel + 1] = ext[sel];
	ext[sel] = tex;

	refreshExceptions();
	SendDlgItemMessage(LB_EXCS, LB_SETCURSEL, sel + 3, 0);
	return 0;
}

LRESULT CDlgCode::OnMenu_Exc_MoveUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT sel = SendDlgItemMessage(LB_EXCS, LB_GETCURSEL, 0, 0);
	if ((sel == LB_ERR) || (sel < 3))
		return 0;

	sel -= 2;
	auto& ext = ac.get_exception_table();

	exception_entry* tex = ext[sel - 1];
	ext[sel - 1] = ext[sel];
	ext[sel] = tex;

	refreshExceptions();
	SendDlgItemMessage(LB_EXCS, LB_SETCURSEL, sel + 1, 0);
	return 0;
}

LRESULT CDlgCode::OnMenu_Exc_Copy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	malloc_ptr<wchar_t> tmpStr = getLBContent(GetDlgItem(LB_EXCS), true);
	if (nullptr != tmpStr)
		CopyWCharToClipBoard(m_hWnd, tmpStr.get());
	return 0;
}

LRESULT CDlgCode::OnMenu_Exc_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	malloc_ptr<wchar_t> tmpStr = getLBContent(GetDlgItem(LB_EXCS), false);
	if (nullptr != tmpStr)
		CopyWCharToClipBoard(m_hWnd, tmpStr.get());
	return 0;
}

LRESULT CDlgCode::OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	stickyWnd.stickyUpdate(*(RECT*)lParam);
	return TRUE;
}

LRESULT CDlgCode::OnWindowMoving(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	stickyWnd.stickTry((HWND)wParam, *(RECT*)lParam);
	bHandled = TRUE;
	return TRUE;
}

LRESULT CDlgCode::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (skipNextUserChar)
	{
		skipNextUserChar = false;
		return 1;
	}
	return 0;
}

LRESULT CDlgCode::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		BOOL dummy;
		if (sc.lastWndKeyDown == GetDlgItem(LB_EXCS))
		{
			skipNextUserChar = true;
			switch (wParam)
			{
			case 'E': OnMenu_Exc_EditException(0, 0, 0, dummy); break;
			case 'A': OnMenu_Exc_AddException(0, 0, 0, dummy); break;
			case 'X': OnMenu_Exc_DeleteException(0, 0, 0, dummy); break;
			case 'U': OnMenu_Exc_MoveUp(0, 0, 0, dummy); break;
			case 'D': OnMenu_Exc_MoveDown(0, 0, 0, dummy); break;
			case 'C': OnMenu_Exc_Copy(0, 0, 0, dummy); break;
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

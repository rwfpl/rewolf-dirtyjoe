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

#include "dlgTabConstantPool.h"
#include "../java_support/helpers.h"
#include "dlgMain.h"
#include "dlgAddCPItem.h"
#include "dlgEditUx.h"
#include "dlgEditRef.h"
#include "dlgEditUtf8.h"
#include "dlgEditVal.h"
#include "dlgSimpleLBView.h"
#include "dlgPythonStringDecrypt.h"
#include "dlgPythonMultiDecrypter.h"

const CDlgTabConstantPool::t_typetab CDlgTabConstantPool::typetab[] =
{
	{ 0, L"All Items"},
	{ CONSTANT_Utf8, CONSTANT_Utf8_STR },
	{ CONSTANT_Integer, CONSTANT_Integer_STR },
	{ CONSTANT_Float, CONSTANT_Float_STR },
	{ CONSTANT_Long, CONSTANT_Long_STR },
	{ CONSTANT_Double, CONSTANT_Double_STR },
	{ CONSTANT_Class, CONSTANT_Class_STR },
	{ CONSTANT_String, CONSTANT_String_STR },
	{ CONSTANT_Fieldref, CONSTANT_Fieldref_STR },
	{ CONSTANT_Methodref, CONSTANT_Methodref_STR },
	{ CONSTANT_InterfaceMethodref, CONSTANT_InterfaceMethodref_STR },
	{ CONSTANT_NameAndType, CONSTANT_NameAndType_STR },
	{ CONSTANT_MethodHandle, CONSTANT_MethodHandle_STR },
	{ CONSTANT_MethodType, CONSTANT_MethodType_STR },
	{ CONSTANT_InvokeDynamic, CONSTANT_InvokeDynamic_STR }
};

int CDlgTabConstantPool::getCPSelection()
{
	LRESULT sel = SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0, 0);
	if (sel == LB_ERR)
		return -1;
	if ((size_t)sel >= cpMap.size())
		return -1;
	//sel = cpMap[(u2)sel];
	if (cpMap[(u2)sel] >= dj.clf->get_constant_pool().size())
		return -1;
	return cpMap[(u2)sel];
}

void CDlgTabConstantPool::editItem()
{
	int sel = getCPSelection();
	if (sel == -1)
		return;

	switch (dj.clf->get_constant_pool()[sel]->tag)
	{
		case CONSTANT_Utf8:
		{
			CDlgEditUtf8 dlg(dj, (u2)sel);
			CONSTANT_Utf8_info* nutf = (CONSTANT_Utf8_info*)dlg.DoModal(m_hWnd);
			if (nutf)
			{
				delete dj.clf->get_constant_pool()[sel];
				dj.clf->get_constant_pool()[sel] = nutf;
				setModified();
			}
		}
		break;
		case CONSTANT_Class:
		case CONSTANT_String:
		case CONSTANT_MethodType:
		{
			CDlgEditUx dlg(dj, CDlgEditUx::CPEDIT, (u2)sel);
			if (dlg.DoModal(m_hWnd))
			{
				((CONSTANT_uX_info<u2>*)dj.clf->get_constant_pool()[sel])->v1 = dlg.getCPIdx();
				setModified();
			}
		}
		break;
		case CONSTANT_Long:
		case CONSTANT_Integer:
		case CONSTANT_Double:
		case CONSTANT_Float:
		{
			CDlgEditVal(dj, (u2)sel).DoModal(m_hWnd);
		}
		break;
		case CONSTANT_Methodref:
		case CONSTANT_Fieldref:
		case CONSTANT_InterfaceMethodref:
		case CONSTANT_NameAndType:
		case CONSTANT_MethodHandle:
		case CONSTANT_InvokeDynamic:
		{
			CDlgEditRef(dj, CDlgEditRef::ConstantPool, (u2)sel).DoModal(m_hWnd);
		}
		break;
	}
	SendMessage(WM_UPDATE_TAB, 0, 0);
	::SendMessage(dlgMain.tabOverView, WM_UPDATE_TAB, 0, 0);
	dlgMain.updateTitle();
}

void CDlgTabConstantPool::addItem()
{
	const INT_PTR res = CDlgAddCPItem(dj).DoModal(m_hWnd);
	SendMessage(WM_UPDATE_TAB, 0, 0);
	::SendMessage(dlgMain.tabOverView, WM_UPDATE_TAB, 0, 0);
	const u1 selFilter = getSelectedFilterTag();
	if ((0 != res) && ((selFilter == res) || (0 == selFilter)))
	{
		LRESULT curCnt = SendDlgItemMessage(CPTAB_LB, LB_GETCOUNT, 0, 0);
		if (LB_ERR != curCnt)
			SendDlgItemMessage(CPTAB_LB, LB_SETCURSEL, curCnt - 1, 0);
	}
	dlgMain.updateTitle();
}

void CDlgTabConstantPool::showRefs()
{
	u2 xt = (u2)getCPSelection();
	if (xt == -1)
		return;
	CDlgSimpleLBView(dj, CDlgSimpleLBView::References, xt).DoModal(m_hWnd);
}

void CDlgTabConstantPool::copyText()
{
	int sel = getCPSelection();
	if (sel == -1)
		return;
	malloc_ptr<wchar_t> cpstr = genStringFromCP(dj.clf->get_constant_pool(), (u2)sel);
	if (nullptr == cpstr)
		return;
	CopyWCharToClipBoard(m_hWnd, cpstr.get());
}

LRESULT CDlgTabConstantPool::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(CPTAB_LB), lbSubClass, 0x29f, 0);

	for (int i = 0; i < _countof(typetab); i++)
	{
		SendDlgItemMessage(CMB_CP_FILTER, CB_ADDSTRING, 0, (LPARAM)typetab[i].tagname);
	}
	SendDlgItemMessage(CMB_CP_FILTER, CB_SETCURSEL, 0, 0);

	CheckDlgButton(CHK_HEXIDX, dj.settings.get_CPHexIdx() ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(CHK_SRCHCASE, dj.settings.get_CPCaseSensitive() ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;
}

LRESULT CDlgTabConstantPool::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgTabConstantPool::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	::MoveWindow(GetDlgItem(CPTAB_LB), 0, 40, LOWORD(lParam), HIWORD(lParam) - 40, 0);
	RECT parent_rt;
	RECT combo_rt;
	RECT txt_rt;
	RECT txt_srch_rt;
	HWND combo_hwnd = GetDlgItem(CMB_CP_FILTER);
	HWND txt_hwnd = GetDlgItem(TXT_OBJ_FILTER);
	HWND txt_srch_hwnd = GetDlgItem(TXT_SEARCH);
	GetWindowRect(&parent_rt);
	::GetWindowRect(combo_hwnd, &combo_rt);
	::GetWindowRect(txt_hwnd, &txt_rt);
	::GetWindowRect(txt_srch_hwnd, &txt_srch_rt);

	int height = combo_rt.bottom - combo_rt.top;
	int width = combo_rt.right - combo_rt.left;
	::MoveWindow(combo_hwnd, LOWORD(lParam) - width, combo_rt.top - parent_rt.top, width, height, TRUE);
	::MoveWindow(txt_hwnd, LOWORD(lParam) - width, txt_rt.top - parent_rt.top, txt_rt.right - txt_rt.left, txt_rt.bottom - txt_rt.top, TRUE);

	::MoveWindow(GetDlgItem(EDT_SEARCH), LOWORD(lParam) - 2*width - grid_space, combo_rt.top - parent_rt.top, width, height, TRUE);
	::MoveWindow(txt_srch_hwnd, LOWORD(lParam) - 2*width - grid_space, txt_srch_rt.top - parent_rt.top, txt_srch_rt.right - txt_srch_rt.left, txt_srch_rt.bottom - txt_srch_rt.top, TRUE);

	return FALSE;
}

u1 CDlgTabConstantPool::getSelectedFilterTag()
{
	u1 type = (u1)SendDlgItemMessage(CMB_CP_FILTER, CB_GETCURSEL, 0, 0);
	if (type >= _countof(typetab))
		return 0;
	else
		return typetab[type].tagid;
}

LRESULT CDlgTabConstantPool::OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DWORD sel = (DWORD)SendDlgItemMessage(CPTAB_LB, LB_GETCURSEL, 0, 0);
	DWORD tpi = (DWORD)SendDlgItemMessage(CPTAB_LB, LB_GETTOPINDEX, 0, 0);
	
	u1 type = getSelectedFilterTag();
	
	//get search filter
	unsigned int olen = 0;
	malloc_ptr<wchar_t> str_filter = getWindowText(GetDlgItem(EDT_SEARCH), &olen);
	if ((nullptr != str_filter) && !dj.settings.get_CPCaseSensitive())
		_wcslwr_s(str_filter.get(), olen + 1);
	
	SendDlgItemMessage(CPTAB_LB, WM_SETREDRAW, FALSE, 0);
	SendDlgItemMessage(CPTAB_LB, LB_RESETCONTENT, 0, 0);
	cpMap.clear();
	if (dj.clf != 0)
	{
		u1 prevTag = 0;
		for (DWORD i = 1; i < dj.clf->get_constant_pool().size(); i++)
		{
			if ((prevTag != CONSTANT_Double) && (prevTag != CONSTANT_Long))
			{
				prevTag = dj.clf->get_constant_pool()[i]->tag;
				if ((type != 0) && (type != prevTag))
					continue;

				malloc_ptr<wchar_t> tmpstr = genStringFromCP(dj.clf->get_constant_pool(), i, dj.clf->get_constant_pool()[i], true, dj.settings.get_CPHexIdx());
				const wchar_t* const newstr = (tmpstr != nullptr) ? tmpstr.get() : strError;

				// this is rather lame approach at preventing ListBox to render really long unicode strings (it's painfully slow)
				const size_t newstr_len = wcslen(newstr);
				if (newstr_len > 0x200)
					tmpstr.get()[0x200] = 0;
				
				// apply search filter
				bool addString = false;
				if ((nullptr != str_filter) && wcslen(str_filter.get()))
				{
					if (dj.settings.get_CPCaseSensitive())
					{
						if (wcsstr(newstr, str_filter.get()))
							addString = true;
					}
					else
					{
						malloc_ptr<wchar_t> tmpdup(_wcsdup(newstr));
						if (nullptr != tmpdup)
						{
							_wcslwr_s(tmpdup.get(), newstr_len + 1);
							if (wcsstr(tmpdup.get(), str_filter.get()))
								addString = true;
						}
					}
				}
				else
					addString = true;

				if (addString)
				{
					u2 idx = (u2)SendDlgItemMessage(CPTAB_LB, LB_ADDSTRING, 0, (LPARAM)newstr);
					cpMap[idx] = (u2)i;
				}
			}
			else
				prevTag = 0;
		}
		SendDlgItemMessage(CPTAB_LB, LB_SETTOPINDEX, tpi, 0);
		SendDlgItemMessage(CPTAB_LB, LB_SETCURSEL, sel, 0);
	}
	SendDlgItemMessage(CPTAB_LB, WM_SETREDRAW, TRUE, 0);
	dlgMain.updateTitle();
	return TRUE;
}

LRESULT CDlgTabConstantPool::OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (dj.clf == 0)
		return 0;

	bHandled = FALSE;
	return TRUE;
}

LRESULT CDlgTabConstantPool::OnChk_CaseSens(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	dj.settings.set_CPCaseSensitive(IsDlgButtonChecked(CHK_SRCHCASE) == BST_CHECKED);
	BOOL dummy;
	OnUpdateTab(0, 0, 0, dummy);
	return 0;
}

LRESULT CDlgTabConstantPool::OnChk_HexIdx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	dj.settings.set_CPHexIdx(IsDlgButtonChecked(CHK_HEXIDX) == BST_CHECKED);
	BOOL dummy;
	OnUpdateTab(0, 0, 0, dummy);
	return 0;
}

LRESULT CDlgTabConstantPool::OnBtn_Edit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editItem();
	return 0;
}

LRESULT CDlgTabConstantPool::OnBtn_Add(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	addItem();
	return 0;
}

LRESULT CDlgTabConstantPool::OnBtn_ShowReferences(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	showRefs();
	return 0;
}

LRESULT CDlgTabConstantPool::OnMenu_EditItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editItem();
	return 0;
}

LRESULT CDlgTabConstantPool::OnMenu_AddItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	addItem();
	return 0;
}

LRESULT CDlgTabConstantPool::OnMenu_ShowReferences(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	showRefs();
	return 0;
}

LRESULT CDlgTabConstantPool::OnMenu_CopyText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyText();
	return 0;
}

void CDlgTabConstantPool::menuCallback(HMENU hMenu, void* obj)
{
	CDlgTabConstantPool& _this = *(CDlgTabConstantPool*)obj;

	bool disableSinglePythonMenu = false;
	bool disableMultiPythonMenu = false;

	int sel = _this.getCPSelection();
	if (sel == -1)
		return;
	
	if ((CONSTANT_String != _this.dj.clf->get_constant_pool()[sel]->tag) &&
		(CONSTANT_Utf8 != _this.dj.clf->get_constant_pool()[sel]->tag))
		disableSinglePythonMenu = true;

	if (CDJSettings::PyNone == _this.dj.settings.getPythonVersion())
	{
		disableSinglePythonMenu = true;
		disableMultiPythonMenu = true;
	}

	if (disableSinglePythonMenu)
		EnableMenuItem(hMenu, ID_RUNPYTHONSCRIPT, MF_GRAYED);

	if (disableMultiPythonMenu)
		EnableMenuItem(hMenu, ID_RUNPYTHONSCRIPTONALLUTF8OBJECTS, MF_GRAYED);
}

LRESULT CDlgTabConstantPool::OnMenu_RunPythonScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int sel = getCPSelection();
	if (sel == -1)
		return 0;

	JavaConstPool cp = dj.clf->get_constant_pool();
	
	if (nullptr == cp[sel])
		return 0;

	CONSTANT_Utf8_info* utf8temp = 0;
	if (CONSTANT_Utf8 == cp[sel]->tag)
		utf8temp = (CONSTANT_Utf8_info*)cp[sel];
	else if (CONSTANT_String == cp[sel]->tag)
	{
		CONSTANT_String_info* strtmp = (CONSTANT_String_info*)cp[sel];
		if (strtmp->v1 >= cp.size())
			return 0;
		if (nullptr == cp[strtmp->v1])
			return 0;
		if (CONSTANT_Utf8 != cp[strtmp->v1]->tag)
			return 0;
		utf8temp = (CONSTANT_Utf8_info*)cp[strtmp->v1];
	}
	else
		return 0;

	if (CDlgPythonStringDecrypt(dj, *utf8temp).DoModal(m_hWnd))
		SendMessage(WM_UPDATE_TAB, 0, 0);

	return 0;
}

LRESULT CDlgTabConstantPool::OnMenu_RunPyOnUtf8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (CDlgPythonMultiDecrypt(dj).DoModal(m_hWnd))
		SendMessage(WM_UPDATE_TAB, 0, 0);

	return 0;
}

LRESULT CDlgTabConstantPool::OnCPLB_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), cp_menu, this, menuCallback);
	return 0;
}

LRESULT CDlgTabConstantPool::OnCPLB_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	editItem();
	return 0;
}

LRESULT CDlgTabConstantPool::OnFilter_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL dummy;
	OnUpdateTab(0, 0, 0, dummy);
	return 0;
}

LRESULT CDlgTabConstantPool::OnSearch_Change(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL dummy;
	OnUpdateTab(0, 0, 0, dummy);
	return 0;
}

void CDlgTabConstantPool::setModified()
{
	dj.clf->setModified();
	dlgMain.updateTitle();
}

LRESULT CDlgTabConstantPool::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (skipNextUserChar)
	{
		skipNextUserChar = false;
		return 1;
	}
	return 0;
}

LRESULT CDlgTabConstantPool::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		BOOL dummy;
		skipNextUserChar = true;
		switch (wParam)
		{
		case 'C': OnMenu_CopyText(0, 0, 0, dummy); break;
		case 'A': OnMenu_AddItem(0, 0, 0, dummy); break;
		case 'E': OnMenu_EditItem(0, 0, 0, dummy); break;
		default:
			skipNextUserChar = false;
			return 0;
		}
		return 1;
	}
	skipNextUserChar = false;
	return 0;
}

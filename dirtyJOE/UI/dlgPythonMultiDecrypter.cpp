/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/
#include "dlgPythonMultiDecrypter.h"
#include "../java_support/helpers.h"
#include "dlgPythonMultiPreview.h"

void CDlgPythonMultiDecrypt::initCommon()
{
	RECT tmpR;
	GetClientRect(&tmpR);
	SendMessage(WM_SIZE, 0, MAKELPARAM(tmpR.right - tmpR.left, tmpR.bottom - tmpR.top));

	const int width = (tmpR.right - tmpR.left - 104) / 2;
	listHwnd = GetDlgItem(LIST_UTF8);
	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.pszText = L"Input string";
	lvc.cx = width;
	ListView_InsertColumn(listHwnd, 0, &lvc);
	lvc.pszText = L"Output string";
	ListView_InsertColumn(listHwnd, 1, &lvc);
}

LRESULT CDlgPythonMultiDecrypt::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	initCommon();
	ListView_SetExtendedListViewStyleEx(listHwnd, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);

	for (size_t i = 1; i < dj.clf->get_constant_pool().size(); i++)
	{
		if (dj.clf->get_constant_pool()[i])
		{
			u1 tTag = dj.clf->get_constant_pool()[i]->tag;
			if (tTag != CONSTANT_Utf8)
				continue;

			malloc_ptr<wchar_t> cpstr = genStringFromCP(dj.clf->get_constant_pool(), (u2)i, false);
			if (nullptr == cpstr)
				continue;
		
			LVITEM lvi = { 0 };
			lvi.mask = LVIF_TEXT;
			lvi.pszText = cpstr.get();
			lvi.iItem = (int)i;

			u2 idx = (u2)ListView_InsertItem(listHwnd, &lvi);
			editMap_1[idx] = (u2)i;
		}
	}

	return TRUE;
}

LRESULT CDlgPythonMultiDecrypt::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgPythonMultiDecrypt::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	const int edtW = LOWORD(lParam) - grid_space - 100;
	const int btnW = LOWORD(lParam) - edtW - 3*grid_space;

	RECT tmpRect;
	HWND tmpHwnd = GetDlgItem(LIST_UTF8);
	::GetWindowRect(tmpHwnd, &tmpRect);
	::MoveWindow(tmpHwnd, grid_space, grid_space, edtW, HIWORD(lParam) - 2*grid_space, TRUE);

	tmpHwnd = GetDlgItem(BTN_LOADSCRIPT);
	::GetWindowRect(tmpHwnd, &tmpRect);
	const int btnH = tmpRect.bottom - tmpRect.top;
	int curY = grid_space;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_DECRYPT);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY += btnH + grid_space;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(CHK_LOADED);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY += btnH + grid_space;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_CANCEL);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY = HIWORD(lParam) - 2*grid_space - 2*btnH;
	::MoveWindow(tmpHwnd, edtW + 8, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_SAVE);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY += grid_space + btnH;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);
	return FALSE;
}

LRESULT CDlgPythonMultiDecrypt::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 725) 
		rptr->right = rptr->left + 725;
	if (rptr->bottom - rptr->top < 470) 
		rptr->bottom = rptr->top + 470;
	return TRUE;
}

LRESULT CDlgPythonMultiDecrypt::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return 0;
}

void CDlgPythonMultiDecrypt::clbck_Decrypt(CDlgPythonMultiDecrypt& _this)
{
	malloc_ptr<wchar_t> tmpStr = preprocessUtf8ToWChar(_this.pyIter_decrStr);
	delete _this.pyIter_decrStr;
	ListView_SetItemText(_this.listHwnd, _this.pyIter_i, 1, (tmpStr) ? tmpStr.get() : L"#JOE#: memory allocation error");
}

void CDlgPythonMultiDecrypt::clbck_Save(CDlgPythonMultiDecrypt& _this)
{
	pyDecryptedUtf8 pydtmp = {_this.pyIter_idx, _this.pyIter_decrStr};
	_this.pyDecryptedUtf8Tab.push_back(pydtmp);
}

bool CDlgPythonMultiDecrypt::pyIterator(pyIterCallback clbck)
{
	listHwnd = GetDlgItem(LIST_UTF8);
	const int maxItems = ListView_GetItemCount(listHwnd);
	for (pyIter_i = 0; pyIter_i < maxItems; pyIter_i++)
	{
		if (ListView_GetCheckState(listHwnd, pyIter_i))
		{
			if (pyIter_i < (int)editMap_1.size())
			{
				pyIter_idx = editMap_1[(u2)pyIter_i];
				JavaConstPool cp = dj.clf->get_constant_pool();
				
				if (nullptr == cp[pyIter_idx])
					continue;

				CONSTANT_Utf8_info* utf8temp = 0;
				if (CONSTANT_Utf8 == cp[pyIter_idx]->tag)
					utf8temp = (CONSTANT_Utf8_info*)cp[pyIter_idx];
				else
					continue;

				pyIter_decrStr = pythonUtf8Decrypter(dj, m_hWnd, scriptSource.get(), *utf8temp);
				if (0 == pyIter_decrStr)
				{
					if (IDYES == MessageBox(L"Python error detected. Do you want to continue ?", L"Error", MB_YESNO | MB_ICONQUESTION))
						continue;
					else
						return false;
				}

				clbck(*this);
			}
		}
	}
	return true;
}

LRESULT CDlgPythonMultiDecrypt::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (nullptr == scriptSource)
	{
		MessageBox(L"Load python script first.", L"Error", MB_ICONERROR);
		return 0;
	}

	pyDecryptedUtf8Tab.clear();
	if (pyIterator(clbck_Save))
	{
		ShowWindow(SW_HIDE);
		if (CDlgPythonMultiPreview(dj, pyDecryptedUtf8Tab).DoModal(m_hWnd))
		{
			EndDialog(1);
			return 0;
		}
		ShowWindow(SW_SHOW);
	}

	//clean vector in case of failure
	for (size_t i = 0; i < pyDecryptedUtf8Tab.size(); i++)
	{
		if (0 != pyDecryptedUtf8Tab[i].newStr)
			delete pyDecryptedUtf8Tab[i].newStr;
	}
	pyDecryptedUtf8Tab.clear();

	return 0;
}

LRESULT CDlgPythonMultiDecrypt::OnBtn_Decrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (nullptr == scriptSource)
	{
		MessageBox(L"Load python script first.", L"Error", MB_ICONERROR);
		return 0;
	}

	pyIterator(clbck_Decrypt);

	return 0;
}

LRESULT CDlgPythonMultiDecrypt::OnBtn_LoadScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	scriptSource = loadScript(m_hWnd, *dj.logptr);
	
	if (nullptr != scriptSource)
		CheckDlgButton(CHK_LOADED, BST_CHECKED);
	else
		CheckDlgButton(CHK_LOADED, BST_UNCHECKED);

	return 0;
}

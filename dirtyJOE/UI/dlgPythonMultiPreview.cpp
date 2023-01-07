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
#include "dlgPythonMultiPreview.h"
#include "../java_support/helpers.h"

LRESULT CDlgPythonMultiPreview::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	initCommon();
	SetWindowText(L"Python multi-decrypter preview");
	::EnableWindow(GetDlgItem(BTN_LOADSCRIPT), FALSE);
	::EnableWindow(GetDlgItem(BTN_DECRYPT), FALSE);
	::EnableWindow(GetDlgItem(CHK_LOADED), FALSE);

	for (size_t i = 0; i < pyDecryptedUtf8Tab.size(); i++)
	{
		malloc_ptr<wchar_t> cpstr = genStringFromCP(dj.clf->get_constant_pool(), pyDecryptedUtf8Tab[i].cpIndex, false);
		if (nullptr == cpstr)
			continue;

		LVITEM lvi = { 0 };
		lvi.mask = LVIF_TEXT;
		lvi.pszText = cpstr.get();
		lvi.iItem = (int)i;

		u2 idx = (u2)ListView_InsertItem(listHwnd, &lvi);

		malloc_ptr<wchar_t> tmpStr = preprocessUtf8ToWChar(pyDecryptedUtf8Tab[i].newStr);
		ListView_SetItemText(listHwnd, idx, 1, (tmpStr) ? tmpStr.get() : L"#JOE#: memory allocation error");
	}

	return TRUE;
}

LRESULT CDlgPythonMultiPreview::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	for (size_t i = 0; i < pyDecryptedUtf8Tab.size(); i++)
	{
		delete dj.clf->get_constant_pool()[pyDecryptedUtf8Tab[i].cpIndex];
		dj.clf->get_constant_pool()[pyDecryptedUtf8Tab[i].cpIndex] = pyDecryptedUtf8Tab[i].newStr;
		dj.clf->setModified();
	}
	pyDecryptedUtf8Tab.clear();
	EndDialog(1);
	return 0;
}

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
#include "dlgSettings.h"
#include "shellext.h"

const CDlgSettings::pyVerStr CDlgSettings::pyVerTab[CDlgSettings::pyVerTabSize] =
{
	{CDJSettings::PyNone, L"None"},
	{CDJSettings::Python25, L"Python 2.5"},
	{CDJSettings::Python26, L"Python 2.6"},
	{CDJSettings::Python27, L"Python 2.7"}
};

LRESULT CDlgSettings::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CheckDlgButton(CHK_UPDATE, (dj.settings.get_StartupUpdateCheck()) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(CHK_SHELLEXT, (dj.settings.get_ShellExtension()) ? BST_CHECKED : BST_UNCHECKED);

#	ifdef DJ_TOTALCMD
	::EnableWindow(GetDlgItem(CHK_SHELLEXT), FALSE);
#	endif

	LOGFONT logfnt = { 0 };
	if (sizeof(logfnt) != GetObject(dj.settings.getFontHandle(), sizeof(logfnt), &logfnt))
		return FALSE;
	SetDlgItemText(EDT_FONT, logfnt.lfFaceName);

	int crpv = 0;
	for (int i = 0; i < pyVerTabSize; i++)
	{
		SendDlgItemMessage(CB_PYVER, CB_ADDSTRING, 0, (LPARAM)pyVerTab[i].pyverstr);
		if (dj.settings.getPythonVersion() == pyVerTab[i].pyver)
		{
			crpv = i;
			//pyVer = pyVerTab[i].pyver;
		}
	}
	SendDlgItemMessage(CB_PYVER, CB_SETCURSEL, crpv, 0);

	return TRUE;
}

void CDlgSettings::closeWithoutSave()
{
	if (intFont)
		DeleteObject(intFont);

	EndDialog(0);
}

LRESULT CDlgSettings::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	closeWithoutSave();
	return TRUE;
}

LRESULT CDlgSettings::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	closeWithoutSave();
	return 0;
}

LRESULT CDlgSettings::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (intFont)
	{
		DeleteObject(dj.settings.getFontHandle());
		dj.settings.setFontHandle(intFont);
	}

	dj.settings.set_StartupUpdateCheck(BST_CHECKED == IsDlgButtonChecked(CHK_UPDATE));

#	ifndef DJ_TOTALCMD
	if (BST_CHECKED == IsDlgButtonChecked(CHK_SHELLEXT))
	{
		dj.settings.set_ShellExtension(true);
		if (shellExt::Check_ShellExt_Error == shellExt::checkShellExtension(m_hWnd, false))
		{
			shellExt::addShellExtension(m_hWnd, true);
			::SendMessage(GetParent(), WM_REFRESH, 0, 0);
		}
	}
	else
	{
		dj.settings.set_ShellExtension(false);
		if (shellExt::Check_ShellExt_Ok == shellExt::checkShellExtension(m_hWnd))
		{
			shellExt::delShellExtension(m_hWnd, true);
			::SendMessage(GetParent(), WM_REFRESH, 0, 0);
		}
	}
#	endif

	dj.settings.saveSettings();

	EndDialog(1);
	return 0;
}

LRESULT CDlgSettings::OnBtn_Font(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CHOOSEFONT chsfont = { 0 };
	LOGFONT logfnt = { 0 };
	GetObject(dj.settings.getFontHandle(), sizeof(logfnt), &logfnt);
	chsfont.lStructSize = sizeof(CHOOSEFONT);
	chsfont.hwndOwner = m_hWnd;
	chsfont.lpLogFont = &logfnt;
	chsfont.Flags = CF_FORCEFONTEXIST | CF_FIXEDPITCHONLY | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	if (TRUE == ChooseFont(&chsfont))
	{
		intFont = CreateFontIndirect(&logfnt);
		SetDlgItemText(EDT_FONT, logfnt.lfFaceName);
	}
	return 0;
}

LRESULT CDlgSettings::OnPyVer_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT cs = SendDlgItemMessage(CB_PYVER, CB_GETCURSEL, 0, 0);
	if ((CB_ERR == cs) || (cs >= pyVerTabSize))
		return 0;

	if (!dj.settings.loadReqPython(pyVerTab[cs].pyver))
	{
		MessageBox(L"Can't load requested Python version.\nThe newest possible version will be loaded instead.", L"Error", MB_ICONINFORMATION);
		dj.settings.loadDefaultPython();
		int idx = 0;
		switch (dj.settings.getPythonVersion())
		{
			case CDJSettings::PyNone: idx = 0; break;
			case CDJSettings::Python25: idx = 1; break;
			case CDJSettings::Python26: idx = 2; break;
			case CDJSettings::Python27: idx = 3; break;
		}
		SendDlgItemMessage(CB_PYVER, CB_SETCURSEL, idx, 0);
	}

	return 0;
}

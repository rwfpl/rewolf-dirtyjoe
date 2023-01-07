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
#include "dlgPythonStringDecrypt.h"
#include "dlgSimpleLBView.h"
#include "../common/PyJOEWrapper.h"
#include "../common/common.h"
#include "../java_support/helpers.h"

LRESULT CDlgPythonStringDecrypt::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{	
	RECT tmpR;
	GetClientRect(&tmpR);
	SendMessage(WM_SIZE, 0, MAKELPARAM(tmpR.right - tmpR.left, tmpR.bottom - tmpR.top));

	SetWindowSubclass(GetDlgItem(TXT_PYSCRIPT), edtCtrlASubClass, 0x29a, 0);

	SendDlgItemMessage(TXT_PYSCRIPT, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);

	char pystub[] = "# 'inBuf' argument is input buffer represented as tuple\r\n# function should return list object\r\ndef dj_decryptUTF8(inBuf):\r\n";
	SendDlgItemMessageA(m_hWnd, TXT_PYSCRIPT, WM_SETTEXT, 0, (LPARAM)pystub);

	setDlgItemTextUtf8(m_hWnd, TXT_INPUTBUFFER, WM_SETTEXT, &inputString);

	return TRUE;
}

LRESULT CDlgPythonStringDecrypt::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

void CDlgPythonStringDecrypt::setLayout(LPARAM lParam)
{
	RECT tmpRect;
	int curY = HIWORD(lParam) - grid_space;
	const int edtW = LOWORD(lParam) - grid_space - 100;

	HWND tmpHwnd = GetDlgItem(TXT_PREVIEW);
	::GetWindowRect(tmpHwnd, &tmpRect);
	const int edtH = tmpRect.bottom - tmpRect.top;
	curY -= edtH;
	::MoveWindow(tmpHwnd, grid_space, curY, edtW, edtH, TRUE);

	tmpHwnd = GetDlgItem(IDC_STATIC3).m_hWnd;
	::GetWindowRect(tmpHwnd, &tmpRect);
	const int stH = tmpRect.bottom - tmpRect.top;
	curY -= (stH + grid_space);
	::MoveWindow(tmpHwnd, grid_space, curY, tmpRect.right - tmpRect.left, stH, TRUE);

	tmpHwnd = GetDlgItem(TXT_INPUTBUFFER);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY -= (edtH + grid_space);
	::MoveWindow(tmpHwnd, grid_space, curY, edtW, edtH, TRUE);

	tmpHwnd = GetDlgItem(IDC_STATIC2).m_hWnd;
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY -= (stH + grid_space);
	::MoveWindow(tmpHwnd, grid_space, curY, tmpRect.right - tmpRect.left, stH, TRUE);

	tmpHwnd = GetDlgItem(TXT_PYSCRIPT);
	::GetWindowRect(tmpHwnd, &tmpRect);
	::MoveWindow(tmpHwnd, grid_space, 2*grid_space + stH, edtW, curY - stH - 3*grid_space, TRUE);

	tmpHwnd = GetDlgItem(IDC_STATIC1);
	::GetWindowRect(tmpHwnd, &tmpRect);
	::MoveWindow(tmpHwnd, grid_space, grid_space, tmpRect.right - tmpRect.left, stH, TRUE);

	const int btnW = LOWORD(lParam) - edtW - 3*grid_space;

	tmpHwnd = GetDlgItem(BTN_LOADSCRIPT);
	::GetWindowRect(tmpHwnd, &tmpRect);
	const int btnH = tmpRect.bottom - tmpRect.top;
	curY = stH + 2*grid_space;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_SAVESCRIPT);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY += btnH + grid_space;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_DECRYPT);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY += btnH + grid_space;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_CANCEL);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY = HIWORD(lParam) - 2*grid_space - 2*btnH;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);

	tmpHwnd = GetDlgItem(BTN_SAVE);
	::GetWindowRect(tmpHwnd, &tmpRect);
	curY += grid_space + btnH;
	::MoveWindow(tmpHwnd, edtW + 2*grid_space, curY, btnW, btnH, TRUE);
}

LRESULT CDlgPythonStringDecrypt::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	setLayout(lParam);
	return 0;
}

LRESULT CDlgPythonStringDecrypt::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 650) 
		rptr->right = rptr->left + 650;
	if (rptr->bottom - rptr->top < 440) 
		rptr->bottom = rptr->top + 440;
	return TRUE;
}

LRESULT CDlgPythonStringDecrypt::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return 0;
}

LRESULT CDlgPythonStringDecrypt::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CONSTANT_Utf8_info* dcrstr = decryptAction();
	if (0 != dcrstr)
	{
		malloc_ptr<wchar_t> inStr = preprocessUtf8ToWChar(&inputString);
		if (nullptr == inStr)
		{
			delete dcrstr;
			return 0;
		}

		malloc_ptr<wchar_t> outStr = preprocessUtf8ToWChar(dcrstr);
		if (nullptr == outStr)
		{
			delete dcrstr;
			return 0;
		}

		malloc_ptr<wchar_t> msg = wcscat_var(L"Input string:\n\"", inStr.get(), L"\"\n\nwill be decrypted to:\n\"", outStr.get(), L"\"\n\nDo you want to continue ?", 0);
		if (nullptr == msg)
		{
			delete dcrstr;
			return 0;
		}

		if (IDYES == MessageBox(msg.get(), L"Python decrypter", MB_YESNO | MB_ICONQUESTION))
		{
			inputString.get_bytes().resize(dcrstr->get_bytes().size());
			memmove(&inputString.get_bytes()[0], &dcrstr->get_bytes()[0], dcrstr->get_bytes().size());
			dj.clf->setModified();
			EndDialog(1);
		}
		delete dcrstr;
	}
	return 0;
}

CONSTANT_Utf8_info* CDlgPythonStringDecrypt::decryptAction()
{
	size_t scriptLen = SendDlgItemMessage(TXT_PYSCRIPT, WM_GETTEXTLENGTH, 0, 0);
	if (0 == scriptLen)
		return 0;

	char* script = (char*)malloc(scriptLen + 1);
	if (0 == script)
		return 0;

	if (SendDlgItemMessageA(m_hWnd, TXT_PYSCRIPT, WM_GETTEXT, scriptLen + 1, (WPARAM)script) != (LRESULT)scriptLen)
		return 0;

	return pythonUtf8Decrypter(dj, m_hWnd, script, inputString);
}

LRESULT CDlgPythonStringDecrypt::OnBtn_Decrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CONSTANT_Utf8_info* dcrstr = decryptAction();
	setDlgItemTextUtf8(m_hWnd, TXT_PREVIEW, WM_SETTEXT, dcrstr);
	if (dcrstr)
		delete dcrstr;
	return 0;
}

LRESULT CDlgPythonStringDecrypt::OnBtn_LoadScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	malloc_ptr<char> fileMem = loadScript(m_hWnd, *dj.logptr);
	if (nullptr == fileMem)
		return 0;
	SetDlgItemTextA(m_hWnd, TXT_PYSCRIPT, fileMem.get());
	return 0;
}

LRESULT CDlgPythonStringDecrypt::OnBtn_SaveScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"Python .py files\0*.py\0All files\0*.*\0\0";
	wchar_t fNameBuf[MAX_PATH];
	fNameBuf[0] = 0;
	ofn.lpstrFile = fNameBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"py";
	if (GetSaveFileName(&ofn))
	{
		HWND edtHWnd = GetDlgItem(TXT_PYSCRIPT);
		int len = ::GetWindowTextLengthA(edtHWnd) + 1;
		malloc_ptr<char> scriptBuf = malloc<char>(len);
		if (nullptr == scriptBuf)
		{
			badAllocMsg(m_hWnd);
			return 0;
		}
		len = GetWindowTextA(edtHWnd, scriptBuf.get(), len);

		FILE* outFile;
		if (0 != _wfopen_s(&outFile, fNameBuf, L"wb"))
		{
			MessageBox(L"Can't create output file.", L"Error", MB_ICONERROR);
			return 0;
		}

		if (len != (int)fwrite(scriptBuf.get(), 1, len, outFile))
			MessageBox(L"Problem during saving script.", L"Error", MB_ICONERROR);

		fclose(outFile);
	}
	return 0;
}

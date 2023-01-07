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

#include "dlgAbout.h"
#include "version_info.h"

LRESULT CDlgAbout::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SendDlgItemMessage(TXT_VERSION, WM_SETTEXT, 0, (LPARAM)DJ_VERSION_SHORT_W);
	thRun = false;
	return TRUE;
}

LRESULT CDlgAbout::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	thRun = false;
	if (0 != threadHandle)
	{
		WaitForSingleObject(threadHandle, 2*1000);
		CloseHandle(threadHandle);
	}
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgAbout::OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;
	toolWindowHandleEscape(m_hWnd, wParam, lParam, bHandled);
	return TRUE;
}

LRESULT CDlgAbout::OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	HWND txtWWW = GetDlgItem(TXT_WWW);
	HWND txtMail = GetDlgItem(TXT_MAIL);
	if (((HWND)lParam == txtMail) || ((HWND)lParam == txtWWW))
	{
		SetTextColor((HDC)wParam, 0xFF0000);
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	return FALSE;
}

LRESULT CDlgAbout::OnTxt_WWWClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd, L"open", L"http://dirty-joe.com", 0, 0, SW_SHOW);
	return FALSE;
}

LRESULT CDlgAbout::OnTxt_MailClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd, L"open", L"mailto:joe@dirty-joe.com", 0, 0, SW_SHOW);
	return FALSE;
}

LRESULT CDlgAbout::OnBmp_Donate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd, L"open", L"http://dirty-joe.com/donate.php", 0, 0, SW_SHOW);
	return FALSE;	
}

LRESULT CDlgAbout::OnBmp_Clicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	POINT crpos;
	GetCursorPos(&crpos);
	RECT bmprt;
	::GetWindowRect(hWndCtl, &bmprt);
	if ((crpos.x >= bmprt.left + 284) &&
		(crpos.x <= bmprt.left + 300) &&
		(crpos.y >= bmprt.top + 35) &&
		(crpos.y <= bmprt.top + 51))
	{
		if (!thRun)
		{
			DWORD thid;
			thRun = true;
			thWnd = hWndCtl;
			threadHandle = CreateThread(0, 0, hiddenThread, (LPVOID)this, 0, &thid);
		}
		else
			thRun = false;
	}
	return FALSE;
}

DWORD WINAPI CDlgAbout::hiddenThread(LPVOID prm)
{
	static const int w = 400;
	static const int h = 150;
	CDlgAbout& _this = *(CDlgAbout*)prm;

	HDC dc = ::GetDC(_this.thWnd);
	HDC backupDC = CreateCompatibleDC(dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, w, h);
	HDC workingDC = CreateCompatibleDC(dc);
	HBITMAP hWorkBitmap = CreateCompatibleBitmap(dc, w, h);
	SelectObject(backupDC, hBitmap);
	SelectObject(workingDC, hWorkBitmap);
	BitBlt(backupDC, 0, 0, w, h, dc, 0, 0, SRCCOPY);

	const RECT ctrt = {0, 0, w, h};

	for (int i = 0; i < h; i++)
	{
		FillRect(workingDC, &ctrt, (HBRUSH)GetStockObject(BLACK_BRUSH));
		BitBlt(workingDC, 0, i, w, h - i, backupDC, 0, 0, SRCCOPY);
		BitBlt(dc, 0, 0, w, h, workingDC, 0, 0, SRCCOPY);
		Sleep(20);
	}

	int x = 0;
	int y = 0;
	bool rightDir = true;
	bool topDir = true;
	bool growBar = true;
	int wdt = 2;
	static const DWORD rop = NOTSRCCOPY;
	while (_this.thRun)
	{
		FillRect(workingDC, &ctrt, (HBRUSH)GetStockObject(BLACK_BRUSH));
		BitBlt(workingDC, x, 0, wdt, h, backupDC, x, 0, rop);
		BitBlt(workingDC, 0, y, w, wdt, backupDC, 0, y, rop);

		BitBlt(dc, 0, 0, w, h, workingDC, 0, 0, SRCCOPY);

		if (growBar)
		{
			if ((wdt < 100) && (x + wdt < w) && (y + wdt < h))
			{
				if (rightDir && topDir)
					wdt++;
				else
					wdt += 2;
			}
			else
				growBar = false;
		}
		else
		{
			if (wdt > 2)
			{
				if (!rightDir && !topDir)
					wdt--;
				else
					wdt -= 2;
			}
			else
			{
				growBar = true;
			}
		}

		if (rightDir)
		{
			x++;
			if (x >= w - wdt)
				rightDir = false;
		}
		else
		{
			x--;
			if (x <= 0)
				rightDir = true;
		}

		if (topDir)
		{
			y++;
			if (y >= h - wdt)
				topDir = false;
		}
		else
		{
			y--;
			if (y <= 0)
				topDir = true;
		}

		Sleep(20);
	}

	for (int i = h; i >= 0; i--)
	{
		BitBlt(dc, 0, i, w, h - i, backupDC, 0, i, SRCCOPY);
		Sleep(20);
	}

	DeleteDC(backupDC);
	DeleteDC(workingDC);
	DeleteObject(hWorkBitmap);
	DeleteObject(hBitmap);
	::ReleaseDC(_this.thWnd, dc);
	return 0;
}

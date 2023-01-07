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

#include "dlgEditUtf8.h"
#include "dlgSimpleLBView.h"
#include "../java_support/helpers.h"

LRESULT CDlgEditUtf8::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!init())
	{
		EndDialog(0);
		return FALSE;
	}
	return TRUE;
}

bool CDlgEditUtf8::init()
{
	malloc_ptr<wchar_t> nstr = preprocessUtf8ToWChar((CONSTANT_Utf8_info*)dj.clf->get_constant_pool()[cpIdx]);
	if (nullptr != nstr)
	{
		SetDlgItemText(EDT_UTF8, nstr.get());
		return true;
	}
	return false;
}

LRESULT CDlgEditUtf8::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditUtf8::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditUtf8::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int len = ::GetWindowTextLength(GetDlgItem(EDT_UTF8));
	if (len)
	{
		malloc_ptr<wchar_t> nt = malloc<wchar_t>(len + 1);
		if (nullptr == nt)
		{
			badAllocMsg(m_hWnd);
			return FALSE;
		}
		if (GetDlgItemText(EDT_UTF8, nt.get(), len + 1) == 0)
		{
			MessageBox(L"Cannot obtain string from dialog.", L"Error", MB_ICONERROR);
			return FALSE;
		}
		CONSTANT_Utf8_info* nobj = new (std::nothrow) CONSTANT_Utf8_info(nt.get(), *dj.logptr);
		if (nobj == 0)
		{
			badAllocMsg(m_hWnd);
			return FALSE;
		}
		if (nobj->getError())
		{

			MessageBox(L"Cannot convert to Utf8 object.", L"Error", MB_ICONERROR);
			return FALSE;
		}
		//ATL EndDialog implementation truncate nResult to int, so it would not work on x64 if
		//we want to return pointer to some object. Standard WinAPI EndDialog() will do the trick.
		::EndDialog(m_hWnd, (INT_PTR)nobj);
	}
	else
	{
		//ask user if he really want to save empty utf8
		if (MessageBox(L"Do you really want to set empty string ?", L"Question", MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			wchar_t nt[] = L"";
			CONSTANT_Utf8_info* nobj = new (std::nothrow) CONSTANT_Utf8_info(nt, *dj.logptr);
			if (nobj == 0)
			{
				badAllocMsg(m_hWnd);
				return FALSE;
			}
			if (nobj->getError())
			{

				MessageBox(L"Cannot convert to Utf8 object.", L"Error", MB_ICONERROR);
				return FALSE;
			}
			::EndDialog(m_hWnd, (INT_PTR)nobj);
		}
	}
	return FALSE;
}

LRESULT CDlgEditUtf8::OnBtn_ShowRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleLBView(dj, CDlgSimpleLBView::References, cpIdx).DoModal(m_hWnd);
	return FALSE;
}

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

#include "dlgEditVal.h"
#include "dlgSimpleLBView.h"

LRESULT CDlgEditVal::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!init())
	{
		EndDialog(0);
		return FALSE;
	}
	return TRUE;
}

bool CDlgEditVal::init()
{
	editValLock = false;
	cp_info* ccp = dj.clf->get_constant_pool()[cpIdx];

	wchar_t tstr_dec[64];

	switch (ccp->tag)
	{
	case CONSTANT_Integer:
		{
			SetWindowText(L"Edit CONSTANT_Integer");
			u4 v1 = ((CONSTANT_Integer_info*)ccp)->v1;
			swprintf_s(tstr_dec, L"%d", v1);
		}
		break;
	case CONSTANT_Long: 
		{
			SetWindowText(L"Edit CONSTANT_Long"); 
			u8 d64 = ((CONSTANT_Long_info*)ccp)->v1;
			d64 <<= 32;
			d64 += ((CONSTANT_Long_info*)ccp)->v2;
			swprintf_s(tstr_dec, L"%I64d", d64);
		}
		break;
	case CONSTANT_Float: 
		{
			SetWindowText(L"Edit CONSTANT_Float");
			u4 v1 = ((CONSTANT_Float_info*)ccp)->v1;
			swprintf_s(tstr_dec, L"%.25g", *(float*)&v1);
		}
		break;
	case CONSTANT_Double: 
		{
			SetWindowText(L"Edit CONSTANT_Double");
			u8 d64 = ((CONSTANT_Double_info*)ccp)->v1;
			d64 <<= 32;
			d64 += ((CONSTANT_Double_info*)ccp)->v2;
			swprintf_s(tstr_dec, L"%.25g", (double)d64);
		}
		break;
	default: 
		return false;
	}

	SetDlgItemText(EDIT_DEC, tstr_dec);
	return true;
}

LRESULT CDlgEditVal::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditVal::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditVal::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	cp_info* ccp = dj.clf->get_constant_pool()[cpIdx];

	wchar_t hstr[64];
	wchar_t dstr[64];
	GetDlgItemText(EDIT_DEC, dstr, 64);
	GetDlgItemText(EDIT_HEX, hstr, 64);
	bool err = false;

	switch (ccp->tag)
	{
	case CONSTANT_Integer:
	case CONSTANT_Float:
		{
			u4 vh;
			if (swscanf_s(hstr, L"%X", &vh) != 1)
			{
				err = true;
				break;
			}
			u4 vd;
			if (ccp->tag == CONSTANT_Integer)
			{
				if (swscanf_s(dstr, L"%d", &vd) != 1)
				{
					err = true;
					break;
				}
			}
			else
			{
				if (swscanf_s(dstr, L"%g", (float*)&vd) != 1)
				{
					err = true;
					break;
				}
			}
			if (vh != vd)
			{
				err = true;
				break;
			}
			((CONSTANT_uX_info<u4>*)ccp)->v1 = vh;
			dj.clf->setModified();
		}
		break;
	case CONSTANT_Long:
	case CONSTANT_Double:
		{
			u8 vh;
			if (swscanf_s(hstr, L"%I64X", &vh) != 1)
			{
				err = true;
				break;
			}
			u8 vd;
			if (ccp->tag == CONSTANT_Long)
			{
				if (swscanf_s(dstr, L"%I64d", &vd) != 1)
				{
					err = true;
					break;
				}
			}
			else
			{
				if (swscanf_s(dstr, L"%Lg", (long double*)&vd) != 1)
				{
					err = true;
					break;
				}
			}
			if (vh != vd)
			{
				err = true;
				break;
			}
			((CONSTANT_uXuX_info<u4, u4>*)ccp)->v1 = vh >> 32;
			((CONSTANT_uXuX_info<u4, u4>*)ccp)->v2 = vh & 0xFFFFFFFF;
			dj.clf->setModified();
		}
		break;
	}//end switch

	if (err)
		MessageBox(L"Please enter correct values.", L"Error", MB_ICONERROR);
	else
		EndDialog(1);
	return FALSE;
}

LRESULT CDlgEditVal::OnBtn_ShowRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleLBView(dj, CDlgSimpleLBView::References, cpIdx).DoModal(m_hWnd);
	return FALSE;
}

LRESULT CDlgEditVal::OnEdt_Change(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	if (editValLock)
	{
		editValLock = false;
		return FALSE;
	}

	int len = ::GetWindowTextLength(hWndCtl);
	if (len > 40)
	{
		MessageBox(L"Too much characters.", L"Info", MB_ICONINFORMATION);
		return false;
	}
	wchar_t tstr[64];
	::GetWindowText(hWndCtl, tstr, 64);
	cp_info* ccp = dj.clf->get_constant_pool()[cpIdx];

	switch (wID)
	{
	case EDIT_HEX:
		{
			wchar_t ostr[64];
			ostr[0] = 0;
			bool err = false;
			switch (ccp->tag)
			{
			case CONSTANT_Integer:
			case CONSTANT_Float:
				{
					u4 v1;
					if (swscanf_s(tstr, L"%X", &v1) != 1)
					{
						err = true;
						break;
					}
					if (ccp->tag == CONSTANT_Integer)
						swprintf_s(ostr, L"%d", v1);
					else
						swprintf_s(ostr, L"%.25g", *(float*)&v1);
				}
				break;
			case CONSTANT_Long:
			case CONSTANT_Double:
				{
					u8 v1;
					if (swscanf_s(tstr, L"%I64X", &v1) != 1)
					{
						err = true;
						break;
					}
					if (ccp->tag == CONSTANT_Long)
						swprintf_s(ostr, L"%I64d", v1);
					else
						swprintf_s(ostr, L"%.25g", (double)v1);
				}
				break;
			}
			if (!err)
			{
				editValLock = true;
				SetDlgItemText(EDIT_DEC, ostr);
				SendDlgItemMessage(CHECK_DEC, BM_SETCHECK, BST_CHECKED, 0);
				SendDlgItemMessage(CHECK_HEX, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				editValLock = true;
				SetDlgItemText(EDIT_DEC, L"error");
				SendDlgItemMessage(CHECK_DEC, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(CHECK_HEX, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}
		break;
	case EDIT_DEC:
		{
			wchar_t ostr[64];
			bool err = false;
			switch (ccp->tag)
			{
			case CONSTANT_Integer:
				{
					u4 v1;
					if (swscanf_s(tstr, L"%d", &v1) != 1)
					{
						err = true;
						break;
					}
					swprintf_s(ostr, L"%08X", v1);
				}
				break;
			case CONSTANT_Long:
				{
					u8 v1;
					if (swscanf_s(tstr, L"%I64d", &v1) != 1)
					{
						err = true;
						break;
					}
					swprintf_s(ostr, L"%016I64X", v1);
				}
				break;
			case CONSTANT_Float:
				{
					u4 v1;
					if (swscanf_s(tstr, L"%g", (float*)&v1) != 1)
					{
						err = true;
						break;
					}
					swprintf_s(ostr, L"%08X", v1);
				}
				break;
			case CONSTANT_Double:
				{
					u8 v1;
					if (swscanf_s(tstr, L"%Lg", (long double*)&v1) != 1)
					{
						err = true;
						break;
					}
					swprintf_s(ostr, L"%016I64X", v1);
				}
				break;
			}
			if (!err)
			{
				editValLock = true;
				SetDlgItemText(EDIT_HEX, ostr);
				SendDlgItemMessage(CHECK_DEC, BM_SETCHECK, BST_CHECKED, 0);
				SendDlgItemMessage(CHECK_HEX, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				editValLock = true;
				SetDlgItemText(EDIT_HEX, L"error");
				SendDlgItemMessage(CHECK_DEC, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(CHECK_HEX, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}
		break;
	}
	return FALSE;
}

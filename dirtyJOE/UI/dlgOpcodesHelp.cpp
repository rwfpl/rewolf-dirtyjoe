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
#include "dlgOpcodesHelp.h"
#include "../common/common.h"

LRESULT CDlgOpcodesHelp::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	RECT rt;
	GetClientRect(&rt);
	SendMessage(WM_SIZE, SIZE_RESTORED, MAKEWPARAM(rt.right - rt.left, rt.bottom - rt.top));

	auto& ents = dj.jvmspec.getRawEntries();
	auto ent_it = ents.begin();
	while (ent_it != ents.end())
	{
		SendDlgItemMessageA(m_hWnd, LB_OP_LIST, LB_ADDSTRING, 0, (LPARAM)ent_it->opcodeName);
		ent_it++;
	}

	dj.opHelper = m_hWnd;

	return TRUE;
}

LRESULT CDlgOpcodesHelp::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (exitOnClose)
		EndDialog(0);
	else
		ShowWindow(SW_HIDE);
	return TRUE;
}

LRESULT CDlgOpcodesHelp::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT trt;
	::GetWindowRect(GetDlgItem(EDT_OP_NAME), &trt);

	static const int lbw = 150;
	static const int edtH = trt.bottom - trt.top;

	::GetWindowRect(GetDlgItem(IDC_STATIC_SH), &trt);
	
	static const int stW = trt.right - trt.left;
	static const int stH = trt.bottom - trt.top;

	::MoveWindow(GetDlgItem(LB_OP_LIST), grid_space, grid_space, lbw, HIWORD(lParam) - 2*grid_space, TRUE);
	::MoveWindow(GetDlgItem(EDT_DESC), 2*grid_space + lbw, 3*edtH + 6*grid_space + stH, LOWORD(lParam) - 3*grid_space - lbw, HIWORD(lParam) - (3*edtH + 7*grid_space + stH), TRUE);

	::MoveWindow(GetDlgItem(EDT_OP_NAME), 2*grid_space + lbw + stW, grid_space, LOWORD(lParam) - 4*grid_space - lbw - stW, edtH, TRUE);
	::MoveWindow(GetDlgItem(EDT_STACK), 2*grid_space + lbw + stW, 2*grid_space + edtH, LOWORD(lParam) - 4*grid_space - lbw - stW, edtH, TRUE);
	::MoveWindow(GetDlgItem(EDT_SHORT), 2*grid_space + lbw + stW, 3*grid_space + 2*edtH, LOWORD(lParam) - 4*grid_space - lbw - stW, edtH, TRUE);

	::MoveWindow(GetDlgItem(IDC_STATIC_NM), 2*grid_space + lbw, grid_space, stW, stH, TRUE);
	::MoveWindow(GetDlgItem(IDC_STATIC_ST), 2*grid_space + lbw, 2*grid_space + edtH, stW, stH, TRUE);
	::MoveWindow(GetDlgItem(IDC_STATIC_SH), 2*grid_space + lbw, 3*grid_space + 2*edtH, stW, stH, TRUE);
	::MoveWindow(GetDlgItem(IDC_STATIC_FD), 2*grid_space + lbw, 4*grid_space + 3*edtH, stW, stH, TRUE);

	return FALSE;
}

LRESULT CDlgOpcodesHelp::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 200) 
		rptr->right = rptr->left + 200;
	if (rptr->bottom - rptr->top < 200) 
		rptr->bottom = rptr->top + 200;
	return TRUE;
}

LRESULT CDlgOpcodesHelp::OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	SendMessage(GetParent(), WM_WINDOW_MOVING, (WPARAM)m_hWnd, lParam);
	bHandled = TRUE;
	return TRUE;
}

void CDlgOpcodesHelp::updateInfo(int sel)
{
	auto& opD = dj.jvmspec.getRawEntries()[sel];

	SetDlgItemTextA(m_hWnd, EDT_OP_NAME, opD.opcodeName);
	SetDlgItemTextA(m_hWnd, EDT_STACK, opD.stackDescription);
	SetDlgItemTextA(m_hWnd, EDT_SHORT, opD.shortDescription);
	SetDlgItemTextA(m_hWnd, EDT_DESC, opD.fullDescription);
}

LRESULT CDlgOpcodesHelp::OnLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DWORD cs = (DWORD)SendDlgItemMessage(LB_OP_LIST, LB_GETCURSEL, 0, 0);
	if ((LB_ERR == cs) || (cs >= dj.jvmspec.getRawEntries().size()))
		return 0;

	updateInfo(cs);
	
	return 0;
}

LRESULT CDlgOpcodesHelp::OnOpcodeHelp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	int rawIdx = dj.jvmspec.getRawIndex((BYTE)wParam);
	if (-1 == rawIdx)
		return 0;

	SendDlgItemMessage(LB_OP_LIST, LB_SETCURSEL, rawIdx, 0);
	updateInfo(rawIdx);
	if (0 != lParam)
		ShowWindow(SW_SHOW);

	return 0;
}

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

#include "dlgCodeEditor.h"
#include "../java_support/jdisasm/jdisasm.h"
#include "../common/common.h"
#include "../common/JVMSpec.h"
#include "dlgMiniHexEdit.h"
#include "dlgCode.h"
#include "dlgSelectColumn.h"
#include "dlgSimpleLBView.h"

LRESULT CDlgCodeEditor::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowSubclass(GetDlgItem(LB_ADDR), lbSubClass, 0x29a, 0);
	SetWindowSubclass(GetDlgItem(LB_HEX), lbSubClass, 0x29b, 0);
	SetWindowSubclass(GetDlgItem(LB_DISASM), lbSubClass, 0x29c, 0);

	SendDlgItemMessage(LB_ADDR, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);
	SendDlgItemMessage(LB_HEX, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);
	SendDlgItemMessage(LB_DISASM, WM_SETFONT, (WPARAM)dj.settings.getFontHandle(), TRUE);

	FindTextWndMsg = RegisterWindowMessage(FINDMSGSTRING);

	refresh();
	return TRUE;
}

LRESULT CDlgCodeEditor::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgCodeEditor::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT edtrt;
	::GetWindowRect(GetDlgItem(EDT_OP_STACK), &edtrt);
	
	const int edtH = edtrt.bottom - edtrt.top;
	const int lbH = HIWORD(lParam) - edtH - grid_space;
	const int edtW = (LOWORD(lParam) - grid_space) / 2;
	
	RECT lbrt;
	LONG crLf = 0;

	::MoveWindow(GetDlgItem(EDT_OP_STACK), 0, 0, edtW, edtH, TRUE);
	::MoveWindow(GetDlgItem(EDT_OP_DESCRIPTION), edtW + grid_space, 0, edtW, edtH, TRUE);

	::GetWindowRect(GetDlgItem(LB_ADDR), &lbrt);
	::MoveWindow(GetDlgItem(LB_ADDR), crLf, edtH + grid_space, lbrt.right - lbrt.left, lbH, TRUE);
	
	crLf += (lbrt.right - lbrt.left) + grid_space;
	
	::GetWindowRect(GetDlgItem(LB_HEX), &lbrt);
	::MoveWindow(GetDlgItem(LB_HEX), crLf, edtH + grid_space, lbrt.right - lbrt.left, lbH, TRUE);
	
	crLf += (lbrt.right - lbrt.left) + grid_space;
	::MoveWindow(GetDlgItem(LB_DISASM), crLf, edtH + grid_space, LOWORD(lParam) - crLf, lbH, TRUE);
	
	return FALSE;
}

LRESULT CDlgCodeEditor::OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	RECT* rptr = (RECT*)lParam;
	if (rptr->right - rptr->left < 400) 
		rptr->right = rptr->left + 400;
	if (rptr->bottom - rptr->top < 200) 
		rptr->bottom = rptr->top + 200;
	return TRUE;
}

LRESULT CDlgCodeEditor::OnHex_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	POINT crpt;
	GetCursorPos(&crpt);
	LRESULT tmp = SendDlgItemMessage(LB_HEX, LB_GETCURSEL, 0 , 0);
	if (tmp == LB_ERR)
		tmp = 0;

	tmp = editMap_1[(u2)tmp];
	CDlgMiniHexEdit dlg(dj, ac, crpt, (u4)tmp);
	std::vector<u1>* newhex = (std::vector<u1>*)dlg.DoModal(m_hWnd);
	if (newhex)
	{
		if (!newhex->empty())
		{
			if (newhex->size() > ac.get_code().size() - tmp)
			{
				ac.get_code().resize(newhex->size() + tmp, 0);
			}
			memmove(&ac.get_code()[tmp], &(*newhex)[0], newhex->size());
			dj.clf->setModified();
			refresh();
			::SendMessage(GetParent(), WM_REFRESH, 0, 0);
		}
		delete newhex;
	}
	return FALSE;
}

void CDlgCodeEditor::lbSelChangeHelper(WORD wID, WORD wID2, DWORD* selItems, DWORD selCount, DWORD curVis)
{
	if (wID != wID2)
	{
		HWND lbh = GetDlgItem(wID2);
		::SendMessage(lbh, WM_SETREDRAW, FALSE, 0);
		::SendMessage(lbh, LB_SETSEL, FALSE, -1);
		for (DWORD i = 0; i < selCount; i++)
			::SendMessage(lbh, LB_SETSEL, TRUE, selItems[i]);
		::SendMessage(lbh, LB_SETTOPINDEX, curVis, 0);
		::SendMessage(lbh, WM_SETREDRAW, TRUE, 0);
	}
}

LRESULT CDlgCodeEditor::OnSelChange(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	DWORD selCount = (DWORD)::SendMessage(hWndCtl, LB_GETSELCOUNT, 0, 0);
	if (LB_ERR == selCount)
		return FALSE;

	malloc_ptr<DWORD> selItems = malloc<DWORD>(selCount);
	if (nullptr == selItems)
		return FALSE;

	selCount = (DWORD)::SendMessage(hWndCtl, LB_GETSELITEMS, selCount, (LPARAM)selItems.get());
	if (LB_ERR == selCount)
		return FALSE;

	DWORD curVis = (DWORD)::SendMessage(hWndCtl, LB_GETTOPINDEX, 0, 0);

	lbSelChangeHelper(wID, LB_ADDR, selItems.get(), selCount, curVis);
	lbSelChangeHelper(wID, LB_HEX, selItems.get(), selCount, curVis);
	lbSelChangeHelper(wID, LB_DISASM, selItems.get(), selCount, curVis);

	if (LB_DISASM == wID)
	{
		LRESULT tmp = SendDlgItemMessage(LB_HEX, LB_GETCURSEL, 0 , 0);
		if (LB_ERR != tmp)
		{
			tmp = editMap_1[(u2)tmp];
			auto& cd = ac.get_code();
			if ((size_t)tmp < cd.size())
			{
				const CJVMSpec::opcodeEntry& opD = dj.jvmspec.getOpcodeSpec(cd[tmp]);
				SetDlgItemTextA(m_hWnd, EDT_OP_DESCRIPTION, opD.shortDescription);
				SetDlgItemTextA(m_hWnd, EDT_OP_STACK, opD.stackDescription);
				::SendMessage(dj.opHelper, WM_OPCODE_HELP, cd[tmp], 0);
			}
		}
	}

	return FALSE;
}

void CDlgCodeEditor::lbLBtnDownHelper(WORD wID, WORD wID2, POINT& cp)
{
	if (wID != wID2)
	{
		HWND lbh = GetDlgItem(wID2);
		RECT rt = { 0 };
		::GetWindowRect(lbh, &rt);
		POINT cpt = { rt.left + 4, cp.y };
		::ScreenToClient(lbh, &cpt);
		::SendMessage(lbh, WM_USER_LB_LBUTTONDOWN, 0, MAKELPARAM(cpt.x, cpt.y));
	}
}

LRESULT CDlgCodeEditor::OnUser_LButonDown(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	/*WPARAM flgs = 0;
	
	if (GetKeyState(VK_LBUTTON))
		flgs |= MK_LBUTTON;
	if (GetKeyState(VK_RBUTTON))
		flgs |= MK_RBUTTON;
	if (GetKeyState(VK_MBUTTON))
		flgs |= MK_MBUTTON;
	if (GetKeyState(VK_CONTROL))
		flgs |= MK_CONTROL;
	if (GetKeyState(VK_SHIFT))
		flgs |= MK_SHIFT;
	printf("%08X\n", flgs);*/
	
	POINT cp;
	GetCursorPos(&cp);

	lbLBtnDownHelper(wID, LB_ADDR, cp);
	lbLBtnDownHelper(wID, LB_HEX, cp);
	lbLBtnDownHelper(wID, LB_DISASM, cp);

	return FALSE;
}

LRESULT CDlgCodeEditor::OnUser_VScroll(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	int scpos = ::GetScrollPos(hWndCtl, SB_VERT);
	if (wID != LB_ADDR)
	{
		::SendMessage(GetDlgItem(LB_ADDR), WM_VSCROLL, (scpos << 16) | SB_THUMBPOSITION, (LPARAM)-1);
	}
	if (wID != LB_HEX)
	{
		::SendMessage(GetDlgItem(LB_HEX), WM_VSCROLL, (scpos << 16) | SB_THUMBPOSITION, (LPARAM)-1);
	}
	if (wID != LB_DISASM)
	{
		::SendMessage(GetDlgItem(LB_DISASM), WM_VSCROLL, (scpos << 16) | SB_THUMBPOSITION, (LPARAM)-1);
	}
	return FALSE;
}

void CDlgCodeEditor::refresh()
{
	LRESULT csl = SendDlgItemMessage(LB_ADDR, LB_GETCURSEL, 0, 0);
	LRESULT tpi = SendDlgItemMessage(LB_ADDR, LB_GETTOPINDEX, 0, 0);

	SendDlgItemMessage(LB_ADDR, WM_SETREDRAW, FALSE, 0);
	SendDlgItemMessage(LB_HEX, WM_SETREDRAW, FALSE, 0);
	SendDlgItemMessage(LB_DISASM, WM_SETREDRAW, FALSE, 0);

	SendDlgItemMessage(LB_ADDR, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(LB_HEX, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(LB_DISASM, LB_RESETCONTENT, 0, 0);

	if (!editMap_1.empty())
		editMap_1.clear();

	u4 pos = 0;
	u2 idx = 0;
	while (pos < ac.get_code().size())
	{
		wchar_t str_pos[0x10];
		swprintf_s(str_pos, L"%08X", pos);
		SendDlgItemMessage(LB_ADDR, LB_ADDSTRING, 0, (LPARAM)str_pos);
		int len = jdisasm_get_len(&ac.get_code()[0], (u4)ac.get_code().size(), pos);
		if (len < 0)
		{
			swprintf_s(str_pos, L"%02X", ac.get_code()[pos]);
			SendDlgItemMessage(LB_HEX, LB_ADDSTRING, 0, (LPARAM)str_pos);
			SendDlgItemMessage(LB_DISASM, LB_ADDSTRING, 0, (LPARAM)L"#UNKNOWN OPCODE#");
			editMap_1[idx] = (u2)pos;
			idx++;
			pos++;
			continue;
		}
		if (pos + len > ac.get_code().size())
			len = (int)(ac.get_code().size() - pos);
		malloc_ptr<wchar_t> str_hex = data2hex(&ac.get_code()[pos], len);
		if (nullptr == str_hex)
		{
			SendDlgItemMessage(LB_HEX, LB_ADDSTRING, 0, (LPARAM)L"#FATAL ERROR#");
			SendDlgItemMessage(LB_DISASM, LB_ADDSTRING, 0, (LPARAM)L"#FATAL ERROR#");
			break;
		}
		SendDlgItemMessage(LB_HEX, LB_ADDSTRING, 0, (LPARAM)str_hex.get());
		editMap_1[idx] = (u2)pos;
		idx++;
		if (len > 0)
		{
			malloc_ptr<wchar_t> opstr = jdisasm_get_str(&ac.get_code()[0], (u4)ac.get_code().size(), pos, &dj.clf->get_constant_pool());
			if (nullptr != opstr)
			{
				SendDlgItemMessage(LB_DISASM, LB_ADDSTRING, 0, (LPARAM)opstr.get());
			}
			else
			{
				SendDlgItemMessage(LB_DISASM, LB_ADDSTRING, 0, (LPARAM)L"#Error#");
			}
			pos += len;
		}
		else
		{
			SendDlgItemMessage(LB_DISASM, LB_ADDSTRING, 0, (LPARAM)L"#FATAL ERROR#");
			break;
		}
	}

	if (csl != LB_ERR)
	{
		SendDlgItemMessage(LB_HEX, LB_SETCURSEL, csl, 0);
		SendDlgItemMessage(LB_HEX, LB_SETTOPINDEX, tpi, 0);
		SendMessage(WM_COMMAND, MAKEWPARAM(LB_HEX, LBN_SELCHANGE), (LPARAM)(HWND)GetDlgItem(LB_HEX));
	}

	SendDlgItemMessage(LB_ADDR, WM_SETREDRAW, TRUE, 0);
	SendDlgItemMessage(LB_HEX, WM_SETREDRAW, TRUE, 0);
	SendDlgItemMessage(LB_DISASM, WM_SETREDRAW, TRUE, 0);
}

LRESULT CDlgCodeEditor::OnHex_RButtonUp(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	int selCount = (int)SendDlgItemMessage(wID, LB_GETSELCOUNT, 0, 0);
	if ((LB_ERR == selCount) || (selCount <= 1))
	{
		if (1 == selCount)
			SendDlgItemMessage(wID, LB_SETSEL, FALSE, -1);

		//hWndCtl contains X and Y coordinates (kind of hack, ListBox is subclassed)
		int curItem = (int)SendDlgItemMessage(wID, LB_ITEMFROMPOINT, 0, (LPARAM)hWndCtl);
		SendDlgItemMessage(wID, LB_SETSEL, TRUE, curItem);
		BOOL dummy = FALSE;
		OnSelChange(0, wID, GetDlgItem(wID), dummy);
	}

	ShowContextMenu(dj.hInstance, m_hWnd, MAKEINTRESOURCE(IDR_CTX_MENU), code_ctx_menu);
	return 0;
}

malloc_ptr<wchar_t> CDlgCodeEditor::copyLBContent(bool selectedOnly, DWORD copyFlags)
{
	HWND lbHex = GetDlgItem(LB_HEX);
	HWND lbAddr = GetDlgItem(LB_ADDR);
	HWND lbDisasm = GetDlgItem(LB_DISASM);

	DWORD cnt = (DWORD)SendMessage(lbHex, LB_GETCOUNT, 0, 0);
	if (LB_ERR == cnt)
		return nullptr;

	malloc_ptr<wchar_t> outStr = nullptr;
	for (DWORD i = 0; i < cnt; i++)
	{
		if (selectedOnly)
		{
			if (0 >= SendMessage(lbHex, LB_GETSEL, i, 0))
				continue;
		}

		malloc_ptr<wchar_t> tmpStrAddr = nullptr;
		if (copyFlags & CDlgSelectColumn::Copy_Address)
		{
			tmpStrAddr = getLBText(lbAddr, i);
			if (nullptr == tmpStrAddr)
				return nullptr;
		}

		malloc_ptr<wchar_t> tmpStrHex = nullptr;
		if (copyFlags & CDlgSelectColumn::Copy_Hex)
		{
			tmpStrHex = getLBText(lbHex, i);
			if (nullptr == tmpStrHex)
				return nullptr;
		}

		malloc_ptr<wchar_t> tmpStrDisasm = nullptr;
		if (copyFlags & CDlgSelectColumn::Copy_Disasm)
		{
			tmpStrDisasm = getLBText(lbDisasm, i);
			if (nullptr == tmpStrDisasm)
				return nullptr;
		}

		outStr = wcscat_var(outStr ? outStr.get() : L"", tmpStrAddr ? tmpStrAddr.get() : L"", tmpStrAddr ? L"\t" : L"", tmpStrHex ? tmpStrHex.get() : L"", tmpStrHex ? L"\t" : L"", tmpStrDisasm ? tmpStrDisasm.get() : L"", L"\r\n", 0);
		if (nullptr == outStr)
			return nullptr;
	}
	return outStr;
}

void CDlgCodeEditor::copyHelper(bool selOnly)
{
	DWORD cpyFlags = (DWORD)CDlgSelectColumn(dj).DoModal(m_hWnd);
	if (CDlgSelectColumn::Copy_None == cpyFlags)
		return;

	malloc_ptr<wchar_t> tmpStr = copyLBContent(selOnly, cpyFlags);
	if (nullptr != tmpStr)
	{
		CopyWCharToClipBoard(m_hWnd, tmpStr.get());
	}
}

LRESULT CDlgCodeEditor::OnMenu_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyHelper(false);
	return 0;
}

LRESULT CDlgCodeEditor::OnMenu_CopySelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	copyHelper(true);
	return 0;
}

LRESULT CDlgCodeEditor::OnMenu_OpcodeDescription(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT tmp = SendDlgItemMessage(LB_DISASM, LB_GETCURSEL, 0 , 0);
	if (LB_ERR != tmp)
	{
		tmp = editMap_1[(u2)tmp];
		auto& cd = ac.get_code();
		if ((size_t)tmp < cd.size())
		{
			::SendMessage(dj.opHelper, WM_OPCODE_HELP, cd[tmp], 1);
		}
	}
	return 0;
}

LRESULT CDlgCodeEditor::OnMenu_AddExcHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	const int lbcnt = (int)SendDlgItemMessage(LB_DISASM, LB_GETCOUNT, 0, 0);
	if (LB_ERR == lbcnt)
		return 0;

	int selCount = (int)SendDlgItemMessage(LB_DISASM, LB_GETSELCOUNT, 0, 0);
	if ((LB_ERR ==  selCount) || (0 == selCount))
		return 0;

	malloc_ptr<int> selItems = malloc<int>(selCount);
	if (nullptr == selItems)
	{
		badAllocMsg(m_hWnd);
		return 0;
	}

	selCount = (int)SendDlgItemMessage(LB_DISASM, LB_GETSELITEMS, selCount, (LPARAM)selItems.get());
	if (LB_ERR == selCount)
		return 0;

	int firstItem = *selItems;
	int lastItem = *selItems;
	for (int i = 1; i < selCount; i++)
	{
		if (selItems.get()[i] == lastItem + 1)
			lastItem = selItems.get()[i];
		else
			break;
	}
	lastItem++;

	u2 end_pc;
	if (lastItem >= lbcnt)
		end_pc = (u2)ac.get_code().size();
	else
		end_pc = editMap_1[(u2)lastItem];

	dlgCode.addException(editMap_1[(u2)firstItem], end_pc, 0, 0);
	return 0;
}

LRESULT CDlgCodeEditor::OnFindTextMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (fns.Flags & FR_DIALOGTERM)
	{
		FindTextHWND = 0;
	}
	else if (fns.Flags & FR_FINDNEXT)
	{
		const bool caseSens = (fns.Flags & FR_MATCHCASE) ? true : false;
		const bool prev = (fns.Flags & FR_DOWN) ? false : true;
		int hexRow = listBoxSearch(GetDlgItem(LB_HEX), fns.lpstrFindWhat, caseSens, prev);
		int addrRow = listBoxSearch(GetDlgItem(LB_ADDR), fns.lpstrFindWhat, caseSens, prev);
		int disasmRow = listBoxSearch(GetDlgItem(LB_DISASM), fns.lpstrFindWhat, caseSens, prev);

		if (!prev)
		{
			if (-1 == hexRow)
				hexRow = INT_MAX;
			if (-1 == addrRow)
				addrRow = INT_MAX;
			if (-1 == disasmRow)
				disasmRow = INT_MAX;

			disasmRow = min(disasmRow, addrRow);
			disasmRow = min(disasmRow, hexRow);
		}
		else
		{
			disasmRow = max(disasmRow, addrRow);
			disasmRow = max(disasmRow, hexRow);
		}

		if ((INT_MAX != disasmRow) && (-1 != disasmRow))
		{
			SendDlgItemMessage(LB_DISASM, LB_SETSEL, TRUE, disasmRow);
			BOOL dummy;
			OnSelChange(0, LB_DISASM, GetDlgItem(LB_DISASM), dummy);
		}
	}
	return 0;
}

LRESULT CDlgCodeEditor::OnMenu_Find(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (0 == FindTextHWND)
	{
		memset(&fns, 0, sizeof(fns));
		fns.lStructSize = sizeof(fns);
		fns.hwndOwner = m_hWnd;
		fns.Flags = FR_DOWN | FR_HIDEWHOLEWORD;
		fns.lpstrFindWhat = findTextBuffer;
		findTextBuffer[0] = 0;
		fns.wFindWhatLen = sizeof(findTextBuffer)/2;
		FindTextHWND = FindText(&fns);
	}
	else
		::SetFocus(FindTextHWND);
	return 0;
}

LRESULT CDlgCodeEditor::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (skipNextUserChar)
	{
		skipNextUserChar = false;
		return 1;
	}
	return 0;
}

LRESULT CDlgCodeEditor::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		BOOL dummy;
		skipNextUserChar = true;
		switch (wParam)
		{
			case 'C': OnMenu_CopySelection(0, 0, 0, dummy); break;
			case 'H': OnMenu_OpcodeDescription(0, 0, 0, dummy); break;
			case 'E': OnHex_DblClk(0, 0, 0, dummy); break;
			case 'F': OnMenu_Find(0, 0, 0, dummy); break;
			case 'A': OnMenu_CopyAll(0, 0, 0, dummy); break;
			default:
				skipNextUserChar = false;
				return 0;
		}
		return 1;
	}
	skipNextUserChar = false;
	return 0;
}

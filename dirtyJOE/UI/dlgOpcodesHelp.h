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
#pragma once


#include "pre_wtl.h"
#include "resource.h"
#include "ui_helpers.h"

class CDlgOpcodesHelp : public CDialogImpl<CDlgOpcodesHelp>
{
public:

	enum { IDD = DJ_OPCODE_HELP };

	BEGIN_MSG_MAP(CDlgOpcodesHelp)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_OPCODE_HELP, OnOpcodeHelp)
		COMMAND_HANDLER(LB_OP_LIST, LBN_SELCHANGE, OnLB_SelChange)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOpcodeHelp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgOpcodesHelp(CDirtyJOE& _dj) : dj(_dj), exitOnClose(false) {}
	CDlgOpcodesHelp(CDirtyJOE& _dj, bool exitOnClose) : dj(_dj), exitOnClose(exitOnClose) {}

private:

	bool exitOnClose;
	CDirtyJOE& dj;
	std::map<int, DWORD> lbmap;

	void updateInfo(int sel);
};

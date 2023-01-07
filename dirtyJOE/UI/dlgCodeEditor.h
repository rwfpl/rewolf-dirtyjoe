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
#pragma once

#include "pre_wtl.h"
#include "resource.h"
#include "CDirtyJOE.h"
#include "ui_helpers.h"

class CDlgCode;

class CDlgCodeEditor : public CDialogImpl<CDlgCodeEditor>
{
public:

	enum { IDD = DJ_CODE_MOD };

	BEGIN_MSG_MAP(CDlgCodeEditor)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_USER_CHAR, OnChar)
		MESSAGE_HANDLER(FindTextWndMsg, OnFindTextMsg)
		COMMAND_HANDLER(LB_HEX, LBN_USER_LBUTTONDBLCLK, OnHex_DblClk)
		COMMAND_HANDLER(LB_ADDR, LBN_USER_LBUTTONDBLCLK, OnHex_DblClk)
		COMMAND_HANDLER(LB_DISASM, LBN_USER_LBUTTONDBLCLK, OnHex_DblClk)
		COMMAND_HANDLER(LB_HEX, LBN_USER_RBUTTONUP, OnHex_RButtonUp)
		COMMAND_HANDLER(LB_ADDR, LBN_USER_RBUTTONUP, OnHex_RButtonUp)
		COMMAND_HANDLER(LB_DISASM, LBN_USER_RBUTTONUP, OnHex_RButtonUp)
		COMMAND_CODE_HANDLER(LBN_SELCHANGE, OnSelChange)
		COMMAND_CODE_HANDLER(LBN_USER_LBUTTONDOWN, OnUser_LButonDown)
		COMMAND_CODE_HANDLER(LBN_USER_VSCROLL, OnUser_VScroll)
		COMMAND_ID_HANDLER(ID_CODE_EDITOPCODE, OnHex_DblClk)
		COMMAND_ID_HANDLER(ID_CODE_ADDEXCEPTIONHANDLERFORSELECTEDCODE, OnMenu_AddExcHandler);
		COMMAND_ID_HANDLER(ID_CODE_COPYALL, OnMenu_CopyAll)
		COMMAND_ID_HANDLER(ID_CODE_COPYSELECTION, OnMenu_CopySelection)
		COMMAND_ID_HANDLER(ID_CODE_OPCODEDESCRIPTION, OnMenu_OpcodeDescription)
		COMMAND_ID_HANDLER(ID_CODE_FIND, OnMenu_Find)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFindTextMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnHex_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHex_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUser_LButonDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUser_VScroll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_CopySelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_OpcodeDescription(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_AddExcHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Find(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgCodeEditor(CDirtyJOE& _dj, CDlgCode& _dc, CAttrCode& _ac) : dj(_dj), ac(_ac), dlgCode(_dc), skipNextUserChar(false), FindTextWndMsg(0), FindTextHWND(0) {} 

private:

	CDirtyJOE& dj;
	CAttrCode& ac;
	CDlgCode& dlgCode;
	std::map<u2, u2> editMap_1;
	bool skipNextUserChar;

	FINDREPLACE fns;
	wchar_t findTextBuffer[1024];
	UINT FindTextWndMsg;
	HWND FindTextHWND;

	void refresh();
	void lbSelChangeHelper(WORD wID, WORD wID2, DWORD* selItems, DWORD selCount, DWORD curVis);
	void lbLBtnDownHelper(WORD wID, WORD wID2, POINT& cp);
	malloc_ptr<wchar_t> copyLBContent(bool selectedOnly, DWORD copyFlags);
	void copyHelper(bool selOnly);
};

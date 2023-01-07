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

class CDlgSimpleLBView: public CDialogImpl<CDlgSimpleLBView>
{
public:

	enum { IDD = DJ_LB_DLG };
	enum TYPE { Attributes, References, AsciiBuffer };

	BEGIN_MSG_MAP(CDlgSimpleLBView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_USER_CHAR, OnChar)
		COMMAND_HANDLER(LB_DJ_LB, LBN_USER_RBUTTONUP, OnLB_RButtonUp)
		COMMAND_HANDLER(EDT_SEARCH, EN_CHANGE, OnSearch_Change)
		COMMAND_ID_HANDLER(ID_ATTRIB_COPYALL, OnMenu_CopyAll)
		COMMAND_ID_HANDLER(ID_ATTRIB_COPYSELECTION, OnMenu_CopySelection)
		COMMAND_ID_HANDLER(BTN_PREV, OnBtn_Prev)
		COMMAND_ID_HANDLER(BTN_NEXT, OnBtn_Next)
		COMMAND_ID_HANDLER(CHK_CASE, OnBtn_CheckCase)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnLB_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_CopySelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Next(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Prev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_CheckCase(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSearch_Change(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgSimpleLBView(CDirtyJOE& _dj, TYPE _t, u2 _idx, attribute_info* _ai = 0) : dj(_dj), t(_t), idx(_idx), ai(_ai), asciiBuffer(0), skipNextUserChar(false) {} 
	CDlgSimpleLBView(CDirtyJOE& _dj, TYPE _t, const char* _asciiBuffer) : dj(_dj), t(_t), asciiBuffer(_asciiBuffer), ai(0), idx(0), skipNextUserChar(false) {} 

private:

	CDirtyJOE& dj;
	TYPE t;
	u2 idx;
	attribute_info* ai;
	const char* asciiBuffer;
	bool skipNextUserChar;

	void initAttributes();
	void initReferences();
	void initAsciiBuffer();
	void copyHelper(bool selOnly);

	void search(bool prev);
};

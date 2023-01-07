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
#include "dlgCodeEditor.h"
#include "StickyWindow.h"

class CDlgCode : public CDialogImpl<CDlgCode>
{
public:

	enum { IDD = DJ_CODE_DLG };

	BEGIN_MSG_MAP(CDlgCode)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_WINDOW_MOVING, OnWindowMoving)
		MESSAGE_HANDLER(WM_REFRESH, OnRefresh)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_USER_CHAR, OnChar)
		COMMAND_HANDLER(LB_ATTRS, LBN_USER_RBUTTONUP, OnAttr_RButtonUp)
		COMMAND_HANDLER(LB_ATTRS, LBN_DBLCLK, OnAttr_DblClk)
		COMMAND_HANDLER(LB_EXCS, LBN_USER_RBUTTONUP, OnExcs_RButtonUp)
		COMMAND_HANDLER(LB_EXCS, LBN_DBLCLK, OnMenu_Exc_EditException)
		COMMAND_ID_HANDLER(BTN_EDIT_MAXSTACK, OnBtn_MaxStack)
		COMMAND_ID_HANDLER(BTN_EDIT_MAXLOCALS, OnBtn_MaxLocals)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_DELETEATTRIBUTE, OnMenu_Attributes_DeleteAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_ADDATTRIBUTE, OnMenu_Attributes_AddAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_VIEWATTRIBUTE, OnMenu_Attributes_ViewAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_EDITATTRIBUTE, OnMenu_Attributes_EditAttribute)
		COMMAND_ID_HANDLER(ID_CODE_ADDEXCEPTION, OnMenu_Exc_AddException)
		COMMAND_ID_HANDLER(ID_CODE_DELETEEXCEPTION, OnMenu_Exc_DeleteException)
		COMMAND_ID_HANDLER(ID_CODE_EDITEXCEPTION, OnMenu_Exc_EditException)
		COMMAND_ID_HANDLER(ID_CODE_MOVEDOWN, OnMenu_Exc_MoveDown)
		COMMAND_ID_HANDLER(ID_CODE_MOVEUP, OnMenu_Exc_MoveUp)
		COMMAND_ID_HANDLER(ID_CODE_COPY, OnMenu_Exc_Copy)
		COMMAND_ID_HANDLER(ID_CODE_COPYALL, OnMenu_Exc_CopyAll)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRefresh(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWindowMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnExcs_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_MaxStack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_MaxLocals(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_DeleteAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_AddAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_ViewAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_EditAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_AddException(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_DeleteException(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_EditException(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_MoveDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_MoveUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_Copy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Exc_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgCode(CDirtyJOE& _dj, CAttrCode& _ac);

	void addException(u2 start_pc = 0, u2 end_pc = 0, u2 handler_pc = 0, u2 catch_type = 0);

private:

	CDirtyJOE& dj;
	CAttrCode& ac;
	StickyWindow stickyWnd;
	CDlgCodeEditor codeEd;
	bool skipNextUserChar;
	SubclassContext sc;

	void refresh();
	void refreshExceptions(bool selectLast = false);
};

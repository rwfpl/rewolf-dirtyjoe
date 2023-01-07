/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2013 ReWolf
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

class CDlgEditLocalVariableTable : public CDialogImpl<CDlgEditLocalVariableTable>
{
public:

	enum { IDD = DJ_EDIT_LOCALVARS };

	BEGIN_MSG_MAP(CDlgEditLocalVariableTable)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(LB_LOCALVARS, LBN_SELCHANGE, OnLB_SelChange)
		COMMAND_HANDLER(LB_LOCALVARS, LBN_USER_RBUTTONUP, OnLB_UserRButtonUp)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_LV_DESCRIPTOR, OnBtn_Descriptor)
		COMMAND_ID_HANDLER(BTN_LV_NAME, OnBtn_Name)
		COMMAND_ID_HANDLER(BTN_LV_ADD, OnMenu_AddLocalVar)
		COMMAND_ID_HANDLER(BTN_LV_DEL, OnMenu_DeleteLocalVar)
		COMMAND_ID_HANDLER(BTN_LV_EDITENTRY, OnBtn_EditEntry)
		COMMAND_ID_HANDLER(BTN_LV_SAVEENTRY, OnBtn_SaveEntry)
		COMMAND_ID_HANDLER(ID_LOCALVARS_ADDLOCALVARIABLE, OnMenu_AddLocalVar)
		COMMAND_ID_HANDLER(ID_LOCALVARS_DELETELOCALVARIABLE, OnMenu_DeleteLocalVar)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLB_UserRButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Descriptor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Name(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditEntry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_SaveEntry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_AddLocalVar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_DeleteLocalVar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditLocalVariableTable(CDirtyJOE& _dj, CAttrLocalVariableTable& _lvt) : dj(_dj), lvt(_lvt) {} 

private:

	CDirtyJOE& dj;
	CAttrLocalVariableTable& lvt;

	void updateList();
	void setItemText(u2 cpid, int ctrl);
	void updateInfo();
	void saveCurrent();
	local_vars* getLV();
	DWORD getLVTableSelection();
	void lockGUI();
	void unlockGUI();
};

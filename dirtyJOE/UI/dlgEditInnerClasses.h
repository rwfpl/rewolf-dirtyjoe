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

class CDlgEditInnerClasses : public CDialogImpl<CDlgEditInnerClasses>
{
public:

	enum { IDD = DJ_EDIT_INNERCLS };

	enum editIC_ID 
	{
		id_inner_class_info_index, 
		id_outer_class_info_index, 
		id_inner_name_index, 
		id_inner_class_access_flags
	};

	BEGIN_MSG_MAP(CDlgEditInnerClasses)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(LB_INNERCLS, LBN_SELCHANGE, OnLB_SelChange)
		COMMAND_HANDLER(LB_INNERCLS, LBN_USER_RBUTTONUP, OnLB_UserRButtonUp)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_IC_ACC_FLAGS, OnBtn_ICAccFlags)
		COMMAND_ID_HANDLER(BTN_IC_NAME_INDEX, OnBtn_ICNameIndex)
		COMMAND_ID_HANDLER(BTN_IC_INFO_INDEX, OnBtn_ICInfoIndex)
		COMMAND_ID_HANDLER(BTN_OC_INFO_INDEX, OnBtn_OCInfoIndex)
		COMMAND_ID_HANDLER(BTN_IC_ADD, OnMenu_AddInnerClass)
		COMMAND_ID_HANDLER(BTN_IC_DEL, OnMenu_DeleteInnerClass)
		COMMAND_ID_HANDLER(ID_INNERCLASSES_ADDINNERCLASS, OnMenu_AddInnerClass)
		COMMAND_ID_HANDLER(ID_INNERCLASSES_DELETEINNERCLASS, OnMenu_DeleteInnerClass)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLB_UserRButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_ICAccFlags(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_ICNameIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_ICInfoIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_OCInfoIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_AddInnerClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_DeleteInnerClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditInnerClasses(CDirtyJOE& _dj, CAttrInnerClasses& _ic) : dj(_dj), ic(_ic) {} 

private:

	CDirtyJOE& dj;
	CAttrInnerClasses& ic;

	void updateList();
	void setItemText(u2 cpid, int ctrl);
	std::vector<inner_class*>* getICTable(DWORD* sel = 0);
	inner_class* getIC();
	void editICField(editIC_ID id);
	void updateInfo();
};

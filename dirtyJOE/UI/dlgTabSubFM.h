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
#include "ui_helpers.h"

class CDlgTabSubFM : public CDialogImpl<CDlgTabSubFM>
{
public:

	enum { IDD = DJ_FIELDS_SUB_TAB_DLG };

	BEGIN_MSG_MAP(CDlgTabSubFM)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, ForwardToParent)
		MESSAGE_HANDLER(WM_USER_CHAR, ForwardToParent)
		COMMAND_ID_HANDLER(BTN_EDIT_NAME, OnBtn_EditName)
		COMMAND_ID_HANDLER(BTN_EDIT_DESCRIPTOR, OnBtn_EditDescriptor)
		COMMAND_ID_HANDLER(BTN_EDIT_FLAGS, OnBtn_EditFlags)
		COMMAND_HANDLER(LB_ATTRIBS, LBN_SELCHANGE, OnAttr_SelChange)
		COMMAND_HANDLER(LB_ATTRIBS, LBN_DBLCLK, OnAttr_DblClk)
		COMMAND_HANDLER(LB_ATTRIBS, LBN_USER_RBUTTONUP, OnAttr_RButtonUp)
		COMMAND_HANDLER(LB_ATTRIBS_PROPS, LBN_USER_RBUTTONUP, OnAttr_RButtonUp)
		COMMAND_HANDLER(LB_ATTRIBS_PROPS, LBN_DBLCLK, OnAttr_DblClk)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT ForwardToParent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_EditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditDescriptor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditFlags(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgTabSubFM(CDirtyJOE& _dj) : dj(_dj) {} 
	SubclassContext sc;

private:
	CDirtyJOE& dj;
};

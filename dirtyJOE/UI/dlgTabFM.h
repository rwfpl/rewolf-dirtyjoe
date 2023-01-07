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
#include "dlgTabSubFM.h"

class CDlgMain;

class CDlgTabFM : public CDialogImpl<CDlgTabFM>
{
	public:

	enum { IDD = DJ_FIELDS_TAB_DLG };

	BEGIN_MSG_MAP(CDlgTabFM)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_UPDATE_TAB, OnUpdateTab)
		MESSAGE_HANDLER(WM_SHOW_ATTRIB, OnShowAttrib)
		MESSAGE_HANDLER(WM_SHOW_ATTR_WND, OnShowAttrWnd)
		MESSAGE_HANDLER(WM_EDIT_FM, OnEditFM)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_USER_CHAR, OnChar)
		COMMAND_HANDLER(CPTAB_LB, LBN_SELCHANGE, OnFMLB_SelChange)
		COMMAND_HANDLER(CPTAB_LB, LBN_DBLCLK, OnFMLB_DblClick)
		COMMAND_HANDLER(CPTAB_LB, LBN_USER_RBUTTONUP, OnFMLB_RButtonUp)
		COMMAND_HANDLER(LB_ATTRIBS, LBN_USER_RBUTTONUP, OnAttr_RButtonUp)
		COMMAND_HANDLER(LB_ATTRIBS_PROPS, LBN_USER_RBUTTONUP, OnAttrProps_RButtonUp)
		COMMAND_ID_HANDLER(ID_METHODS_ADDMETHOD, OnMenu_MethodAdd)
		COMMAND_ID_HANDLER(ID_METHODS_DELETEMETHOD, OnMenu_MethodDelete)
		COMMAND_ID_HANDLER(ID_FIELDS_ADDFIELD, OnMenu_MethodAdd)
		COMMAND_ID_HANDLER(ID_FIELDS_DELETEFIELD, OnMenu_MethodDelete)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_DELETEATTRIBUTE, OnMenu_Attributes_DeleteAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_ADDATTRIBUTE, OnMenu_Attributes_AddAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_VIEWATTRIBUTE, OnMenu_Attributes_ViewAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_EDITATTRIBUTE, OnMenu_Attributes_EditAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIB_COPYALL, OnMenu_AttribProps_CopyAll)
		COMMAND_ID_HANDLER(ID_ATTRIB_COPYSELECTION, OnMenu_AttribProps_CopySelection)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowAttrib(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowAttrWnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEditFM(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnFMLB_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFMLB_DblClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFMLB_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_MethodAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_MethodDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttrProps_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_DeleteAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_AddAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_ViewAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_EditAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_AttribProps_CopyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_AttribProps_CopySelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgTabFM(CDirtyJOE& _dj, CDlgMain& dm, fm_type _tp) : dj(_dj), dlgMain(dm), tp(_tp), x_info(0), subTab(_dj), skipNextUserChar(false) {}
	void setLBFont(HFONT hFont);

	private:

	CDirtyJOE& dj;
	CDlgMain& dlgMain;
	fm_type tp;
	std::vector<field_info*>* x_info;
	CDlgTabSubFM subTab;
	bool skipNextUserChar;

	bool updateXInfo();
	void updateInfo(field_info& fi, bool method);
	bool isMethod();
	void updateList(bool setLastSel = false);
	void clearData(bool clearMainList = true);
	field_info* getCurrentFI();
	void editAttribute(bool firstAttr = false);
	void copyHelper(bool selOnly);
	void setModified();
};

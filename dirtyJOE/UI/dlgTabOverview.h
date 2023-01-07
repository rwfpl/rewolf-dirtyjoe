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

class CDlgMain;

class CDlgTabOverview : public CDialogImpl<CDlgTabOverview>
{
	public:

	enum { IDD = DJ_MAIN_TAB_DLG };

	BEGIN_MSG_MAP(CDlgTabOverview)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_UPDATE_TAB, OnUpdateTab)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_USER_CHAR, OnChar)
		COMMAND_ID_HANDLER(ID_INTERFACES_COPYTEXT, OnMenu_Interfaces_CopyText)
		COMMAND_ID_HANDLER(ID_INTERFACES_ADDINTERFACE, OnMenu_Interfaces_AddInterface)
		COMMAND_ID_HANDLER(ID_INTERFACES_EDITINTERFACE, OnMenu_Interfaces_EditInterface)
		COMMAND_ID_HANDLER(ID_INTERFACES_DELETEINTERFACE, OnMenu_Interfaces_DeleteInterface)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_DELETEATTRIBUTE, OnMenu_Attributes_DeleteAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_ADDATTRIBUTE, OnMenu_Attributes_AddAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_VIEWATTRIBUTE, OnMenu_Attributes_ViewAttribute)
		COMMAND_ID_HANDLER(ID_ATTRIBUTES_EDITATTRIBUTE, OnMenu_Attributes_EditAttribute)
		COMMAND_ID_HANDLER(BTN_EDIT_MIN_VER, OnBtn_EditMinVer)
		COMMAND_ID_HANDLER(BTN_EDIT_MAJ_VER, OnBtn_EditMajVer)
		COMMAND_ID_HANDLER(BTN_EDIT_ACCESS_FLAGS, OnBtn_EditAccessFlags)
		COMMAND_ID_HANDLER(BTN_EDIT_SUPER_CLASS, OnBtn_EditSuperClass)
		COMMAND_ID_HANDLER(BTN_EDIT_THIS_CLASS, OnBtn_EditThisClass)
		COMMAND_HANDLER(LB_ATTRIBUTES, LBN_USER_RBUTTONUP, OnAttr_RButtonUp)
		COMMAND_HANDLER(LB_ATTRIBUTES, LBN_DBLCLK, OnAttr_DblClk)
		COMMAND_HANDLER(LB_INTERFACES, LBN_USER_RBUTTONUP, OnInterfaces_RButtonUp)
		COMMAND_HANDLER(LB_INTERFACES, LBN_DBLCLK, OnInterfaces_DblClk)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnMenu_Interfaces_CopyText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Interfaces_AddInterface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Interfaces_EditInterface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Interfaces_DeleteInterface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_DeleteAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_AddAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_ViewAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_Attributes_EditAttribute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditMinVer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditMajVer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditAccessFlags(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditSuperClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_EditThisClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAttr_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInterfaces_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInterfaces_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgTabOverview(CDirtyJOE& _dj, CDlgMain& dm) : dj(_dj), dlgMain(dm), skipNextUserChar(false) {}

	private:

	CDirtyJOE& dj;
	CDlgMain& dlgMain;
	
	bool skipNextUserChar;

	void updateThisClass();
	void updateSuperClass();
	void updateAccessFlags();
	void updateInterfaces();
	void editInterface();
	void setModified();
};

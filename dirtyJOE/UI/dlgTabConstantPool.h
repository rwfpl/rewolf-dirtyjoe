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

class CDlgTabConstantPool : public CDialogImpl<CDlgTabConstantPool>
{
	public:

	enum { IDD = DJ_CPOOL_TAB_DLG };

	BEGIN_MSG_MAP(CDlgTabConstantPool)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_UPDATE_TAB, OnUpdateTab)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_USER_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_USER_CHAR, OnChar)
		COMMAND_ID_HANDLER(CHK_HEXIDX, OnChk_HexIdx)
		COMMAND_ID_HANDLER(CHK_SRCHCASE, OnChk_CaseSens)
		COMMAND_ID_HANDLER(BTN_CP_EDIT, OnBtn_Edit)
		COMMAND_ID_HANDLER(BTN_CP_ADD, OnBtn_Add)
		COMMAND_ID_HANDLER(BTN_CP_SHOWREF, OnBtn_ShowReferences)
		COMMAND_ID_HANDLER(ID_EDITITEM, OnMenu_EditItem)
		COMMAND_ID_HANDLER(ID_ADDITEM, OnMenu_AddItem)
		COMMAND_ID_HANDLER(ID_SHOWREFERENCES, OnMenu_ShowReferences)
		COMMAND_ID_HANDLER(ID_COPYTEXT, OnMenu_CopyText)
		COMMAND_ID_HANDLER(ID_RUNPYTHONSCRIPT, OnMenu_RunPythonScript)
		COMMAND_ID_HANDLER(ID_RUNPYTHONSCRIPTONALLUTF8OBJECTS, OnMenu_RunPyOnUtf8);
		COMMAND_HANDLER(CPTAB_LB, LBN_USER_RBUTTONUP, OnCPLB_RButtonUp)
		COMMAND_HANDLER(CPTAB_LB, LBN_DBLCLK, OnCPLB_DblClk)
		COMMAND_HANDLER(CMB_CP_FILTER, CBN_SELCHANGE, OnFilter_SelChange)
		COMMAND_HANDLER(EDT_SEARCH, EN_CHANGE, OnSearch_Change)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnChk_HexIdx(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnChk_CaseSens(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Edit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Add(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_ShowReferences(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_EditItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_AddItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_ShowReferences(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_CopyText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_RunPythonScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenu_RunPyOnUtf8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCPLB_RButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCPLB_DblClk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilter_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSearch_Change(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgTabConstantPool(CDirtyJOE& _dj, CDlgMain& dm) : dj(_dj), dlgMain(dm), skipNextUserChar(false) {}

private:

	static const struct t_typetab
	{
		u1 tagid;
		const wchar_t* tagname;
	} typetab[];

	CDirtyJOE& dj;
	CDlgMain& dlgMain;
	std::map<u2, u2> cpMap;
	bool skipNextUserChar;

	int getCPSelection();
	void editItem();
	void addItem();
	void showRefs();
	void copyText();
	void setModified();
	u1 getSelectedFilterTag();

	static void menuCallback(HMENU hMenu, void* obj);
};

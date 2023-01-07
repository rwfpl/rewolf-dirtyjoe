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
#include "CDJSettings.h"
#include "CDirtyJOE.h"

class CDlgSettings : public CDialogImpl<CDlgSettings>
{
public:

	enum { IDD = DJ_SETTINGS };

	BEGIN_MSG_MAP(CDlgSettings)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_SAVE, OnBtn_Save)
		COMMAND_ID_HANDLER(BTN_FONT, OnBtn_Font)
		COMMAND_HANDLER(CB_PYVER, CBN_SELCHANGE, OnPyVer_SelChange)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Font(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPyVer_SelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgSettings(CDirtyJOE& _dj) : dj(_dj), intFont(0)/*, pyVer(CDJSettings::PyNone)*/ {} 

private:
	CDirtyJOE& dj;

	static const int pyVerTabSize = 4;
	struct pyVerStr
	{
		CDJSettings::PyVer pyver;
		const wchar_t* pyverstr;
	};
	static const pyVerStr pyVerTab[pyVerTabSize];

	HFONT intFont;
	//CDJSettings::PyVer pyVer;

	void closeWithoutSave();
};

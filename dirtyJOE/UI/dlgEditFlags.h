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

class CDlgEditFlags : public CDialogImpl<CDlgEditFlags>
{
public:

	enum { IDD = DJ_EDIT_FLAGS };
	enum TYPE { CLASS, METHOD, FIELD, INNER_CLASS };

	BEGIN_MSG_MAP(CDlgEditFlags)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_SAVE, OnBtn_Save)
		COMMAND_HANDLER(EDT_FLAGS_RAW, EN_CHANGE, OnEdit_FlagsChange)
		COMMAND_CODE_HANDLER(BN_CLICKED, OnBtn_Clicked)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEdit_FlagsChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Clicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditFlags(CDirtyJOE& _dj, TYPE _t, u2 _flags) : dj(_dj), t(_t), flags(_flags) {} 
	u2 getRFlags() const;

private:

	static const struct _checkflags
	{
		u2 mask;
		int control;
	} 
	checkflags[];

	CDirtyJOE& dj;
	u2 flags;
	TYPE t;

	void setCheckBoxes(u2 _flags);
	u2 getCheckBoxes();
	u2 getFlags();
	void setFlags(u2 _flags);
};

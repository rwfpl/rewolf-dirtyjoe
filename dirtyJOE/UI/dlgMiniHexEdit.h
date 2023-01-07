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

class CDlgMiniHexEdit : public CDialogImpl<CDlgMiniHexEdit>
{
public:

	enum { IDD = DJ_MINI_HEX_EDIT };

	BEGIN_MSG_MAP(CDlgMiniHexEdit)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(EDT_HEX, EN_CHANGE, OnHex_Change)
		COMMAND_HANDLER(EDT_HEX, EN_ENTER, OnHex_Enter)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnHex_Change(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHex_Enter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgMiniHexEdit(CDirtyJOE& _dj, CAttrCode& _ac, POINT& _pt, u4 _offs) : dj(_dj), ac(_ac), pt(_pt), offs(_offs) {} 

private:

	CDirtyJOE& dj;
	CAttrCode& ac;
	POINT& pt;
	u4 offs;

	void refreshInfo(std::vector<u1>& vec);
};

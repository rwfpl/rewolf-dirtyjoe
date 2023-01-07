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

class CDlgEditRef : public CDialogImpl<CDlgEditRef>
{
public:

	enum { IDD = DJ_EDIT_REF };
	enum TYPE { ConstantPool, Attribute, Field, Method };

	BEGIN_MSG_MAP(CDlgEditRef)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDCANCEL, 0, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_SAVE, OnBtn_Save)
		COMMAND_ID_HANDLER(BTN_SHOW_REF, OnBtn_ShowRef)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_ShowRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgEditRef(CDirtyJOE& _dj, TYPE _t, u2 _cpIdx, attribute_info* _ai = 0) : dj(_dj), t(_t), cpIdx(_cpIdx), ai(_ai) {} 

private:

	CDirtyJOE& dj;
	std::map<u2, u2> editMap_1;
	std::map<u2, u2> editMap_2;
	u2 cpIdx;
	TYPE t;
	attribute_info* ai;

	void setStaticsClassNameAndType();
	void setStaticsNameDescriptor();
	void setStaticsMethodHandle();
	void setStaticsInvokeDynamic();
	bool init();
};

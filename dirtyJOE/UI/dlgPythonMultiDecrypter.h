/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
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
#include "../common/common_macros.h"

class CDlgPythonMultiDecrypt : public CDialogImpl<CDlgPythonMultiDecrypt>
{
public:

	struct pyDecryptedUtf8
	{
		u2 cpIndex;
		CONSTANT_Utf8_info* newStr;
	};

	enum { IDD = DJ_PYMULTIDECRYPT };

	BEGIN_MSG_MAP(CDlgPythonMultiDecrypt)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_SAVE, OnBtn_Save)
		COMMAND_ID_HANDLER(BTN_DECRYPT, OnBtn_Decrypt)
		COMMAND_ID_HANDLER(BTN_LOADSCRIPT, OnBtn_LoadScript)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Decrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_LoadScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgPythonMultiDecrypt(CDirtyJOE& _dj) : dj(_dj), scriptSource(nullptr) {}
	
protected:
	void initCommon();
	HWND listHwnd;
	CDirtyJOE& dj;

private:
	typedef void (*pyIterCallback)(CDlgPythonMultiDecrypt&);

	std::map<u2, u2> editMap_1;
	malloc_ptr<char> scriptSource;
	std::vector<pyDecryptedUtf8> pyDecryptedUtf8Tab;

	bool pyIterator(pyIterCallback clbck);

	static void clbck_Decrypt(CDlgPythonMultiDecrypt& _this);
	static void clbck_Save(CDlgPythonMultiDecrypt& _this);

	//variables used like a local vars in pyIterator, to simplyfy callbacks parameters
	u2 pyIter_idx;
	CONSTANT_Utf8_info* pyIter_decrStr;
	int pyIter_i;
};

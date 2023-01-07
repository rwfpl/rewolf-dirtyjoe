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
#include "dlgPythonMultiDecrypter.h"

class CDlgPythonMultiPreview : public CDlgPythonMultiDecrypt
{
public:
	BEGIN_MSG_MAP(CDlgPythonMultiPreview)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_SAVE, OnBtn_Save)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgPythonMultiPreview(CDirtyJOE& _dj, std::vector<CDlgPythonMultiDecrypt::pyDecryptedUtf8>& pyDU8T) : CDlgPythonMultiDecrypt(_dj), pyDecryptedUtf8Tab(pyDU8T) {}

private:
	std::vector<CDlgPythonMultiDecrypt::pyDecryptedUtf8>& pyDecryptedUtf8Tab;
};

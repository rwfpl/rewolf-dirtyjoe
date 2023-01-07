/*
* dirtyJOE - Java Overall Editor
* Copyright 2013 ReWolf
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
#include "../common/CListBoxLogger.h"

class CDlgRestoreDebugInfo : public CDialogImpl<CDlgRestoreDebugInfo>
{
public:

	enum { IDD = DJ_RESTORE_DEBUG };

	BEGIN_MSG_MAP(CDlgRestoreDebugInfo)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInit)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(BTN_CANCEL, OnBtn_Cancel)
		COMMAND_ID_HANDLER(BTN_RESTORE, OnBtn_Restore)
		COMMAND_ID_HANDLER(BTN_OUTPUT_FILE, OnBtn_OutputFile)
	END_MSG_MAP()

	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_Restore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtn_OutputFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CDlgRestoreDebugInfo(CDirtyJOE& _dj) : dj(_dj), classDir(nullptr), lbLog(nullptr), restoreLines(false), restoreLocals(false), restoreThreadHandle(0) {}
	~CDlgRestoreDebugInfo();

private:

	CDirtyJOE& dj;
	malloc_ptr<wchar_t> classDir;

	HANDLE restoreThreadHandle;
	CListBoxLogger* lbLog;
	bool restoreLines;
	bool restoreLocals;
	static DWORD __stdcall restoreThread(CDlgRestoreDebugInfo* _this);

	void close();
	bool isRestoreThreadActive();
};

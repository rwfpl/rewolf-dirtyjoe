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
#include "dlgRestoreDebugInfo.h"
#include "../common/common.h"
#include "../java_support/RestoreDebugInfo.h"
#include "../java_support/helpers.h"

CDlgRestoreDebugInfo::~CDlgRestoreDebugInfo()
{
	if (nullptr != lbLog)
		delete lbLog;
}

LRESULT CDlgRestoreDebugInfo::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if ((0 == dj.clf) || (0 == dj.logptr))
		return TRUE;

	CheckDlgButton(CHK_LOCAL_VARS, BST_CHECKED);
	CheckDlgButton(CHK_RESTORE_LINES, BST_CHECKED);

	const wchar_t* filePathName = dj.clf->GetFileName();
	SetDlgItemText(EDT_INPUT_CLASS, filePathName);

	wchar_t fileNameNoExt[_MAX_FNAME];
	wchar_t fileNameDrive[_MAX_DRIVE];
	wchar_t fileNameDir[_MAX_DIR];
	wchar_t fileNameExt[_MAX_EXT];
	if (0 != _wsplitpath_s(filePathName, fileNameDrive, fileNameDir, fileNameNoExt, fileNameExt))
		return TRUE;

	malloc_ptr<wchar_t> linesFileName = wcscat_var(fileNameDrive, fileNameDir, fileNameNoExt, L".joe", 0);
	if (nullptr == linesFileName)
		return TRUE;

	classDir = wcscat_var(fileNameDrive, fileNameDir, 0);

	SetDlgItemText(EDT_OUTPUT_JOE, linesFileName.get());

	//no need to check return value here
	lbLog = new (std::nothrow) CListBoxLogger(GetDlgItem(LB_DBG_LOG));

	return TRUE;
}

bool CDlgRestoreDebugInfo::isRestoreThreadActive()
{
	if (0 != restoreThreadHandle)
	{
		DWORD exitCode;
		GetExitCodeThread(restoreThreadHandle, &exitCode);
		if (STILL_ACTIVE == exitCode)
		{
			MessageBox(L"Restore Debug Info operation is still active, please wait.", L"Info", MB_ICONEXCLAMATION);
			return true;
		}
		CloseHandle(restoreThreadHandle);
		restoreThreadHandle = 0;
	}
	return false;
}

void CDlgRestoreDebugInfo::close()
{
	if (!isRestoreThreadActive())
		EndDialog(0);
}

LRESULT CDlgRestoreDebugInfo::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	close();
	return TRUE;
}

LRESULT CDlgRestoreDebugInfo::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	close();
	return FALSE;
}

DWORD __stdcall CDlgRestoreDebugInfo::restoreThread(CDlgRestoreDebugInfo* _this)
{
	if (nullptr == _this)
		return 0;

	if (nullptr == _this->lbLog)
	{
		_this->MessageBox(L"Logging interface problem.", L"Error", MB_ICONERROR);
		return 0;
	}

	if (_this->restoreLines)
	{
		_this->lbLog->log(1, "Restoring Line Numbers...\n");
		_this->lbLog->log(1, "\n");
		malloc_ptr<wchar_t> outName = getWindowText(_this->GetDlgItem(EDT_OUTPUT_JOE));
		if ((nullptr != outName) && RestoreLineNumbers(*_this->dj.clf, outName.get(), *_this->dj.logptr, *_this->lbLog))
			_this->lbLog->log(1, "Line Numbers restored successfully.\n");
		else
			_this->lbLog->log(1, "Failed to restore Line Numbers.\n");
		//empty line
		_this->lbLog->log(1, "\n");
	}

	if (_this->restoreLocals)
	{
		_this->lbLog->log(1, "Restoring Local Variables...\n");
		_this->lbLog->log(1, "\n");
		if (RestoreLocalVariables(*_this->dj.clf, *_this->dj.logptr, *_this->lbLog))
			_this->lbLog->log(1, "Local Variables restored successfully.\n");
		else
			_this->lbLog->log(1, "Failed to restore Local Variables.\n");
	}

	return 0;
}

LRESULT CDlgRestoreDebugInfo::OnBtn_Restore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ((0 == dj.clf) || (0 == dj.logptr))
		return TRUE;

	if (isRestoreThreadActive())
		return TRUE;

	SendDlgItemMessage(LB_DBG_LOG, LB_RESETCONTENT, 0, 0);

	restoreLines = (IsDlgButtonChecked(CHK_RESTORE_LINES) == BST_CHECKED);
	restoreLocals = (IsDlgButtonChecked(CHK_LOCAL_VARS) == BST_CHECKED);
	bool containSourceFile = false;
	bool containLineNumbers = false;
	bool containLocalVars = false;

	if (restoreLines && containsAttribute(dj.clf->get_constant_pool(), dj.clf->get_attributes(), ATTR_SOURCE_FILE))
		containSourceFile = true;
	
	if (restoreLines || restoreLocals)
	{
		auto mthIt = dj.clf->get_methods().begin();
		while (mthIt != dj.clf->get_methods().end())
		{
			CAttrCode* atCode = getCodeAttribute(**mthIt);
			if (nullptr != atCode)
			{
				if (!containLineNumbers && containsAttribute(dj.clf->get_constant_pool(), atCode->get_attributes(), ATTR_LINE_NUMBER_TABLE))
					containLineNumbers = true;
				
				if (!containLocalVars && containsAttribute(dj.clf->get_constant_pool(), atCode->get_attributes(), ATTR_LOCAL_VARIABLE_TABLE))
					containLocalVars = true;

				if (containLineNumbers && containLocalVars)
					break;
			}
			++mthIt;
		}

	}

	if ((restoreLines && (containSourceFile || containLineNumbers)) ||
		(restoreLocals && containLocalVars))
	{
		wchar_t strMsg[0x200];
		static const wchar_t msgFmt[] = 
			L"dirtyJOE detected that current .class file contains one of the following attributes:\n%s%s%s\nIf You click Yes, those attributes will be overwritten.";
		swprintf_s(strMsg, msgFmt, 
			(restoreLines && containSourceFile) ? L"\t- SourceFile\n" : L"", 
			(restoreLines && containLineNumbers) ? L"\t- LineNumberTable\n" : L"", 
			(restoreLocals && containLocalVars) ? L"\t- LocalVariableTable\n" : L"");
		if (IDNO == MessageBox(strMsg, L"Overwrite existing attributes?", MB_ICONQUESTION | MB_YESNO))
			return FALSE;
	}

	restoreThreadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)restoreThread, this, 0, nullptr);

	return FALSE;
}

LRESULT CDlgRestoreDebugInfo::OnBtn_OutputFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"dirtyJOE disassembly files (*.joe)\0*.joe\0All files\0*.*\0\0";
	wchar_t fNameBuf[MAX_PATH];
	fNameBuf[0] = 0;
	ofn.lpstrFile = fNameBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrInitialDir = classDir.get();
	if (GetSaveFileName(&ofn))
		SetDlgItemText(EDT_OUTPUT_JOE, fNameBuf);

	return FALSE;
}

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
#include <windows.h>
#include "CDJSettings.h."
#include "ui_helpers.h"
#include "../common/common.h"
#include "../common/AppDir.h"
#include "shellext.h"
#include "dlgSettings.h"

const CDJSettings::__pylib CDJSettings::pylibs[pycnt] =
{
	{L"PyJOE27.dll", L"python27.dll", Python27},
	{L"PyJOE26.dll", L"python26.dll", Python26},
	{L"PyJOE25.dll", L"python25.dll", Python25}
};

CDJSettings::CDJSettings() : error(false), hPyJOE(0), hMonoFont(0), iniFileName(nullptr), pyjoewrapper(0), hPython(0), shellExtAsked(false)
{
	memset(recentFiles, 0, sizeof(wchar_t*)*maxRexentFiles);

	malloc_ptr<wchar_t> backDir = SetAppDirAsCurrent();
	if (nullptr == backDir)
	{
		error = true;
		return;
	}

	malloc_ptr<wchar_t> curDir = GetCurrentDirectory();
	if (nullptr == curDir)
	{
		error = true;
		return;
	}

	iniFileName = wcscat_var(curDir.get(), L"\\dirtyjoe.ini", 0);
	if (nullptr == iniFileName)
	{
		error = true;
		return;
	}

	if (!(FileExist(iniFileName.get()) && loadSettings()))
	{
		if (!loadDefault())
		{
			error = true;
			return;
		}
		
		if (!saveSettings())
		{
			error = true;
			return;
		}
	}
	SetCurrentDirectory(backDir.get());
}

bool CDJSettings::loadDefaultFont()
{
	hMonoFont = (HFONT)GetStockObject(OEM_FIXED_FONT);
	if (0 == hMonoFont)
		return false;
	else
		return true;

// 	if (sizeof(fontStruct) != GetObject(hMonoFont, sizeof(fontStruct), &fontStruct))
// 		return false;
// 	else
// 		return true;
}

bool CDJSettings::loadDefault()
{
	//set default values
	SET_BOOL_DEFAULT(CopyAddr);
	SET_BOOL_DEFAULT(CopyHex);
	SET_BOOL_DEFAULT(CopyDisasm);
	SET_BOOL_DEFAULT(CPHexIdx);
	SET_BOOL_DEFAULT(CPCaseSensitive);
	SET_BOOL_DEFAULT(StartupUpdateCheck);
	SET_BOOL_DEFAULT(ShellExtension);
	SET_BOOL_DEFAULT(LBCaseSensitive);
	bool_setting_ShellExtension = (shellExt::Check_ShellExt_Ok == shellExt::checkShellExtension()) ? true : false;
	shellExtAsked = true;

	if (!loadDefaultFont())
		return false;

	pythonVersion = loadDefaultPython_internal();
	return true;
}

bool CDJSettings::writeSetting(const wchar_t* key, const wchar_t* value)
{
	if (FALSE == WritePrivateProfileString(L"Settings", key, value, iniFileName.get()))
		return false;
	else
		return true;
}

malloc_ptr<wchar_t> CDJSettings::readSettingStr(const wchar_t* key)
{
	malloc_ptr<wchar_t> retBuf = malloc<wchar_t>(1024);
	if (nullptr == retBuf)
		return nullptr;

	if (!readSettingStr(key, retBuf.get(), 1024))
		return nullptr;

	return retBuf;
}

bool CDJSettings::readSettingStr(const wchar_t* key, wchar_t* value, size_t valueSize)
{
	if (0 == GetPrivateProfileString(L"Settings", key, 0, value, (DWORD)valueSize, iniFileName.get()))
		return false;

	if (0 == wcslen(value))
		return false;
	else
		return true;
}

bool CDJSettings::readSettingInt(const wchar_t* key, int& value)
{
	value = GetPrivateProfileInt(L"Settings", key, 0xDEADBEEF, iniFileName.get());
	if (0xDEADBEEF == value)
		return false;

	return true;
}

bool CDJSettings::saveSettings()
{
	if (nullptr == iniFileName)
		return false;

	wchar_t tempStr[0x100];
	swprintf_s(tempStr, L"%d", pythonVersion);
	if (!writeSetting(L"PythonVersion", tempStr))
		return false;

	LOGFONT fontStruct = { 0 };
	if (sizeof(fontStruct) != GetObject(hMonoFont, sizeof(fontStruct), &fontStruct))
		return false;

	if (!writeSetting(L"FontFace", fontStruct.lfFaceName))
		return false;

	swprintf_s(tempStr, L"%d", fontStruct.lfHeight);
	if (!writeSetting(L"FontHeight", tempStr))
		return false;

	swprintf_s(tempStr, L"%d", fontStruct.lfWeight);
	if (!writeSetting(L"FontWeight", tempStr))
		return false;

	swprintf_s(tempStr, L"%d", fontStruct.lfCharSet);
	if (!writeSetting(L"FontCharset", tempStr))
		return false;

	if (!writeSetting(L"FontItalic", fontStruct.lfItalic ? L"1" : L"0"))
		return false;

	WRITE_BOOL_SETTING(StartupUpdateCheck);
	WRITE_BOOL_SETTING(ShellExtension);
	WRITE_BOOL_SETTING(CPHexIdx);
	WRITE_BOOL_SETTING(CPCaseSensitive);
	WRITE_BOOL_SETTING(CopyAddr);
	WRITE_BOOL_SETTING(CopyHex);
	WRITE_BOOL_SETTING(CopyDisasm);
	WRITE_BOOL_SETTING(LBCaseSensitive);

	for (int i = 0; i < maxRexentFiles; i++)
	{
		swprintf_s(tempStr, L"file%d", i);
		writeSetting(tempStr, recentFiles[i]);
	}

	return true;
}

bool CDJSettings::loadSettings()
{
	if (nullptr == iniFileName)
		return false;

	int tmp = 0;

	if (readSettingInt(L"PythonVersion", tmp))
		pythonVersion = (PyVer)tmp;
	else
		pythonVersion = PyNone;
	
	READ_BOOL_SETTING(StartupUpdateCheck);
	READ_BOOL_SETTING(ShellExtension);
	READ_BOOL_SETTING(CPHexIdx);
	READ_BOOL_SETTING(CPCaseSensitive);
	READ_BOOL_SETTING(CopyAddr);
	READ_BOOL_SETTING(CopyHex);
	READ_BOOL_SETTING(CopyDisasm);
	READ_BOOL_SETTING(LBCaseSensitive);

	for (int i = 0; i < maxRexentFiles; i++)
	{
		wchar_t tempStr[0x10];
		swprintf_s(tempStr, L"file%d", i);
		recentFiles[i] = readSettingStr(tempStr).release();
	}

	LOGFONT fontStruct = { 0 };
	wchar_t* t = fontStruct.lfFaceName;
	if (!readSettingStr(L"FontFace", t, LF_FACESIZE))
		return false;

	if (!readSettingInt(L"FontHeight", tmp))
		return false;
	fontStruct.lfHeight = tmp;

	if (!readSettingInt(L"FontWeight", tmp))
		return false;
	fontStruct.lfWeight = tmp;

	if (!readSettingInt(L"FontCharset", tmp))
		return false;
	fontStruct.lfCharSet = (BYTE)tmp;

	if (!readSettingInt(L"FontItalic", tmp))
		return false;
	fontStruct.lfItalic = tmp ? true : false;

	//load requested font (default if req failed)
	hMonoFont = CreateFontIndirect(&fontStruct);
	if ((0 == hMonoFont) && !loadDefaultFont())
		return false;

	//load requested Python (default if req failed)
	if (!loadReqPython(pythonVersion))
		pythonVersion = loadDefaultPython_internal();

	return true;
}

bool CDJSettings::loadReqPython(PyVer v)
{
	if ((PyNone != pythonVersion) && (0 != hPyJOE))
	{
		pyjoewrapper.Deinit();
		pyjoewrapper.SetNewModule(0);
		FreeLibrary(hPyJOE);
		FreeLibrary(hPython);
		hPython = 0;
	}

	hPyJOE = 0;
	pythonVersion = PyNone;

	if (PyNone == v)
		return true;

	malloc_ptr<wchar_t> appDir = GetAppDir();
	if (nullptr == appDir)
		return false;

	for (int i = 0; i < pycnt; i++)
	{
		if (pylibs[i].pyVer == v)
		{
			//check if specific python version is installed in the system
			hPython = LoadLibrary(pylibs[i].pythonDllName);
			if (0 == hPython)
				return false;

			malloc_ptr<wchar_t> pyJoePath = wcscat_var(appDir.get(), L"\\", pylibs[i].pyJoeLibName, 0);
			if (nullptr == pyJoePath)
				return false;

			hPyJOE = LoadLibrary(pyJoePath.get());
			if (0 == hPyJOE)
			{
				FreeLibrary(hPython);
				hPython = 0;
				return false;
			}

			if (!pyjoewrapper.SetNewModule(hPyJOE) || !pyjoewrapper.Init(hPython))
			{
				pyjoewrapper.Deinit();
				pyjoewrapper.SetNewModule(0);
				FreeLibrary(hPyJOE);
				FreeLibrary(hPython);
				hPython = 0;
				return false;
			}

			pythonVersion = v;
			break;
		}
	}

	return true;
}

CDJSettings::~CDJSettings()
{
	for (int i = 0; i < maxRexentFiles; i++)
	{
		if (nullptr != recentFiles[i])
			free(recentFiles[i]);
	}

	if (hMonoFont)
		DeleteObject(hMonoFont);

	if (hPyJOE)
	{
		pyjoewrapper.Deinit();
		FreeLibrary(hPyJOE);
	}

	if (hPython)
		FreeLibrary(hPython);
}

CDJSettings::PyVer CDJSettings::loadDefaultPython_internal()
{
	CAppDir appdir;
	if (appdir.getError())
		return PyNone;

	PyVer ret = PyNone;

	if (0 != hPyJOE)
	{
		pyjoewrapper.Deinit();
		FreeLibrary(hPyJOE);
		FreeLibrary(hPython);
		hPyJOE = 0;
		hPython = 0;
		pyjoewrapper.SetNewModule(0);
	}

	for (int i = 0; i < pycnt; i++)
	{
		//check if specific python version is installed in the system
		hPython = LoadLibrary(pylibs[i].pythonDllName);
		if (0 == hPython)
			continue;

		hPyJOE = LoadLibrary(pylibs[i].pyJoeLibName);
		if (hPyJOE)
		{
			if (!pyjoewrapper.SetNewModule(hPyJOE) || !pyjoewrapper.Init(hPython))
			{
				pyjoewrapper.Deinit();
				pyjoewrapper.SetNewModule(0);
				FreeLibrary(hPyJOE);
				FreeLibrary(hPython);
				hPyJOE = 0;
				hPython = 0;
				continue;
			}

			ret = pylibs[i].pyVer;
			break;
		}

		FreeLibrary(hPython);
		hPython = 0;
	}

	return ret;
}

void CDJSettings::addRecentFile(const wchar_t* rf)
{
	if (nullptr == rf)
		return;

	//check if it is not duplicate
	for (int i = 0; i < maxRexentFiles; i++)
	{
		if (nullptr != recentFiles[i])
		{
			if (0 == wcscmp(recentFiles[i], rf))
			{
				//exchange elements and return (except 0)
				if (0 != i)
				{
					wchar_t* tmp = recentFiles[i];
					memmove(recentFiles + 1, recentFiles, sizeof(wchar_t*)*i);
					recentFiles[0] = tmp;
				}
				return;
			}
		}
	}

	wchar_t* newRecent = _wcsdup(rf);
	if (nullptr == newRecent)
		return;

	if (nullptr != recentFiles[maxRexentFiles - 1])
		free(recentFiles[maxRexentFiles - 1]);

	memmove(recentFiles + 1, recentFiles, sizeof(wchar_t*)*(maxRexentFiles - 1));

	recentFiles[0] = newRecent;
}

void CDJSettings::removeRecentFile(wchar_t* rf)
{
	if (nullptr == rf)
		return;

	for (int i = 0; i < maxRexentFiles; i++)
	{
		if (nullptr != recentFiles[i])
		{
			if (0 == wcscmp(recentFiles[i], rf))
			{
				free(recentFiles[i]);
				if (i != maxRexentFiles - 1)
					memmove(recentFiles + i, recentFiles + i + 1, sizeof(wchar_t*)*(maxRexentFiles - i - 1));
				recentFiles[maxRexentFiles - 1] = nullptr;
				return; 
			}
		}
	}
}

wchar_t* CDJSettings::getRecentFile(int idx) const
{
	if (idx < maxRexentFiles)
		return recentFiles[idx];
	else
		return nullptr;
}

void CDJSettings::loadDefaultPython()
{
	pythonVersion = loadDefaultPython_internal();
}

PyJOEWrapper& CDJSettings::getPyModule() { return pyjoewrapper; }
void CDJSettings::setFontHandle(HFONT hFont) { hMonoFont = hFont; }
HFONT CDJSettings::getFontHandle() const { return hMonoFont; }
CDJSettings::PyVer CDJSettings::getPythonVersion() const { return pythonVersion; }
bool CDJSettings::getError() const { return error; }
int CDJSettings::getMaxRecentFiles() const { return maxRexentFiles; };

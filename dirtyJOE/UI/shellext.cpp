/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2008 ReWolf
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
#include "shellext.h"
#include "../common/common.h"
#include "../common/common_macros.h"
#include "ui_helpers.h"

malloc_ptr<wchar_t> shellExt::getClassFileSubKey(bool command)
{
	HKEY hKey = 0;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\.class\\", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return 0;
	
	malloc_ptr<wchar_t> subKey = regQueryWChar(hKey, 0);
	RegCloseKey(hKey);

	if (nullptr == subKey)
		return nullptr;

	return wcscat_var(L"SOFTWARE\\Classes\\", subKey.get(), L"\\shell\\Edit with dirtyJOE", (command) ? L"\\command" : L"", 0);
}

shellExt::checkShellExtRet shellExt::checkShellExtension(HWND hWnd /*= 0*/, bool askIfExist /*= true*/)
{
	malloc_ptr<wchar_t> subKey = getClassFileSubKey();
	if (nullptr == subKey)
		return Check_ShellExt_Error;

	HKEY hKey = 0;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey.get(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return Check_ShellExt_Error;

	malloc_ptr<wchar_t> keyVal = regQueryWChar(hKey, 0, 0);
	if (nullptr == keyVal)
	{
		RegCloseKey(hKey);
		return Check_ShellExt_Error;
	}

	malloc_ptr<wchar_t> appname = getAppNameString();
	if (nullptr == appname)
	{
		RegCloseKey(hKey);
		return Check_ShellExt_Error;
	}

	shellExt::checkShellExtRet ret = Check_ShellExt_Ok;
	if (0 != wcscmp(keyVal.get(), appname.get()))
	{
		if (askIfExist)
		{
			malloc_ptr<wchar_t> mbstr = wcscat_var(L"dirtyJOE shell extension is already registered for the different executable:\n", keyVal.get(), L"\n\nWould you like to update it to: ?\n", appname.get(), 0);
			const wchar_t* mbt = 0;
			if (nullptr == mbstr)
				mbt = L"dirtyJOE shell extension is already registered for the different executable, would you like to update it ?";
			else
				mbt = mbstr.get();

			if (IDYES == MessageBox(hWnd, mbt, L"dirtyJOE", MB_ICONQUESTION | MB_YESNO))
				addShellExtension(hWnd, false);
			else
				ret = Check_ShellExt_Keep;
		}
		else
			ret = Check_ShellExt_Keep;
	}

	RegCloseKey(hKey);
	return ret;
}

void shellExt::delShellExtension(HWND hWnd, bool silent)
{
	static const wchar_t errMsg[] = L"Cannot unregister shell extension.";

	malloc_ptr<wchar_t> delKey = getClassFileSubKey();
	if (nullptr == delKey)
		return;

	if (RegDeleteKey(HKEY_CURRENT_USER, delKey.get()) != ERROR_SUCCESS)
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	size_t ll = wcslen(delKey.get());
	while(delKey.get()[ll] != '\\')
		ll--;
	delKey.get()[ll] = 0;
	if (RegDeleteKey(HKEY_CURRENT_USER, delKey.get()) != ERROR_SUCCESS)
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	if (!silent)
		MessageBox(hWnd, L"Shell extension successfully unregistered.", L"dirtyJOE", MB_ICONINFORMATION);
}

malloc_ptr<wchar_t> shellExt::getAppNameString()
{
	wchar_t appname[MAX_PATH + 0x100] = L"\"";
	DWORD dummy = GetModuleFileName(0, appname + 1, MAX_PATH);
	if ((dummy == 0) || (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
		return nullptr;

	return wcscat_var(appname, L"\" \"%1\"", 0);
}

void shellExt::addShellExtension(HWND hWnd, bool silent)
{
	static const wchar_t errMsg[] = L"Cannot register shell extension.";
	HKEY dotKey = 0;
	DWORD dispo = 0;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\.class\\", 0, 0, 0, KEY_ALL_ACCESS, 0, &dotKey, &dispo) != ERROR_SUCCESS)
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	wchar_t subKey[0x100];
	if (dispo == REG_CREATED_NEW_KEY)
	{
		//create default subkey value "\classfile"
		static const wchar_t _classfile[] = L"classfile";	
		if (RegSetValueEx(dotKey, 0, 0, REG_SZ, (BYTE*)_classfile, sizeof(_classfile)) != ERROR_SUCCESS)
		{
			RegCloseKey(dotKey);
			MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
			return;
		}
		wcscpy_s(subKey, _classfile);
	}
	else
	{
		//read default subkey value
		DWORD t = _countof(subKey);
		DWORD keyType = 0;
		if ((RegQueryValueEx(dotKey, 0, 0, &keyType, (BYTE*)subKey, &t) != ERROR_SUCCESS) || (keyType != REG_SZ))
		{
			RegCloseKey(dotKey);
			MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
			return;
		}
	}
	RegCloseKey(dotKey);
	wchar_t mKeyStr[0x200] = L"SOFTWARE\\Classes\\";
	wcscat_s(mKeyStr, subKey);
	wcscat_s(mKeyStr, L"\\shell\\Edit with dirtyJOE\\command");
	HKEY mKey = 0;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, mKeyStr, 0, 0, 0, KEY_ALL_ACCESS, 0, &mKey, 0) != ERROR_SUCCESS)
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	malloc_ptr<wchar_t> appname = getAppNameString();
	if (nullptr == appname)
	{
		RegCloseKey(mKey);
		delShellExtension(hWnd);
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	if (RegSetValueEx(mKey, 0, 0, REG_SZ, (BYTE*)appname.get(), (DWORD)(2*(wcslen(appname.get()) + 1))) != ERROR_SUCCESS)
	{
		RegCloseKey(mKey);
		delShellExtension(hWnd);
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	RegCloseKey(mKey);
	
	if (!silent)
		MessageBox(hWnd, L"Shell extension registered successfully.", L"dirtyJOE", MB_ICONINFORMATION);
}

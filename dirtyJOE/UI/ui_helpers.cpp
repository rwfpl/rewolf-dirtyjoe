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
#include <CommCtrl.h>
#include <UrlMon.h>
#include <WinInet.h>
#include <Ole2.h>
#include "resource.h"
#include "version_info.h"
#include "../java_support/java_class_def.h"
#include "../java_support/helpers.h"
#include "../java_support/jdisasm/jdisasm.h"
#include "../common/common.h"
#include "../common/common_templates.h"
#include "ui_helpers.h"
#include "dlgCode.h"
#include "dlgSimpleLBView.h"

void setDlgItemTextWChar(HWND hWnd, UINT msg, wchar_t* wch)
{
	if (nullptr != wch)
		SendMessage(hWnd, msg, 0, (LPARAM)wch);
	else 
		SendMessage(hWnd, msg, 0, (LPARAM)L"#JOE#: error");
}

void setDlgItemTextWChar(HWND hWnd, int ID, UINT msg, wchar_t* wch)
{
	setDlgItemTextWChar(GetDlgItem(hWnd, ID), msg, wch);
}

void setDlgItemTextUtf8(HWND hWnd, UINT msg, CONSTANT_Utf8_info* utf8)
{
	if (!utf8) 
		SendMessage(hWnd, msg, 0, (LPARAM)L"#JOE#: error");
	else
		setDlgItemTextWChar(hWnd, msg, preprocessUtf8ToWChar(utf8).get());
}

void setDlgItemTextUtf8(HWND hWnd, int ID, UINT msg, CONSTANT_Utf8_info* utf8)
{
	setDlgItemTextUtf8(GetDlgItem(hWnd, ID), msg, utf8);
}

void showDisasm(HWND hWnd, int itemID, UINT msg, CAttrCode& attrObj, const JavaConstPool& cpInfo)
{
	u4 pos = 0;
#define ADD_CODE_DISSTR(_opstr, err_str) \
	malloc_ptr<wchar_t> dop = wcscat_var(str_pos, L" : ", (_opstr), 0); \
	if (nullptr != dop) \
		SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)dop.get()); \
	else \
		SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)(err_str))

	while (pos < attrObj.get_code().size())
	{
		int len = jdisasm_get_len(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos);
		wchar_t str_pos[0x10];
		swprintf_s(str_pos, L"%08X", pos);
		if (len > 0)
		{
			malloc_ptr<wchar_t> opstr = jdisasm_get_str(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos, &cpInfo);
			if (nullptr != opstr)
			{
				ADD_CODE_DISSTR(opstr.get(), L"error");
			}
			else
			{
				ADD_CODE_DISSTR(L"error", L"error");
			}
			pos += len;
		}
		else
		{
			ADD_CODE_DISSTR(L"#UNKNOWN OPCODE#", L"fatal error");
			pos++;
		}
	}
#undef ADD_CODE_DISSTR
}

void addCPStringWithPrefix(const JavaConstPool& cpInfo, HWND hWnd, int itemID, UINT msg, u2 cpIdx, const wchar_t* strLinePrefix)
{
	if (cpIdx)
	{
		malloc_ptr<wchar_t> scp = genStringFromCP(cpInfo, cpIdx, false);
		wchar_t ostr[0x1000];
		if (nullptr != scp)
			swprintf_s(ostr, L"%ws%ws", strLinePrefix, scp.get());
		else
			swprintf_s(ostr, L"%ws##ERROR##", strLinePrefix);
		
		SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)ostr);
	}
	else
	{
		wchar_t ostr[0x200];
		swprintf_s(ostr, L"%wsNULL", strLinePrefix);
		SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)ostr);
	}
}

void showAttributeWindow(attribute_info* attr, const JavaConstPool& cpInfo, HWND hWnd, int itemID, UINT msg)
{
	CONSTANT_Utf8_info* utf8 = getUtf8FromCP(attr->get_attribute_name_index(), cpInfo);
	if (nullptr == utf8) 
		return;

	JavaAttribute attrType = getAttributeType(utf8);
	bool showType = true;
	switch (attrType)
	{
		case ATTR_SIGNATURE:
		case ATTR_SOURCE_FILE:
			showType = false;
		case ATTR_CONSTANT_VALUE:
			{
				malloc_ptr<wchar_t> tstr = nullptr;
				if (typeid(attr->get_attr()) != typeid(CAttrUxTemplate<u2>&))
					tstr = malloc_ptr<wchar_t>(_wcsdup(strErrAttrMismatch));
				else
				{
					CAttrUxTemplate<u2>& attrObj = (CAttrUxTemplate<u2>&)attr->get_attr();
					tstr = genStringFromCP(cpInfo, (u4)-1, cpInfo[attrObj.get_value()], showType);
				}

				if (nullptr == tstr)
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (WPARAM)strErrMemory);
				}
				else
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (WPARAM)tstr.get());
					if (ATTR_SIGNATURE == attrType)
					{
						malloc_ptr<wchar_t> demangled = decodeDescriptor(tstr.get(), L"", L"");
						if (nullptr != demangled)
						{
							SendDlgItemMessage(hWnd, itemID, msg, 0, (WPARAM)L"Demangled signature: ");
							setDlgItemTextWChar(hWnd, itemID, msg, demangled.get());
						}
					}
				}
			}
			break;
		case ATTR_CODE:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrCode&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrCode& attrObj = (CAttrCode&)attr->get_attr();
				wchar_t tstr[0x200];
				#define ADD_CODE_PARAM(fnc, nm) \
					swprintf_s(tstr, L#nm L": %d", fnc); \
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tstr);

				ADD_CODE_PARAM(attrObj.get_max_stack(), max_stack);
				ADD_CODE_PARAM(attrObj.get_max_locals(), max_locals);
				ADD_CODE_PARAM((int)attrObj.get_code().size(), code_length);
				ADD_CODE_PARAM((int)attrObj.get_attributes().size(), attributes_count);
				ADD_CODE_PARAM((int)attrObj.get_exception_table().size(), exception_table_length);
				#undef ADD_CODE_PARAM

				SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"");

				showDisasm(hWnd, itemID, msg, attrObj, cpInfo);
			}
			break;
		case ATTR_EXCEPTIONS:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrExceptions&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrExceptions& attrExc = (CAttrExceptions&)attr->get_attr();
				for (DWORD i = 0; i < attrExc.get_table().size(); i++)
					setDlgItemTextWChar(hWnd, itemID, msg, genStringFromCP(cpInfo, attrExc.get_table()[i]->get_value()).get());
			}
			break;
		case ATTR_INNER_CLASSES:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrInnerClasses&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrInnerClasses& attrInnerC = (CAttrInnerClasses&)attr->get_attr();
				for (DWORD i = 0; i < attrInnerC.get_table().size(); i++)
				{
					wchar_t inCIdx[256];
					swprintf_s(inCIdx, L"InnerClass[%d]:", i);
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)inCIdx);
				
					addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, attrInnerC.get_table()[i]->get_inner_class_info_index(), L"    inner_class_info : ");
					addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, attrInnerC.get_table()[i]->get_outer_class_info_index(), L"    outer_class_info : ");
					addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, attrInnerC.get_table()[i]->get_inner_name_index(), L"    inner_name : ");

					malloc_ptr<wchar_t> flags = genWStringFromAccessFlags(attrInnerC.get_table()[i]->get_inner_class_access_flags(), false);
					wchar_t ostr[0x1000];
					if (nullptr != flags)
						swprintf_s(ostr, L"    access_flags : %ws", flags.get());
					else
						wcscpy_s(ostr, L"    access_flags: ##ERROR##");
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)ostr);
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"");
				}
			}
			break;
		case ATTR_ENCLOSING_METHOD:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrEnclosingMethod&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrEnclosingMethod& attrEncMeth = (CAttrEnclosingMethod&)attr->get_attr();

				SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"Class index:");
				addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, attrEncMeth.get_v1(), L"    ");
				SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"Method index:");
				addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, attrEncMeth.get_v2(), L"    ");
			}
			break;
		case ATTR_LINE_NUMBER_TABLE:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrLineNumberTable&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"    Start PC    |    Line number    ");
				SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"------------------------------------");
				CAttrLineNumberTable& attrLineNums = (CAttrLineNumberTable&)attr->get_attr();
				auto linesIt = attrLineNums.get_table().begin();
				while (linesIt != attrLineNums.get_table().end())
				{
					wchar_t tmpstr[256];
					swprintf_s(tmpstr, L"    %08X    |    %d", (*linesIt)->get_start_pc(), (*linesIt)->get_line_number());
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);
					++linesIt;
				}
			}
			break;
		case ATTR_LOCAL_VARIABLE_TABLE:
		case ATTR_LOCAL_VARIABLE_TYPE_TABLE:
			{
				//RTTI works also for CAttrLocalVariableTypeTable as it is the same type
				if (typeid(attr->get_attr()) != typeid(CAttrLocalVariableTable&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrLocalVariableTable& attrLocVars = (CAttrLocalVariableTable&)attr->get_attr();
				auto varsIt = attrLocVars.get_table().begin();
				int i = 0;
				while (varsIt != attrLocVars.get_table().end())
				{
					wchar_t tmpstr[256];
					swprintf_s(tmpstr, L"LocalVar[%d]:", i);
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					swprintf_s(tmpstr, L"  start_pc         : %08X", (*varsIt)->get_start_pc());
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					swprintf_s(tmpstr, L"  length           : %08X", (*varsIt)->get_length());
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, (*varsIt)->get_name_index(), L"  name_index       : ");
					addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, (*varsIt)->get_signature_index(), (attrType == ATTR_LOCAL_VARIABLE_TABLE) ? L"  descriptor_index : " : L"  signature_index  : ");

					swprintf_s(tmpstr, L"  index            : %08X", (*varsIt)->get_index());
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"");
					++varsIt;
					i++;
				}
			}
			break;
		case ATTR_SYNTHETIC:
			SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"Attribute Synthetic is an empty attribute.");
			break;
		case ATTR_DEPRECATED:
			SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"Attribute Deprecated is an empty attribute.");
			break;
		case ATTR_STACK_MAP:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrStackMap&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrStackMap& attrStackMap = (CAttrStackMap&)attr->get_attr();
				auto stackMapIt = attrStackMap.get_table().begin();
				int i = 0;
				while (stackMapIt != attrStackMap.get_table().end())
				{
					wchar_t tmpstr[256];
					swprintf_s(tmpstr, L"Entry[%d]:", i);
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					swprintf_s(tmpstr, L"  offset: %04X", (*stackMapIt)->get_offset());
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					auto localsIt = (*stackMapIt)->get_locals().begin();
					int j = 0;
					while (localsIt != (*stackMapIt)->get_locals().end())
					{
						malloc_ptr<wchar_t> strVti = genWStringFromVerificationTypeInfo(cpInfo, *localsIt);
						swprintf_s(tmpstr, L"  local[%d]: %ws", j, nullptr != strVti ? strVti.get() : L"#ERROR#");
						SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);
						++localsIt;
						j++;
					}

					auto stackIt = (*stackMapIt)->get_stack_items().begin();
					j = 0;
					while (stackIt != (*stackMapIt)->get_stack_items().end())
					{
						malloc_ptr<wchar_t> strVti = genWStringFromVerificationTypeInfo(cpInfo, *stackIt);
						swprintf_s(tmpstr, L"  stack_item[%d]: %ws", j, nullptr != strVti ? strVti.get() : L"#ERROR#");
						SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);
						++stackIt;
						j++;
					}
					
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"");

					++stackMapIt;
					i++;
				}
			}
			break;
		case ATTR_BOOTSTRAP_METHODS:
			{
				if (typeid(attr->get_attr()) != typeid(CAttrBootstrapMethods&))
				{
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)strErrAttrMismatch);
					break;
				}
				CAttrBootstrapMethods& attrBootstrapMethods = (CAttrBootstrapMethods&)attr->get_attr();
				auto bootStrapIt = attrBootstrapMethods.get_table().begin();
				int i = 0;
				while (bootStrapIt != attrBootstrapMethods.get_table().end())
				{
					wchar_t tmpstr[256];
					swprintf_s(tmpstr, L"Bootstrap Method[%d]:", i);
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);

					addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, (*bootStrapIt)->get_bootstrap_method_ref(), L"  ");

					swprintf_s(tmpstr, L"  Arguments:");
					SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)tmpstr);
					auto args = (*bootStrapIt)->get_bootstrap_arguments().begin();
					while (args != (*bootStrapIt)->get_bootstrap_arguments().end())
					{
						addCPStringWithPrefix(cpInfo, hWnd, itemID, msg, *args, L"    ");
						++args;
					}

					++bootStrapIt;
					i++;
				}
			}
			break;
		default:
			SendDlgItemMessage(hWnd, itemID, msg, 0, (LPARAM)L"Unknown attribute.");
			return;
	}
}

void badAllocMsg(HWND hWnd)
{
	MessageBox(hWnd, L"Memory allocation failure", L"Fatal Error", MB_ICONERROR);
}

void badAllocMsg(CLogger& log)
{
	log.log(1, "Fatal Error: Memory allocation failure");
}

malloc_ptr<wchar_t> getWindowText(HWND hWnd, unsigned int* olen)
{
	int len = GetWindowTextLength(hWnd);
	if (0 == len)
		return nullptr;

	malloc_ptr<wchar_t> wtxt  = malloc<wchar_t>(len + 1);
	if (nullptr == wtxt)
		return nullptr;

	if (0 == GetWindowText(hWnd, wtxt.get(), len + 1))
		return nullptr;

	if (nullptr != olen)
		*olen = len;

	return wtxt;
}

malloc_ptr<u1> GetWindowHexBuf(HWND hWnd, unsigned int* olen)
{
	malloc_ptr<wchar_t> wtxt = getWindowText(hWnd);
	if (nullptr == wtxt)
		return nullptr;

	u4 osz = 0;
	malloc_ptr<u1> hbuf = hexstr2data(wtxt.get(), &osz);
	if (nullptr != olen)
		*olen = osz;
	return hbuf;
}

std::vector<u1>* GetWindowHexBufV(HWND hWnd)
{
	unsigned int osz = 0;
	malloc_ptr<u1> hbuf = GetWindowHexBuf(hWnd, &osz);
	if (nullptr == hbuf)
		return nullptr;
	
	std::vector<u1>* retvec = new (std::nothrow) std::vector<u1>;
	if (nullptr == retvec)
		return nullptr;

	retvec->resize(osz);
	memmove(&(*retvec)[0], hbuf.get(), osz);
	return retvec;
}

void CopyWCharToClipBoard(HWND hWnd, wchar_t* str)
{
	size_t strsize = 2*(wcslen(str) + 1);
	HGLOBAL clipdata = GlobalAlloc(GMEM_MOVEABLE, strsize);
	if (0 == clipdata)
		return;
	void* clipptr = GlobalLock(clipdata);
	memmove(clipptr, str, strsize);
	GlobalUnlock(clipdata);

	if (OpenClipboard(hWnd) == FALSE)
	{
		GlobalFree(clipdata);
		return;
	}
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, clipdata);
	CloseClipboard();
}

void ShowContextMenu(HINSTANCE hInstance, HWND hWnd, LPCWSTR menuID, ctx_menu subMenu, void* obj, ContextMenuCallback menuCallback)
{
	HMENU hMenu = LoadMenu(hInstance, menuID);
	if (0 == hMenu)
		return;
	HMENU hSubMenu = GetSubMenu(hMenu, subMenu);
	if (hSubMenu)
	{
		if (obj && menuCallback)
			menuCallback(hSubMenu, obj);

		POINT cpt;
		GetCursorPos(&cpt);
		TrackPopupMenuEx(hSubMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpt.x, cpt.y, hWnd, 0);
	}
	DestroyMenu(hMenu);
}

bool FileExist(const wchar_t* fullFileName)
{
	HANDLE hFile = CreateFile(fullFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	CloseHandle(hFile);
	return true;
}

malloc_ptr<wchar_t> GetCurrentDirectory()
{
	DWORD bufLen = GetCurrentDirectory(0, 0);
	if (0 == bufLen)
		return nullptr;

	malloc_ptr<wchar_t> ret = malloc<wchar_t>(bufLen + 1);
	if (nullptr == ret)
		return nullptr;

	if (GetCurrentDirectory(bufLen + 1, ret.get()) == 0)
		return nullptr;

	return ret;
}

void appendScriptsPath()
{
	malloc_ptr<wchar_t> crdir = GetAppDir();
	if (nullptr == crdir)
		return;

	malloc_ptr<wchar_t> scdir = wcscat_var(crdir.get(), L"\\scripts\\", 0);
	if (nullptr == scdir)
		return;

	appendEnvironmentVariable(L"PYTHONPATH", scdir.get());
}

malloc_ptr<wchar_t> GetAppDir()
{
	static const size_t maxPath = 0x10000;	//in theory it shouldn't exceed 0x7FFF, but there are some statements in documentation that it can...
	malloc_ptr<wchar_t> appname = malloc<wchar_t>(maxPath);
	if (nullptr == appname)
		return nullptr;

#	ifdef DJ_TOTALCMD
	DWORD dummy = GetModuleFileName(GetModuleHandle(L"TotalJOE.wlx"), appname.get(), maxPath);
#	else
	DWORD dummy = GetModuleFileName(0, appname.get(), maxPath);
#	endif
	
	if ((0 == dummy) || (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
		return nullptr;

	const size_t maxLen = wcslen(appname.get()) + 1;

	malloc_ptr<wchar_t> appdir = malloc<wchar_t>(maxLen);
	if (nullptr == appdir)
		return nullptr;

	malloc_ptr<wchar_t> drive = malloc<wchar_t>(maxLen);
	if (nullptr == drive)
		return nullptr;

	if (0 != _wsplitpath_s(appname.get(), drive.get(), maxLen, appdir.get(), maxLen, 0, 0, 0, 0))
		return nullptr;

	return wcscat_var(drive.get(), appdir.get(), 0);
}

malloc_ptr<wchar_t> SetAppDirAsCurrent()
{
	malloc_ptr<wchar_t> ret = GetCurrentDirectory();
	if (nullptr == ret)
		return nullptr;

	malloc_ptr<wchar_t> npth = GetAppDir();
	if (nullptr == npth)
		return nullptr;

	SetCurrentDirectory(npth.get());

	return ret;
}

void RestoreCurrentDir(wchar_t* str)
{
	if (nullptr != str)
		SetCurrentDirectory(str);
}

malloc_ptr<wchar_t> CanonicalizePath(wchar_t* path)
{
	if (!PathIsRelative(path))
	{
		return malloc_ptr<wchar_t>(_wcsdup(path));
	}
	else
	{
		malloc_ptr<wchar_t> curDir = GetCurrentDirectory();
		if (nullptr == curDir)
			return nullptr;

		malloc_ptr<wchar_t> newPath = wcscat_var(curDir.get(), L"\\", path, 0);
		if (nullptr == newPath)
			return nullptr;

		if (normalizePath(newPath.get()))
			return newPath;
		else
			return nullptr;
	}
}

LRESULT CALLBACK edtCtrlASubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, edtCtrlASubClass, uIdSubclass);
		break; 
	case WM_KEYDOWN:
		{
			if (('A' == wParam) && (GetKeyState(VK_CONTROL) & 0x8000))
			{
				SendMessage(hWnd, EM_SETSEL, 0, -1);
				return 0;
			}
		}
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK edtHexSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, edtHexSubClass, uIdSubclass);
		break;
	case WM_CHAR:
		{
			if (isHexChar((wchar_t)wParam) || //hex chars
				(wParam == ' ') ||	          //space
				(wParam == 0x08) ||	          //0x08 - backspace
				(((wParam == 0x1A) || (wParam == 0x16) || (wParam == 0x18) || (wParam == 0x03)) && ((HIWORD(lParam) & 0xFC) == 0x2C))) //ctrl+(z,x,c,v)
			{
				break;
			}
			else
				return 0;
		}
		break;
	case WM_KEYUP:
		{
			if (wParam == VK_RETURN)
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), EN_ENTER), (LPARAM)hWnd);
		}
		break;
	case WM_PASTE:
		{
			if (OpenClipboard(hWnd) == FALSE)
				break;
			HANDLE clip = GetClipboardData(CF_UNICODETEXT);
			if (clip)
			{
				wchar_t* clip2 = (wchar_t*)GlobalLock(clip);
				while (*clip2)
				{
					if (*(DWORD*)(clip2) == 0x00780030)	//"0x"
					{
						clip2 += 2;
						while (*clip2 == '0')
							clip2++;
					}
					else
					{
						SendMessage(hWnd, WM_CHAR, *clip2, 0);
						clip2++;
					}
				}
				GlobalUnlock(clip);
			}
			CloseClipboard();
			return 0;
		}
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK lbSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	//printf("%08X %08X %08X %08X\n", hWnd, uMsg, wParam, lParam);
	switch (uMsg)
	{
	case WM_CHAR:
		{
			if (1 == SendMessage(GetParent(hWnd), WM_USER_CHAR, wParam, lParam))
				return 0;
		}
		break;
	case WM_KEYDOWN:
		{
			if (0 != dwRefData)
				((SubclassContext*)dwRefData)->lastWndKeyDown = hWnd;
			if (1 == SendMessage(GetParent(hWnd), WM_USER_KEYDOWN, wParam, lParam))
				return 0;
		}
		break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, lbSubClass, uIdSubclass);
		break;
	case WM_VSCROLL:
		{
			LRESULT tmp = DefSubclassProc(hWnd, uMsg, wParam, lParam);
			if (lParam != (LPARAM)-1)
			{
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), LBN_USER_VSCROLL), (LPARAM)hWnd);
			}
			return tmp;
		}
	case WM_MOUSEWHEEL:
		{
			LRESULT tmp = DefSubclassProc(hWnd, uMsg, wParam, lParam);
			SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), LBN_USER_VSCROLL), (LPARAM)hWnd);
			return tmp;
		}
	case WM_RBUTTONUP:
		{
			LRESULT tmp = DefSubclassProc(hWnd, uMsg, wParam, lParam);
			SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), LBN_USER_RBUTTONUP), lParam);
			return tmp;
		}
		break;
	case WM_USER_LB_LBUTTONDOWN:
		uMsg = WM_LBUTTONDOWN;
		break;
	case WM_LBUTTONDOWN:
		SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), LBN_USER_LBUTTONDOWN), (LPARAM)hWnd);
		break;
	case WM_LBUTTONDBLCLK:
		SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), LBN_USER_LBUTTONDBLCLK), (LPARAM)hWnd);
		break;
// 	case WM_MOUSEMOVE:
// 		{
// 			if (wParam & MK_LBUTTON)
// 			{	
// 				printf("lb drag: %d\n", SendMessage(hWnd, LB_GETSELCOUNT, 0, 0));
// 				//printf()
// 			}
// 		}
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

/*
//I'll keep this for the possible future use !
class BindStatusCallback : public IBindStatusCallback
{
	public:
		BindStatusCallback() {}
		~BindStatusCallback() {}
		STDMETHOD(OnStartBinding)(DWORD dwReserved, IBinding *pib) { return E_NOTIMPL; }
		STDMETHOD(GetPriority)(LONG *pnPriority) { return E_NOTIMPL; }
		STDMETHOD(OnLowResource)(DWORD reserved) { return E_NOTIMPL; }
		STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText) { return E_NOTIMPL; }
		STDMETHOD(OnStopBinding)(HRESULT hresult, LPCWSTR szError) { return E_NOTIMPL; }
		STDMETHOD(GetBindInfo)(DWORD *grfBINDF, BINDINFO *pbindinfo) { return E_NOTIMPL; }
		STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed) { return E_NOTIMPL; }
		STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown *punk) { return E_NOTIMPL; }
		STDMETHOD_(ULONG,AddRef)() { return 0; }
		STDMETHOD_(ULONG,Release)() { return 0; }
		STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject) { return E_NOTIMPL; }
};
*/

DWORD WINAPI updateThreadProc(LPVOID lpParameter)
{
	static const wchar_t* updateLink = L"http://dirty-joe.com/update" DJ_ARCH_W L".php?ver=" STR_W(DJ_VERSION_3);
	DeleteUrlCacheEntry(updateLink);
	if (URLDownloadToFile(0, updateLink, (LPCWSTR)lpParameter, 0, 0) != S_OK)
		return (DWORD)-1;

	return 0;
}

void checkForUpdate(HWND hWnd, bool silent)
{
	static const wchar_t errMsg[] = L"Update check failed.";
	wchar_t tempPath[MAX_PATH + 0x20];
	if (0 == GetTempPath(MAX_PATH, tempPath))
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}
	wcscat_s(tempPath, L"dirtyjoe.upd");
	// delete just to be sure
	DeleteFile(tempPath);

	HANDLE hThread = CreateThread(nullptr, 0, updateThreadProc, tempPath, 0, 0);
	if (0 != WaitForSingleObject(hThread, 5*1000))
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		CloseHandle(hThread);
		return;
	}
	CloseHandle(hThread);

	FILE* updf;
	if (_wfopen_s(&updf, tempPath, L"r") != 0)
	{
		DeleteFile(tempPath);
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	wchar_t verStr[0x20];
	if (fgetws(verStr, 0x20, updf) == 0)
	{
		fclose(updf);
		DeleteFile(tempPath);
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}
	wchar_t downLink[0x100];
	if (fgetws(downLink, 0x100, updf) == 0)
	{
		fclose(updf);
		DeleteFile(tempPath);
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}
	fclose(updf);
	DeleteFile(tempPath);

	int ver = 0;
	if (swscanf_s(verStr, L"%d", &ver) == 0)
	{
		MessageBox(hWnd, errMsg, L"Error", MB_ICONERROR);
		return;
	}

	if (ver <= DJ_VERSION_3)
	{
		if (!silent)
			MessageBox(hWnd, L"You're using the latest version of dirtyJOE.", L"Update Info", MB_ICONINFORMATION);

		return;
	}

	if (MessageBox(hWnd, L"Newer version of dirtyJOE is available. Do You want to download ?", L"Update Info", MB_ICONQUESTION | MB_YESNO) == IDYES)
		ShellExecute(hWnd, L"open", downLink, 0, 0, SW_SHOW);
}

malloc_ptr<char> loadScript(HWND ownerWnd, CLogger& log)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ownerWnd;
	ofn.lpstrFilter = L"Python .py files\0*.py\0All files\0*.*\0\0";
	wchar_t fNameBuf[MAX_PATH];
	fNameBuf[0] = 0;
	ofn.lpstrFile = fNameBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		size_t fSize = 0;
		return loadFileAsChar(fNameBuf, fSize, log);
	}
	return nullptr;
}

CONSTANT_Utf8_info* pythonUtf8Decrypter(CDirtyJOE& dj, HWND parentHWND, char* script, CONSTANT_Utf8_info& inputString)
{
	if (nullptr == script)
		return nullptr;

	tcs_remove(script, '\r');

	PyJOEWrapper& pyjoe = dj.settings.getPyModule();
	DWORD retSize = 0;
	wchar_t* errStr = 0;
	BYTE* retBuf = pyjoe.DecryptBuffer(script, "dj_decryptUTF8", &inputString.get_bytes()[0], (DWORD)inputString.get_bytes().size(), &retSize, &errStr);

	if (nullptr != errStr)
	{
		char* errExact = 0;
		if (pyjoe.GetError(&errExact))
		{
			malloc_ptr<char> asciiErrStr = wcs2char(errStr);
			if (nullptr != asciiErrStr)
			{
				size_t sepStrSize = strlen(asciiErrStr.get()) + 2;
				malloc_ptr<char> sepStr = malloc<char>(sepStrSize);
				if (nullptr != sepStr)
				{
					for (size_t i = 0; i < sepStrSize - 2; i++)
						sepStr.get()[i] = '-';

					sepStr.get()[sepStrSize - 2] = '\n';
					sepStr.get()[sepStrSize - 1] = 0;
					malloc_ptr<char> finalStr = tcscat_var(sepStr.get(), asciiErrStr.get(), "\n", sepStr.get(), "\n", errExact, 0);
					if (nullptr != finalStr)
						CDlgSimpleLBView(dj, CDlgSimpleLBView::AsciiBuffer, finalStr.get()).DoModal(parentHWND);
				}
			}
			pyjoe.FreeBuffer((BYTE*)errExact);
		}
		else
			MessageBox(parentHWND, errStr, L"Error", MB_ICONERROR);
	}
	else if (nullptr != retBuf)
	{
		CONSTANT_Utf8_info* utf8temp = new (std::nothrow) CONSTANT_Utf8_info(*dj.logptr);
		if (nullptr == utf8temp)
			return nullptr;

		utf8temp->get_bytes().resize(retSize + 1);
		memmove(&utf8temp->get_bytes()[0], retBuf, retSize);
		utf8temp->get_bytes()[retSize] = 0;
		pyjoe.FreeBuffer(retBuf);

		return utf8temp;
	}
	return nullptr;
}

malloc_ptr<wchar_t> getLBText(HWND lbh, DWORD index)
{
	size_t len = (size_t)SendMessage(lbh, LB_GETTEXTLEN, index, 0);
	if (LB_ERR == len)
		return nullptr;

	malloc_ptr<wchar_t> tmpStr = malloc<wchar_t>(len + 1);
	if (nullptr == tmpStr)
		return nullptr;

	if (LB_ERR == SendMessage(lbh, LB_GETTEXT, index, (LPARAM)tmpStr.get()))
		return nullptr;

	return tmpStr;
}

malloc_ptr<wchar_t> getLBContent(HWND lbh, bool onlySelected)
{
	DWORD cnt = (DWORD)SendMessage(lbh, LB_GETCOUNT, 0, 0);
	if (LB_ERR == cnt)
		return nullptr;

	malloc_ptr<wchar_t> outStr = nullptr;
	for (DWORD i = 0; i < cnt; i++)
	{
		if (onlySelected)
		{
			if (0 >= SendMessage(lbh, LB_GETSEL, i, 0))
				continue;
		}

		malloc_ptr<wchar_t> tmpStr = getLBText(lbh, i);
		if (nullptr == tmpStr)
			return nullptr;

		outStr = wcscat_var(outStr ? outStr.get() : L"", tmpStr.get(), L"\r\n", 0);
		if (nullptr == outStr)
			return nullptr;
	}

	return outStr;
}

bool appendEnvironmentVariable(wchar_t* envVar, wchar_t* addValue)
{
	static const int tbufSize = 0x7FFF;
	wchar_t tbuf[tbufSize] = { 0 };
	if (0 == GetEnvironmentVariable(envVar, tbuf, tbufSize))
	{
		if (ERROR_ENVVAR_NOT_FOUND != GetLastError())
			return false;
	}
	else
	{
		//check if already exist
		if (wcsstr(tbuf, addValue))
			return true;

		wcscat_s(tbuf, L";");
	}

	wcscat_s(tbuf, addValue);

	if (0 == SetEnvironmentVariable(envVar, tbuf))
		return false;

	return true;
}

malloc_ptr<BYTE> regQueryValue(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpType, DWORD* outSize)
{
	if (nullptr != outSize)
		*outSize = 0;

	DWORD os = 0;
	if (ERROR_SUCCESS != RegQueryValueEx(hKey, lpValueName, 0, lpType, 0, &os))
		return nullptr;

	malloc_ptr<BYTE> ret = malloc<BYTE>(os + 2);		//kind of hack to support non-zero ended char/wchar
	if (nullptr == ret)
		return nullptr;

	memset(ret.get(), 0, os + 2);

	if (ERROR_SUCCESS != RegQueryValueEx(hKey, lpValueName, 0, lpType, ret.get(), &os))
		return nullptr;

	if (nullptr != outSize)
		*outSize = os;

	return ret;
}

malloc_ptr<wchar_t> regQueryWChar(HKEY hKey, LPCWSTR lpValueName, DWORD* outSize)
{
	DWORD _type = 0;
	malloc_ptr<BYTE> rt = regQueryValue(hKey, lpValueName, &_type, outSize);
	if (REG_SZ != _type)
		return nullptr;

	return malloc_ptr<wchar_t>((wchar_t*)rt.release());
}

void dlgPrintf(HWND hWnd, int ID, wchar_t* fmtSTr, ...)
{
	va_list args;
	va_start(args, fmtSTr);
	int len = _vscwprintf(fmtSTr, args) + 1;
	malloc_ptr<wchar_t> tbuf = malloc<wchar_t>(len);
	if (nullptr == tbuf)
		return;
	_vsnwprintf_s(tbuf.get(), len, len - 1, fmtSTr, args);
	SetDlgItemText(hWnd, ID, tbuf.get());
}

bool dlgGetHexValue(HWND hWnd, int ID, int& v)
{
	wchar_t tbuf[0x100];
	if (GetDlgItemText(hWnd, ID, tbuf, 0x100))
	{
		if (swscanf_s(tbuf, L"0x%X", &v) == 1)
			return true;
	}
	return false;
}

void toolWindowHandleEscape(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((IDCANCEL == wParam) && (0 == lParam))
	{
		bHandled = true;
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
}

int listBoxSearch(HWND lbh, wchar_t* schText, bool caseSensitive, bool prev)
{
	const int lbcnt = (int)SendMessage(lbh, LB_GETCOUNT, 0, 0);
	if ((LB_ERR == lbcnt) || (nullptr == schText))
		return -1;

	size_t olen = wcslen(schText);

  	if (!caseSensitive)
		_wcslwr_s(schText, olen + 1);

	int lbpos = 0;
	if (0 != SendMessage(lbh, LB_GETSELITEMS, 1, (LPARAM)&lbpos))
	{
		if (prev)
			lbpos--;
		else
			lbpos++;

		if (lbpos < 0)
			lbpos = lbcnt + lbpos;
		else
			lbpos %= lbcnt;
	}


	SendMessage(lbh, LB_SETSEL, FALSE, -1);
	for (int i = 0; i < lbcnt; i++)
	{
		int curIt = lbpos + (prev ? -i : i);
		if (curIt < 0)
			curIt = lbcnt + curIt;
		else
			curIt %= lbcnt;

		malloc_ptr<wchar_t> itemText = getLBText(lbh, curIt);
		if (nullptr == itemText)
			continue;

		if (!caseSensitive)
			_wcslwr_s(itemText.get(), wcslen(itemText.get()) + 1);

		if (nullptr != wcsstr(itemText.get(), schText))
			return curIt;
	}

	return -1;
}

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
#pragma once

#include "CDirtyJOE.h"
#include "../common/common_macros.h"

enum fm_type {FIELDS, METHODS};

enum ctx_menu
{
	cp_menu,
	interfaces_menu,
	attributes_menu,
	innerclasses_menu,
	methods_menu,
	fields_menu,
	code_exc_menu,
	tcmd_menu,
	attrib_props_menu,
	code_ctx_menu,
	localvars_menu
};

struct SubclassContext
{
	HWND lastWndKeyDown;
	SubclassContext() : lastWndKeyDown(0) {}
};

typedef void (*ContextMenuCallback)(HMENU, void*);

void setDlgItemTextWChar(HWND hWnd, UINT msg, wchar_t* wch);
void setDlgItemTextWChar(HWND hWnd, int ID, UINT msg, wchar_t* wch);
void setDlgItemTextUtf8(HWND hWnd, UINT msg, CONSTANT_Utf8_info* utf8);
void setDlgItemTextUtf8(HWND hWnd, int ID, UINT msg, CONSTANT_Utf8_info* utf8);
void dlgPrintf(HWND hWnd, int ID, wchar_t* fmtSTr, ...);
bool dlgGetHexValue(HWND hWnd, int ID, int& v);
void showAttributeWindow(attribute_info* attr, const JavaConstPool& cpInfo, HWND hWnd, int itemID, UINT msg);
void showDisasm(HWND hWnd, int itemID, UINT msg, CAttrCode& attrObj, const JavaConstPool& cpInfo);
void badAllocMsg(HWND hWnd);
void badAllocMsg(CLogger& log);
malloc_ptr<u1> GetWindowHexBuf(HWND hWnd, unsigned int* olen = 0);
std::vector<u1>* GetWindowHexBufV(HWND hWnd);
void CopyWCharToClipBoard(HWND hWnd, wchar_t* str);
void ShowContextMenu(HINSTANCE hInstance, HWND hWnd, LPCWSTR menuID, ctx_menu subMenu, void* obj = 0, ContextMenuCallback menuCallback = 0);
malloc_ptr<wchar_t> SetAppDirAsCurrent();
malloc_ptr<wchar_t> GetCurrentDirectory();
bool FileExist(const wchar_t* fullFileName);
void RestoreCurrentDir(wchar_t* str);
void checkForUpdate(HWND hWnd, bool silent = false);
malloc_ptr<char> loadScript(HWND ownerWnd, CLogger& log);
CONSTANT_Utf8_info* pythonUtf8Decrypter(CDirtyJOE& dj, HWND parentHWND, char* script, CONSTANT_Utf8_info& inputString);
malloc_ptr<wchar_t> getLBText(HWND lbh, DWORD index);
malloc_ptr<wchar_t> getLBContent(HWND lbh, bool onlySelected = false);
malloc_ptr<wchar_t> getWindowText(HWND hWnd, unsigned int* olen = 0);
bool appendEnvironmentVariable(wchar_t* envVar, wchar_t* addValue);
void appendScriptsPath();
malloc_ptr<wchar_t> GetAppDir();
malloc_ptr<wchar_t> CanonicalizePath(wchar_t* path);
malloc_ptr<BYTE> regQueryValue(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpType, DWORD* outSize = 0);
malloc_ptr<wchar_t> regQueryWChar(HKEY hKey, LPCWSTR lpValueName, DWORD* outSize = 0);
void toolWindowHandleEscape(HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
int listBoxSearch(HWND lbh, wchar_t* schText, bool caseSensitive, bool prev);

//subclassed controls
LRESULT CALLBACK edtHexSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK edtCtrlASubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK lbSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

static const int WM_UPDATE_TAB            = WM_USER + 0x0666;
static const int WM_SHOW_ATTRIB           = WM_USER + 0x0667;
static const int WM_SHOW_ATTR_WND         = WM_USER + 0x0668;
static const int WM_USER_POST_VSCROLL     = WM_USER + 0x0669;
static const int WM_USER_LB_LBUTTONDOWN   = WM_USER + 0x066A;
static const int WM_EDIT_FM               = WM_USER + 0x066B;
static const int WM_REFRESH               = WM_USER + 0x066C;
static const int WM_OPCODE_HELP           = WM_USER + 0x066D;
static const int WM_USER_KEYDOWN          = WM_USER + 0x066E;
static const int WM_USER_CHAR             = WM_USER + 0x066F;
static const int WM_WINDOW_MOVING         = WM_USER + 0x0670;
static const int EN_ENTER                 = 0x0800;
static const int LBN_USER_VSCROLL         = 6;
static const int LBN_USER_LBUTTONDOWN     = 7;
static const int LBN_USER_RBUTTONUP       = 8;
static const int LBN_USER_LBUTTONDBLCLK   = 9;

static const int EDIT_FM_NAME             = 1;
static const int EDIT_FM_DESCRIPTOR       = 2;
static const int EDIT_FM_FLAGS            = 3;

static const int grid_space               = 4;

static const wchar_t* const strErrAttrMismatch = L"##ERROR## Attribute name or type mismatch";
static const wchar_t* const strErrMemory = L"##ERROR## memory allocation failure";
static const wchar_t* const strError = L"##ERROR##";

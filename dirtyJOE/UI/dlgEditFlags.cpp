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

#include <windows.h>
#include <Commctrl.h>
#include "CDirtyJOE.h"
#include "ui_helpers.h"

#include "dlgEditFlags.h"

const CDlgEditFlags::_checkflags CDlgEditFlags::checkflags[] =
{
	{ ACC_PUBLIC, CHK_PUBLIC },
	{ ACC_PRIVATE, CHK_PRIVATE },
	{ ACC_PROTECTED, CHK_PROTECTED },
	{ ACC_STATIC, CHK_STATIC },
	{ ACC_FINAL, CHK_FINAL },
	{ ACC_SUPER, CHK_SUPER },
	{ ACC_VOLATILE, CHK_VOLATILE },
	{ ACC_TRANSIENT, CHK_TRANSIENT },
	{ ACC_NATIVE, CHK_NATIVE },
	{ ACC_INTERFACE, CHK_INTERFACE },
	{ ACC_ABSTRACT, CHK_ABSTRACT },
	{ ACC_STRICT, CHK_STRICT },
	{ ACC_SYNTHETIC, CHK_SYNTHETIC },
	{ ACC_ANNOTATION, CHK_ANNOTATION },
	{ ACC_ENUM, CHK_ENUM }
};

LRESULT CDlgEditFlags::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	#define ENABLE_CHK(a) ::EnableWindow(GetDlgItem(a), TRUE)
	if (t == CLASS)
	{
		ENABLE_CHK(CHK_PUBLIC);
		ENABLE_CHK(CHK_FINAL);
		ENABLE_CHK(CHK_SUPER);
		ENABLE_CHK(CHK_INTERFACE);
		ENABLE_CHK(CHK_ABSTRACT);
		ENABLE_CHK(CHK_SYNTHETIC);
		ENABLE_CHK(CHK_ANNOTATION);
		ENABLE_CHK(CHK_ENUM);
	}
	else if (t == INNER_CLASS)
	{
		ENABLE_CHK(CHK_PUBLIC);
		ENABLE_CHK(CHK_PRIVATE);
		ENABLE_CHK(CHK_PROTECTED);
		ENABLE_CHK(CHK_STATIC);
		ENABLE_CHK(CHK_FINAL);
		ENABLE_CHK(CHK_INTERFACE);
		ENABLE_CHK(CHK_ABSTRACT);
		ENABLE_CHK(CHK_SYNTHETIC);
		ENABLE_CHK(CHK_ANNOTATION);
		ENABLE_CHK(CHK_ENUM);
	}
	else if (t == FIELD)
	{
		ENABLE_CHK(CHK_PUBLIC);
		ENABLE_CHK(CHK_PRIVATE);
		ENABLE_CHK(CHK_PROTECTED);
		ENABLE_CHK(CHK_STATIC);
		ENABLE_CHK(CHK_FINAL);
		ENABLE_CHK(CHK_VOLATILE);
		ENABLE_CHK(CHK_TRANSIENT);
		ENABLE_CHK(CHK_SYNTHETIC);
		ENABLE_CHK(CHK_ENUM);
	}
	else if (t == METHOD)
	{
		ENABLE_CHK(CHK_PUBLIC);
		ENABLE_CHK(CHK_PRIVATE);
		ENABLE_CHK(CHK_PROTECTED);
		ENABLE_CHK(CHK_STATIC);
		ENABLE_CHK(CHK_FINAL);
		ENABLE_CHK(CHK_SUPER);		//"synchronized"
		ENABLE_CHK(CHK_VOLATILE);	//"bridge"
		ENABLE_CHK(CHK_TRANSIENT);	//"varargs"
		ENABLE_CHK(CHK_NATIVE);
		ENABLE_CHK(CHK_ABSTRACT);
		ENABLE_CHK(CHK_STRICT);
		ENABLE_CHK(CHK_SYNTHETIC);
		SetDlgItemText(CHK_SUPER, L"synchronized");
		SetDlgItemText(CHK_VOLATILE, L"bridge");
		SetDlgItemText(CHK_TRANSIENT, L"varargs");
	}
#undef ENABLE_CHK

	SendDlgItemMessage(EDT_FLAGS_RAW, EM_LIMITTEXT, 4, 0);
	SetWindowSubclass(GetDlgItem(EDT_FLAGS_RAW), edtHexSubClass, 0x29e, 0);

	setFlags(flags);
	setCheckBoxes(flags);

	return TRUE;
}

LRESULT CDlgEditFlags::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditFlags::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditFlags::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	flags = getFlags();
	EndDialog(1);
	return FALSE;
}

LRESULT CDlgEditFlags::OnEdit_FlagsChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	setCheckBoxes(getFlags());
	return FALSE;
}

LRESULT CDlgEditFlags::OnBtn_Clicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ((wID == BTN_CANCEL) || (wID == BTN_SAVE))
		return FALSE;
	setFlags(getCheckBoxes());
	return FALSE;
}

void CDlgEditFlags::setCheckBoxes(u2 _flags)
{
	for (int i = 0; i < _countof(checkflags); i++)
		CheckDlgButton(checkflags[i].control, (_flags & checkflags[i].mask) ? BST_CHECKED : BST_UNCHECKED);
}

u2 CDlgEditFlags::getCheckBoxes()
{
	u2 ret = 0;
	for (int i = 0; i < _countof(checkflags); i++)
	{
		if (IsDlgButtonChecked(checkflags[i].control) == BST_CHECKED)
			ret |= checkflags[i].mask;
	}
	return ret;
}

u2 CDlgEditFlags::getFlags()
{
	wchar_t hexstr[0x21];
	::GetWindowText(GetDlgItem(EDT_FLAGS_RAW), hexstr, 0x20);
	u4 _flags = 0;
	swscanf_s(hexstr, L"%04X", &_flags);
	return (u2)_flags;
}

void CDlgEditFlags::setFlags(u2 _flags)
{
	wchar_t hexflags[0x20];
	swprintf_s(hexflags, L"%04X", _flags);
	SetDlgItemText(EDT_FLAGS_RAW, hexflags);
}

u2 CDlgEditFlags::getRFlags() const
{
	return flags;
}

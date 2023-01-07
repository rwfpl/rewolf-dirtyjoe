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

#include <typeinfo>
#include "dlgAddCPItem.h"
#include "../java_support/helpers.h"
#include "dlgEditUx.h"
#include "dlgEditRef.h"
#include "dlgEditUtf8.h"
#include "dlgEditVal.h"

LRESULT CDlgAddCPItem::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	for (int i = 0; i < _countof(cp_type_tab); i++)
	{
		SendDlgItemMessage(CB_ADD_CP, CB_ADDSTRING, 0, (LPARAM)cp_type_tab[i].tag_name);
	}
	SendDlgItemMessage(CB_ADD_CP, CB_SETCURSEL, 0, 0);

	return TRUE;
}

LRESULT CDlgAddCPItem::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgAddCPItem::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgAddCPItem::OnBtn_Add(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT idx = SendDlgItemMessage(CB_ADD_CP, CB_GETCURSEL, 0, 0);
	if (idx == CB_ERR)
		return false;

	u2 currentCPItem = (u2)dj.clf->get_constant_pool().size();
	bool emptyadded = false;
	switch (cp_type_tab[idx].tag_id)
	{
		case CONSTANT_Utf8:
		{
			CONSTANT_Utf8_info* tmp = new (std::nothrow) CONSTANT_Utf8_info(L"", *dj.logptr);
			if (tmp == 0)
			{
				badAllocMsg(m_hWnd);
				return true;
			}
			dj.clf->get_constant_pool().push_back(tmp);
			CDlgEditUtf8 dlg(dj, currentCPItem);
			CONSTANT_Utf8_info* nutf = (CONSTANT_Utf8_info*)dlg.DoModal(m_hWnd);
			if (nutf)
			{
				delete dj.clf->get_constant_pool()[currentCPItem];
				dj.clf->get_constant_pool()[currentCPItem] = nutf;
			}
			else
			{
				emptyadded = true;
			}
		}
		break;
		case CONSTANT_Class:
		case CONSTANT_String:
		case CONSTANT_MethodType:
		{
			u2 futf8 = getIdxOfFirstObjectByType(dj.clf->get_constant_pool(), CONSTANT_Utf8);
			if (futf8 == 0)
			{
				MessageBox(L"You've to add CONSTANT_Utf8 object first.", L"Info", MB_ICONINFORMATION);
				return true;
			}
			CONSTANT_uX_info<u2>* tmp = new (std::nothrow) CONSTANT_uX_info<u2>(cp_type_tab[idx].tag_id, futf8, *dj.logptr);
			if (tmp == 0)
			{
				badAllocMsg(m_hWnd);
				return true;
			}
			dj.clf->get_constant_pool().push_back(tmp);
			CDlgEditUx dlg(dj, CDlgEditUx::CPEDIT, currentCPItem);
			if (dlg.DoModal(m_hWnd))
			{
				((CONSTANT_uX_info<u2>*)dj.clf->get_constant_pool().back())->v1 = dlg.getCPIdx();
			}
			else
			{
				emptyadded = true;
			}
		}
		break;
		case CONSTANT_Long:
		case CONSTANT_Double:
		{
			CONSTANT_uXuX_info<u4, u4>* tmp = new (std::nothrow) CONSTANT_uXuX_info<u4, u4>(cp_type_tab[idx].tag_id, 0, 0, *dj.logptr);
			if (tmp == 0)
			{
				badAllocMsg(m_hWnd);
				return true;
			}
			dj.clf->get_constant_pool().push_back(tmp);
			dj.clf->get_constant_pool().push_back(0);
			if (CDlgEditVal(dj, currentCPItem).DoModal(m_hWnd) == 0)
			{
				emptyadded = true;
			}
		}
		break;
		case CONSTANT_Integer:
		case CONSTANT_Float:
		{
			CONSTANT_uX_info<u4>* tmp =  new (std::nothrow) CONSTANT_uX_info<u4>(cp_type_tab[idx].tag_id, 0, *dj.logptr);
			if (tmp == 0)
			{
				badAllocMsg(m_hWnd);
				return true;
			}
			dj.clf->get_constant_pool().push_back(tmp);
			if (CDlgEditVal(dj, currentCPItem).DoModal(m_hWnd) == 0)
			{
				emptyadded = true;
			}
		}
		break;
		case CONSTANT_Methodref:
		case CONSTANT_Fieldref:
		case CONSTANT_InterfaceMethodref:
		case CONSTANT_NameAndType:
		case CONSTANT_InvokeDynamic:
		{
			u2 f1;
			u2 f2;
			if (cp_type_tab[idx].tag_id == CONSTANT_NameAndType)
			{
				f1 = getIdxOfFirstObjectByType(dj.clf->get_constant_pool(), CONSTANT_Utf8);
				if (f1 == 0)
				{
					MessageBox(L"You've to add CONSTANT_Utf8 object first.", L"Info", MB_ICONINFORMATION);
					return true;
				}
				f2 = f1;
			}
			if (cp_type_tab[idx].tag_id == CONSTANT_InvokeDynamic)
			{
				attribute_info* attr = getAttributeByType(dj.clf->get_constant_pool(), dj.clf->get_attributes(), ATTR_BOOTSTRAP_METHODS);
				if (nullptr == attr)
				{
					MessageBox(L"You've to add BootstrapMethods attribute first.", L"Info", MB_ICONINFORMATION);
					return true;
				}
				else if (typeid(attr->get_attr()) == typeid(CAttrBootstrapMethods&))
				{
					CAttrBootstrapMethods& bootstrapMethods = (CAttrBootstrapMethods&)attr->get_attr();
					if (0 == bootstrapMethods.get_table().size())
					{
						MessageBox(L"You've to add at least one bootstrap method descriptor into BootstrapMethods attribute.", L"Info", MB_ICONINFORMATION);
						return true;
					}
					else
						f1 = 0;
				}
				else
				{
					MessageBox(L"Invalid BootstrapMethods attribute.", L"Info", MB_ICONINFORMATION);
					return true;
				}

				f2 = getIdxOfFirstObjectByType(dj.clf->get_constant_pool(), CONSTANT_NameAndType);
				if (f2 == 0)
				{
					MessageBox(L"You've to add CONSTANT_NameAndType object first.", L"Info", MB_ICONINFORMATION);
				}
			}
			else
			{
				f1 = getIdxOfFirstObjectByType(dj.clf->get_constant_pool(), CONSTANT_Class);
				if (f1 == 0)
				{
					MessageBox(L"You've to add CONSTANT_Class object first.", L"Info", MB_ICONINFORMATION);
				}

				f2 = getIdxOfFirstObjectByType(dj.clf->get_constant_pool(), CONSTANT_NameAndType);
				if (f2 == 0)
				{
					MessageBox(L"You've to add CONSTANT_NameAndType object first.", L"Info", MB_ICONINFORMATION);
				}
			}
			CONSTANT_uXuX_info<u2, u2>* tmp = new (std::nothrow) CONSTANT_uXuX_info<u2, u2>(cp_type_tab[idx].tag_id, f1, f2, *dj.logptr);
			if (nullptr == tmp)
			{
				badAllocMsg(m_hWnd);
				return true;
			}

			dj.clf->get_constant_pool().push_back(tmp);
			CDlgEditRef dlg(dj, CDlgEditRef::ConstantPool, currentCPItem);
			if (dlg.DoModal(m_hWnd) == 0)
			{
				emptyadded = true;
			}
		}
		break;
		case CONSTANT_MethodHandle:
		{
			CONSTANT_MethodHandle_info* tmp = new (std::nothrow) CONSTANT_MethodHandle_info(cp_type_tab[idx].tag_id, REF_getField, 0, *dj.logptr);
			if (nullptr == tmp)
			{
				badAllocMsg(m_hWnd);
				return true;
			}
			dj.clf->get_constant_pool().push_back(tmp);
			CDlgEditRef dlg(dj, CDlgEditRef::ConstantPool, currentCPItem);
			if (dlg.DoModal(m_hWnd) == 0)
			{
				emptyadded = true;
			}
		}
		break;
		default:
		{
			MessageBox(L"Not implemented yet.", L"Info", MB_ICONINFORMATION);
			return FALSE;
		}
		break;
	}
	if (emptyadded)
	{
		delete dj.clf->get_constant_pool()[currentCPItem];
		dj.clf->get_constant_pool().pop_back();
	}
	else
	{
		dj.clf->setModified();
		EndDialog(cp_type_tab[idx].tag_id);
	}
	return FALSE;
}

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
#include "dlgEditRef.h"
#include "dlgSimpleLBView.h"
#include "../java_support/helpers.h"

LRESULT CDlgEditRef::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!init())
	{
		EndDialog(0);
		return FALSE;
	}
	return TRUE;
}

bool CDlgEditRef::init()
{
	u2 _v1;
	u2 _v2;
	u1 cb1type = CONSTANT_Class;
	u1 cb2type = CONSTANT_NameAndType;
	u1 cb2type2 = CONSTANT_Undefined;

	u2 curSel_1 = 0;
	u2 curSel_2 = 0;

	if (t == ConstantPool)
	{
		cp_info* ccp = dj.clf->get_constant_pool()[cpIdx];
		if (ccp == 0)
			return false;

		if (CONSTANT_MethodHandle == ccp->tag)
		{
			_v1 = ((CONSTANT_uXuX_info<u1, u2>*)ccp)->v1;
			_v2 = ((CONSTANT_uXuX_info<u1, u2>*)ccp)->v2;
		}
		else
		{
			_v1 = ((CONSTANT_uXuX_info<u2, u2>*)ccp)->v1;
			_v2 = ((CONSTANT_uXuX_info<u2, u2>*)ccp)->v2;
		}

		switch (ccp->tag)
		{
		case CONSTANT_Methodref:
			SetWindowText(L"Edit CONSTANT_Methodref");
			setStaticsClassNameAndType();
			break;
		case CONSTANT_Fieldref:
			SetWindowText(L"Edit CONSTANT_Fieldref");
			setStaticsClassNameAndType();
			break;
		case CONSTANT_InterfaceMethodref:
			SetWindowText(L"Edit CONSTANT_InterfaceMethodref");
			setStaticsClassNameAndType();
			break;
		case CONSTANT_NameAndType:
			SetWindowText(L"Edit CONSTANT_NameAndType");
			setStaticsNameDescriptor();
			cb1type = CONSTANT_Utf8;
			cb2type = CONSTANT_Utf8;
			break;
		case CONSTANT_InvokeDynamic:
			{
				SetWindowText(L"Edit CONSTANT_InvokeDynamic");
				setStaticsInvokeDynamic();
				cb1type = CONSTANT_Undefined;

				attribute_info* attr = getAttributeByType(dj.clf->get_constant_pool(), dj.clf->get_attributes(), ATTR_BOOTSTRAP_METHODS);
				if ((nullptr != attr) && (typeid(attr->get_attr()) == typeid(CAttrBootstrapMethods&)))
				{
					CAttrBootstrapMethods& bootstrapMethods = (CAttrBootstrapMethods&)attr->get_attr();
					for (size_t i = 0; i < bootstrapMethods.get_table().size(); i++)
					{
						wchar_t tstr[64];
						tstr[0] = 0;
						swprintf_s(tstr, L"BootstrapMethod index: %d", _v1);
						u2 idx = (u2)SendDlgItemMessage(CB_EDIT_1, CB_ADDSTRING, 0, (LPARAM)tstr);
						editMap_1[idx] = (u2)i;
						if (_v1 == i)
							curSel_1 = idx;
					}
				}

				// reset _v1, so it will not mess with the rest of combo box initialization
				_v1 = 0;
			}
			break;
		case CONSTANT_MethodHandle:
			{
				SetWindowText(L"Edit CONSTANT_MethodHandle");
				setStaticsMethodHandle();
				cb1type = CONSTANT_Undefined;
				switch (_v1)
				{
				case REF_getField:
				case REF_getStatic:
				case REF_putField:
				case REF_putStatic:
					cb2type = CONSTANT_Fieldref;
					break;
				case REF_invokeVirtual:
				case REF_newInvokeSpecial:
					cb2type = CONSTANT_Methodref;
					break;
				case REF_invokeSpecial:
				case REF_invokeStatic:
					cb2type = CONSTANT_Methodref;
					if (dj.clf->get_major_version() >= 52)
						cb2type2 = CONSTANT_InterfaceMethodref;
					break;
				case REF_invokeInterface:
					cb2type = CONSTANT_InterfaceMethodref;
					break;

				}

				#define ADDREFKIND(a) \
				{ \
					u2 idx = (u2)SendDlgItemMessage(CB_EDIT_1, CB_ADDSTRING, 0, (LPARAM)L#a); \
					editMap_1[idx] = (u2)a; \
					if (_v1 == a) \
						curSel_1 = idx; \
				}
				ADDREFKIND(REF_getField);
				ADDREFKIND(REF_getStatic);
				ADDREFKIND(REF_putField);
				ADDREFKIND(REF_putStatic);
				ADDREFKIND(REF_invokeVirtual);
				ADDREFKIND(REF_invokeStatic);
				ADDREFKIND(REF_invokeSpecial);
				ADDREFKIND(REF_newInvokeSpecial);
				ADDREFKIND(REF_invokeInterface);
				#undef ADDREFKIND

				// reset _v1, so it will not mess with the rest of combo box initialization
				_v1 = 0;
			}
			break;
		default:
			return false;
		}
	}
	else if (t == Attribute)
	{
		setStaticsClassNameAndType();
		SetWindowText(L"Edit EnclosingMethods attribute");
		::ShowWindow(GetDlgItem(BTN_SHOW_REF), SW_HIDE);

		if (typeid(ai->get_attr()) != typeid(CAttrEnclosingMethod&))
		{
			MessageBox(strErrAttrMismatch, L"Error", MB_ICONERROR);
			return false;
		}
		CAttrEnclosingMethod& attrEM = (CAttrEnclosingMethod&)ai->get_attr();

		_v1 = attrEM.get_v1();
		_v2 = attrEM.get_v2();
	}
	else if ((t == Field) || (t == Method))
	{
		SetWindowText((t == Field) ? L"Add field" : L"Add method");
		setStaticsNameDescriptor();
		::ShowWindow(GetDlgItem(BTN_SHOW_REF), SW_HIDE);
		cb1type = CONSTANT_Utf8;
		cb2type = CONSTANT_Utf8;
		std::vector<field_info*>& x = (t == Field) ? dj.clf->get_fields() : dj.clf->get_methods();
		if (cpIdx >= x.size())
			return false;
		_v1 = x[cpIdx]->get_name_index();
		_v2 = x[cpIdx]->get_descriptor_index();
	}
	else
		return false;

	for (size_t i = 1; i < dj.clf->get_constant_pool().size(); i++)
	{
		cp_info* icp = dj.clf->get_constant_pool()[i];

		if (icp == 0)
			continue;

		malloc_ptr<wchar_t> tmpstr = nullptr;
		const wchar_t* newstr = nullptr;
		if ((cb1type == icp->tag) || (cb2type == icp->tag) || (cb2type2 == icp->tag) || (_v1 == i) || (_v2 == i))
		{
			tmpstr = genStringFromCP(dj.clf->get_constant_pool(), (u4)i, icp, true, dj.settings.get_CPHexIdx());
			newstr = (nullptr != tmpstr) ? tmpstr.get() : strErrMemory;
		}

		if ((cb1type == icp->tag) || (_v1 == i))
		{
			u2 idx = (u2)SendDlgItemMessage(CB_EDIT_1, CB_ADDSTRING, 0, (LPARAM)newstr);
			editMap_1[idx] = (u2)i;
			if (_v1 == i)
				curSel_1 = idx;
		}

		if ((cb2type == icp->tag) || (cb2type2 == icp->tag) || (_v2 == i))
		{
			u2 idx = (u2)SendDlgItemMessage(CB_EDIT_2, CB_ADDSTRING, 0, (LPARAM)newstr);
			editMap_2[idx] = (u2)i;
			if (_v2 == i)
				curSel_2 = idx;
		}
	}

	SendDlgItemMessage(CB_EDIT_1, CB_SETCURSEL, curSel_1, 0);
	SendDlgItemMessage(CB_EDIT_2, CB_SETCURSEL, curSel_2, 0);

	return true;
}

LRESULT CDlgEditRef::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgEditRef::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgEditRef::OnBtn_Save(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT idx_1 = SendDlgItemMessage(CB_EDIT_1, CB_GETCURSEL, 0, 0);
	LRESULT idx_2 = SendDlgItemMessage(CB_EDIT_2, CB_GETCURSEL, 0, 0);
	if ((idx_1 != CB_ERR) && (idx_2 != CB_ERR))
	{
		if (t == ConstantPool)
		{
			if (CONSTANT_MethodHandle ==  dj.clf->get_constant_pool()[cpIdx]->tag)
			{
				((CONSTANT_uXuX_info<u1, u2>*)dj.clf->get_constant_pool()[cpIdx])->v1 = (u1)editMap_1[(u2)idx_1];
				((CONSTANT_uXuX_info<u1, u2>*)dj.clf->get_constant_pool()[cpIdx])->v2 = editMap_2[(u2)idx_2];
			}
			else
			{
				((CONSTANT_uXuX_info<u2, u2>*)dj.clf->get_constant_pool()[cpIdx])->v1 = editMap_1[(u2)idx_1];
				((CONSTANT_uXuX_info<u2, u2>*)dj.clf->get_constant_pool()[cpIdx])->v2 = editMap_2[(u2)idx_2];
			}
		}
		else if (t == Attribute)
		{
			CAttrEnclosingMethod& attrEM = (CAttrEnclosingMethod&)ai->get_attr();
			attrEM.set_v1(editMap_1[(u2)idx_1]);
			attrEM.set_v2(editMap_2[(u2)idx_2]);
		}
		else if (t == Field)
		{
			dj.clf->get_fields()[cpIdx]->set_name_index(editMap_1[(u2)idx_1]);
			dj.clf->get_fields()[cpIdx]->set_descriptor_index(editMap_2[(u2)idx_2]);
		}
		else if (t == Method)
		{
			dj.clf->get_methods()[cpIdx]->set_name_index(editMap_1[(u2)idx_1]);
			dj.clf->get_methods()[cpIdx]->set_descriptor_index(editMap_2[(u2)idx_2]);
		}
		dj.clf->setModified();
		EndDialog(1);
	}
	else
	{
		MessageBox(L"Cannot get proper selection, value will not be changed.", L"Error", MB_ICONERROR);
	}
	return FALSE;
}

LRESULT CDlgEditRef::OnBtn_ShowRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDlgSimpleLBView(dj, CDlgSimpleLBView::References, cpIdx).DoModal(m_hWnd);
	return FALSE;
}

void CDlgEditRef::setStaticsInvokeDynamic()
{
	SetDlgItemText(STATIC_CB_1, L"Bootstrap method index:");
	SetDlgItemText(STATIC_CB_2, L"Name and Type:");
}

void CDlgEditRef::setStaticsClassNameAndType()
{
	SetDlgItemText(STATIC_CB_1, L"Class:");
	SetDlgItemText(STATIC_CB_2, L"Name and Type:");
}

void CDlgEditRef::setStaticsNameDescriptor()
{
	SetDlgItemText(STATIC_CB_1, L"Name:");
	SetDlgItemText(STATIC_CB_2, L"Descriptor:");
}

void CDlgEditRef::setStaticsMethodHandle()
{
	SetDlgItemText(STATIC_CB_1, L"Reference Kind:");
	SetDlgItemText(STATIC_CB_2, L"Reference Index:");
}

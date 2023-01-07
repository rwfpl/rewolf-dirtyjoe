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
#include "dlgAddAttribute.h"
#include "../java_support/helpers.h"
#include "dlgEditAttrConstantValue.h"
#include "dlgEditUx.h"
#include "dlgEditInnerClasses.h"
#include "dlgEditRef.h"
#include "dlgCode.h"
#include "dlgEditAttrExceptions.h"
#include "dlgEditLocalVariableTable.h"

LRESULT CDlgAddAttribute::OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(L"Add Attribute");
	for (int i = 0; i < _countof(attr_tab); i++)
	{
		if (checkSupport(attr_tab[i].attr_id))
		{
			u2 idx = (u2)SendDlgItemMessage(CB_ADD_CP, CB_ADDSTRING, 0, (LPARAM)attr_tab[i].attr_name);
			addAttrMap[idx] = (u2)i;
		}
	}
	SendDlgItemMessage(CB_ADD_CP, CB_SETCURSEL, 0, 0);
	return TRUE;
}

LRESULT CDlgAddAttribute::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return TRUE;
}

LRESULT CDlgAddAttribute::OnBtn_Cancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return FALSE;
}

LRESULT CDlgAddAttribute::OnBtn_Add(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LRESULT idx = SendDlgItemMessage(CB_ADD_CP, CB_GETCURSEL, 0, 0);
	if (idx == CB_ERR)
		return FALSE;

	idx = addAttrMap[(u2)idx];
	switch (attr_tab[idx].attr_id)
	{
		case ATTR_INNER_CLASSES:
		{
			attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
			if ((x == 0) || x->getError())
				break;

			aiv.push_back(x);
			dj.clf->setModified();
			if (typeid(x->get_attr()) != typeid(CAttrInnerClasses&))
			{
				MessageBox(strErrAttrMismatch, L"Error", MB_ICONERROR);
				break;
			}
			CDlgEditInnerClasses(dj, (CAttrInnerClasses&)x->get_attr()).DoModal(m_hWnd);
		}
		break;
		case ATTR_LOCAL_VARIABLE_TABLE:
			{
				attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
				if ((x == 0) || x->getError())
					break;

				aiv.push_back(x);
				dj.clf->setModified();
				if (typeid(x->get_attr()) != typeid(CAttrLocalVariableTable&))
				{
					MessageBox(strErrAttrMismatch, L"Error", MB_ICONERROR);
					break;
				}
				CDlgEditLocalVariableTable(dj, (CAttrLocalVariableTable&)x->get_attr()).DoModal(m_hWnd);
			}
			break;
		case ATTR_SYNTHETIC:
		case ATTR_DEPRECATED:
		{
			attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
			if ((x == 0) || x->getError())
				break;

			aiv.push_back(x);
			dj.clf->setModified();
		}
		break;
		case ATTR_CONSTANT_VALUE:
		case ATTR_SIGNATURE:
		case ATTR_SOURCE_FILE:
		{
			attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
			if ((x == 0) || x->getError())
				break;

			aiv.push_back(x);
			CAttrUxTemplate<u2>& attrSrc = (CAttrUxTemplate<u2>&)x->get_attr();
			INT_PTR dlgRes = 0;
			u2 dlgRes2 = 0;
			if (attr_tab[idx].attr_id == ATTR_CONSTANT_VALUE)
			{
				CDlgEditAttrConstantValue dlg(dj, attrSrc.get_value());
				dlgRes = dlg.DoModal(m_hWnd);
				dlgRes2 = dlg.getCPIdx();
			}
			else
			{
				CDlgEditUx dlg(dj, CDlgEditUx::UTF8, attrSrc.get_value());
				dlgRes = dlg.DoModal(m_hWnd);
				dlgRes2 = dlg.getCPIdx();
			}

			if (dlgRes)
			{
				attrSrc.set_value(dlgRes2);
				dj.clf->setModified();
			}
			else
			{
				aiv.pop_back();
				delete x;
			}
		}
		break;
		case ATTR_ENCLOSING_METHOD:
		{
			attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
			if ((x == 0) || x->getError())
				break;

			CDlgEditRef dlg(dj, CDlgEditRef::Attribute, 0, x);
			if (dlg.DoModal(m_hWnd) == 0)
			{
				delete x;
				break;
			}
			aiv.push_back(x);
			dj.clf->setModified();
		}
		break;
		case ATTR_CODE:
		{
			attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
			if ((x == 0) || x->getError())
				break;

			aiv.push_back(x);
			dj.clf->setModified();
			if (typeid(x->get_attr()) != typeid(CAttrCode&))
			{
				MessageBox(strErrAttrMismatch, L"Error", MB_ICONERROR);
				break;
			}
			CDlgCode(dj, (CAttrCode&)x->get_attr()).DoModal(m_hWnd);
		}
		break;
		case ATTR_EXCEPTIONS:
		{
			attribute_info* x = new (std::nothrow) attribute_info(attr_tab[idx].attr_id, dj.clf->get_constant_pool(), *dj.logptr);
			if ((x == 0) || x->getError())
				break;

			aiv.push_back(x);
			dj.clf->setModified();
			if (typeid(x->get_attr()) != typeid(CAttrExceptions&))
			{
				MessageBox(strErrAttrMismatch, L"Error", MB_ICONERROR);
				break;
			}
			CDlgEditAttrExceptions(dj, (CAttrExceptions&)x->get_attr()).DoModal(m_hWnd);
		}
		break;
		/*case ATTR_UNKNOWN:
			break;
		case ATTR_LINE_NUMBER_TABLE:
			break;
		case ATTR_SOURCE_DEBUG_EXTENSION:
			break;
		case ATTR_LOCAL_VARIABLE_TYPE_TABLE:
			break;
		case ATTR_RUNTIME_VISIBLE_ANNOTATIONS:
			break;
		case ATTR_RUNTIME_INVISIBLE_ANNOTATIONS:
			break;
		case ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS:
			break;
		case ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS:
			break;
		case ATTR_ANNOTATION_DEFAULT:
			break;
		case ATTR_STACK_MAP:
			break;*/
		default:
		MessageBox(L"Not supported yet ;(", L"Add attribute", MB_ICONEXCLAMATION);
		break;
	}
	EndDialog(0);
	return FALSE;
}

bool CDlgAddAttribute::checkSupport(JavaAttribute attrID)
{
	switch (attrID)
	{
	case ATTR_INNER_CLASSES:
	case ATTR_SYNTHETIC:
	case ATTR_DEPRECATED:
	case ATTR_CONSTANT_VALUE:
	case ATTR_SIGNATURE:
	case ATTR_SOURCE_FILE:
	case ATTR_ENCLOSING_METHOD:
	case ATTR_CODE:
	case ATTR_EXCEPTIONS:
	case ATTR_LOCAL_VARIABLE_TABLE:
		return true;
	}
	return false;
}

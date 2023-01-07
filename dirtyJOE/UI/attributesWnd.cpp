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

#include <typeinfo>
#include "attributesWnd.h"
#include "ui_helpers.h"
#include "dlgAddAttribute.h"
#include "dlgSimpleLBView.h"
#include "dlgEditUx.h"
#include "dlgEditRef.h"
#include "dlgEditInnerClasses.h"
#include "dlgCode.h"
#include "dlgEditAttrExceptions.h"
#include "dlgEditAttrConstantValue.h"
#include "dlgEditLocalVariableTable.h"
#include "../java_support/helpers.h"

void attributesWnd::deleteCurrentAttribute(HWND attrWnd, std::vector<attribute_info*>& aiv)
{
	LRESULT crs = SendMessage(attrWnd, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return;

	if ((size_t)crs >= aiv.size())
		return;

	if (aiv[crs])
		delete aiv[crs];

	aiv.erase(aiv.begin() + crs);
}

void attributesWnd::updateAttributes(HWND attrWnd, std::vector<attribute_info*>& aiv, JavaConstPool& cp)
{
	SendMessage(attrWnd, LB_RESETCONTENT, 0, 0);
	for (DWORD i = 0; i < aiv.size(); i++)
	{
		CONSTANT_Utf8_info* attName = getUtf8FromCP(aiv[i]->get_attribute_name_index(), cp);
		setDlgItemTextUtf8(attrWnd, LB_ADDSTRING, attName);
	}
}

void attributesWnd::addAttribute(HWND parentWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj)
{
	CDlgAddAttribute(dj, aiv).DoModal(parentWnd);
}

attribute_info* attributesWnd::getAttribute(HWND attrWnd, std::vector<attribute_info*>& aiv)
{
	LRESULT crs = SendMessage(attrWnd, LB_GETCURSEL, 0, 0);
	if (crs == LB_ERR)
		return 0;

	if ((size_t)crs >= aiv.size())
		return 0;

	return aiv[crs];
}

void attributesWnd::viewAttribute(HWND parentWnd, HWND attrWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj)
{
	CDlgSimpleLBView(dj, CDlgSimpleLBView::Attributes, 0, getAttribute(attrWnd, aiv)).DoModal(parentWnd);
}

bool attributesWnd::isEditableAttribute(HWND attrWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj)
{
	attribute_info* ai = getAttribute(attrWnd, aiv);
	if (ai == 0)
		return false;

	CONSTANT_Utf8_info* utf8 = getUtf8FromCP(ai->get_attribute_name_index(), dj.clf->get_constant_pool());
	if (!utf8) 
		return false;

	JavaAttribute attrType = getAttributeType(utf8);
	switch (attrType)
	{
		case ATTR_SIGNATURE:
		case ATTR_SOURCE_FILE:
		case ATTR_CONSTANT_VALUE:
		case ATTR_ENCLOSING_METHOD:
		case ATTR_INNER_CLASSES:
		case ATTR_CODE:
		case ATTR_EXCEPTIONS:
		case ATTR_LOCAL_VARIABLE_TABLE:
			return true;
	}
	return false;
}

void attributesWnd::editAttribute(HWND parentWnd, attribute_info* ai, CDirtyJOE& dj)
{
	if (ai == 0)
		return;

	CONSTANT_Utf8_info* utf8 = getUtf8FromCP(ai->get_attribute_name_index(), dj.clf->get_constant_pool());
	if (!utf8) 
		return;

	JavaAttribute attrType = getAttributeType(utf8);
	switch (attrType)
	{
	case ATTR_SIGNATURE:
	case ATTR_SOURCE_FILE:
	case ATTR_CONSTANT_VALUE:
		{
			if (typeid(ai->get_attr()) != typeid(CAttrUxTemplate<u2>&))
			{
				MessageBox(parentWnd, strErrAttrMismatch, L"Error", MB_ICONERROR);
				return;
			}
			CAttrUxTemplate<u2>& attrSrc = (CAttrUxTemplate<u2>&)ai->get_attr();

			INT_PTR dlg_r = 0;
			u2 dlg_r2 = 0;
			if (attrType == ATTR_CONSTANT_VALUE)
			{
				CDlgEditAttrConstantValue dlg(dj, attrSrc.get_value());
				dlg_r = dlg.DoModal(parentWnd);
				dlg_r2 = dlg.getCPIdx();
			}
			else
			{
				CDlgEditUx dlg(dj, CDlgEditUx::UTF8, attrSrc.get_value());
				dlg_r = dlg.DoModal(parentWnd);
				dlg_r2 = dlg.getCPIdx();
			}
			if (dlg_r)
			{
				attrSrc.set_value(dlg_r2);
				dj.clf->setModified();
			}
		}
		break;
	case ATTR_ENCLOSING_METHOD:
		{
			CDlgEditRef(dj, CDlgEditRef::Attribute, 0, ai).DoModal(parentWnd);
		}
		break;
	case ATTR_INNER_CLASSES:
		{
			if (typeid(ai->get_attr()) != typeid(CAttrInnerClasses&))
			{
				MessageBox(parentWnd, strErrAttrMismatch, L"Error", MB_ICONERROR);
				return;
			}
			CDlgEditInnerClasses(dj, (CAttrInnerClasses&)ai->get_attr()).DoModal(parentWnd);
		}
		break;
	case ATTR_LOCAL_VARIABLE_TABLE:
		{
			if (typeid(ai->get_attr()) != typeid(CAttrLocalVariableTable&))
			{
				MessageBox(parentWnd, strErrAttrMismatch, L"Error", MB_ICONERROR);
				return;
			}
			CDlgEditLocalVariableTable(dj, (CAttrLocalVariableTable&)ai->get_attr()).DoModal(parentWnd);
		}
		break;
	case ATTR_CODE:
		{
			if (typeid(ai->get_attr()) != typeid(CAttrCode&))
			{
				MessageBox(parentWnd, strErrAttrMismatch, L"Error", MB_ICONERROR);
				return;
			}
			CDlgCode(dj, (CAttrCode&)ai->get_attr()).DoModal(parentWnd);
		}
		break;
	case ATTR_EXCEPTIONS:
		{
			if (typeid(ai->get_attr()) != typeid(CAttrExceptions&))
			{
				MessageBox(parentWnd, strErrAttrMismatch, L"Error", MB_ICONERROR);
				return;
			}
			CDlgEditAttrExceptions(dj, (CAttrExceptions&)ai->get_attr()).DoModal(parentWnd);
		}
		break;
	}
}

void attributesWnd::editAttribute(HWND parentWnd, HWND attrWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj)
{
	editAttribute(parentWnd, getAttribute(attrWnd, aiv), dj);
}

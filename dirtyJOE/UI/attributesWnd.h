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

#include <windows.h>
#include <vector>
#include "CDirtyJOE.h"
#include "../java_support/attribute_info.h"

namespace attributesWnd
{
	void deleteCurrentAttribute(HWND attrWnd, std::vector<attribute_info*>& aiv);
	void updateAttributes(HWND attrWnd, std::vector<attribute_info*>& aiv, JavaConstPool& cp);
	void addAttribute(HWND parentWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj);
	void viewAttribute(HWND parentWnd, HWND attrWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj);
	void editAttribute(HWND parentWnd, HWND attrWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj);
	void editAttribute(HWND parentWnd, attribute_info* ai, CDirtyJOE& dj);
	bool isEditableAttribute(HWND attrWnd, std::vector<attribute_info*>& aiv, CDirtyJOE& dj);
	attribute_info* getAttribute(HWND attrWnd, std::vector<attribute_info*>& aiv);
};

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
#pragma once

#include <vector>
#include "CClassFile.h"
#include "java_types.h"
#include "JavaConstPool_type.h"
#include "attribute_info.h"
#include "field_info.h"

std::vector<wchar_t*>* getGlobalRefs(CClassFile& cf, u2 item, std::vector<wchar_t*>* ret = 0);
std::vector<wchar_t*>* getCPRefs(const JavaConstPool& cp, u2 item, std::vector<wchar_t*>* ret = 0);
std::vector<wchar_t*>* getAttrRefs(const JavaConstPool& cp, const u2 item, std::vector<attribute_info*>& attrs, const wchar_t* prefix, std::vector<wchar_t*>* ret = 0);
std::vector<wchar_t*>* getFMRefs(const JavaConstPool& cp, u2 item, std::vector<field_info*>& fms, bool fields, std::vector<wchar_t*>* ret = 0);
std::vector<wchar_t*>* getAllRefs(CClassFile& cf, u2 item);
wchar_t* parseReference(CClassFile& cf, wchar_t* str);

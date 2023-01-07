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

#include <windows.h>
#include "CLogger.h"
#include "common_macros.h"

malloc_ptr<wchar_t> wcs_escape_whitespace(const wchar_t* str);
int wcschr_xchgall(wchar_t* str, wchar_t c1, wchar_t c2);
malloc_ptr<char> wctmb(const wchar_t* wstr, UINT outCodePage, size_t* outSize = 0);
malloc_ptr<char> wcs2char(const wchar_t* wstr, size_t* outSize = 0);
malloc_ptr<wchar_t> data2hex(const unsigned char* buffer, unsigned int size);
bool isHexChar(wchar_t wc);
int getIntFromHexChar(wchar_t wc);
malloc_ptr<unsigned char> hexstr2data(const wchar_t* str, unsigned int* outSize);
int countHexChars(const wchar_t* str);

malloc_ptr<unsigned char> loadFile(const wchar_t* fileName, size_t& fileSize, CLogger& log);
malloc_ptr<char> loadFileAsChar(const wchar_t* fileName, size_t& fileSize, CLogger& log);
malloc_ptr<wchar_t> loadFileWChar(const wchar_t* fileName, size_t& fileSize, CLogger& log);

#include "common_templates.h"

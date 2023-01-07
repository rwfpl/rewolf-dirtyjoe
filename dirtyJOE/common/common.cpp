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
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string.h>
#include "common.h"

malloc_ptr<wchar_t> wcs_escape_whitespace(const wchar_t* str)
{
	// 0x5C -> L"\\\\"
	// 0x22 -> L"\\\""
	static const wchar_t* wsct[256] =
	{/*	0       1  2        3  4  5  6  7       8       9       A       B       C      D         E  F  */
	 /*0*/	L"\\0", 0, 0      , 0, 0, 0, 0, L"\\a", L"\\b", L"\\t", L"\\n", L"\\v", L"\\f" , L"\\r", 0, 0, 
	 /*1*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*2*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*3*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*4*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*5*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*6*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*7*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*8*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*9*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*A*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*B*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*C*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*D*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*E*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	 /*F*/	0     , 0, 0      , 0, 0, 0, 0, 0     , 0     , 0     , 0     , 0     , 0      , 0     , 0, 0, 
	};

	if (nullptr == str)
		return nullptr;

	const wchar_t* tmp_str = str;
	size_t outlen = wcslen(str) + 1;
	while (*tmp_str)
	{
		if ((0 != wsct[*tmp_str & 0xFF]) && (*tmp_str >> 8 == 0))
		{
			outlen += wcslen(wsct[*tmp_str & 0xFF]);
		}
		++tmp_str;
	}

	malloc_ptr<wchar_t> ret = malloc<wchar_t>(outlen);
	if (nullptr == ret)
		return nullptr;

	tmp_str = str;
	wchar_t* crret = ret.get();
	while (*tmp_str)
	{
		if ((0 != wsct[*tmp_str & 0xFF]) && (*tmp_str >> 8 == 0))
		{
			size_t tl = wcslen(wsct[*tmp_str & 0xFF]);
			memcpy(crret, wsct[*tmp_str & 0xFF], tl*sizeof(wchar_t));
			crret += tl;
		}
		else
			*(crret++) = *tmp_str;
		++tmp_str;
	}
	*crret = 0;

	return ret;
}

int wcschr_xchgall(wchar_t* str, wchar_t c1, wchar_t c2)
{
	int ret = 0;
	while ((str = wcschr(str, c1)) != 0)
	{
		*str = c2;
		ret++;
	}
	return ret;
}

malloc_ptr<char> wctmb(const wchar_t* wstr, UINT outCodePage, size_t* outSize)
{
	if (nullptr == wstr)
		return nullptr;

	int osize = WideCharToMultiByte(outCodePage, 0, wstr, -1, 0, 0, 0, 0);
	if (0 == osize)
		return nullptr;

	malloc_ptr<char> retBuf = malloc<char>(osize);
	if (nullptr == retBuf)
		return nullptr;

	int k = WideCharToMultiByte(outCodePage, 0, wstr, -1, (LPSTR)retBuf.get(), osize, 0, 0);
	if (0 == k)
		return nullptr;
	
	if (nullptr != outSize)
		*outSize = osize;

	return retBuf;
}

malloc_ptr<char> wcs2char(const wchar_t* wstr, size_t* outSize)
{
	return wctmb(wstr, CP_ACP, outSize);
}

malloc_ptr<wchar_t> data2hex(const unsigned char* buffer, unsigned int size)
{
	malloc_ptr<wchar_t> ret = malloc<wchar_t>(size*3 + 1);
	if (nullptr == ret)
		return nullptr;

	for (unsigned int i = 0; i < size; i++)
	{
		swprintf(ret.get() + 3*i, size*3, L"%02X ", buffer[i]);
	}
	ret.get()[size*3 - 1] = 0;

	return ret;
}

bool isHexChar(wchar_t wc)
{
	if (((wc >= 'A') && (wc <= 'F')) ||
		((wc >= 'a') && (wc <= 'f')) ||
		((wc >= '0') && (wc <= '9')))
		return true;
	return false;
}

int getIntFromHexChar(wchar_t wc)
{
	if ((wc >= 'A') && (wc <= 'F'))
		return wc - 0x37;
	if ((wc >= 'a') && (wc <= 'f'))
		return wc - 0x57;
	if ((wc >= '0') && (wc <= '9'))
		return wc - 0x30;
	return -1;
}

int countHexChars(const wchar_t* str)
{
	int hexcnt = 0;
	while (*str)
	{
		if (isHexChar(*str))
			hexcnt++;
		str++;
	}
	return hexcnt;
}

malloc_ptr<unsigned char> hexstr2data(const wchar_t* str, unsigned int* outSize = 0)
{
	int hexcnt = countHexChars(str);
	if (0 == hexcnt)
		return nullptr;
	
	unsigned int os = hexcnt / 2;
	if (hexcnt % 2)
		os++;

	malloc_ptr<unsigned char> tmp = malloc<unsigned char>(os);
	if (nullptr == tmp)
		return nullptr;
	
	unsigned char t = 0;
	int localcnt = 0;
	unsigned char* ctmp = tmp.get();
	while (*str)
	{
		int cr = 0;
		if (-1 != (cr = getIntFromHexChar(*str)))
		{
			t <<= 4;
			t |= cr;
			localcnt++;
			if (localcnt == 2)
			{
				*ctmp = t;
				ctmp++;
				localcnt = 0;
				t = 0;
			}
		}
		str++;
	}
	if (localcnt == 1)
		*ctmp = t;

	if (outSize)
		*outSize = os;

	return tmp;
}

malloc_ptr<unsigned char> loadFile(const wchar_t* fileName, size_t& fileSize, CLogger& log)
{
	if (nullptr == fileName) 
	{
		log.log(1, "Null fileName.\n");
		return nullptr;
	}

	FILE* inFile;
	if (0 != _wfopen_s(&inFile, fileName, L"rb"))
	{
		log.log(1, "Cannot open %ws file.\n", fileName);
		return nullptr;
	}
	fseek(inFile, 0, SEEK_END);
	fileSize = ftell(inFile);

	//allocate 2 bytes more for some possible 0-ending in char and wchar buffers
	malloc_ptr<unsigned char> fileMem = malloc<unsigned char>(fileSize + 2);
	if (nullptr == fileMem)
	{
		fclose(inFile);
		log.log(1, "Cannot allocate %d bytes of memory.\n", fileSize);
		return nullptr;
	}

	log.log(5, "file size: %d\n", fileSize);

	fseek(inFile, 0, SEEK_SET);
	if (fread(fileMem.get(), 1, fileSize, inFile) != fileSize)
	{
		fclose(inFile);
		log.log(1, "Cannot read input file.\n");
		return nullptr;
	}
	fclose(inFile);

	return fileMem;
}

malloc_ptr<char> loadFileAsChar(const wchar_t* fileName, size_t& fileSize, CLogger& log)
{
	malloc_ptr<unsigned char> ret = loadFile(fileName, fileSize, log);
	if (nullptr == ret)
		return nullptr;
	if (fileSize)
		ret.get()[fileSize - 1] = 0;
	return malloc_ptr<char>((char*)ret.release());
}

malloc_ptr<wchar_t> loadFileWChar(const wchar_t* fileName, size_t& fileSize, CLogger& log)
{
	malloc_ptr<unsigned char> ret = loadFile(fileName, fileSize, log);
	if (nullptr == ret)
		return nullptr;
	if (fileSize > 2)
	{
		ret.get()[fileSize - 2] = 0;
		ret.get()[fileSize - 1] = 0;
	}
	fileSize >>= 1;
	return malloc_ptr<wchar_t>((wchar_t*)ret.release());
}

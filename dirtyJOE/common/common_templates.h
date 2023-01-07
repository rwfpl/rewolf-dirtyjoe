/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
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

#include <typeinfo>
#include <vector>

template <class T>
size_t c_tcslen(const T* _Str)
{
	if (typeid(T) == typeid(wchar_t))
		return wcslen((wchar_t*)_Str);
	else if (typeid(T) == typeid(char))
		return strlen((char*)_Str);

	return 0;
}

template <class T>
errno_t c_tcscpy_s(T* _Dst, rsize_t _Size, const T* _Src)
{
	if (typeid(T) == typeid(wchar_t))
		return wcscpy_s((wchar_t*)_Dst, _Size, (wchar_t*)_Src);
	else if (typeid(T) == typeid(char))
		return strcpy_s((char*)_Dst, _Size, (char*)_Src);

	return 0;
}

template <class T>
errno_t c_tcscat_s(T* _Dst, rsize_t _Size, const T* _Src)
{
	if (typeid(T) == typeid(wchar_t))
		return wcscat_s((wchar_t*)_Dst, _Size, (wchar_t*)_Src);
	else if (typeid(T) == typeid(char))
		return strcat_s((char*)_Dst, _Size, (char*)_Src);

	return 0;
}

template <class T>
malloc_ptr<T> tcscat_var(const T* first, ...)
{
	va_list args;
	int cnt = 0;
	va_start(args, first);

	while (va_arg(args, const T*) != 0)
		cnt++;

	va_start(args, first);

	size_t newlen = 1 + c_tcslen(first);
	for (int i = 0; i < cnt; i++)
		newlen += c_tcslen(va_arg(args, const T*));

	malloc_ptr<T> ret = malloc<T>(newlen);
	if (nullptr == ret)
		return nullptr;

	va_start(args, first);

	c_tcscpy_s(ret.get(), newlen, first);
	for (int i = 0; i < cnt; i++)
		c_tcscat_s(ret.get(), newlen, va_arg(args, const T*));

	return ret;
}

template <class T>
void tcs_remove(T* str, T ch)
{
	size_t len = c_tcslen(str);
	for (size_t i = 0; i < len; i++)
	{
		if (str[i] == ch)
		{
			memmove(str + i, str + i + 1, len - i);
			i--;
			len--;
		}
	}
}

#define wcscat_var tcscat_var<wchar_t>

template <class T>
bool normalizePath(T* path)
{
	//TODO: fix the case with path: c:\xxx\..\\test.class - it leaves double slash
	std::vector<T*> slashes;
	T* cur = path;
	size_t tokenLen = 0;
	while (0 != *cur)
	{
		if (('\\' == *cur) && ('\\' == *(cur + 1)))
		{
			if (cur != path)
			{
				// handle double slash in the middle of path, just delete one slash, DO NOT ADVANCE 'cur' pointer
				memmove((void*)cur, (void*)(cur + 1), sizeof(T)*(c_tcslen(cur + 1) + 1));
			}
			else
			{
				// double slash at the beginning is the network path, skip one slash and proceed the next as a normal
				cur++;
			}
		}
		else if ('\\' == *cur)
		{
			// at this point there is only one slash guaranteed, so reset tokenLen and store its position
			slashes.push_back(cur);
			tokenLen = 0;
			cur++;
		}
		else if (('.' == *cur) && ('\\' == *(cur + 1)) && (0 == tokenLen))
		{
			// case similar to double slash, but two characters (".\") can be skipped
			// DO NOT ADVANCE 'cur' pointer
			memmove((void*)cur, (void*)(cur + 2), sizeof(T)*(c_tcslen(cur + 2) + 1));
		}
		else if (('.' == *cur) && ('.' == *(cur + 1)) && ('\\' == *(cur + 2)) && (0 == tokenLen))
		{
			if (0 != slashes.size())
			{
				slashes.pop_back();
				T* prevSlash2;
				if (0 != slashes.size())
					prevSlash2 = *(slashes.end() - 1) + 1;
				else
					prevSlash2 = path;

				memmove((void*)prevSlash2, (void*)(cur + 3), sizeof(T)*(c_tcslen(cur + 3) + 1));
				cur = prevSlash2;
			}
			else
			{
				// wrong path was passed to the function
				return false;
			}
		}
		else
		{
			tokenLen++;
			cur++;
		}
	}
	return true;
}

template <class V>
malloc_ptr<wchar_t> to_hex(V v)
{
	malloc_ptr<wchar_t> ret = malloc<wchar_t>(2*(sizeof(V) + 1));
	if (nullptr == ret)
		return nullptr;

	wchar_t* fmt = nullptr;
	switch (sizeof(V))
	{
		case 1: fmt = L"%02X"; break;
		case 2: fmt = L"%04X"; break;
		case 4: fmt = L"%08X"; break;
		case 8: fmt = L"%016X"; break;
		default: return nullptr;
	}
	swprintf_s(ret.get(), sizeof(V) + 1, fmt, v);
	return ret;
}
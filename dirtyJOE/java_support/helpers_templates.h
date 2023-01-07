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

template <class T, class U>
bool readObjects(const U dummy, std::vector<T*>& objs, u1* mem, u4& currentPos, size_t maxSize, const JavaConstPool& cp, CLogger& logger)
{
	LOGFSTART();
	if (maxSize < currentPos + sizeof(U)) 
		return false;
#pragma warning (suppress : 4333)
	U _count = BE2LE(*(U*)(mem + currentPos));
	currentPos += sizeof(U);

	for (u4 i = 0; i < _count; i++)
	{
		T* tmpTPtr = new (std::nothrow) T(mem + currentPos, (u4)(maxSize - currentPos), cp, logger);
		if ((!tmpTPtr) || (tmpTPtr->getError()))
		{
			for (u4 j = 0; j < i; j++) 
				if (objs[j]) 
					delete objs[j];
			objs.clear();
			if (tmpTPtr) 
				delete tmpTPtr;
			logger.log(1, "Cannot create object %d from %d\n", i, _count);
			LOGFEND();
			return false;
		}
		objs.push_back(tmpTPtr);
		currentPos += tmpTPtr->getSize();
	}
	LOGFEND();
	return true;
}

template <class T, class U>
bool storeObjects(const U dummy, std::vector<T*>& objs, u1* mem, u4& currentPos, size_t maxSize)
{
	U _count = (U)objs.size();
	if (maxSize < currentPos + sizeof(U)) 
		return false;
#pragma warning (suppress : 4333)
	*(U*)(mem + currentPos) = BE2LE(_count);
	currentPos += sizeof(U);
	for (u4 i = 0; i < _count; i++)
	{
		if (!objs[i]->storeData(mem + currentPos, (unsigned int)(maxSize - currentPos)))
			return false;
		currentPos += objs[i]->getSize();
	}
	return true;
}

template <class T> 
void cleanUpObjects(std::vector<T*>& objs)
{
	for (u4 i = 0; i < objs.size(); i++) 
		if (objs[i]) 
			delete objs[i];
	objs.clear();
}

template <class T>
u4 getSizeOfObjects(const std::vector<T*>& objs)
{
	u4 ret = 0;
	for (u4 i = 0; i < objs.size(); i++) 
		ret += objs[i]->getSize();
	return ret;
}

template <class T>
malloc_ptr<wchar_t> genDisAsmStringFromCP_hlp(wchar_t* fmt, T v)
{
	wchar_t tstr[0x40];
	int size = swprintf_s(tstr, fmt, v);
	if (size == -1)
		return 0;
	malloc_ptr<wchar_t> ret = malloc<wchar_t>(size + 1);
	if (nullptr == ret)
		return nullptr;
	memmove(ret.get(), tstr, 2*(size + 1));
	return ret;
}

template <class T>
bool writeUx(FILE* file, T v)
{
	T tv;
#pragma warning (suppress : 4127)
	if (sizeof(v) == 1)
		tv = v;
	else
	{
#pragma warning (suppress : 4333)
		tv = BE2LE(v);
	}
	if (fwrite(&tv, 1, sizeof(T), file) != sizeof(T))
		return false;
	return true;
}

template <class T, class U>
bool writeObjects(FILE* file, std::vector<T*>& objs, const U dummy)
{
	u4 total_size = getSizeOfObjects(objs) + sizeof(u2);
	malloc_ptr<u1> mem = malloc<u1>(total_size);
	if (nullptr == mem)
		return false;

	u4 cpos = 0;

	if (!storeObjects(dummy, objs, mem.get(), cpos, total_size))
		return false;

	if (fwrite(mem.get(), 1, total_size, file) != total_size)
		return false;

	return true;
}

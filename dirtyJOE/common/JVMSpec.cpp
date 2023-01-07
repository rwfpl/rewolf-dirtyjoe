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
#include <memory>
#include <algorithm>
#include "JVMSpec.h"
#include "AppDir.h"
#include "../ui/ui_helpers.h"
#include "../third_party/aplib/depacks.h"
#include "../java_support/jdisasm/jdisasm.h"

const CJVMSpec::opcodeEntry CJVMSpec::opcodeInvalid = { "no data", "no data", "no data", "no data" };

bool CJVMSpec::sort_predicate(const opcodeEntry& o1, const opcodeEntry& o2)
{
	return strcmp(o1.opcodeName, o2.opcodeName) < 0;
}

CJVMSpec::CJVMSpec() : error(true), dcrBuf(nullptr)
{
	memset(indexTab, -1, sizeof(int)*256);

	CAppDir appdir;
	if (appdir.getError())
		return;

	HANDLE hFile = CreateFile(L"jvmspec.dat", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	const DWORD fSize = GetFileSize(hFile, 0);
	if (INVALID_FILE_SIZE == fSize)
	{
		CloseHandle(hFile);
		return;
	}

	malloc_ptr<BYTE> inBuf = malloc<BYTE>(fSize);
	if (nullptr == inBuf)
	{
		CloseHandle(hFile);
		return;
	}

	DWORD tmp;
	if ((FALSE == ReadFile(hFile, inBuf.get(), fSize, &tmp, 0)) || (tmp != fSize))
	{
		CloseHandle(hFile);
		return;
	}
	CloseHandle(hFile);

	const DWORD dcrSize = *(DWORD*)inBuf.get();
	
	malloc_ptr<BYTE> outBuf = malloc<BYTE>(dcrSize+1);
	if (nullptr == outBuf)
		return;
	outBuf.get()[dcrSize] = 0;

	if (APLIB_ERROR == aP_depack_safe(inBuf.get() + 4, fSize - 4, outBuf.get(), dcrSize))
		return;

	const BYTE * const maxPtr = outBuf.get() + dcrSize;
	BYTE* crPtr = outBuf.get();
	while (crPtr < maxPtr)
	{
		opcodeEntry opEnt;
		crPtr = readString(crPtr, maxPtr, opEnt.opcodeName);
		if (nullptr == crPtr)
			return;

		crPtr = readString(crPtr, maxPtr, opEnt.shortDescription);
		if (nullptr == crPtr)
			return;

		crPtr = readString(crPtr, maxPtr, opEnt.stackDescription);
		if (nullptr == crPtr)
			return;

		crPtr = readString(crPtr, maxPtr, opEnt.fullDescription);
		if (nullptr == crPtr)
			return;

		opcodeEntries.push_back(opEnt);
	}

	std::sort(opcodeEntries.begin(), opcodeEntries.end(), sort_predicate);
	for (int i = 0; i < 256; i++)
	{
		if (jdisasm_get_specName((u1)i))
		{
			opcodeEntry opDummy = { (char*)jdisasm_get_specName((u1)i), 0, 0, 0 };
			auto opr = std::equal_range(opcodeEntries.begin(), opcodeEntries.end(), opDummy, sort_predicate);
			if (opr.first == opr.second)
				return;		//something wrong !

			indexTab[i] = (int)std::distance(opcodeEntries.begin(), opr.first);
		}
	}

	dcrBuf = malloc_ptr<BYTE>(outBuf.release());
	error = false;
}

BYTE* CJVMSpec::readString(BYTE* ptr, const BYTE* const maxPtr, char*& outStr)
{
	WORD tLen = *(WORD*)ptr;
	*(WORD*)ptr = 0;			//append zero to the previous read string
	ptr += 2;
	outStr = (char*)ptr;
	if (ptr + tLen > maxPtr)
		return 0;

	ptr += tLen;
	
	return ptr;
}

CJVMSpec::~CJVMSpec()
{
}

bool CJVMSpec::getError() const
{
	return error;
}

const CJVMSpec::opcodeEntry& CJVMSpec::getOpcodeSpec(BYTE op)
{
	if (-1 != indexTab[op])
		return opcodeEntries[indexTab[op]];
	else
		return opcodeInvalid;
}

std::vector<CJVMSpec::opcodeEntry>& CJVMSpec::getRawEntries()
{
	return opcodeEntries;
}

int CJVMSpec::getRawIndex(BYTE op) const
{
	return indexTab[op];
}

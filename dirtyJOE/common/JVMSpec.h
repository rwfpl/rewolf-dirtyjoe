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
#include <Windows.h>
#include <vector>
#include "common_macros.h"

class CJVMSpec
{
	public:
		
		struct opcodeEntry
		{
			char* opcodeName;
			char* shortDescription;
			char* stackDescription;
			char* fullDescription;
		};

		CJVMSpec();
		~CJVMSpec();
		bool getError() const;
		const opcodeEntry& getOpcodeSpec(BYTE op);
		std::vector<opcodeEntry>& getRawEntries();
		int getRawIndex(BYTE op) const;

	private:

		bool error;
		malloc_ptr<BYTE> dcrBuf;
		std::vector<opcodeEntry> opcodeEntries;

		BYTE* readString(BYTE* ptr, const BYTE* const maxPtr, char*& outStr);
		int indexTab[256];

		static const opcodeEntry opcodeInvalid;

		static bool sort_predicate(const opcodeEntry& o1, const opcodeEntry& o2);
};

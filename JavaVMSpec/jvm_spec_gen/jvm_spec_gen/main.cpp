#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <vector>
#include "aplib.h"

int wmain(int argc, wchar_t *argv[])
{
	if (2 != argc)
	{
		printf("Not enough parameteres.\n");
		return 0;
	}

	HANDLE hFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		wprintf(L"Can't open file: %ws\n", argv[1]);
		return 0;
	}

	DWORD fSize = GetFileSize(hFile, 0);
	char* buf = (char*)malloc(fSize + 1);
	if (0 == buf)
	{
		printf("Can't allocate memory for file.\n");
		CloseHandle(hFile);
		return 0;
	}

	DWORD tmp;
	ReadFile(hFile, buf, fSize, &tmp, 0);
	if (tmp != fSize)
	{
		printf("ReadFile error.\n");
		CloseHandle(hFile);
		return 0;
	}
	CloseHandle(hFile);
	buf[fSize] = 0;

	static const char str_jvm_opcode[] = "jvm_opcode:";
	static const char str_jvm_short[] = "jvm_short:";
	static const char str_jvm_stack[] = "jvm_stack:";
	static const char str_jvm_full[] = "jvm_full:";

	struct customStr
	{
		WORD len;
		char* str;
	};

	struct opStruct
	{
		customStr opcode;
		customStr shortd;
		customStr stack;
		customStr fulld;
	};

	std::vector<opStruct> jvm_opcodes;

	char* crBuf = buf;
	while (crBuf < buf + fSize)
	{
		char* tptr_1 = strstr(crBuf, str_jvm_opcode);
		if (0 == tptr_1)
			break;		//end

		char* tptr_2 = strstr(tptr_1, str_jvm_short);
		char* tptr_3 = strstr(tptr_2, str_jvm_stack);
		char* tptr_4 = strstr(tptr_3, str_jvm_full);
		char* tptr_5 = strstr(tptr_4, str_jvm_opcode);
		if (0 == tptr_5)
			tptr_5 = buf + fSize;

		opStruct opTmp;
		opTmp.opcode.str = tptr_1 + sizeof(str_jvm_opcode) - 1;
		opTmp.opcode.len = tptr_2 - opTmp.opcode.str - 2;		//2 stands for "\r\n"
		
		opTmp.shortd.str = tptr_2 + sizeof(str_jvm_short) - 1;
		opTmp.shortd.len = tptr_3 - opTmp.shortd.str - 2;

		opTmp.stack.str = tptr_3 + sizeof(str_jvm_stack) - 1;
		opTmp.stack.len = tptr_4 - opTmp.stack.str - 2;

		opTmp.fulld.str = tptr_4 + sizeof(str_jvm_full) - 1;
		opTmp.fulld.len = tptr_5 - opTmp.fulld.str - 2;

		jvm_opcodes.push_back(opTmp);
		crBuf = tptr_5;
	}

	HANDLE hOutFile = CreateFile(L"jvmspec.bin", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hOutFile)
	{
		free(buf);
		printf("Can't create output file.\n");
		return 0;
	}

	auto it = jvm_opcodes.begin();
	while (it < jvm_opcodes.end())
	{
		WriteFile(hOutFile, &it->opcode.len, sizeof(WORD), &tmp, 0);
		WriteFile(hOutFile, it->opcode.str, it->opcode.len, &tmp, 0);

		WriteFile(hOutFile, &it->shortd.len, sizeof(WORD), &tmp, 0);
		WriteFile(hOutFile, it->shortd.str, it->shortd.len, &tmp, 0);
		
		WriteFile(hOutFile, &it->stack.len, sizeof(WORD), &tmp, 0);
		WriteFile(hOutFile, it->stack.str, it->stack.len, &tmp, 0);

		WriteFile(hOutFile, &it->fulld.len, sizeof(WORD), &tmp, 0);
		WriteFile(hOutFile, it->fulld.str, it->fulld.len, &tmp, 0);
		it++;
	}

	CloseHandle(hOutFile);
	free(buf);

	HANDLE hSecFile = CreateFile(L"jvmspec.bin", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD nSize = GetFileSize(hSecFile, 0);
	BYTE* inbuf = (BYTE*)malloc(nSize);
	BYTE* outbuf = (BYTE*)malloc(aP_max_packed_size(nSize));
	BYTE* workmem = (BYTE*)malloc(aP_workmem_size(nSize));
	ReadFile(hSecFile, inbuf, nSize, &tmp, 0);
	CloseHandle(hSecFile);

	size_t outSize = aP_pack(inbuf, outbuf, nSize, workmem, 0, 0);

	free(inbuf);

	HANDLE hOutFile2 = CreateFile(L"jvmspec.dat", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(hOutFile2, &nSize, 4, &tmp, 0);
	WriteFile(hOutFile2, outbuf, outSize, &tmp, 0);
	CloseHandle(hOutFile2);

	free(outbuf);
	free(workmem);
	return 0;
}

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

#include "CClassFile.h"
#include <cstdio>
#include "../common/CLogger.h"
#include "../common/common.h"
#include "helpers.h"

cp_info* CClassFile::constPoolSwitch(u1* mem, u4 size)
{
	LOGFSTART();
	cp_info* ret = 0;
	#define CONCASE(a, b) case a: logger.log(5, "%s\n", #b); ret = new (std::nothrow) b(mem, size, logger); break
	switch (mem[0])
	{
		CONCASE(CONSTANT_Class, CONSTANT_Class_info);
		CONCASE(CONSTANT_Fieldref, CONSTANT_Fieldref_info);
		CONCASE(CONSTANT_Methodref, CONSTANT_Methodref_info);
		CONCASE(CONSTANT_InterfaceMethodref, CONSTANT_InterfaceMethodref_info);
		CONCASE(CONSTANT_String, CONSTANT_String_info);
		CONCASE(CONSTANT_Integer, CONSTANT_Integer_info);
		CONCASE(CONSTANT_Float, CONSTANT_Float_info);
		CONCASE(CONSTANT_Long, CONSTANT_Long_info);
		CONCASE(CONSTANT_Double, CONSTANT_Double_info);
		CONCASE(CONSTANT_NameAndType, CONSTANT_NameAndType_info);
		CONCASE(CONSTANT_Utf8, CONSTANT_Utf8_info);
		CONCASE(CONSTANT_MethodHandle, CONSTANT_MethodHandle_info);
		CONCASE(CONSTANT_MethodType, CONSTANT_MethodType_info);
		CONCASE(CONSTANT_InvokeDynamic, CONSTANT_InvokeDynamic_info);
		default: logger.log(5, "unknown const pool (%02X)\n", mem[0]); break;
	}
	#undef CONCASE
	if (ret)
	{
		if (ret->getError())
		{
			delete ret;
			ret = 0;
		}
	}
	LOGFEND();
	return ret;
}

#define CCF_ERROR() \
	{ \
	cleanUpMemory(); \
	LOGFEND(); \
	return false; \
	}

bool CClassFile::LoadFile(const wchar_t* classFileName)
{
	LOGFSTART();
	if (loaded) 
		UnloadFile();

	size_t fileSize = 0;
	const malloc_ptr<u1> fileMem = loadFile(classFileName, fileSize, logger);
	if (nullptr == fileMem)
	{
		LOGFEND();
		return false;
	}

	u4 currentPos = 0;
	magic = BE2LEU4(*(u4*)(fileMem.get()));
	minor_version = BE2LEU2(*(u2*)(fileMem.get() + 4));
	major_version = BE2LEU2(*(u2*)(fileMem.get() + 6));

	logger.log(5, "magic: %08X\nmin ver: %d\nmaj ver: %d\n", magic, minor_version, major_version);

	constant_pool_count = BE2LEU2(*(u2*)(fileMem.get() + 8));
	if (constant_pool_count == 0)
	{
		logger.log(1, "constant_pool_count cannot be 0\n");
		LOGFEND();
		return false;
	}
	currentPos += 10;
	logger.log(5, "magic: %08X\nmin ver: %d\nmaj ver: %d\nconst pools: %d\n", magic, minor_version, major_version, constant_pool_count);

	constant_pool.push_back(0);
	bool prevSkipped = false;
	for (int i = 0; i < (int)constant_pool_count - 1; i++)
	{
		if (!prevSkipped)
		{
			cp_info* cpTmpPtr = constPoolSwitch(fileMem.get() + currentPos, (u4)(fileSize - currentPos));
			if (!cpTmpPtr)
			{
				logger.log(1, "Cannot create cp_info object (num %d).\n", i);
				CCF_ERROR();
			}
			constant_pool.push_back(cpTmpPtr);
			logger.log(5, "%03d. pos: %08X, size: %d\n", i, currentPos, cpTmpPtr->getSize());

			currentPos += cpTmpPtr->getSize();
			//special case for LONG and DOUBLE types, which occupies 2 entries in constant_pool
			if ((cpTmpPtr->tag == CONSTANT_Double) || (cpTmpPtr->tag == CONSTANT_Long))
				prevSkipped = true;
		}
		else
		{
			constant_pool.push_back(nullptr);
			prevSkipped = false;
		}
	}

	access_flags = BE2LEU2(*(u2*)(fileMem.get() + currentPos));
	this_class = BE2LEU2(*(u2*)(fileMem.get() + currentPos + 2));
	super_class = BE2LEU2(*(u2*)(fileMem.get() + currentPos + 4));
	u2 interfaces_count = BE2LEU2(*(u2*)(fileMem.get() + currentPos + 6));
	logger.log(5, "interfaces: %d\n", interfaces_count);
	currentPos += 8;

	for (u2 i = 0; i < interfaces_count; i++) 
	{
		interfaces.push_back((u2)BE2LEU2(*(u2*)(fileMem.get() + currentPos + i*sizeof(u2))));
		logger.log(5, "%d\n", (u2)BE2LEU2(*(u2*)(fileMem.get() + currentPos + i*sizeof(u2))));
		logger.log(5, "interface[%d]: ", i);
		if ((interfaces[i] >= constant_pool.size()) || (constant_pool[interfaces[i]] == 0))
		{
			logger.log(1, "Invalid interface index (%d of %d).\n", interfaces[i], constant_pool.size());
			CCF_ERROR();
		}
		if (constant_pool[interfaces[i]]->tag != CONSTANT_Class)
		{
			logger.log(1, "Invalid interface type (%d).\n", constant_pool[interfaces[i]]->tag);
			CCF_ERROR();
		}
		if (((CONSTANT_Class_info*)constant_pool[interfaces[i]])->v1 >= constant_pool.size())
		{
			logger.log(1, "Invalid interface name index (%d of %d).\n", ((CONSTANT_Class_info*)constant_pool[interfaces[i]])->v1, constant_pool.size());
			CCF_ERROR();
		}
		if (constant_pool[((CONSTANT_Class_info*)constant_pool[interfaces[i]])->v1]->tag != CONSTANT_Utf8)
		{
			logger.log(1, "Invalid interface name type (%d).\n", constant_pool[((CONSTANT_Class_info*)constant_pool[interfaces[i]])->v1]->tag);
			CCF_ERROR();
		}
		char tmpstr[64];
		sprintf_s(tmpstr, 64, "%%.%zds\n", ((CONSTANT_Utf8_info*)constant_pool[((CONSTANT_Class_info*)constant_pool[interfaces[i]])->v1])->get_bytes().size() - 1);
		logger.log(5, tmpstr, &((CONSTANT_Utf8_info*)constant_pool[((CONSTANT_Class_info*)constant_pool[interfaces[i]])->v1])->get_bytes()[0]);

	}
	currentPos += interfaces_count * sizeof(u2);

	if (!readObjects(_U2_, fields, fileMem.get(), currentPos, fileSize, constant_pool, logger) ||
		!readObjects(_U2_, methods, fileMem.get(), currentPos, fileSize, constant_pool, logger) ||
		!readObjects(_U2_, attributes, fileMem.get(), currentPos, fileSize, constant_pool, logger))
	{
		CCF_ERROR();
	}

	if (!SetFileName(classFileName))
	{
		CCF_ERROR();
	}

	loaded = true;
	LOGFEND();
	return true;
}

void CClassFile::cleanUpMemory()
{
	LOGFSTART();
	cleanUpObjects(constant_pool);
	cleanUpObjects(fields);
	cleanUpObjects(methods);
	cleanUpObjects(attributes);
	interfaces.clear();
	zeroCntrs();
	LOGFEND();
}

bool CClassFile::SaveFile(const wchar_t* outputFileName)
{
	LOGFSTART();
	
	if (nullptr == outputFileName)
	{
		logger.log(1, "Null fileName.\n");
		return false;
	}

	FILE* outFile;
	if (_wfopen_s(&outFile, outputFileName, L"wb") != 0)
	{
		logger.log(1, "Cannot create %ws file.\n", outputFileName);
		LOGFEND();
		return false;
	}

	constant_pool_count = (u2)constant_pool.size();

	if (!writeUx(outFile, magic) ||
		!writeUx(outFile, minor_version) ||
		!writeUx(outFile, major_version) ||
		!writeUx(outFile, constant_pool_count))
	{
		fclose(outFile);
		LOGFEND();
		return false;
	}

	u1 prevTag = 0;
	for (u2 i = 1; i < constant_pool_count; i++)
	{
		if ((prevTag != CONSTANT_Double) && (prevTag != CONSTANT_Long))
		{
			prevTag = constant_pool[i]->tag;
			if (!writeStruct(outFile, *(constant_pool[i])))
			{
				fclose(outFile);
				LOGFEND();
				return false;
			}
		}
		else
			prevTag = 0;
	}
	
	u2 interfaces_count = (u2)interfaces.size();

	if (!writeUx(outFile, access_flags) ||
		!writeUx(outFile, this_class) ||
		!writeUx(outFile, super_class) ||
		!writeUx(outFile, interfaces_count))
	{
		fclose(outFile);
		LOGFEND();
		return false;
	}

	for (u2 i = 0; i < interfaces_count; i++)
	{
		if (!writeUx(outFile, interfaces[i]))
		{
			fclose(outFile);
			LOGFEND();
			return false;
		}
	}

	if (!writeObjects(outFile, fields, _U2_) ||
		!writeObjects(outFile, methods, _U2_) ||
		!writeObjects(outFile, attributes, _U2_))
	{
		fclose(outFile);
		LOGFEND();
		return false;
	}

	fclose(outFile);
	SetFileName(outputFileName);
	modified = false;
	LOGFEND();
	return true;
}

bool CClassFile::UnloadFile()
{
	LOGFSTART();
	cleanUpMemory();
	originalFileName = nullptr;
	loaded = false;
	modified = false;
	LOGFEND();
	return true;
}

void CClassFile::zeroCntrs()
{
	LOGFSTART();
	constant_pool_count = 0;
	LOGFEND();
}

CClassFile::CClassFile(CLogger& log) : CDefClass(log), loaded(false), error(false), originalFileName(nullptr), modified(false)
{
	LOGFSTART();
	zeroCntrs();
	LOGFEND();
}

CClassFile::~CClassFile()
{
	LOGFSTART();
	if (loaded) 
		UnloadFile();
	LOGFEND();
}

const wchar_t* CClassFile::GetFileName() const
{
	LOGFSTART();
	LOGFEND();
	return originalFileName.get();
}

bool CClassFile::SetFileName(const wchar_t* str)
{
	LOGFSTART();
	if (nullptr == str)
	{
		logger.log(1, "NULL is not a valid file name.\n");
		LOGFEND();
		return false;
	}
	
	originalFileName = malloc_ptr<wchar_t>(_wcsdup(str));
	if (nullptr == originalFileName)
	{
		logger.log(1, "Can't allocate memory for new file name\n");
		LOGFEND();
		return false;
	}

	LOGFEND();
	return true;
}

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

#include <vector>

#include "java_class_def.h"
#include "../common/CDefClass.h"
#include "../common/common_macros.h"

class CClassFile : public CDefClass
{
	private:
		GETTER(u4, magic);
		GETSET(u2, minor_version);
		GETSET(u2, major_version);
		u2 constant_pool_count;
		GETTER_PTR(JavaConstPool, constant_pool);	//[constant_pool_count - 1];
		GETSET(u2, access_flags);
		GETSET(u2, this_class);
		GETSET(u2, super_class);
		//u2 interfaces_count;
		GETTER_PTR(std::vector<u2>, interfaces);				//[interfaces_count];
		//u2 fields_count;
		GETTER_PTR(std::vector<field_info*>, fields);		//[fields_count];
		//u2 methods_count;
		GETTER_PTR(std::vector<method_info*>, methods);		//[methods_count];
		//u2 attributes_count;
		GETTER_PTR(std::vector<attribute_info*>, attributes);//[attributes_count];
		//
		GETTER(bool, loaded);
		bool error;
		cp_info* constPoolSwitch(u1* mem, u4 size);
		void cleanUpMemory();
		void zeroCntrs();

		//internal data
		malloc_ptr<wchar_t> originalFileName;
		bool modified;

		//dummy
		CClassFile();

	public:
		bool LoadFile(const wchar_t* classFileName);
		bool SaveFile(const wchar_t* outputFileName);
		bool UnloadFile();
		const wchar_t* GetFileName() const;
		bool SetFileName(const wchar_t* str);
		bool isModified() const { return modified; }
		void setModified() { modified = true; }
		CClassFile(CLogger& log);
		~CClassFile();
};

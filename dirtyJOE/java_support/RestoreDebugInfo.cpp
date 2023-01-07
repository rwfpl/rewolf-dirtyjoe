/*
* dirtyJOE - Java Overall Editor
* Copyright 2013 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/
#include <Windows.h>
#include "RestoreDebugInfo.h"
#include "helpers.h"
#include "jdisasm/jdisasm.h"
#include "../common/common.h"
#include "../common/common_templates.h"
#include "../UI/version_info.h"
#include "../UI/ui_helpers.h"
#include <algorithm>

struct rdi_local_var_desc
{
	u2 lv_index;
	_local_var_type lv_type;
	u2 start_pc;
	u2 len;
	rdi_local_var_desc(u2 idx, _local_var_type tp, u2 _start, u2 _len) : lv_index(idx), lv_type(tp), start_pc(_start), len(_len) {}
};

bool exception_cmp(const exception_entry* a, const exception_entry* b)
{
	if (a->get_handler_pc() < b->get_handler_pc())
		return true;
	return false;
}

bool exception_cmp_u4(const exception_entry* a, u4 b)
{
	if (a->get_handler_pc() < b)
		return true;
	return false;
}

bool RestoreLocalVariables(CClassFile& clf, CLogger& logger, CLogger& loggerLocal)
{
	int index = 0;
	auto method = clf.get_methods().begin();
	while (method != clf.get_methods().end())
	{
		index++;
		CONSTANT_Utf8_info* nameUtf8 = getUtf8FromCP((*method)->get_name_index(), clf.get_constant_pool());
		malloc_ptr<wchar_t> name_wstr = preprocessUtf8ToWChar(nameUtf8);
		if (nullptr != name_wstr)
			loggerLocal.log(1, "\r  Analysing (%d/%d): %ws...", index, clf.get_methods().size(), name_wstr.get());

		CAttrCode* _attrObj_ptr = getCodeAttribute(**method);
		if (nullptr == _attrObj_ptr)
		{
			++method;
			continue;
		}
		CAttrCode& attrObj = *_attrObj_ptr;

		// create temporary exception vector with sorted elements
		std::vector<exception_entry*> tempExceptionTable = attrObj.get_exception_table();
		std::sort(tempExceptionTable.begin(), tempExceptionTable.end(), exception_cmp);

		const size_t tcps = clf.get_constant_pool().size();
		// add LocalVariableTable string to CP if needed
		attribute_info* attrLV = new (std::nothrow) attribute_info(ATTR_LOCAL_VARIABLE_TABLE, clf.get_constant_pool(), logger);
		if (tcps != clf.get_constant_pool().size())
			clf.setModified();
		if ((nullptr == attrLV) || attrLV->getError())
		{
			++method;
			continue;
		}

		CAttrLocalVariableTable& aLVT = (CAttrLocalVariableTable&)attrLV->get_attr();
		eraseAttributeByType(clf.get_constant_pool(), attrObj.get_attributes(), ATTR_LOCAL_VARIABLE_TABLE);
		attrObj.get_attributes().push_back(attrLV);

		u4 last_branch_pos = 0;
		std::vector<rdi_local_var_desc> local_vars_vector;
		u4 pos = 0;
		while (pos < attrObj.get_code().size())
		{
			_jdisasm_opcodes op_type = jdisasm_get_type(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos);

			_local_var_type local_type = jdisasm_get_local_type(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos);
			if (local_var_undefined != local_type)
			{
				u2 local_idx = 0;
				switch (op_type)
				{
				case j_opcode_iinc: 
				case j_opcode_fstore:
				case j_opcode_istore:
				case j_opcode_dstore:
				case j_opcode_lstore:
				case j_opcode_astore:
				case j_opcode_fload:
				case j_opcode_iload:
				case j_opcode_dload:
				case j_opcode_lload:
				case j_opcode_aload: local_idx = attrObj.get_code()[pos + 1]; break;
				case j_opcode_fstore_0:
				case j_opcode_istore_0:
				case j_opcode_dstore_0:
				case j_opcode_lstore_0:
				case j_opcode_astore_0:
				case j_opcode_fload_0:
				case j_opcode_iload_0:
				case j_opcode_dload_0:
				case j_opcode_lload_0:
				case j_opcode_aload_0: local_idx = 0; break;
				case j_opcode_fstore_1:
				case j_opcode_istore_1:
				case j_opcode_dstore_1:
				case j_opcode_lstore_1:
				case j_opcode_astore_1:
				case j_opcode_fload_1:
				case j_opcode_iload_1:
				case j_opcode_dload_1:
				case j_opcode_lload_1:
				case j_opcode_aload_1: local_idx = 1; break;
				case j_opcode_fstore_2:
				case j_opcode_istore_2:
				case j_opcode_dstore_2:
				case j_opcode_lstore_2:
				case j_opcode_astore_2:
				case j_opcode_fload_2:
				case j_opcode_iload_2:
				case j_opcode_dload_2:
				case j_opcode_lload_2:
				case j_opcode_aload_2: local_idx = 2; break;
				case j_opcode_fstore_3:
				case j_opcode_istore_3:
				case j_opcode_dstore_3:
				case j_opcode_lstore_3:
				case j_opcode_astore_3:
				case j_opcode_fload_3:
				case j_opcode_iload_3:
				case j_opcode_dload_3:
				case j_opcode_lload_3:
				case j_opcode_aload_3: local_idx = 3; break;
				case j_opcode_wide: 
					// 'wide'support is a bit simplified, but should work as expected
					local_idx = (attrObj.get_code()[pos + 2] << 8) | attrObj.get_code()[pos + 3]; 
					break;
				}
				//logger.log(1, "local %02d, type: %d\n", local_idx, local_type);

				auto lvvit = local_vars_vector.begin();
				bool addLocal = true;
				u4 local_var_begin = last_branch_pos;
				while (lvvit != local_vars_vector.end())
				{
					if ((lvvit->lv_index == local_idx) /*&& (((*lvvit).lv_type == local_type))*/)
					{
						if (lvvit->lv_type != local_type)
						{
							//local_amabig = true;
							if (pos < (u4)(lvvit->start_pc + lvvit->len))
							{
								if (last_branch_pos != lvvit->start_pc)
									lvvit->len = (u2)(last_branch_pos - lvvit->start_pc - 1);
								else
								{
									lvvit->len = (u2)(pos - lvvit->start_pc - 1);
									local_var_begin = pos;
								}
								//logger.log(1, "local %02d, type: %02d - %02d\n", local_idx, lvvit->lv_type, local_type);
								addLocal = true;
								break;
							}
						}
						else
							addLocal = false;
					}
					++lvvit;
				}

				if (addLocal)
					local_vars_vector.push_back(rdi_local_var_desc(local_idx, local_type, (u2)local_var_begin, (u2)(attrObj.get_code().size() - local_var_begin - 1)));
			}
		
			int len = jdisasm_get_len(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos);
			if (len > 0)
				pos += len;
			else
				pos++;
		
			if (jdisasm_is_block_end_opcode(op_type))
			{
				//logger.log(1, "lbr: %d - pos: %d\n", last_branch_pos, pos);
				last_branch_pos = pos;
			}
			else
			{
				auto lowIt = lower_bound(tempExceptionTable.begin(), tempExceptionTable.end(), last_branch_pos, exception_cmp_u4);
				if (tempExceptionTable.end() != lowIt)
				{
					//logger.log(1, "lbr: %d - pos: %d - low: %d\n", last_branch_pos, pos, (*lowIt)->get_handler_pc());
					if (pos >= (*lowIt)->get_handler_pc())
						last_branch_pos = (*lowIt)->get_handler_pc();
				}	
			}
		}

		auto locit = local_vars_vector.begin();
		while (locit != local_vars_vector.end())
		{
			wchar_t lvname[0x20];
			swprintf_s(lvname, L"local_%02d", locit->lv_index);
			wchar_t* lvdesc = nullptr;
			switch (locit->lv_type)
			{
			case local_var_object: lvdesc = L"Ljava/lang/Object;"; break;
			case local_var_double: lvdesc = L"D"; break;
			case local_var_float: lvdesc = L"F"; break;
			case local_var_int: lvdesc = L"I"; break;
			case local_var_long: lvdesc = L"J"; break;
			}
			local_vars* lv = new (std::nothrow) local_vars(clf.get_constant_pool(), logger, locit->start_pc, locit->len, lvname, lvdesc, locit->lv_index);
			if (nullptr == lv)
			{
				++locit;
				continue;
			}
			if  (lv->getError())
			{
				delete lv;
				++locit;
				continue;
			}
			aLVT.get_table().push_back(lv);
			++locit;
		}

		++method;
	}
	loggerLocal.log(1, "\n");
	return true;
}

bool RestoreLineNumbers(CClassFile& clf, wchar_t* linesFileName, CLogger& logger, CLogger& loggerLocal)
{
	if (nullptr == linesFileName)
	{
		loggerLocal.log(1, "Null disassembly file name.\n");
		return false;
	}

	FILE* linesFile;
	if (0 != _wfopen_s(&linesFile, linesFileName, L"w"))
	{
		loggerLocal.log(1, "Can't open disassembly file for writing.\n");
		return false;
	}

	static const wchar_t disHead[] =
		L";-------------------------------------------------------------------------------\n"
		L"; File generated by dirtyJOE " DJ_VERSION_SHORT_W L"\n"
		L"; Copyright © 2008-2014 ReWolf\n"
		L"; http://dirty-joe.com\n"
		L"; joe@dirty-joe.com\n"
		L";-------------------------------------------------------------------------------\n\n";

	static const wchar_t separator[] = L";-------------------------------------------------------------------------------\n";

	fwprintf(linesFile, disHead);

	int currentLine = 8;

	auto& methods = clf.get_methods();
	for (size_t i = 0; i < methods.size(); i++)
	{
		CONSTANT_Utf8_info* nameUtf8 = getUtf8FromCP(methods[i]->get_name_index(), clf.get_constant_pool());
		CONSTANT_Utf8_info* descUtf8 = getUtf8FromCP(methods[i]->get_descriptor_index(), clf.get_constant_pool());
		malloc_ptr<wchar_t> acc_wstr = genWStringFromAccessFlags(methods[i]->get_access_flags(), true);
		malloc_ptr<wchar_t> name_wstr = preprocessUtf8ToWChar(nameUtf8);
		malloc_ptr<wchar_t> desc_wstr = preprocessUtf8ToWChar(descUtf8);
		// no need to check all those pointers above, as they're checked inside decodeDescriptor() and preprocessUtf8ToWChar()
		malloc_ptr<wchar_t> demangled = decodeDescriptor(desc_wstr.get(), name_wstr.get(), acc_wstr.get());
		fwprintf(linesFile, L"%s\n", (nullptr != demangled) ? demangled.get() : L"ERROR: couldn't generate method name");
		currentLine++;

		if (nullptr != name_wstr)
			loggerLocal.log(1, "\r  Analysing (%d/%d): %ws...", i + 1, methods.size(), name_wstr.get());

		CAttrCode* _attrObj_ptr = getCodeAttribute(*methods[i]);
		if (nullptr == _attrObj_ptr)
		{
			//'Code' attribute not found (!?!)
			fwprintf(linesFile, L"\t;no code\n\n");
			currentLine++;
			continue;
		}
		CAttrCode& attrObj = *_attrObj_ptr;

		const size_t tcps = clf.get_constant_pool().size();
		// add LineNumberTable string to CP if needed
		attribute_info* attrLN = new (std::nothrow) attribute_info(ATTR_LINE_NUMBER_TABLE, clf.get_constant_pool(), logger);
		if (tcps != clf.get_constant_pool().size())
			clf.setModified();
		if ((nullptr == attrLN) || attrLN->getError())
		{
			//memory allocation problem
			fwprintf(linesFile, L"\t;no memory for LineNumberTable attribute\n");
			currentLine++;
			continue;
		}
		CAttrLineNumberTable& aLNT = (CAttrLineNumberTable&)attrLN->get_attr();
		eraseAttributeByType(clf.get_constant_pool(), attrObj.get_attributes(), ATTR_LINE_NUMBER_TABLE);
		attrObj.get_attributes().push_back(attrLN);
		clf.setModified();

		u4 pos = 0;
		while (pos < attrObj.get_code().size())
		{
			line_number_tab* lnt = new (std::nothrow) line_number_tab((u2)pos, (u2)currentLine, clf.get_constant_pool(), logger);
			if (nullptr == lnt)
			{
				badAllocMsg(loggerLocal);
				fclose(linesFile);
				return false;
			}
			aLNT.get_table().push_back(lnt);

			int len = jdisasm_get_len(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos);
			wchar_t str_pos[0x10];
			swprintf_s(str_pos, L"%08X", pos);
			if (len > 0)
			{
				malloc_ptr<wchar_t> opstr = jdisasm_get_str(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos, &clf.get_constant_pool());
				malloc_ptr<wchar_t> opstr2 = wcs_escape_whitespace(opstr.get());
				if (nullptr != opstr2)
				{
					fwprintf(linesFile, L"%08X: \t%.120s\n", pos, opstr2.get());
					if (jdisasm_is_block_end_opcode(jdisasm_get_type(&attrObj.get_code()[0], (u4)attrObj.get_code().size(), pos)))
					{
						fwprintf(linesFile, separator);
						currentLine++;
					}
				}
				else
				{
					fwprintf(linesFile, L"\t;#DISASM ERROR#\n");
				}
				pos += len;
			}
			else
			{
				fwprintf(linesFile, L"\t;#UNKNOWN OPCODE#\n");
				pos++;
			}
			currentLine++;
		}

		fwprintf(linesFile, L"\n");
		currentLine++;
	}
	loggerLocal.log(1, "\n");
	fclose(linesFile);

	// create CONSTANT_Utf8_info object with linesFileName
	wchar_t fName[_MAX_FNAME];
	wchar_t fExt[_MAX_EXT];
	if (0 != _wsplitpath_s(linesFileName, nullptr, 0, nullptr, 0, fName, _MAX_FNAME, fExt, _MAX_EXT))
	{
		loggerLocal.log(1, "splitpath problem.");
		return false;
	}
	malloc_ptr<wchar_t> sourceName = wcscat_var(fName, fExt, 0);
	if (nullptr == sourceName)
	{
		badAllocMsg(loggerLocal);
		return false;
	}

	CONSTANT_Utf8_info* linesCPUtf8 = nullptr;
	u2 sourceNameIdx = checkStringInCP(clf.get_constant_pool(), sourceName.get(), logger);
	if (0 == sourceNameIdx)
	{
		linesCPUtf8 = new (std::nothrow) CONSTANT_Utf8_info(sourceName.get(), logger);
		if (nullptr == linesCPUtf8)
		{
			badAllocMsg(loggerLocal);
			return false;
		}
	}

	// create SourceFile attribute object
	const size_t tcps = clf.get_constant_pool().size();
	attribute_info* sfa = new (std::nothrow) attribute_info(ATTR_SOURCE_FILE, clf.get_constant_pool(), logger);
	if (tcps != clf.get_constant_pool().size())
		clf.setModified();
	if ((nullptr == sfa) || sfa->getError())
	{
		if (nullptr != linesCPUtf8)
			delete linesCPUtf8;
		badAllocMsg(loggerLocal);
		return false;
	}

	CAttrSourceFile& attrSrc = (CAttrSourceFile&)sfa->get_attr();
	// add linesFileName to Constant Pool
	if (nullptr != linesCPUtf8)
	{
		attrSrc.set_value((u2)clf.get_constant_pool().size());
		clf.get_constant_pool().push_back(linesCPUtf8);
	}
	else
		attrSrc.set_value(sourceNameIdx);

	// add attribute to the class attributes
	eraseAttributeByType(clf.get_constant_pool(), clf.get_attributes(), ATTR_SOURCE_FILE);
	clf.get_attributes().push_back(sfa);
	clf.setModified();

	return true;
}

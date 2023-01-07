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

/*

.class file references specification (draft)

global specifiers:

'g' -> item referenced in global class structure
'c' -> item referenced in constant pool
'f' -> item referenced in fields table
'm' -> item referenced in methods table
'a' -> item referenced in attributes table

detailed info about each specifier:

--

'g' specifier:

possible values will be equal to the original fields name from the class:

'g:this_class;'
'g:super_class;'

--

'c' specifier:

possible values will be equal to the index from the constant pool:

'c:%02X:n;' -> 'n' stands for field name from the specific structure

--

'f' and 'm' specifier:

possible value can be equal to the original field name from the structure
or it can be sub-specifier 'a' if it is related to attribute:

'f:name_index;'
'f:descriptor_index;'
'f:a:%04X;' -> attribute[%04X]

for nested attributes structure can look like this:

'f:a:%02X:at:at_xxx;'

where 'at' will be one of the attributes types described below and
'at_xxx' will be field name allowed for specific attribute

--

simple sttributes:

a:%04X:sourcefile;
a:%04X:constantvalue;
a:%04X:signature;

*/

#include "hlp_references.h"
#include "jdisasm/jdisasm.h"
#include "java_class_def.h"
#include "helpers.h"
#include "../common/common.h"

//some const strings used by parser
static const wchar_t spr_name_index[] = L"name_index";
static const wchar_t spr_string_index[] = L"string_index";
static const wchar_t spr_class_index[] = L"class_index";
static const wchar_t spr_name_and_type_index[] = L"name_and_type_index";
static const wchar_t spr_descriptor_index[] = L"descriptor_index";
static const wchar_t spr_reference_index[] = L"reference_index";
static const wchar_t spr_super_class[] = L"super_class";
static const wchar_t spr_this_class[] = L"this_class";
static const wchar_t spr_interfaces[] = L"interfaces";
static const wchar_t spr_attribute_name_index[] = L"attribute_name_index";
static const wchar_t spr_catch_type[] = L"catch_type";
static const wchar_t spr_exception_table[] = L"exception_table";
static const wchar_t spr_bc[] = L"bc";
static const wchar_t spr_inner_class_info_index[] = L"inner_class_info_index";
static const wchar_t spr_outer_class_info_index[] = L"outer_class_info_index";
static const wchar_t spr_inner_name_index[] = L"inner_name_index";
static const wchar_t spr_signature_index[] = L"signature_index";
static const wchar_t spr_bootstrap_method_ref[] =L"bootstrap_method_ref";
static const wchar_t spr_bootstrap_arguments[] =L"bootstrap_arguments";
//below separators and IDs must be one-char !
static const wchar_t COLON[] = L":";
static const wchar_t SEMICOLON[] = L";";
static const wchar_t EMPTYSTR[] = L"";

static const wchar_t cidATTR = 'a';
static const wchar_t cidGLOB = 'g';
static const wchar_t cidCPOL = 'c';
static const wchar_t cidMETH = 'm';
static const wchar_t cidFILD = 'f';
static const wchar_t idATTR[] = {cidATTR, 0};
static const wchar_t idGLOB[] = {cidGLOB, 0};
static const wchar_t idCPOL[] = {cidCPOL, 0};
static const wchar_t idMETH[] = {cidMETH, 0};
static const wchar_t idFILD[] = {cidFILD, 0};

bool getXRefs_pfx_type_CLN_04X_CLN_str_sufx(std::vector<wchar_t*>* ret, const wchar_t* cstr, u4 i, const wchar_t* type, const wchar_t* pfx, const wchar_t* sufx)
{
	wchar_t xs[8];
	swprintf_s(xs, L"%04X", i);
	malloc_ptr<wchar_t> pbstr = wcscat_var(pfx, type, COLON, xs, COLON, cstr, sufx, 0);
	if (nullptr == pbstr)
	{
		cleanUpObjects(*ret);
		delete ret;
		return false;
	}
	ret->push_back(pbstr.release());
	return true;
}

bool getXRefs_pfx_04X_CLN_str_sufx(std::vector<wchar_t*>* ret, const wchar_t* cstr, u4 i, wchar_t* pfx, const wchar_t* sufx)
{
	wchar_t xs[8];
	swprintf_s(xs, L"%04X", i);
	malloc_ptr<wchar_t> pbstr = wcscat_var(pfx, xs, COLON, cstr, sufx, 0);
	if (nullptr == pbstr)
	{
		cleanUpObjects(*ret);
		delete ret;
		return false;
	}
	ret->push_back(pbstr.release());
	return true;
}

bool getXRefs_pfx_04X_CLN_str_CLN_04X_sufx(std::vector<wchar_t*>* ret, const wchar_t* cstr, u4 i, u4 i2, wchar_t* pfx, const wchar_t* sufx)
{
	wchar_t xs[8];
	swprintf_s(xs, L"%04X", i);
	wchar_t xs2[8];
	swprintf_s(xs2, L"%04X", i2);
	malloc_ptr<wchar_t> pbstr = wcscat_var(pfx, xs, COLON, cstr, COLON, xs2, sufx, 0);
	if (nullptr == pbstr)
	{
		cleanUpObjects(*ret);
		delete ret;
		return false;
	}
	ret->push_back(pbstr.release());
	return true;
}

bool getXRefs_pfx_str_CLN_08X_sufx(std::vector<wchar_t*>* ret, const wchar_t* cstr, u4 i, wchar_t* pfx, const wchar_t* sufx)
{
	wchar_t xs[16];
	swprintf_s(xs, L"%08X", i);
	malloc_ptr<wchar_t> pbstr = wcscat_var(pfx, cstr, COLON, xs, sufx, 0);
	if (nullptr == pbstr)
	{
		cleanUpObjects(*ret);
		delete ret;
		return false;
	}
	ret->push_back(pbstr.release());
	return true;
}

bool getXRefs_pfx_CLN_str_sufx(std::vector<wchar_t*>* ret, const wchar_t* cstr, const wchar_t* pfx, const wchar_t* sufx)
{
	malloc_ptr<wchar_t> pbstr = wcscat_var(pfx, COLON, cstr, sufx, 0);
	if (nullptr == pbstr)
	{
		cleanUpObjects(*ret);
		delete ret;
		return false;
	}
	ret->push_back(pbstr.release());
	return true;
}

bool getXRefs_pfx_04X_sufx(std::vector<wchar_t*>* ret, u4 i, wchar_t* pfx, const wchar_t* sufx)
{
	wchar_t xs[8];
	swprintf_s(xs, L"%04X", i);
	malloc_ptr<wchar_t> pbstr = wcscat_var(pfx, xs, sufx, 0);
	if (nullptr == pbstr)
	{
		cleanUpObjects(*ret);
		delete ret;
		return false;
	}
	ret->push_back(pbstr.release());
	return true;
}

bool getCPRefs_helper(std::vector<wchar_t*>* ret, const wchar_t* cstr, u4 i)
{
	return getXRefs_pfx_type_CLN_04X_CLN_str_sufx(ret, cstr, i, idCPOL, EMPTYSTR, SEMICOLON);
}

bool getARefs_helper(std::vector<wchar_t*>* ret, const wchar_t* cstr, u4 i, const wchar_t* pfx, const wchar_t* sufx)
{
	return getXRefs_pfx_type_CLN_04X_CLN_str_sufx(ret, cstr, i, idATTR, pfx, sufx);
}

template <class T>
bool getCPRefs_CONSTANT_uX(const JavaConstPool& cp, std::vector<wchar_t*>* ret, u4 i, const wchar_t* nm, T item)
{
	if (nullptr == cp[i])
		return false;

	if (((CONSTANT_uX_info<T>*)cp[i])->v1 == item)
	{
		if (!getCPRefs_helper(ret, nm, i))
			return false;
	}
	return true;
}

template <class T, class U>
bool getCPRefs_CONSTANT_uXuX(const JavaConstPool& cp, std::vector<wchar_t*>* ret, u4 i, const wchar_t* nm1, const wchar_t* nm2, u2 item)
{
	if (nullptr == cp[i])
		return false;

	if ((typeid(T) == typeid(item)) && 
		(nullptr != nm1) && 
		(((CONSTANT_uXuX_info<T, U>*)cp[i])->v1 == item))
	{
		if (!getCPRefs_helper(ret, nm1, i))
			return false;
	}

	if ((typeid(U) == typeid(item)) && 
		(nullptr != nm2) && 
		(((CONSTANT_uXuX_info<T, U>*)cp[i])->v2 == item))
	{
		if (!getCPRefs_helper(ret, nm2, i))
			return false;
	}
	return true;
}

std::vector<wchar_t*>* getCPRefs(const JavaConstPool& cp, u2 item, std::vector<wchar_t*>* ret)
{
	if (nullptr == ret)
		ret = new (std::nothrow) std::vector<wchar_t*>;

	if (nullptr == ret)
		return ret;

	for (u4 i = 1; i < cp.size(); i++)
	{
		if (nullptr == cp[i])
			continue;

		switch (cp[i]->tag)
		{
		case CONSTANT_Class:
			if (!getCPRefs_CONSTANT_uX(cp, ret, i, spr_name_index, item))
				return nullptr;
			break;
		case CONSTANT_String:
			if (!getCPRefs_CONSTANT_uX(cp, ret, i, spr_string_index, item))
				return nullptr;
			break;
		case CONSTANT_MethodType:
			if (!getCPRefs_CONSTANT_uX(cp, ret, i, spr_descriptor_index, item))
				return nullptr;
			break;
		case CONSTANT_Fieldref:
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			if (!getCPRefs_CONSTANT_uXuX<u2, u2>(cp, ret, i, spr_class_index, spr_name_and_type_index, item))
				return nullptr;
			break;
		case CONSTANT_NameAndType:
			if (!getCPRefs_CONSTANT_uXuX<u2, u2>(cp, ret, i, spr_name_index, spr_descriptor_index, item))
				return nullptr;
			break;
		case CONSTANT_InvokeDynamic:
			if (!getCPRefs_CONSTANT_uXuX<u2, u2>(cp, ret, i, nullptr, spr_name_and_type_index, item))
				return nullptr;
			break;
		case CONSTANT_MethodHandle:
			if (!getCPRefs_CONSTANT_uXuX<u1, u2>(cp, ret, i, nullptr, spr_reference_index, item))
				return nullptr;
			break;
		}
	}

	return ret;
}

std::vector<wchar_t*>* getFMRefs(const JavaConstPool& cp, u2 item, std::vector<field_info*>& fms, bool fields, std::vector<wchar_t*>* ret)
{
	if (nullptr == ret)
		ret = new (std::nothrow) std::vector<wchar_t*>;

	if (nullptr == ret)
		return ret;

	const wchar_t* type;
	if (fields)
		type = idFILD;
	else
		type = idMETH;

	for (u4 i = 0; i < fms.size(); i++)
	{
		if (fms[i]->get_name_index() == item)
		{
			if (!getXRefs_pfx_type_CLN_04X_CLN_str_sufx(ret, spr_name_index, i, type, EMPTYSTR, SEMICOLON))
				return nullptr;
		}
		if (fms[i]->get_descriptor_index() == item)
		{
			if (!getXRefs_pfx_type_CLN_04X_CLN_str_sufx(ret, spr_descriptor_index, i, type, EMPTYSTR, SEMICOLON))
				return nullptr;
		}
		//prepare prefix and check attribs
		wchar_t tstr[0x20];
		swprintf_s(tstr, L"%s%s%04X%s", type, COLON, i, COLON);
		if (!getAttrRefs(cp, item, fms[i]->get_attributes(), tstr, ret))
			return nullptr;
	}

	return ret;
}

std::vector<wchar_t*>* getGlobalRefs(CClassFile& cf, u2 item, std::vector<wchar_t*>* ret)
{
	if (nullptr == ret)
		ret = new (std::nothrow) std::vector<wchar_t*>;

	if (nullptr == ret)
		return ret;

	if (cf.get_super_class() == item)
	{
		if (!getXRefs_pfx_CLN_str_sufx(ret, spr_super_class, idGLOB, SEMICOLON))
			return nullptr;
	}

	if (cf.get_this_class() == item)
	{
		if (!getXRefs_pfx_CLN_str_sufx(ret, spr_this_class, idGLOB, SEMICOLON))
			return nullptr;
	}

	for (u4 i = 0; i < cf.get_interfaces().size(); i++)
	{
		if (cf.get_interfaces()[i] == item)
		{
			wchar_t t2[3] = {idGLOB[0], COLON[0], 0};
			if (!getXRefs_pfx_str_CLN_08X_sufx(ret, spr_interfaces, i, t2, SEMICOLON))
				return nullptr;
		}
	}

	return ret;
}

bool getAttrRefs_AttrUxTmpt(std::vector<attribute_info*>& attrs, std::vector<wchar_t*>* ret, u4 i, u2 item, const wchar_t* nm, const wchar_t* prefix, const wchar_t* sufx)
{
	if (((CAttrUxTemplate<u2>&)attrs[i]->get_attr()).get_value() == item)
	{
		if (!getARefs_helper(ret, nm, i, prefix, sufx))
			return false;
	}
	return true;
}

std::vector<wchar_t*>* getAttrRefs(const JavaConstPool& cp, const u2 item, std::vector<attribute_info*>& attrs, const wchar_t* prefix, std::vector<wchar_t*>* ret)
{
	if (ret == 0)
		ret = new (std::nothrow) std::vector<wchar_t*>;

	if (ret == 0)
		return ret;

	for (u4 i = 0; i < attrs.size(); i++)
	{
		if (attrs[i]->get_attribute_name_index() == item)
		{
			if (!getARefs_helper(ret, spr_attribute_name_index, i, prefix, SEMICOLON))
				return false;
		}
		JavaAttribute attrType = getAttributeType(cp, attrs[i]->get_attribute_name_index());
		switch (attrType)
		{
			case ATTR_SOURCE_FILE:
				if (!getAttrRefs_AttrUxTmpt(attrs, ret, i, item, ATTR_SOURCE_FILE_STR, prefix, SEMICOLON))
					return 0;
				break;
			case ATTR_CONSTANT_VALUE:
				if (!getAttrRefs_AttrUxTmpt(attrs, ret, i, item, ATTR_CONSTANT_VALUE_STR, prefix, SEMICOLON))
					return 0;
				break;
			case ATTR_SIGNATURE:
				if (!getAttrRefs_AttrUxTmpt(attrs, ret, i, item, ATTR_SIGNATURE_STR, prefix, SEMICOLON))
					return 0;
				break;
			case ATTR_CODE:
				{
					if (!getARefs_helper(ret, ATTR_CODE_STR, i, prefix, COLON))
						return 0;
					malloc_ptr<wchar_t> pref((*ret)[ret->size() - 1]);
					ret->pop_back();
					CAttrCode& cac = (CAttrCode&)attrs[i]->get_attr();
					for (u4 j = 0; j < cac.get_exception_table().size(); j++)
					{
						if (cac.get_exception_table()[j]->get_catch_type() == item)
						{
							if (!getXRefs_pfx_type_CLN_04X_CLN_str_sufx(ret, spr_catch_type, j, spr_exception_table, pref.get(), SEMICOLON))
								return 0;
						}
					}
					if (!getAttrRefs(cp, item, cac.get_attributes(), pref.get(), ret))
						return 0;
					//searching for references in bytecode
					std::vector<u1>& cd = cac.get_code();
					u4 cpos = 0;
					do
					{
						int len = jdisasm_get_len(&cd[0], (u4)cd.size(), cpos);
						if (len < 0)
							return 0;

						int ref = jdisasm_get_ref(&cd[cpos], (u4)cd.size() - cpos);
						if (ref == item)
						{
							if (!getXRefs_pfx_str_CLN_08X_sufx(ret, spr_bc, cpos, pref.get(), SEMICOLON))
								return 0;
						}

						cpos += len;
					}
					while (cpos < cd.size());
				}
				break;
			case ATTR_EXCEPTIONS:
				{
					if (!getARefs_helper(ret, ATTR_EXCEPTIONS_STR, i, prefix, COLON))
						return 0;
					malloc_ptr<wchar_t> pref((*ret)[ret->size() - 1]);
					ret->pop_back();
					CAttrTable<uX_dynStruct<u2>*>& cat = (CAttrTable<uX_dynStruct<u2>*>&)attrs[i]->get_attr();
					for (u4 j = 0; j < cat.get_table().size(); j++)
					{
						if (cat.get_table()[j]->get_value() == item)
						{
							if (!getXRefs_pfx_04X_sufx(ret, j, pref.get(), SEMICOLON))
								return 0;
						}
					}
				}
				break;
			case ATTR_INNER_CLASSES:
				{
					if (!getARefs_helper(ret, ATTR_INNER_CLASSES_STR, i, prefix, COLON))
						return 0;
					malloc_ptr<wchar_t> pref((*ret)[ret->size() - 1]);
					ret->pop_back();
					CAttrTable<inner_class*>& cat = (CAttrTable<inner_class*>&)attrs[i]->get_attr();
					for (u4 j = 0; j < cat.get_table().size(); j++)
					{
						if (cat.get_table()[j]->get_inner_class_info_index() == item)
						{
							if (!getXRefs_pfx_04X_CLN_str_sufx(ret, spr_inner_class_info_index, j, pref.get(), SEMICOLON))
								return 0;
						}
						if (cat.get_table()[j]->get_outer_class_info_index() == item)
						{
							if (!getXRefs_pfx_04X_CLN_str_sufx(ret, spr_outer_class_info_index, j, pref.get(), SEMICOLON))
								return 0;
						}
						if (cat.get_table()[j]->get_inner_name_index() == item)
						{
							if (!getXRefs_pfx_04X_CLN_str_sufx(ret, spr_inner_name_index, j, pref.get(), SEMICOLON))
								return 0;
						}
					}
				}
				break;
			case ATTR_BOOTSTRAP_METHODS:
				{
					if (!getARefs_helper(ret, ATTR_BOOTSTRAP_METHODS_STR, i, prefix, COLON))
						return 0;
					malloc_ptr<wchar_t> pref((*ret)[ret->size() - 1]);
					ret->pop_back();
					CAttrTable<bootstrap_methods*>& cat = (CAttrTable<bootstrap_methods*>&)attrs[i]->get_attr();
					for (u4 j = 0; j < cat.get_table().size(); j++)
					{
						if (cat.get_table()[j]->get_bootstrap_method_ref() == item)
						{
							if (!getXRefs_pfx_04X_CLN_str_sufx(ret, spr_bootstrap_method_ref, j, pref.get(), SEMICOLON))
								return 0;
						}
						for (u4 k = 0; k < cat.get_table()[j]->get_bootstrap_arguments().size(); k++)
						{
							if (cat.get_table()[j]->get_bootstrap_arguments()[k] == item)
							{
								if (!getXRefs_pfx_04X_CLN_str_CLN_04X_sufx(ret, spr_bootstrap_arguments, j, k, pref.get(), SEMICOLON))
									return 0;
							}
						}
					}
				}
				break;
			case ATTR_LOCAL_VARIABLE_TABLE:
			case ATTR_LOCAL_VARIABLE_TYPE_TABLE:
				{
					if (!getARefs_helper(ret, (attrType == ATTR_LOCAL_VARIABLE_TABLE) ? ATTR_LOCAL_VARIABLE_TABLE_STR : ATTR_LOCAL_VARIABLE_TYPE_TABLE_STR, i, prefix, COLON))
						return 0;
					malloc_ptr<wchar_t> pref((*ret)[ret->size() - 1]);
					ret->pop_back();
					CAttrTable<local_vars*>& cat = (CAttrTable<local_vars*>&)attrs[i]->get_attr();
					for (u4 j = 0; j < cat.get_table().size(); j++)
					{
						if (cat.get_table()[j]->get_name_index() == item)
						{
							if (!getXRefs_pfx_04X_CLN_str_sufx(ret, spr_name_index, j, pref.get(), SEMICOLON))
								return 0;
						}
						if (cat.get_table()[j]->get_descriptor_index() == item)
						{
							if (!getXRefs_pfx_04X_CLN_str_sufx(ret, (attrType == ATTR_LOCAL_VARIABLE_TABLE) ? spr_descriptor_index : spr_signature_index, j, pref.get(), SEMICOLON))
								return 0;
						}
					}
				}
				break;
		}
	}

	return ret;
}

std::vector<wchar_t*>* getAllRefs(CClassFile& cf, u2 item)
{
	std::vector<wchar_t*>* ret = getGlobalRefs(cf, item);
	if (ret == 0)
		return 0;
	if (getCPRefs(cf.get_constant_pool(), item, ret) == 0)
		return 0;
	if (getAttrRefs(cf.get_constant_pool(), item, cf.get_attributes(), EMPTYSTR, ret) == 0)
		return 0;
	if (getFMRefs(cf.get_constant_pool(), item, cf.get_fields(), true, ret) == 0)
		return 0;
	if (getFMRefs(cf.get_constant_pool(), item, cf.get_methods(), false, ret) == 0)
		return 0;
	return ret;
}

u4 parseReference_getVal(wchar_t* str, wchar_t*& cptr)
{
	u4 val = 0;
	cptr = 0;
	while ((str[0] != COLON[0]) && (str[0] != SEMICOLON[0]))
	{
		val <<= 4;
		if ((str[0] >= 'a') && (str[0] <= 'f'))
		{
			val |= (str[0] - 0x57);
		}
		else if ((str[0] >= 'A') && (str[0] <= 'F'))
		{
			val |= (str[0] - 0x37);
		}
		else if ((str[0] >= '0') && (str[0] <= '9'))
		{
			val |= (str[0] - 0x30);
		}
		else
			return (u4)-1;
		str++;
	}
	cptr = str;
	return val;
}

malloc_ptr<wchar_t> parseReference_getStrVal(wchar_t* str, wchar_t*& cptr)
{
	cptr = 0;
	int slen = 0;
	while ((str[slen] != COLON[0]) && (str[slen] != SEMICOLON[0]))
	{
		slen++;
	}

	malloc_ptr<wchar_t> ret = malloc<wchar_t>(slen + 1);
	if (nullptr == ret)
		return nullptr;

	memmove(ret.get(), str, 2*slen);
	ret.get()[slen] = 0;

	cptr = str + slen;

	return ret;
}

wchar_t* parseReference_sub(CClassFile& cf, wchar_t* str_in, const wchar_t* str_out);

wchar_t* parseReference_rethlp(CClassFile& cf, wchar_t* furt_ptr, wchar_t* nret)
{
	if (nullptr == nret)
		return nullptr;

	if ((furt_ptr[0] == SEMICOLON[0]) || (furt_ptr[0] == 0))
	{
		nret[wcslen(nret) - 2] = 0;
		return nret;
	}

	malloc_ptr<wchar_t> nret_watch(nret);
	return parseReference_sub(cf, furt_ptr + 1, nret);
}

wchar_t* parseReference_fm(CClassFile& cf, wchar_t* str_in, const wchar_t* str_out, std::vector<field_info*>& fm, wchar_t* pfx)
{
	wchar_t* furt_ptr = 0;
	u4 v = parseReference_getVal(str_in + 2, furt_ptr);
	if (v == (u4)-1)
		return 0;

	if (v >= fm.size())
		return 0;

	malloc_ptr<wchar_t> mname = genStringFromCP(cf.get_constant_pool(), fm[v]->get_name_index(), false);
	if (nullptr == mname)
		return nullptr;

	return parseReference_rethlp(cf, furt_ptr, wcscat_var(str_out, pfx, mname.get(), L", ", 0).release());
}

bool parseReference_vtab(wchar_t*& furt_ptr, malloc_ptr<wchar_t>& atname)
{
	u4 v2 = parseReference_getVal(furt_ptr + 1, furt_ptr);
	if (v2 == (u4)-1)
		return false;
	wchar_t t2[0x20];
	swprintf_s(t2, L"[%d]", v2);
	atname = wcscat_var(atname.get(), t2, 0);
	if (nullptr == atname)
		return false;
	return true;
}

malloc_ptr<wchar_t> parseReference_vstr(u4& v, wchar_t* str_in, wchar_t*& furt_ptr)
{
	v = parseReference_getVal(str_in, furt_ptr);
	if (v == (u4)-1)
		return nullptr;
	malloc_ptr<wchar_t> atname = parseReference_getStrVal(furt_ptr + 1, furt_ptr);
	if (nullptr == atname)
		return nullptr;
	return atname;
}

wchar_t* parseReference_sub(CClassFile& cf, wchar_t* str_in, const wchar_t* str_out)
{
	if (str_in[1] == COLON[0])
	{
		switch (str_in[0])
		{
		case cidMETH:
			{
				return parseReference_fm(cf, str_in, str_out, cf.get_methods(), L"Method: ");
			}
			break;
		case cidFILD:
			{
				return parseReference_fm(cf, str_in, str_out, cf.get_fields(), L"Field: ");
			}
			break;
		case cidCPOL:
			{
				wchar_t* furt_ptr = 0;
				u4 v = parseReference_getVal(str_in + 2, furt_ptr);
				if (v == (u4)-1)
					return 0;
				wchar_t tmp[0x20];
				swprintf_s(tmp, L"%d", v);
				return wcscat_var(str_out, L"Constant pool: ", tmp, 0).release();
			}
			break;
		case cidATTR:
			{
				wchar_t* furt_ptr = 0;
				u4 v;
				malloc_ptr<wchar_t> atname = parseReference_vstr(v, str_in + 2, furt_ptr);
				if (nullptr == atname)
					return nullptr;

				if (0 == wcscmp(atname.get(), ATTR_INNER_CLASSES_STR))
				{
					u4 v2;
					malloc_ptr<wchar_t> atname2 = parseReference_vstr(v2, furt_ptr + 1, furt_ptr);
					if (nullptr == atname2)
						return nullptr;

					wchar_t t2[0x20];
					swprintf_s(t2, L"[%d].", v2);
					atname = wcscat_var(atname.get(), t2, atname2.get(), 0);
					if (nullptr == atname)
						return nullptr;
				}
				else if (0 == wcscmp(atname.get(), ATTR_BOOTSTRAP_METHODS_STR))
				{
					if (!parseReference_vtab(furt_ptr, atname))
						return nullptr;

					malloc_ptr<wchar_t> itemName = parseReference_getStrVal(furt_ptr + 1, furt_ptr);
					if (nullptr == itemName)
						return nullptr;

					u4 itemIdx = parseReference_getVal(furt_ptr + 1, furt_ptr);
					if (-1 == itemIdx)
						return nullptr;

					wchar_t t2[0x20];
					swprintf_s(t2, L"[%d]", itemIdx);
					atname = wcscat_var(atname.get(), L".", itemName.get(), t2, 0);
					if (nullptr == atname)
						return nullptr;
				}
				else if (0 == wcscmp(atname.get(), ATTR_EXCEPTIONS_STR))
				{
					if (!parseReference_vtab(furt_ptr, atname))
						return nullptr;
				}

				return parseReference_rethlp(cf, furt_ptr, wcscat_var(str_out, L"attribute: ", atname.get(), L", ", 0).release());
			}
			break;
		case cidGLOB:
			{
				wchar_t* furt_ptr = 0;
				malloc_ptr<wchar_t> atname = parseReference_getStrVal(str_in + 2, furt_ptr);
				if (nullptr == atname)
					return nullptr;

				if (!wcscmp(atname.get(), spr_interfaces))
				{
					if (!parseReference_vtab(furt_ptr, atname))
						return nullptr;
				}

				return parseReference_rethlp(cf, furt_ptr, wcscat_var(str_out, L"Overview: ", atname.get(), L", ", 0).release());
			}
			break;
		}
	}
	else
	{
		if (!wcsncmp(str_in, spr_bc, _countof(spr_bc) - 1))
		{
			wchar_t* furt_ptr = 0;
			u4 v = parseReference_getVal(str_in + _countof(spr_bc), furt_ptr);
			if (v == (u4)-1)
				return 0;
			wchar_t tmp[0x20];
			swprintf_s(tmp, L"%08X", v);
			return parseReference_rethlp(cf, furt_ptr, wcscat_var(str_out, L"bytecode@", tmp, L", ", 0).release());
		}
		else if (!wcsncmp(str_in, spr_descriptor_index, _countof(spr_descriptor_index) - 1))
		{
			return parseReference_rethlp(cf, str_in + _countof(spr_descriptor_index) - 1, wcscat_var(str_out, spr_descriptor_index, L", ", 0).release());
		}
		else if (!wcsncmp(str_in, spr_name_index, _countof(spr_name_index) - 1))
		{
			return parseReference_rethlp(cf, str_in + _countof(spr_name_index) - 1, wcscat_var(str_out, spr_name_index, L", ", 0).release());
		}
		else if (!wcsncmp(str_in, spr_exception_table, _countof(spr_exception_table) - 1))
		{
			wchar_t* furt_ptr = 0;
			u4 v2;
			malloc_ptr<wchar_t> atname2 = parseReference_vstr(v2, str_in + _countof(spr_exception_table), furt_ptr);
			if (nullptr == atname2)
				return nullptr;

			wchar_t t2[0x20];
			swprintf_s(t2, L"[%d].", v2);
			return parseReference_rethlp(cf, furt_ptr, wcscat_var(str_out, spr_exception_table, t2, atname2.get(), L", ", 0).release());
		}
	}

	return wcscat_var(str_out, L"[", str_in, L"]", 0).release();
}

wchar_t* parseReference(CClassFile& cf, wchar_t* str)
{
	return parseReference_sub(cf, str, EMPTYSTR);
}

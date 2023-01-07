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

#include "helpers.h"
#include "../common/common.h"
#include "jdisasm/jdisasm.h"
#include "same_locals_1_stack_item_frame.h"
#include "same_locals_1_stack_item_frame_extended.h"
#include "append_frame.h"
#include "full_frame.h"

const wchar_t* const getWCharFromCPTag(u1 tag)
{
	for (int i = 0; i < _countof(cp_type_tab); i++)
	{
		if (tag == cp_type_tab[i].tag_id)
			return cp_type_tab[i].tag_name;
	}
	return nullptr;
}

CONSTANT_Utf8_info* getUtf8FromV1(CONSTANT_uX_info<u2>* classS, const JavaConstPool& cpInfo)
{
	if (classS->v1 >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[classS->v1])
		return nullptr;
	if (cpInfo[classS->v1]->tag != CONSTANT_Utf8) 
		return nullptr;
	return (CONSTANT_Utf8_info*)cpInfo[classS->v1];
}

CONSTANT_Utf8_info* getUtf8FromV1(u2 classNum, const JavaConstPool& cpInfo)
{
	if (classNum >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[classNum])
		return nullptr;
	if (cpInfo[classNum]->tag != CONSTANT_Class) 
		return nullptr;
	return getUtf8FromV1((CONSTANT_Class_info*)cpInfo[classNum], cpInfo);
}

malloc_ptr<wchar_t> getNameFromNAT(CONSTANT_NameAndType_info* nat, const JavaConstPool& cpInfo)
{
	if (nat->v1 >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[nat->v1])
		return nullptr;
	if (cpInfo[nat->v1]->tag != CONSTANT_Utf8) 
		return nullptr;

	return preprocessUtf8ToWChar((CONSTANT_Utf8_info*)cpInfo[nat->v1]);
}

malloc_ptr<wchar_t> getTypeFromNAT(CONSTANT_NameAndType_info* nat, const JavaConstPool& cpInfo)
{
	if (nat->v2 >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[nat->v2])
		return nullptr;
	if (cpInfo[nat->v2]->tag != CONSTANT_Utf8) 
		return nullptr;

	return preprocessUtf8ToWChar((CONSTANT_Utf8_info*)cpInfo[nat->v2]);
}

malloc_ptr<wchar_t> getNameAndTypeString(CONSTANT_NameAndType_info* nat, const JavaConstPool& cpInfo)
{
	static const wchar_t s1[] = L"name: ";
	static const wchar_t s2[] = L", descriptor: ";

	malloc_ptr<wchar_t> __v1 = getNameFromNAT(nat, cpInfo);
	if (nullptr == __v1)
		return nullptr;

	malloc_ptr<wchar_t> __v2 = getTypeFromNAT(nat, cpInfo);
	if (nullptr ==  __v2)
		return nullptr;

	u4 totalSize = (u4)(_countof(s1) + _countof(s2) + wcslen(__v1.get()) + wcslen(__v2.get()) + 1);
	malloc_ptr<wchar_t> retVal = malloc<wchar_t>(totalSize);
	if (nullptr == retVal) 
		return nullptr;

	swprintf_s(retVal.get(), totalSize, L"%s%s%s%s", s1, __v1.get(), s2, __v2.get());
	return retVal;
}

malloc_ptr<wchar_t> getClassFromRef(CONSTANT_uXuX_info<u2, u2>* ref, const JavaConstPool& cpInfo)
{
	if (ref->v1 >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[ref->v1])
		return nullptr;
	if (cpInfo[ref->v1]->tag != CONSTANT_Class) 
		return nullptr;

	CONSTANT_Utf8_info* clsN = getUtf8FromV1((CONSTANT_Class_info*)cpInfo[ref->v1], cpInfo);
	if (nullptr == clsN) 
		return nullptr;

	return preprocessUtf8ToWChar(clsN);
}

CONSTANT_NameAndType_info* getNATFromRef(CONSTANT_uXuX_info<u2, u2>* ref, const JavaConstPool& cpInfo)
{
	if (ref->v2 >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[ref->v2])
		return nullptr;
	if (cpInfo[ref->v2]->tag != CONSTANT_NameAndType) 
		return nullptr;

	return (CONSTANT_NameAndType_info*)cpInfo[ref->v2];
}

malloc_ptr<wchar_t> getRefString(CONSTANT_uXuX_info<u2, u2>* ref, const JavaConstPool& cpInfo)
{
	CONSTANT_NameAndType_info* natInfo = getNATFromRef(ref, cpInfo);
	if (nullptr == natInfo)
		return nullptr;

	malloc_ptr<wchar_t> natS = getNameAndTypeString(natInfo, cpInfo);
	if (nullptr == natS) 
		return nullptr;

	static const wchar_t s1[] = L"class: ";
	static const wchar_t s2[] = L", ";
	
	malloc_ptr<wchar_t> __clsN = getClassFromRef(ref, cpInfo);
	if (nullptr == __clsN)
		return nullptr;

	u4 totalSize = (u4)(_countof(s1) + _countof(s2) + wcslen(__clsN.get()) + wcslen(natS.get()) + 1);
	malloc_ptr<wchar_t> retVal = malloc<wchar_t>(totalSize);
	if (nullptr == retVal) 
		return nullptr;
	swprintf_s(retVal.get(), totalSize, L"%s%s%s%s", s1, __clsN.get(), s2, natS.get());
	return retVal;
}

malloc_ptr<wchar_t> genDisAsmStringFromCP(const JavaConstPool& cpInfo, u4 index)
{
	if (index >= cpInfo.size())
		return nullptr;

	cp_info* ccp = cpInfo[index];
	if (nullptr == ccp)
		return nullptr;

	switch (ccp->tag)
	{
		case CONSTANT_Class:
		{
			CONSTANT_Utf8_info* ccl = getUtf8FromV1((CONSTANT_Class_info*)ccp, cpInfo);
			if (nullptr == ccl) 
				return nullptr;
			int size_ccl = 0;
			malloc_ptr<wchar_t> __ccl = preprocessUtf8ToWChar(ccl, &size_ccl);
			if (nullptr == __ccl) 
				return nullptr;
			malloc_ptr<wchar_t> ret = decodeDescriptor(__ccl.get(), L"", L"");
			if (nullptr == ret)
			{
				//try to just exchange '/' to '.'
				wcschr_xchgall(__ccl.get(), L'/', L'.');
				return __ccl;
			}
			else				
				return ret;
		}
		break;
		case CONSTANT_Utf8:
		{
			int size_ccp = 0;
			malloc_ptr<wchar_t> __ccp = preprocessUtf8ToWChar((CONSTANT_Utf8_info*)ccp, &size_ccp);
			if (nullptr == __ccp) 
				return nullptr;
			return wcscat_var(L"\"", __ccp.get(), L"\"", 0);
		}
		break;
		case CONSTANT_InterfaceMethodref:
		case CONSTANT_Methodref:
		case CONSTANT_Fieldref:
		case CONSTANT_InvokeDynamic:
		{
			CONSTANT_NameAndType_info* natInfo = getNATFromRef((CONSTANT_uXuX_info<u2, u2>*)ccp, cpInfo);
			if (nullptr == natInfo)
				return nullptr;
			
			malloc_ptr<wchar_t> className = nullptr;
			wchar_t* strConnector = nullptr;
			if (CONSTANT_InvokeDynamic == ccp->tag)
			{
				wchar_t tempStr[0x20];
				swprintf_s(tempStr, L"BootstrapMethod[%d]", ((CONSTANT_uXuX_info<u2, u2>*)ccp)->v1);
				className = malloc_ptr<wchar_t>(_wcsdup(tempStr));
				strConnector = L":";
			}
			else
			{
				className = getClassFromRef((CONSTANT_uXuX_info<u2, u2>*)ccp, cpInfo);
				strConnector = L".";
			}

			if (nullptr == className)
				return nullptr;
			wcschr_xchgall(className.get(), L'/', L'.');

			malloc_ptr<wchar_t> refName = getNameFromNAT(natInfo, cpInfo);
			if (nullptr == refName)
				return nullptr;

			malloc_ptr<wchar_t> typeName = getTypeFromNAT(natInfo, cpInfo);
			if (nullptr == typeName)
				return nullptr;

			malloc_ptr<wchar_t> ctname = wcscat_var(className.get(), strConnector, refName.get(), 0);
			if (nullptr == ctname)
				return nullptr;

			return malloc_ptr<wchar_t>(decodeDescriptor(typeName.get(), ctname.get(), L""));
		}
		break;
		case CONSTANT_String:
		{
			CONSTANT_Utf8_info* ccl = getUtf8FromV1((CONSTANT_Class_info*)ccp, cpInfo);
			if (ccl == 0) 
				return 0;
			int size_ccl = 0;
			malloc_ptr<wchar_t> __ccl = preprocessUtf8ToWChar(ccl, &size_ccl);
			if (nullptr == __ccl) 
				return nullptr;
			return wcscat_var(L"\"", __ccl.get(), L"\"", 0);
		}
		break;
		case CONSTANT_Integer:
		{
			return genDisAsmStringFromCP_hlp(L"%d", (su4)((CONSTANT_Integer_info*)ccp)->v1);
		}
		break;
		case CONSTANT_Float:
		{
			return genDisAsmStringFromCP_hlp(L"%.25g", *(float*)&((CONSTANT_Float_info*)ccp)->v1);
		}
		break;
		case CONSTANT_Long:
		{
			u8 d64 = ((CONSTANT_Long_info*)ccp)->v1;
			d64 <<= 32;
			d64 += ((CONSTANT_Long_info*)ccp)->v2;
			return genDisAsmStringFromCP_hlp(L"%I64d", d64);
		}
		break;
		case CONSTANT_Double:
		{
			u8 d64 = ((CONSTANT_Long_info*)ccp)->v1;
			d64 <<= 32;
			d64 += ((CONSTANT_Long_info*)ccp)->v2;
			return genDisAsmStringFromCP_hlp(L"%.25g", *(double*)&d64);
		}
		break;
		default:
			return nullptr;
	}
	//return 0;
}

malloc_ptr<wchar_t> genStringFromCP(const JavaConstPool& cpInfo, u2 index, bool typeSpec)
{
	if (index >= cpInfo.size())
		return nullptr;
	
	return genStringFromCP(cpInfo, (u4)-1, cpInfo[index], typeSpec);
}

malloc_ptr<wchar_t> genStringFromCP(CClassFile* clf, u4 index, cp_info* ccp)
{
	return genStringFromCP(clf->get_constant_pool(), index, ccp);
}

malloc_ptr<wchar_t> genStringFromCP(const JavaConstPool& cpInfo, u4 index, cp_info* ccp, bool typeSpec, bool hexIdx)
{
	malloc_ptr<wchar_t> ret = nullptr;

#define LOCAL_MEMCHK(size, citm) \
	const size_t retSize = size + wcslen(citm) + 11; \
	ret = malloc<wchar_t>(retSize); \
	if (nullptr == ret) \
		return nullptr; \
	ret.get()[0] = 0;

#define LOCAL_INITSTR(a, citm) \
	if (index == (u4)-1) \
		ret.get()[0] = 0; \
	else \
		swprintf_s(ret.get(), retSize, hexIdx ? L"%04X: " : L"%04d: ", index); \
	if (typeSpec) \
	{ \
		wcscat_s(ret.get(), retSize, citm); \
		wcscat_s(ret.get(), retSize, L" : "); \
	} \
	wcscat_s(ret.get(), retSize, (a));

	if (nullptr == ccp)
		return false;

	switch (ccp->tag)
	{
	case CONSTANT_Class:
	case CONSTANT_MethodType:
	case CONSTANT_String:
		{
			CONSTANT_Utf8_info* ccl = getUtf8FromV1((CONSTANT_uX_info<u2>*)ccp, cpInfo);
			if (!ccl) 
				return false;
			int size_ccl = 0;
			malloc_ptr<wchar_t> __ccl = preprocessUtf8ToWChar(ccl, &size_ccl);
			if (nullptr == __ccl) 
				return false;
			const wchar_t* const cptagStr = getWCharFromCPTag(ccp->tag);
			LOCAL_MEMCHK(size_ccl, cptagStr);
			LOCAL_INITSTR(__ccl.get(), cptagStr);
		}
		break;
	case CONSTANT_Utf8:
		{
			int size_ccp = 0;
			malloc_ptr<wchar_t> __ccp = preprocessUtf8ToWChar((CONSTANT_Utf8_info*)ccp, &size_ccp);
			if (nullptr == __ccp) 
				return false;
			LOCAL_MEMCHK(size_ccp, CONSTANT_Utf8_STR);
			LOCAL_INITSTR(__ccp.get(), CONSTANT_Utf8_STR);
		}
		break;
	case CONSTANT_Fieldref:
	case CONSTANT_Methodref:
	case CONSTANT_InterfaceMethodref:
		{
			malloc_ptr<wchar_t> natS = getRefString((CONSTANT_uXuX_info<u2, u2>*)ccp, cpInfo);
			if (nullptr == natS) 
				return false;
			const wchar_t* const cptagStr = getWCharFromCPTag(ccp->tag);
			LOCAL_MEMCHK(wcslen(natS.get()), cptagStr);
			LOCAL_INITSTR(natS.get(), cptagStr);
		}
		break;
	case CONSTANT_Integer:
		{
			wchar_t tstr[64];
			tstr[0] = 0;
			swprintf_s(tstr, L"%d (%08X)", ((CONSTANT_Integer_info*)ccp)->v1, ((CONSTANT_Integer_info*)ccp)->v1);
			LOCAL_MEMCHK(wcslen(tstr), CONSTANT_Integer_STR);
			LOCAL_INITSTR(tstr, CONSTANT_Integer_STR);
		}
		break;
	case CONSTANT_Float:
		{
			wchar_t tstr[64];
			tstr[0] = 0;
			swprintf_s(tstr, L"%.25g", *(float*)&((CONSTANT_Float_info*)ccp)->v1);
			LOCAL_MEMCHK(wcslen(tstr), CONSTANT_Float_STR);
			LOCAL_INITSTR(tstr, CONSTANT_Float_STR);
		}
		break;
	case CONSTANT_Long:
		{
			u8 d64 = ((CONSTANT_Long_info*)ccp)->v1;
			d64 <<= 32;
			d64 += ((CONSTANT_Long_info*)ccp)->v2;
			wchar_t tstr[64];
			tstr[0] = 0;
			swprintf_s(tstr, L"%I64d (%016I64X)", d64, d64);
			LOCAL_MEMCHK(wcslen(tstr), CONSTANT_Long_STR);
			LOCAL_INITSTR(tstr, CONSTANT_Long_STR);
		}
		break;
	case CONSTANT_Double:
		{
			u8 d64 = ((CONSTANT_Double_info*)ccp)->v1;
			d64 <<= 32;
			d64 += ((CONSTANT_Double_info*)ccp)->v2;
			wchar_t tstr[64];
			tstr[0] = 0;
			swprintf_s(tstr, L"%.25g", *(double*)&d64);
			LOCAL_MEMCHK(wcslen(tstr), CONSTANT_Double_STR);
			LOCAL_INITSTR(tstr, CONSTANT_Double_STR);
		}
		break;
	case CONSTANT_NameAndType:
		{
			malloc_ptr<wchar_t> natS = getNameAndTypeString((CONSTANT_NameAndType_info*)ccp, cpInfo);
			if (nullptr == natS) 
				return false;
			LOCAL_MEMCHK(wcslen(natS.get()), CONSTANT_NameAndType_STR);
			LOCAL_INITSTR(natS.get(), CONSTANT_NameAndType_STR);
		}
		break;
	case CONSTANT_MethodHandle:
		{
			malloc_ptr<wchar_t> ref_idx = genStringFromCP(cpInfo, ((CONSTANT_MethodHandle_info*)ccp)->v2, false);
			if (nullptr == ref_idx)
				return false;
			const wchar_t* ref_kind = genWCharFromReferenceIndexType((reference_index_type)((CONSTANT_MethodHandle_info*)ccp)->v1);
			if (nullptr == ref_kind)
				return false;
			malloc_ptr<wchar_t> mhs = wcscat_var(ref_kind, L", ", ref_idx.get(), 0);
			if (nullptr == mhs)
				return false;
			LOCAL_MEMCHK(wcslen(mhs.get()), CONSTANT_MethodHandle_STR);
			LOCAL_INITSTR(mhs.get(), CONSTANT_MethodHandle_STR);
		}
		break;
	case CONSTANT_InvokeDynamic:
		{
			malloc_ptr<wchar_t> nat_idx = genStringFromCP(cpInfo, ((CONSTANT_InvokeDynamic_info*)ccp)->v2, false);
			if (nullptr == nat_idx)
				return false;

			wchar_t tstr[64];
			tstr[0] = 0;
			swprintf_s(tstr, L"BootstrapMethod index: %d", ((CONSTANT_InvokeDynamic_info*)ccp)->v1);

			malloc_ptr<wchar_t> ids = wcscat_var(tstr, L", ", nat_idx.get(), 0);
			if (nullptr == ids)
				return false;
			LOCAL_MEMCHK(wcslen(ids.get()), CONSTANT_InvokeDynamic_STR);
			LOCAL_INITSTR(ids.get(), CONSTANT_InvokeDynamic_STR);
		}
		break;
	default:
		return nullptr;
	}
	return ret;
#undef LOCAL_MEMCHK
#undef LOCAL_INITSTR
}

CONSTANT_Utf8_info* getUtf8FromCP(u2 index, const JavaConstPool& cpInfo)
{
	if (index >= cpInfo.size()) 
		return nullptr;
	if (nullptr == cpInfo[index])
		return nullptr;
	if (cpInfo[index]->tag != CONSTANT_Utf8) 
		return nullptr;
	return (CONSTANT_Utf8_info*)cpInfo[index];
}

malloc_ptr<wchar_t> genWStringFromAccessFlags(u2 flags, bool method)
{
	#define STF_1(a) ((flags & a) ? ##a##_STR : L"")
	#define STF_2(a) ((flags & a) ? L" " : L"")
	#define STF(a) STF_1(a), STF_2(a)
	#define CSTF_I(f, a, b) f ? STF_1(a) : STF_1(b), f ? STF_2(a) : STF_2(b)
	#define CSTF(a, b) CSTF_I(method, a, b)

	malloc_ptr<wchar_t> retVal = wcscat_var(
		STF(ACC_PUBLIC), 
		STF(ACC_PRIVATE), 
		STF(ACC_PROTECTED), 
		STF(ACC_STATIC),
		STF(ACC_FINAL), 
		CSTF(ACC_SYNCHRONIZED, ACC_SUPER), 
		CSTF(ACC_BRIDGE, ACC_VOLATILE),
		CSTF(ACC_VARARGS, ACC_TRANSIENT), 
		STF(ACC_NATIVE), 
		STF(ACC_INTERFACE),
		STF(ACC_ABSTRACT), 
		STF(ACC_STRICT), 
		STF(ACC_SYNTHETIC), 
		STF(ACC_ANNOTATION), 
		STF(ACC_ENUM), 
		0);

	#undef STF_1
	#undef STF_2
	#undef STF
	#undef CSTF_I
	#undef CSTF

	if (nullptr == retVal)
		return nullptr;

	size_t sz = wcslen(retVal.get());
	if (sz >= 2) 
		retVal.get()[sz - 1] = 0;

	return retVal;
}

malloc_ptr<wchar_t> preprocessUtf8ToWChar(CONSTANT_Utf8_info* utf8, int* outSize)
{
	if (nullptr == utf8) 
		return nullptr;

	int osize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&utf8->get_bytes()[0], -1, 0, 0);
	if (0 == osize) 
		return nullptr;

	malloc_ptr<wchar_t> retVal = malloc<wchar_t>(osize);
	if (nullptr == retVal) 
		return nullptr;

	int k = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&utf8->get_bytes()[0], -1, retVal.get(), osize);
	if (0 == k)
		return nullptr;

	if (nullptr != outSize) 
		*outSize = k;

	return retVal;
}

malloc_ptr<u1> preprocessWCharToUtf8(const wchar_t* wstr, size_t& outSize)
{
	return malloc_ptr<u1>((u1*)wctmb(wstr, CP_UTF8, &outSize).release());
}

int decodeDescriptorLen(const wchar_t* descriptor)
{
	int curPos = 0;
	int curLen = 0;
	int semiModifier = 0;
	int arrayCounter = 0;
	while (descriptor[curPos])
	{
		switch (descriptor[curPos])
		{
			case 'I':				//"int, "
				curLen += 3 + (semiModifier != 0 ? 2 : 0) + 2*arrayCounter;
				arrayCounter = 0;
				curPos++;
				break;
			case 'J':				//"long, "
			case 'C':				//"char, "
			case 'B':				//"byte, "
			case 'V':				//"void, "
				curLen += 4 + (semiModifier != 0 ? 2 : 0) + 2*arrayCounter;
				arrayCounter = 0;
				curPos++;
				break;
			case 'F':				//"float, "
			case 'S':				//"short, "
				curLen += 5 + (semiModifier != 0 ? 2 : 0) + 2*arrayCounter;
				arrayCounter = 0;
				curPos++;
				break;
			case 'D':				//"double, "
				curLen += 6 + (semiModifier != 0 ? 2 : 0) + 2*arrayCounter;
				arrayCounter = 0;
				curPos++;
				break;
			case 'Z':				//"boolean, "
				curLen += 7 + (semiModifier != 0 ? 2 : 0) + 2*arrayCounter;
				arrayCounter = 0;
				curPos++;
				break;
			case 'L':
				{
					const wchar_t* chevronPos = wcschr(descriptor + curPos, '<');
					const wchar_t* res = wcschr(descriptor + curPos, ';');
					if (nullptr == res) 
						return 0;

					bool chevron = false;
					if ((nullptr != chevronPos) && (chevronPos < res))
					{
						chevron = true;
						res = chevronPos;
						curLen++;
					}

					size_t tlen = res - (descriptor + curPos + 1);
					curLen += (int)(tlen + (semiModifier != 0 ? 2 : 0) + 2*arrayCounter);
					arrayCounter = 0;
					curPos += (int)(tlen + 2);
					if (chevron)
						semiModifier++;
				}
				break;
			case '[':
				arrayCounter++;
				curPos++;
				break;
			case '(':
				semiModifier++;
				curLen++;
				curPos++;
				break;
			case ')':
			case '>':
				semiModifier--;
				curLen++;
				curPos++;
				break;
			case ';':
				curPos++;
				break;
			default:
				return 0;
		}
	}
	return curLen;
}

malloc_ptr<wchar_t> decodeDescriptor(const wchar_t* descriptor, const wchar_t* name, const wchar_t* accflags)
{
	if ((nullptr == descriptor) || (nullptr == name) || (nullptr == accflags)) 
		return nullptr;

	int len = decodeDescriptorLen(descriptor);
	if (0 == len) 
		return nullptr;

	const int nameLen = (int)wcslen(name);
	len += nameLen + 10;	//sic! some extra bytes to avoid heap corruption (3 should be ok)
	const int acclen = (int)wcslen(accflags);
	len += acclen + 2;
	malloc_ptr<wchar_t> retVal = malloc<wchar_t>(len);
	if (nullptr == retVal) 
		return nullptr;

	retVal.get()[0] = 0;

	wcscat_s(retVal.get(), len, accflags);
	if (0 != acclen) 
		wcscat_s(retVal.get(), len, L" ");

	int curPos = 0;
	int semiModifier = 0;
	int arrayCounter = 0;
	wchar_t last = 0;
	int closeBracketPos = -1;
	while (descriptor[curPos])
	{
		switch (descriptor[curPos])
		{
		#define CASETYPE(b, a) \
			case b: \
			wcscat_s(retVal.get(), len, (a)); \
			for (int i = 0; i < arrayCounter; i++) \
				wcscat_s(retVal.get(), len, L"[]"); \
			if (0 != semiModifier) \
				wcscat_s(retVal.get(), len, L", "); \
			arrayCounter = 0; \
			curPos++; \
			last = b; \
			break;
		CASETYPE('I', L"int");
		CASETYPE('J', L"long");
		CASETYPE('C', L"char");
		CASETYPE('B', L"byte");
		CASETYPE('V', L"void");
		CASETYPE('F', L"float");
		CASETYPE('S', L"short");
		CASETYPE('D', L"double");
		CASETYPE('Z', L"boolean");
		#undef CASETYPE
		case 'L':
			{
				const wchar_t* chevronPos = wcschr(descriptor + curPos, '<');
				const wchar_t* res = wcschr(descriptor + curPos, ';');
				if (nullptr == res) 
					return nullptr;

				bool chevron = false;
				if ((nullptr != chevronPos) && (chevronPos < res))
				{
					chevron = true;
					res = chevronPos;
				}

				size_t tlen = res - (descriptor + curPos + 1);
				int cpos = (int)wcslen(retVal.get());
				wcsncat_s(retVal.get(), len, descriptor + curPos + 1, tlen);
				if (chevron)
					wcscat_s(retVal.get(), len, L"<");
				for (int i = 0; i < len - cpos; i++) 
					if (retVal.get()[cpos + i] == L'/') 
						retVal.get()[cpos + i] = L'.';
				for (int i = 0; i < arrayCounter; i++) 
					wcscat_s(retVal.get(), len, L"[]");
				if (0 != semiModifier) 
					wcscat_s(retVal.get(), len, L", ");
				arrayCounter = 0;
				curPos += (int)(tlen + 2);
				last = 'L';

				if (chevron)
					semiModifier++;
			}
			break;
		case '[':
			arrayCounter++;
			curPos++;
			last = '[';
			break;
		case '(':
			wcscat_s(retVal.get(), len, L"(");
			semiModifier++;
			curPos++;
			last = '(';
			break;
		case ')':
			if (last != '(') 
				retVal.get()[wcslen(retVal.get()) - 2] = 0;
			wcscat_s(retVal.get(), len, L")");
			semiModifier--;
			curPos++;
			last = ')';
			closeBracketPos = (int)wcslen(retVal.get());
			break;
		case '>':
			retVal.get()[wcslen(retVal.get()) - 2] = 0;
			wcscat_s(retVal.get(), len, L">");
			semiModifier--;
			curPos++;
			break;
		case ';':
			curPos++;
			break;
		default:
			return nullptr;
		}
	}
	if (0 != nameLen)
	{
		wcscat_s(retVal.get(), len, L" ");
		wcscat_s(retVal.get(), len, name);
	}
	if (-1 != closeBracketPos)
	{
		int curLen = (int)wcslen(retVal.get());
		if (curLen + 1 > len)
			return nullptr;

		retVal.get()[curLen + 1] = 0;
		const int ts = curLen - closeBracketPos;
		malloc_ptr<wchar_t> temp = malloc<wchar_t>(ts);
		if (nullptr == temp) 
			return nullptr;

		memmove(temp.get(), retVal.get() + closeBracketPos, ts*sizeof(wchar_t));
		int t_acclen = ((acclen) ? acclen + 1 : 0);
		memmove(retVal.get() + ts + t_acclen, retVal.get() + t_acclen, (closeBracketPos - t_acclen)*sizeof(wchar_t));
		memmove(retVal.get() + t_acclen, temp.get(), ts*sizeof(wchar_t));
	}

	return retVal;
}

CAttrCode* getCodeAttribute(method_info& method)
{
	//search 'Code' attribute
	auto& attrs = method.get_attributes();
	auto code_it = attrs.begin();
	while (code_it != attrs.end())
	{
		if (typeid((*code_it)->get_attr()) == typeid(CAttrCode&))
			break;
		++code_it;
	}

	if (code_it == attrs.end())
		return nullptr;

	return &(CAttrCode&)(*code_it)->get_attr();
}

JavaAttribute getAttributeType(CONSTANT_Utf8_info* utf8)
{
	malloc_ptr<wchar_t> str = preprocessUtf8ToWChar(utf8);
	if (nullptr == str) 
		return ATTR_UNKNOWN;

	for (int i = 0; i < _countof(attr_tab); i++)
	{
		if (0 == wcscmp(str.get(), attr_tab[i].attr_name)) 
			return attr_tab[i].attr_id;
	}
	return ATTR_UNKNOWN;
}

JavaAttribute getAttributeType(const JavaConstPool& cp, u2 attrIdx)
{
	CONSTANT_Utf8_info* utf8 = getUtf8FromCP(attrIdx, cp);
	if (nullptr == utf8) 
		return ATTR_UNKNOWN;

	return getAttributeType(utf8);
}

bool containsAttribute(const JavaConstPool& cp, std::vector<attribute_info*>& aiv, JavaAttribute attrType)
{
	auto ait = aiv.begin();
	while (ait != aiv.end())
	{
		if (getAttributeType(cp, (*ait)->get_attribute_name_index()) == attrType)
			return true;
		++ait;
	}
	return false;
}

void eraseAttributeByType(const JavaConstPool& cp, std::vector<attribute_info*>& aiv, JavaAttribute attrType)
{
	auto ait = aiv.begin();
	while (ait != aiv.end())
	{
		if (getAttributeType(cp, (*ait)->get_attribute_name_index()) == attrType)
		{
			if (*ait)
				delete *ait;
			aiv.erase(ait);
			eraseAttributeByType(cp, aiv, attrType);
			return;
		}
		++ait;
	}
}

attribute_info* getAttributeByType(const JavaConstPool& cp, std::vector<attribute_info*>& aiv, JavaAttribute attrType)
{
	auto ait = aiv.begin();
	while (ait != aiv.end())
	{
		if (getAttributeType(cp, (*ait)->get_attribute_name_index()) == attrType)
			return *ait;
		++ait;
	}
	return nullptr;
}

CDynamicStructWithCP* frameSwitch(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log)
{
	if (maxLen < 1)
		return 0;
	
	if ((mem[0] >= 0) && (mem[0] <= 63))
		return new (std::nothrow) same_frame(mem, maxLen, cp, log);
	else if ((mem[0] >= 64) && (mem[0] <= 127))
		return new (std::nothrow) same_locals_1_stack_item_frame(mem, maxLen, cp, log);
	else if (mem[0] == 247)
		return new (std::nothrow) same_locals_1_stack_item_frame_extended(mem, maxLen, cp, log);
	else if ((mem[0] >= 248) && (mem[0] <= 250))
		return new (std::nothrow) chop_frame(mem, maxLen, cp, log);
	else if (mem[0] == 251)
		return new (std::nothrow) same_frame_extended(mem, maxLen, cp, log);
 	else if ((mem[0] >= 252) && (mem[0] <= 254))
 		return new (std::nothrow) append_frame(mem, maxLen, cp, log);
 	else if (mem[0] == 255)
 		return new (std::nothrow) full_frame(mem, maxLen, cp, log);
	else
		return 0;
}

const wchar_t* const genWCharFromReferenceIndexType(reference_index_type rit)
{
	switch (rit)
	{
		#define RITCASE(a) case a: return L#a;
		RITCASE(REF_getField);
		RITCASE(REF_getStatic);
		RITCASE(REF_putField);
		RITCASE(REF_putStatic);
		RITCASE(REF_invokeVirtual);
		RITCASE(REF_invokeStatic);
		RITCASE(REF_invokeSpecial);
		RITCASE(REF_newInvokeSpecial);
		RITCASE(REF_invokeInterface);
		#undef RITCASE
	}
	return nullptr;
}

malloc_ptr<wchar_t> genWStringFromVerificationTypeInfo(const JavaConstPool& constant_pool, verification_type_info* vti)
{
	if (nullptr != vti)
	{
		#define VERCASE(a) case a: return malloc_ptr<wchar_t>(_wcsdup(L#a))
		switch (vti->get_value())
		{
			VERCASE(ITEM_Top);
			VERCASE(ITEM_Integer);
			VERCASE(ITEM_Float);
			VERCASE(ITEM_Double);
			VERCASE(ITEM_Long);
			VERCASE(ITEM_Null);
			VERCASE(ITEM_UninitializedThis);
			case ITEM_Object:
				{
					malloc_ptr<wchar_t> cpstr = genStringFromCP(constant_pool, ((Object_variable_info*)vti)->get_value2());
					if (nullptr == cpstr)
						break;
					return wcscat_var(L"ITEM_Object, ", cpstr.get(), 0);
				}
			case ITEM_Uninitialized: 
				{
					wchar_t tmpstr[256];
					swprintf_s(tmpstr, L"ITEM_Uninitialized, offset %04X", ((Uninitialized_variable_info*)vti)->get_value2());
					return malloc_ptr<wchar_t>(_wcsdup(tmpstr));
				}
		}
		#undef VERCASE
	}
	return nullptr;
}

verification_type_info* verificationSwitch(u1* mem, u4 maxLen, const JavaConstPool& constant_pool, CLogger& logger)
{
	LOGFSTART();
	if (maxLen < 1)
		return 0;

	verification_type_info* ret = 0;
	#define VERCASE(a, b) case a: logger.log(5, "%s\n", #b); ret = new (std::nothrow) b(mem, maxLen, constant_pool, logger); break
	switch (mem[0])
	{
		VERCASE(ITEM_Top, Top_variable_info);
		VERCASE(ITEM_Integer, Integer_variable_info);
		VERCASE(ITEM_Float, Float_variable_info);
		VERCASE(ITEM_Double, Double_variable_info);
		VERCASE(ITEM_Long, Long_variable_info);
		VERCASE(ITEM_Null, Null_variable_info);
		VERCASE(ITEM_UninitializedThis, UninitializedThis_variable_info);
		VERCASE(ITEM_Object, Object_variable_info);
		VERCASE(ITEM_Uninitialized, Uninitialized_variable_info);
	default: logger.log(5, "unknown verification type (%02X)\n", mem[0]); break;
	}
	#undef VERCASE
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

bool writeStruct(FILE* file, CDynamicStruct& str)
{
	size_t msize = str.getSize();
	malloc_ptr<u1> mem = malloc<u1>(msize);
	if (nullptr == mem)
		return false;

	if (!str.storeData(mem.get(), (u4)msize))
		return false;

	if (fwrite(mem.get(), 1, msize, file) != msize)
		return false;

	return true;
}

u2 getIdxOfFirstObjectByType(const JavaConstPool& cp, u1 type)
{
	for (u4 i = 1; i < cp.size(); i++)
	{
		if (cp[i] && (cp[i]->tag == type))
			return (u2)i;
	}
	return 0;
}

u2 checkStringInCP(JavaConstPool& cp, const wchar_t* cpStr, CLogger& lgr)
{
	if (nullptr == cpStr)
		return 0;

	for (u4 i = 1; i < cp.size(); i++)
	{
		if (nullptr == cp[i])
			continue;

		if (cp[i]->tag != CONSTANT_Utf8)
			continue;

		malloc_ptr<wchar_t> t = preprocessUtf8ToWChar((CONSTANT_Utf8_info*)cp[i]);
		if (nullptr == t)
			continue;

		if (0 == wcscmp(cpStr, t.get()))
			return (u2)i;
	}
	return 0;
}

u2 getStringIdxFromCP(JavaConstPool& cp, const wchar_t* cpStr, CLogger& lgr)
{
	//locate attrTypeStr in constant pool, if not found then add it to cp
	if (nullptr == cpStr)
		return 0;

	u2 ret = checkStringInCP(cp, cpStr, lgr);
	if (0 == ret)
	{
		//attrTypeStr not found
		CONSTANT_Utf8_info* nu = new (std::nothrow) CONSTANT_Utf8_info(cpStr, lgr);
		if (nullptr == nu)
			return 0;
		
		ret = (u2)cp.size();
		cp.push_back(nu);
	}

	return ret;
}

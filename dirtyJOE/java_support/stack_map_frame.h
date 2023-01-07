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
#pragma once

#include "CDynamicStructWithCP.h"
#include "uXuX_dynStruct.h"

enum verification_type_info_tag : u1
#pragma warning (suppress : 4480)
{
	ITEM_Top = 0,
	ITEM_Integer = 1,
	ITEM_Float = 2,
	ITEM_Double = 3,
	ITEM_Long = 4,
	ITEM_Null = 5,
	ITEM_UninitializedThis = 6,
	ITEM_Object = 7,
	ITEM_Uninitialized = 8
};

typedef uX_dynStruct<u1> verification_type_info;

typedef uX_dynStruct<u1> Top_variable_info;
typedef uX_dynStruct<u1> Integer_variable_info;
typedef uX_dynStruct<u1> Float_variable_info;
typedef uX_dynStruct<u1> Long_variable_info;
typedef uX_dynStruct<u1> Double_variable_info;
typedef uX_dynStruct<u1> Null_variable_info;
typedef uX_dynStruct<u1> UninitializedThis_variable_info;
typedef uXuX_dynStruct<u1, u2> Object_variable_info;
typedef uXuX_dynStruct<u1, u2> Uninitialized_variable_info;

class stack_map_frame : public CDynamicStructWithCP
{
	CDynamicStructWithCP* frame;

public:
	virtual bool storeData(unsigned char* mem, unsigned int maxSize);
	virtual u4 getSize() const;
	stack_map_frame(u1* mem, u4 maxLen, const JavaConstPool& cp, CLogger& log);
	virtual ~stack_map_frame();
};

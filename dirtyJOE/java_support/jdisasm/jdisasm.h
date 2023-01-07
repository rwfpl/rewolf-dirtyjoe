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

#include "../java_class_def.h"
#include "../java_types.h"
#include "../cp_info.h"
#include "jdisasm_enums.h"

extern const _opcode_struct opcodes[];
extern const _opcode_struct jvm_opcode_error;

int jdisasm_get_len(const u1* buf, u4 size, u4 buf_pos);
malloc_ptr<wchar_t> jdisasm_get_str(const u1* buf, u4 size, u4 buf_pos, const JavaConstPool* cpInfo = NULL);
int jdisasm_get_ref(const u1* buf, u4 size);
_jdisasm_opcodes jdisasm_get_type(const u1* buf, u4 size, u4 buf_pos);
_local_var_type jdisasm_get_local_type(const u1* buf, u4 size, u4 buf_pos);
const char* jdisasm_get_specName(u1 op);
bool jdisasm_is_block_end_opcode(_jdisasm_opcodes op);
const wchar_t* const jdisasm_get_pad2(const wchar_t* const opName, size_t tabSize);

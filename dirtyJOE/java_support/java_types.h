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

typedef unsigned __int64 u8;
typedef unsigned int u4;
typedef unsigned short u2;
typedef unsigned char u1;

typedef signed __int64 su8;
typedef signed int su4;
typedef signed short su2;
typedef signed char su1;

#define BE2LEU2(a) ((u2)(((a) >> 8) | ((a) << 8)))
#define BE2LEU4(a) (((a) >> 24) | (((a) >> 8) & 0xFF00) | (((a) << 8) & 0xFF0000) | ((a) << 24))
#define BE2LE(a) (sizeof(a) == 2) ? BE2LEU2(a) : (sizeof(a) == 4) ? BE2LEU4(a) : (a)

enum JavaAttribute
{
	ATTR_UNKNOWN                                 =  0,
	ATTR_CONSTANT_VALUE                          =  1,
	ATTR_CODE                                    =  2,
	ATTR_EXCEPTIONS                              =  3,
	ATTR_INNER_CLASSES                           =  4,
	ATTR_SYNTHETIC                               =  5,
	ATTR_SOURCE_FILE                             =  6,
	ATTR_LINE_NUMBER_TABLE                       =  7,
	ATTR_LOCAL_VARIABLE_TABLE                    =  8,
	ATTR_DEPRECATED                              =  9,
	ATTR_ENCLOSING_METHOD                        = 10,
	ATTR_SIGNATURE                               = 11,
	ATTR_SOURCE_DEBUG_EXTENSION                  = 12,
	ATTR_LOCAL_VARIABLE_TYPE_TABLE               = 13,
	ATTR_RUNTIME_VISIBLE_ANNOTATIONS             = 14,
	ATTR_RUNTIME_INVISIBLE_ANNOTATIONS           = 15,
	ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS   = 16,
	ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS = 17,
	ATTR_ANNOTATION_DEFAULT                      = 18,
	ATTR_STACK_MAP_TABLE                         = 19,
	ATTR_STACK_MAP                               = 20,
	ATTR_RUNTIME_VISIBLE_TYPE_ANNOTATIONS        = 21,
	ATTR_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS      = 22,
	ATTR_BOOTSTRAP_METHODS                       = 23,
	ATTR_METHOD_PARAMETERS                       = 24
};

#ifdef _DEBUG
	#define LOGFSTART() logger.logFunctionStart(__FUNCTION__)
	#define LOGFEND() logger.logFunctionEnd(__FUNCTION__)
#else
	#define LOGFSTART()
	#define LOGFEND()
#endif

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

#define DJ_VERSION_0 1
#define DJ_VERSION_1 8
#define DJ_VERSION_2 0
#define DJ_VERSION_3 575

#ifdef _WIN64
#	define DJ_ARCH "(x64)"
#	define DJ_ARCH_W L"(x64)"
#else
#	ifdef DJ_TOTALCMD
#		define DJ_ARCH "(totalcmd)"
#		define DJ_ARCH_W L"(totalcmd)"
#	else
#		define DJ_ARCH "(x86)"
#		define DJ_ARCH_W L"(x86)"
#	endif
#endif

#define STR2(a) #a
#define STR(a) STR2(a)
#define STR2_W(a) L#a
#define STR_W(a) STR2_W(a)
#define DJ_VERSION_SHORT "v" STR(DJ_VERSION_0) "." STR(DJ_VERSION_1) " (c" STR(DJ_VERSION_3) ") " DJ_ARCH
#define DJ_VERSION_SHORT_W L"v" STR_W(DJ_VERSION_0) L"." STR_W(DJ_VERSION_1) L" (c" STR_W(DJ_VERSION_3) L") " DJ_ARCH_W
#define DJ_VERSION_LONG STR(DJ_VERSION_0) ", " STR(DJ_VERSION_1) ", " STR(DJ_VERSION_2) ", " STR(DJ_VERSION_3)

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
#pragma once

#include <windows.h>
#include "CClassFile.h"

bool RestoreLineNumbers(CClassFile& clf, wchar_t* linesFileName, CLogger& logger, CLogger& loggerLocal);
bool RestoreLocalVariables(CClassFile& clf, CLogger& logger, CLogger& loggerLocal);

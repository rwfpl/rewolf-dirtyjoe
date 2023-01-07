/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
* Contact:
* rewolf@dirty-joe.com
* http://dirty-joe.com
*
* --
*
* see license.txt for more info
*
*/
#include "AppDir.h"
#include "../ui/ui_helpers.h"

CAppDir::CAppDir() : error(false), cd(SetAppDirAsCurrent())
{
	if (nullptr == cd)
		error = true;
}

CAppDir::~CAppDir()
{
	RestoreCurrentDir(cd.get());
}

bool CAppDir::getError() const
{
	return error;
}

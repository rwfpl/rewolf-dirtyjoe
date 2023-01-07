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
#include <Windows.h>
#include <vector>

// link against release version of python even in _DEBUG configuration
#ifdef _DEBUG
#	undef _DEBUG
#	include <python.h>
#	define _DEBUG
#else
#	include <python.h>
#endif

// void printPyObject(PyObject* ob)
// {
// 	if (0 == ob)
// 		return;
// 
// 	PyObject* ob_str = PyObject_Str(ob);
// 	if (ob_str)
// 		printf("%s\n", PyString_AsString(ob_str));
// }
// 
// void printPyErr()
// {
// 	PyObject* type = 0;
// 	PyObject* value = 0;
// 	PyObject* traceback = 0;
// 	PyErr_Fetch(&type, &value, &traceback);
// 	printf("type: "); printPyObject(type);
// 	printf("value: "); printPyObject(value);
// 	printf("traceback: "); printPyObject(traceback);
// 	PyErr_Restore(type, value, traceback);
// 	if (PyErr_Occurred())
// 		PyErr_Print();
// }

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}

void cleanPyObjects(std::vector<PyObject*>& obs)
{
	auto ob = obs.rbegin();
	while (ob != obs.rend())
	{
		Py_DECREF(*ob);
		ob++;
	}
	obs.clear();
}

PyObject* _pPyobStringIO = 0;
PyObject* _pPyGetValFunc = 0;
PyObject* _pPyTruncateFunc = 0;

bool redirectPyErr()
{
	std::vector<PyObject*> obs;

	PyObject* modStringIO = PyImport_ImportModule("cStringIO");
	if (0 == modStringIO)
		return false;
	obs.push_back(modStringIO);

	PyObject* obFuncStringIO = PyObject_GetAttrString(modStringIO, "StringIO");
	if (0 == obFuncStringIO)
	{
		cleanPyObjects(obs);
		return false;
	}
	obs.push_back(obFuncStringIO);

	_pPyobStringIO = PyObject_CallObject(obFuncStringIO, NULL);
	if (0 == _pPyobStringIO)
	{
		cleanPyObjects(obs);
		return false;
	}

	_pPyGetValFunc = PyObject_GetAttrString(_pPyobStringIO, "getvalue");
	if (0 == _pPyGetValFunc)
	{
		cleanPyObjects(obs);
		return false;
	}

	_pPyTruncateFunc = PyObject_GetAttrString(_pPyobStringIO, "truncate");
	if (0 == _pPyTruncateFunc)
	{
		cleanPyObjects(obs);
		return false;
	}

	if (-1 == PySys_SetObject("stderr", _pPyobStringIO))
	{
		cleanPyObjects(obs);
		return false;
	}

	cleanPyObjects(obs);
	return true;
}

void Py_FinalizeLocal()
{
	if (_pPyTruncateFunc)
	{
		Py_DECREF(_pPyTruncateFunc);
		_pPyTruncateFunc = 0;
	}
	if (_pPyGetValFunc)
	{
		Py_DECREF(_pPyGetValFunc);
		_pPyGetValFunc = 0;
	}
	if (_pPyobStringIO)
	{
		Py_DECREF(_pPyobStringIO);
		_pPyobStringIO = 0;
	}
	Py_Finalize();
}

// typedef void (__cdecl *__Py_FatalError)(const char *strError);
// 
// void __cdecl hook_Py_FatalError(const char* strError)
// {
// 	MessageBoxA(0, "Fatal Python Error", strError, MB_ICONERROR);
// }
// 
// bool SetHookOnFatalError(HMODULE hPythonDll)
// {
// #pragma pack(push)
// #pragma pack(1)
// 	struct hookCode
// 	{
// 		BYTE movOp[2];
// 		__Py_FatalError funcAddr;
// 		WORD jmpEaxOp;
// 	} hook;
// #pragma pack(pop)
// 
// 	FARPROC addrFatalError = GetProcAddress(hPythonDll, "Py_FatalError");
// 	if (0 == addrFatalError)
// 		return false;
// 
// 	DWORD oldProtect = 0;
// 	if (!VirtualProtect(addrFatalError, sizeof(hookCode), PAGE_EXECUTE_READWRITE, &oldProtect))
// 		return false;
// 
// #ifndef _WIN64
// 	hook.movOp[0] = 0x90;
// #else
// 	hook.movOp[0] = 0x48;
// #endif
// 	hook.movOp[1] = 0xB8;
// 	hook.funcAddr = hook_Py_FatalError;
// 	hook.jmpEaxOp = 0xE0FF;
// 
// 	memcpy(addrFatalError, &hook, sizeof(hook));
// 
// 	VirtualProtect(addrFatalError, sizeof(hookCode), oldProtect, &oldProtect);
// 	return true;
// }

#ifndef _WIN64
#	pragma comment(linker, "/export:pyjoe_Init@4=pyjoe_Init")
#else
#	pragma comment(linker, "/export:pyjoe_Init")
#endif
extern "C" bool WINAPI pyjoe_Init(HMODULE hPythonDll)
{
// 	if (!SetHookOnFatalError(hPythonDll))
// 		return false;

	Py_NoSiteFlag = 1;
	Py_InitializeEx(0);
	if (!redirectPyErr())
	{
		Py_FinalizeLocal();
		return false;
	}
	return true;
}

#ifndef _WIN64
#	pragma comment(linker, "/export:pyjoe_Deinit@0=pyjoe_Deinit")
#else
#	pragma comment(linker, "/export:pyjoe_Deinit")
#endif
extern "C" void WINAPI pyjoe_Deinit()
{
	Py_FinalizeLocal();
}

#ifndef _WIN64
#	pragma comment(linker, "/export:pyjoe_FreeBuffer@4=pyjoe_FreeBuffer")
#else
#	pragma comment(linker, "/export:pyjoe_FreeBuffer")
#endif
extern "C" void WINAPI pyjoe_FreeBuffer(BYTE* buf)
{
	free(buf);
}

#ifndef _WIN64
#	pragma comment(linker, "/export:pyjoe_GetError@4=pyjoe_GetError")
#else
#	pragma comment(linker, "/export:pyjoe_GetError")
#endif
extern "C" bool WINAPI pyjoe_GetError(char** outBuf)
{
	//
	PyObject* arg = PyTuple_New(1);
	if (0 == arg)
		return false;

	PyObject* tint = PyInt_FromLong(0);
	if (0 == tint)
	{
		Py_DECREF(arg);
		return false;
	}

	if (0 != PyTuple_SetItem(arg, 0, tint))		//SetItem steals reference
	{
		Py_DECREF(tint);
		Py_DECREF(arg);
		return false;
	}

	PyObject* tret = PyObject_CallObject(_pPyTruncateFunc, arg);
	if (0 == tret)
	{
		Py_DECREF(arg);
		return false;
	}
	Py_DECREF(tret);
	Py_DECREF(arg);
	//

	if (PyErr_Occurred())
		PyErr_Print();

	PyObject* obResult = PyObject_CallObject(_pPyGetValFunc, NULL);
	if (0 == obResult)
		return false;

	if (!PyString_Check(obResult))
	{
		Py_DECREF(obResult);
		return false;
	}

	const char* err = PyString_AsString(obResult);
	if (0 == err)
	{
		Py_DECREF(obResult);
		return false;
	}

	*outBuf = _strdup(err);
	Py_DECREF(obResult);

	if (*outBuf)
		return true;
	else
		return false;
}

#ifndef _WIN64
#	pragma comment(linker, "/export:pyjoe_DecryptBuffer@24=pyjoe_DecryptBuffer")
#else
#	pragma comment(linker, "/export:pyjoe_DecryptBuffer")
#endif
extern "C" BYTE* WINAPI pyjoe_DecryptBuffer(char* script, char* funcName, BYTE* inBuf, DWORD inBufSize, DWORD* retSize, wchar_t** errStr)
{
	//just to be sure that there is no error ;>
	PyErr_Clear();

	std::vector<PyObject*> objs;
	*errStr = 0;
	*retSize = 0;

	PyObject* pcode = Py_CompileString(script, "dirtyjoe_internal", Py_file_input);
	if (0 == pcode)
	{
		*errStr = L"Py_CompileString failed.";
		return 0;
	}
	objs.push_back(pcode);

	PyObject* pmodule = PyImport_ExecCodeModule("dirtyjoe_internal", pcode);
	if (0 == pmodule)
	{
		cleanPyObjects(objs);
		*errStr = L"PyImport_ExecCodeModule failed.";
		return 0;
	}
	objs.push_back(pmodule);

	PyObject* pfunc = PyObject_GetAttrString(pmodule, funcName);
	if (0 == pfunc)
	{
		cleanPyObjects(objs);
		*errStr = L"PyObject_GetAttrString failed (probably invalid function name).";
		return 0;
	}
	objs.push_back(pfunc);

	if (0 == PyCallable_Check(pfunc))
	{
		cleanPyObjects(objs);
		*errStr = L"PyCallable_Check failed.";
		return 0;
	}

	PyObject* inBufTuple = PyTuple_New(inBufSize);
	if (0 == inBufTuple)
	{
		cleanPyObjects(objs);
		*errStr = L"PyTuple_New failed.";
		return 0;
	}
	objs.push_back(inBufTuple);

	for (DWORD i = 0; i < inBufSize; i++)
	{
		PyObject* tint = PyInt_FromLong(inBuf[i]);
		if (0 == tint)
		{
			cleanPyObjects(objs);
			*errStr = L"PyInt_FromLong failed.";
			return 0;
		}
		if (0 != PyTuple_SetItem(inBufTuple, i, tint))
		{
			Py_DECREF(tint);
			cleanPyObjects(objs);
			*errStr = L"PyTuple_SetItem failed.";
			return 0;
		}
	}

	PyObject* arg = PyTuple_New(1);
	if (0 == arg)
	{
		cleanPyObjects(objs);
		*errStr = L"PyTuple_New failed.";
		return 0;
	}
	objs.push_back(arg);

	if (0 != PyTuple_SetItem(arg, 0, inBufTuple))
	{
		cleanPyObjects(objs);
		*errStr = L"PyTuple_SetItem failed.";
		return 0;
	}
	//remove inBufTuple from objs, because reference was stolen by PyTuple_SetItem
	objs.erase(objs.end() - 2);

	PyObject* retBuf = PyObject_CallObject(pfunc, arg);
	if (0 == retBuf)
	{
		cleanPyObjects(objs);
		*errStr = L"PyObject_CallObject failed.";
		return 0;
	}
	objs.push_back(retBuf);

	if (0 == PyList_CheckExact(retBuf))
	{
		cleanPyObjects(objs);
		*errStr = L"Object returned from function isn't list (PyList_CheckExact failed).";
		return 0;
	}

	Py_ssize_t retsize = PyList_Size(retBuf);
	if (0 == retsize)
	{
		cleanPyObjects(objs);
		*errStr = L"List returned from function is zero-sized, it isn't supported.";
		return 0;
	}

	BYTE* retVal = (BYTE*)malloc(retsize);
	if (0 == retVal)
	{
		cleanPyObjects(objs);
		*errStr = L"Can't allocate memory for output buffer.";
		return 0;
	}

	for (Py_ssize_t i = 0; i < retsize; i++)
	{
		//PyList_GetItem doesn't increase ref count, so we don't need to decrease it
		PyObject* tint = PyList_GetItem(retBuf, i);
		if (0 == tint)
		{
			free(retVal);
			cleanPyObjects(objs);
			*errStr = L"PyList_GetItem failed.";
			return 0;
		}
		if (0 == PyInt_CheckExact(tint))
		{
			free(retVal);
			cleanPyObjects(objs);
			*errStr = L"List returned from function contains non-int items (PyInt_CheckExact failed).";
			return 0;
		}
		retVal[i] = (BYTE)PyInt_AsLong(tint);
	}

	cleanPyObjects(objs);
	*retSize = (DWORD)retsize;
	return retVal;
}

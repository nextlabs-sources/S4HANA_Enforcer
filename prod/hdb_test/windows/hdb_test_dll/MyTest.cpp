#include "stdafx.h"
#include "MyTest.h"

#define _NO_CVCONST_H
#include <dbghelp.h>
#pragma comment( lib, "dbghelp.lib" )

MyTest MyTest::_instance;
fn_read_test_data_from_file MyTest::_read_test_data_from_file_old = NULL;

MyTest::MyTest()
{
	_hookMgr = NULL;
}


MyTest::~MyTest()
{
	if (_hookMgr != NULL)
	{
		delete _hookMgr;
		_hookMgr = NULL;
	}
}

bool MyTest::Init()
{
	const char* szModName = "hdb_test.exe";
	const char* szFuncName = "read_test_data_from_file";
	DWORD offset = GetFuncOffset(szModName, szFuncName);

	unsigned char* mod = (unsigned char*)LoadLibraryA(szModName);
	if (mod == NULL) {
		OutputDebugStringA("LoadLibrary(\"hdb_test.exe\") failed");
		return false;
	}

	unsigned char* func = mod + offset;

	char szBase[512] = { 0 };
	sprintf(szBase, "LoadLibrary: 0x%X, Offset: 0x%X, Func: 0x%X\n", mod, offset, func);
	OutputDebugStringA(szBase);

	//hook
	_hookMgr = new HookManager();
	_read_test_data_from_file_old = (fn_read_test_data_from_file)(func);
	_hookMgr->AddHookItem((void**)&_read_test_data_from_file_old,
		My_read_test_data_from_file,
		szModName, szFuncName);
	_hookMgr->Hook();

	return true;
}

DWORD MyTest::GetFuncOffset(const char* szModName, const char* szFuncName)
{
	char szBase[512] = { 0 };

	SetLastError(0);

	HANDLE hProcess = GetCurrentProcess();
	BOOL ret = SymInitialize(hProcess, NULL, TRUE);
	if (!ret) {
		DWORD error = GetLastError();
		memset(szBase, 0, strlen(szBase));
		sprintf(szBase, "SymInitialize error: 0x%X\n", error);
		OutputDebugStringA(szBase);
		return 0;
	}

	DWORD64 modBase = SymLoadModuleEx(hProcess, NULL, szModName, NULL, 0, 0, NULL, 0);
	if (!modBase) {
		DWORD error = GetLastError();
		memset(szBase, 0, strlen(szBase));
		sprintf(szBase, "SymLoadModuleEx error: 0x%X\n", error);
		OutputDebugStringA(szBase);
		SymCleanup(hProcess);
		return 0;
	}

	memset(szBase, 0, strlen(szBase));
	sprintf(szBase, "ModBase(%s): 0x%X\n", szModName, modBase);
	OutputDebugStringA(szBase);

	//ret = SymEnumSymbols(hProcess, modBase, szFuncName, mycallback, NULL);
	//if (!ret) {
	//	DWORD error = GetLastError();
	//	memset(szBase, 0, strlen(szBase));
	//	sprintf(szBase, "SymEnumSymbols error: 0x%X\n", error);
	//	OutputDebugStringA(szBase);
	//	SymCleanup(hProcess);
	//	return 0;
	//}

	SYMBOL_INFO symbol = { 0 };
	symbol.SizeOfStruct = sizeof(symbol);
	ret = SymFromName(hProcess, szFuncName, &symbol);
	if (!ret) {
		DWORD error = GetLastError();
		memset(szBase, 0, strlen(szBase));
		sprintf(szBase, "SymFromName error: 0x%X\n", error);
		OutputDebugStringA(szBase);
		SymCleanup(hProcess);
		return 0;
	}

	DWORD offset = (DWORD)(symbol.Address - symbol.ModBase);

	memset(szBase, 0, strlen(szBase));
	sprintf(szBase, "Func(%s): 0x%X, offset: 0x%X\n", szFuncName, symbol.Address, offset);
	OutputDebugStringA(szBase);

	SymCleanup(hProcess);

	return offset;
}

bool MyTest::My_read_test_data_from_file(std::vector<EMSqlItem *> &sqls, const std::string &file)
{
	char szBase[512] = { 0 };
	sprintf(szBase, "My_read_test_data_from_file: %s\n", file.c_str());
	OutputDebugStringA(szBase);

	if (_read_test_data_from_file_old) {
		return _read_test_data_from_file_old(sqls, file);
	}
	return false;
}

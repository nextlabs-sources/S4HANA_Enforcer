// hdb_demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

#define _NO_CVCONST_H
#include <dbghelp.h>
#pragma comment( lib, "dbghelp.lib" )

using namespace std;

int main0();
int main1();

FILE *file = NULL;

BOOL CALLBACK mycallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext) {
	char szSym[3000] = { 0 };
	sprintf(szSym, "0x%X - %s\n", pSymInfo->Address, pSymInfo->Name);
	if (file) {
		fwrite(szSym, strlen(szSym), 1, file);
	}
	printf("%s", szSym);
	return TRUE;
}

int main()
{
	main0();
	return 0;
}

int main0()
{
	file = fopen("out.txt", "wb+");

	SetLastError(0);

	HANDLE hProcess = GetCurrentProcess();
	//const char* szPdbPath = "E:\\usr\\sap\\HE9\\D00\\exe";
	//const char* szPdbPath = "C:\\MySource\\dev\\s4hana_enforcer\\prod\\hdb_new\\windows\\bin";
	//SymInitialize(hProcess, szPdbPath, TRUE);
	BOOL ret = SymInitialize(hProcess, NULL, TRUE);
	if (!ret) {
		DWORD error = GetLastError();
		cout << "SymInitialize error: 0x" << hex << error << endl;
		getchar();
		return -1;
	}

	//const char* szFileName = "E:\\usr\\sap\\HE9\\D00\\exe\\disp+work.exe";
	const char* szFileName = "C:\\MySource\\dev\\s4hana_enforcer\\prod\\hdb_new\\windows\\bin\\hdb_test.exe";
	DWORD64 ModBase = SymLoadModuleEx(hProcess, NULL, szFileName, NULL, 0, 0, NULL, 0);
	if (!ModBase) {
		DWORD error = GetLastError();
		cout << "SymLoadModuleEx error: 0x" << hex << error << endl;
		SymCleanup(hProcess);
		getchar();
		return -1;
	}
	
	char szBase[512] = { 0 };
	sprintf(szBase, "Base: 0x%X\n", ModBase);
	if (file) {
		fwrite(szBase, strlen(szBase), 1, file);
	}
	printf("%s", szBase);

	////ret = SymEnumSymbols(hProcess, ModBase, NULL, mycallback, NULL);
	//ret = SymEnumSymbols(hProcess, ModBase, "*read_test_data_from_file*", mycallback, NULL);
	////ret = SymEnumSymbols(hProcess, ModBase, "ThRqSetCurrentRequest", mycallback, NULL);
	//if (!ret) {
	//	DWORD error = GetLastError();
	//	cout << "SymEnumSymbols error: 0x" << hex << error << endl;
	//	SymCleanup(hProcess);
	//	getchar();
	//	return -1;
	//}

	SYMBOL_INFO symbol = { 0 };
	symbol.SizeOfStruct = sizeof(symbol);
	ret = SymFromName(hProcess, "*read_test_data_from_file*", &symbol);
	//ret = SymFromName(hProcess, "ThRqSetCurrentRequest", &symbol);
	if (!ret) {
		DWORD error = GetLastError();
		cout << "SymFromName error: 0x" << hex << error << endl;
		SymCleanup(hProcess);
		getchar();
		return -1;
	}

	memset(szBase, 0, strlen(szBase));
	sprintf(szBase, "0x%X - %s\n", symbol.Address, symbol.Name);
	if (file) {
		fwrite(szBase, strlen(szBase), 1, file);
	}
	printf("%s", szBase);

	printf("offset: 0x%X\n", symbol.Address - ModBase);

	getchar();

	SymCleanup(hProcess);
	if (file) {
		fclose(file);
		file = NULL;
	}
	return 0;
}

int main1()
{
	file = fopen("out.txt", "wb+");
	//const char* szFileName = "E:\\usr\\sap\\HE9\\D00\\exe\\disp+work.exe";
	const char* szFileName = "C:\\MySource\\dev\\s4hana_enforcer\\prod\\hdb_new\\windows\\bin\\hdb_test.exe";
	HMODULE hmod = LoadLibraryA(szFileName);
	char szBase[512] = { 0 };
	sprintf(szBase, "Base: 0x%x\n", hmod);
	if (file) {
		fwrite(szBase, strlen(szBase), 1, file);
	}
	printf("%s", szBase);
	HANDLE hProcess = GetCurrentProcess();
	//const char* szPdbPath = "E:\\usr\\sap\\HE9\\D00\\exe";
	//const char* szPdbPath = "C:\\MySource\\dev\\s4hana_enforcer\\prod\\hdb_new\\windows\\bin";
	//SymInitialize(hProcess, szPdbPath, TRUE);
	SymInitialize(hProcess, NULL, TRUE);
	//SymEnumSymbols(hProcess, (ULONG64)hmod, NULL, mycallback, NULL);
	SymEnumSymbols(hProcess, (ULONG64)hmod, "*read_test_data_from_file*", mycallback, NULL);
	//SymEnumSymbols(hProcess, (ULONG64)hmod, "disp_work!ThRqSetCurrentRequest", mycallback, NULL);
	getchar();
	SymCleanup(hProcess);
	if (file) {
		fclose(file);
		file = NULL;
	}
	return 0;
}
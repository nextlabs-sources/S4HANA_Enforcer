#pragma once

#include <string>
#include <sal.h>
#include <windows.h>

// PatchPE --add-needed <ImportDll> <TargetPE>
// PatchPE --add-needed <ImportDll> <ImportFunc> <TargetPE>

static const std::string g_kstrKey_Add_Needed = "--add-needed";
static const std::string g_kstrKey_Trace = "--trace";

enum class EMCL_Action
{
    emCLAction_Unknown,
    emCLAction_AddNeeded
};
typedef struct _STUCL_AddNeeded
{
    std::string strImportDll;
    WORD kwHint;
    std::string strImportFunc;
    std::string strTargetPEFile;
}STUCL_AddNeeded, * PSTUCL_AddNeeded;

void PrintCmdHelpe();
bool IsCmdContainsTrace(_In_ const int nArgc, _In_ char* const pszArgv[]);

EMCL_Action GetCmdActionType(_In_ const std::string& kstrCLActionIn);
// If return value is not NULL, invoker need invoke HZFreeResource to free resource
PSTUCL_AddNeeded GetCmdAddNeededInfo(_In_ int nArgc, _In_ char* const pszArgv[]);


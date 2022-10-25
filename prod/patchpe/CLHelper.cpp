#include "CLHelper.h"
#include "MyLog.h"

const std::string g_kstrDefaultImportFunc = "NxlInit";

void PrintCmdHelpe()
{
    HZPrintUserLogA("Command line:\n");
    HZPrintUserLogA("\tPatchPE %s <ImportDll> <TargetPE>\n", g_kstrKey_Add_Needed.c_str());
    HZPrintUserLogA("\tPatchPE %s <ImportDll> <FuncHint, a decimalism number> <ImportFunc> <TargetPE>\n", g_kstrKey_Add_Needed.c_str());
    HZPrintUserLogA("If you want to get more details output info, you can add %s at end of the command line\n", g_kstrKey_Trace.c_str());
    HZPrintUserLogA("\tPatchPE %s <ImportDll> <TargetPE> %s\n", g_kstrKey_Add_Needed.c_str(), g_kstrKey_Trace.c_str());
}
bool IsCmdContainsTrace(_In_ const int nArgc, _In_ char* const pszArgv[])
{
    bool bContains = false;
    if (0 < nArgc)
    {
        bContains = (0 == _stricmp(g_kstrKey_Trace.c_str(), pszArgv[nArgc - 1]));
    }
    return bContains;
}

EMCL_Action GetCmdActionType(_In_ const std::string& kstrCLActionIn)
{
    if (0 == _stricmp(g_kstrKey_Add_Needed.c_str(), kstrCLActionIn.c_str()))
    {
        return EMCL_Action::emCLAction_AddNeeded;
    }
    return EMCL_Action::emCLAction_Unknown;
}

PSTUCL_AddNeeded GetCmdAddNeededInfo(_In_ int nArgc, _In_ char* const pszArgv[])
{
    // PatchPE --add-needed <ImportDll> <TargetPE>
    // PatchPE --add-needed <ImportDll> <FuncHint> <ImportFunc> <TargetPE>
    PSTUCL_AddNeeded pstuAddNeeded = nullptr;
    if (IsCmdContainsTrace(nArgc, pszArgv))
    {
        nArgc -= 1;
    }
    if ((4 == nArgc) || (6 == nArgc))
    {
        EMCL_Action emCLAction = GetCmdActionType(pszArgv[1]);
        if (EMCL_Action::emCLAction_AddNeeded == emCLAction)
        {
            pstuAddNeeded = new STUCL_AddNeeded;
            pstuAddNeeded->strImportDll = pszArgv[2];
            if (4 == nArgc)
            {
                pstuAddNeeded->kwHint = 0;
                pstuAddNeeded->strImportFunc = g_kstrDefaultImportFunc;
                pstuAddNeeded->strTargetPEFile = pszArgv[3];
            }
            else
            {
                // equals 6
                pstuAddNeeded->kwHint = (WORD)(atoi(pszArgv[3]));
                pstuAddNeeded->strImportFunc = pszArgv[4];
                pstuAddNeeded->strTargetPEFile = pszArgv[5];
            }
        }
    }
    else
    {
        HZPrintDebugLogA("The command value count is incorrect");
    }
    return pstuAddNeeded;
}

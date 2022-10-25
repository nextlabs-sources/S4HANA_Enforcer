#include <iostream>

#include "CLHelper.h"
#include "CommonTools.h"
#include "PEIDTInjection.h"
#include "MyLog.h"
#include "PEMgr.h"

using namespace PEAssistant;
static const std::string g_kstrExtendSectionName = ".nxlsec";

int main(int nArgc, char* pszArgv[])
{
    g_bOutputDebugLog = IsCmdContainsTrace(nArgc, pszArgv);
    if (g_bOutputDebugLog)
    {
        HZPrintDebugLogA("Arguments\n");
        for (int i = 0; i < nArgc; ++i)
        {
            HZPrintDebugLogA("\t[%d]:[%s]\n", i, pszArgv[i]);
        }
        HZPrintDebugLogA("Input any key to continue\n");
    }

    bool bRet = false;
    PSTUCL_AddNeeded pstuAddNeeded = GetCmdAddNeededInfo(nArgc, pszArgv);
    if (nullptr == pstuAddNeeded)
    {
        PrintCmdHelpe();
    }
    else
    {
       // Injection with extend IDT
        HZPrintUserLogA("Begin add extend dll:[%s] with function:[%d,%s] to file:[%s]\n",
            pstuAddNeeded->strImportDll.c_str(), pstuAddNeeded->kwHint, pstuAddNeeded->strImportFunc.c_str(), pstuAddNeeded->strTargetPEFile.c_str());
        PEMgr obPESectionExtend(pstuAddNeeded->strTargetPEFile);
        bRet = obPESectionExtend.AddIDTItem(pstuAddNeeded->strImportDll, pstuAddNeeded->kwHint, pstuAddNeeded->strImportFunc, g_kstrExtendSectionName);
        HZPrintUserLogA("End add extend dll:[%s] with function:[%d,%s] to file:[%s] with result:[%s]\n",
            pstuAddNeeded->strImportDll.c_str(), pstuAddNeeded->kwHint, pstuAddNeeded->strImportFunc.c_str(),
            pstuAddNeeded->strTargetPEFile.c_str(), HZBSuccessA(bRet));
        FreeResource(&pstuAddNeeded);
    }
    return bRet ? 0 : -1;
}
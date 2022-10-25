#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <windows.h>
#include <dbghelp.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include "log.h"
#include "tool.h"
#include "CommonTools.h"

struct SAP_SYMBOL
{
    std::string _strFunName;
    std::string _symbolName;
    long long _addr;
};

 static  SAP_SYMBOL g_symbols[] ={ {"ThWpSetCurrentSession",   "",  0},
                        {"ThRqCheckQueues",  "",   0},
                     //  {"DpSesGetLogonNetworkAddress",  "",   0},
                       {"DpSesGetLogonInfo",   "",  0}  };


static unsigned int g_nFunc = 0;

BOOL CALLBACK EnumSymProc( PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
    UNREFERENCED_PARAMETER(SymbolSize);
	UNREFERENCED_PARAMETER(UserContext);

	if (g_nFunc == 3){
        return TRUE;
    }	
        
	for (int iSym = 0; iSym < sizeof(g_symbols)/sizeof(g_symbols[0]); iSym++) {
        SAP_SYMBOL* pSymbol= &g_symbols[iSym];
		if (strcmp(pSymbol->_strFunName.c_str(), pSymInfo->Name) == 0) {
             pSymbol->_addr = pSymInfo->Address;
			 LOG::WriteLog("GetSapSymbolAddr got addr for symbol:%s addr:%lld.", pSymbol->_strFunName.c_str(), pSymbol->_addr);
			g_nFunc++;
		}
	}
	return TRUE;
}

void GetSapSymbolAddr(const char* szDispWork)
{
    STEPS::SetStepStatus(STEP_GET_INTERNAL_SYMBOLS, false);
    //get sap system name from disp+work path
    std::string strSapSystemID, strSapInstanceID;
    GetSapSystemIDFromPath(szDispWork, strSapSystemID, strSapInstanceID);
    LOG::WriteLog("SID=%s, Instance=%s", strSapSystemID.c_str(), strSapInstanceID.c_str() );
    if (strSapSystemID.empty()) {
        LOG::WriteLog("GetSapSymbolAddr can't system ID");
        return;
    }

    //get symbol offset
    HANDLE hProcess = GetCurrentProcess();
	DWORD64 BaseOfDll = 0;
	char *Mask = "*!*";
	BOOL status = FALSE;
	status = SymInitialize(hProcess, NULL, FALSE);
	if (status == FALSE){
        LOG::WriteLog("SymInitialize returned error: %d\n", GetLastError() );
        return;
    }	

		const char  *szImageName = szDispWork;
		DWORD64 dwBaseAddr = 0;
		BaseOfDll = SymLoadModuleEx(hProcess,    // target process 
			NULL,        // handle to image - not used
			szImageName, // name of image file
			NULL,        // name of module - not required
			dwBaseAddr,  // base address - not required
			0,           // size of image - not required
			NULL,        // MODLOAD_DATA used for special cases 
			0);          // flags - not required
		if (BaseOfDll == 0) {
            LOG::WriteLog("SymLoadModuleEx returned error: %d\n", GetLastError() );
			return;
		}
        LOG::WriteLog("SymLoadModuleEx base:%lld\n", BaseOfDll );

		if (!SymEnumSymbols(hProcess,     // Process handle from SymInitialize.
			BaseOfDll,   // Base address of module.
			Mask,        // Name of symbols to match.
			EnumSymProc, // Symbol handler procedure.
			NULL))       // User context.
		{
			// SymEnumSymbols failed
			LOG::WriteLog("SymEnumSymbols failed: %d\n", GetLastError());
		}
		SymCleanup(hProcess);

    //write symbol addr informaiton to file
    std::string strProgDataDir =  CommonFun::GetProgramDataFolder() + strSapSystemID + "\\";
    std::string strSymbolAddr = strProgDataDir + "symaddr";
    LOG::WriteLog("SymbolAddr Path:%s",  strSymbolAddr.c_str() );
    
    if(!PathFileExistsA(strProgDataDir.c_str())){
        int nRes = SHCreateDirectoryExA(NULL, strProgDataDir.c_str(), NULL);
        if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_ALREADY_EXISTS) && (nRes != ERROR_FILE_EXISTS))  {
                strSymbolAddr.clear();
        }
    }
    if(strSymbolAddr.empty()){
        LOG::WriteLog("can't access program data folder:%s",  strProgDataDir.c_str() );
        return;
    }

    if (!strSymbolAddr.empty()){
        FILE* pFileSymbolAddr = fopen(strSymbolAddr.c_str(), "w");
        if (pFileSymbolAddr==NULL){
            LOG::WriteLog("GetSapSymbolAddr open file failed. errno=%d, file=%s", errno, strSymbolAddr.c_str() );
            return;
        }

        bool bSuccess = true;
        char szLine[10*1024];
        for (size_t iSymbol = 0; iSymbol < sizeof(g_symbols)/sizeof(g_symbols[0]); iSymbol++) {
            SAP_SYMBOL* pSymbol= &g_symbols[iSymbol];
            if (pSymbol->_addr==0){
                LOG::WriteLog("Get internal function address(offset):%s failed.", pSymbol->_strFunName.c_str() );
                bSuccess = false;
            } else{
                snprintf(szLine, sizeof(szLine), "%s,%s,%lld\n", pSymbol->_strFunName.c_str(), pSymbol->_symbolName.c_str(), pSymbol->_addr-BaseOfDll );
                size_t len = fwrite(szLine, 1, strlen(szLine), pFileSymbolAddr);
                if ( len != strlen(szLine) ) bSuccess = false;
            }     
        }
    if(bSuccess) STEPS::SetStepStatus(STEP_GET_INTERNAL_SYMBOLS, true);
        fclose(pFileSymbolAddr);
        pFileSymbolAddr = NULL;
    }
}


void GetInternalFunction(const char* szDispWork)
{
     //get symbol address
    LOG::WriteLog("\nBegin get symbol address...");
    GetSapSymbolAddr(szDispWork); 

}



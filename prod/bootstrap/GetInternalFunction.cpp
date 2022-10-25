#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include "log.h"
#include "tool.h"
#include "CommonTools.h"

struct SAP_SYMBOL
{
    std::string _strFunName;
    std::string _symbolName;
    long _addr;
};

void GetSapSymbolAddr(const char* szDispWork)
{
    STEPS::SetStepStatus(STEP_GET_INTERNAL_SYMBOLS, false);

    SAP_SYMBOL symbos[] ={
                  {"ThWpSetCurrentSession", 
                  #ifdef _AIX
                  "ThWpSetCurrentSession__F15DP_SESSION_INFO T",
                   #else 
                   "_Z21ThWpSetCurrentSession15DP_SESSION_INFO",
                   #endif 
                   0},

                  {"ThRqCheckQueues", 
                  #ifdef _AIX
                  ".@20@ThRqCheckQueues__FiPP11REQUEST_BUF t",
                  #else
                  "_ZL15ThRqCheckQueuesiPP11REQUEST_BUF",
                  #endif  
                  0},

                  {"DpSesGetLogonNetworkAddress",
                  #ifdef _AIX 
                  ".DpSesGetLogonNetworkAddress__Fi T",
                  #else 
                  "_Z27DpSesGetLogonNetworkAddressi",
                  #endif 
                   0},


                  {"DpSesGetLogonInfo", 
                  #ifdef _AIX
                  ".DpSesGetLogonInfo__FiP13DP_LOGON_INFO T",
                  #else
                  "_Z17DpSesGetLogonInfoiP13DP_LOGON_INFO",
                  #endif 
                   0}
    };
    const int iSymbolCount = sizeof(symbos)/sizeof(symbos[0]);


     //get sap system name from disp+work path
    std::string strSapSystemID, strSapInstanceID;
    GetSapSystemIDFromPath(szDispWork, strSapSystemID, strSapInstanceID);
    LOG::WriteLog("SID=%s, Instance=%s", strSapSystemID.c_str(), strSapInstanceID.c_str() );
    if (strSapSystemID.empty()) {
        LOG::WriteLog("GetSapSymbolAddr can't system ID");
        return;
    }

    //get symbol offset
    std::string strCmd= "nm ";
    #ifdef _AIX
    strCmd += "-X64 -C " ;
    #endif

    strCmd += szDispWork;
    FILE *fCmd = popen(strCmd.c_str(), "r");
    if (fCmd == nullptr) {
        LOG::WriteLog("GetSapSymbolAddr popen failed.error=%d", errno);
        return;
    }

    bool bSuccess = true;
    char line[1024] = {0};
    while (fgets(line, sizeof(line) - 1, fCmd)) {

        for (int iSymbol = 0; iSymbol < iSymbolCount; iSymbol++)  {
            SAP_SYMBOL* pSymbol = &symbos[iSymbol];
            if (strstr(line, pSymbol->_symbolName.c_str())) {
                   
                //in the new version of SAP, there are many function have GCC attributre ".cold", we need to ignore it
                if(strstr(line,".cold")){
                    continue;
                }

                #ifdef _AIX
                   //for AIX, it is absolute address
                    char *pAddr = strrchr(line, ' ');
                    if (NULL != pAddr) {
                        pSymbol->_addr = strtoul(pAddr+1, NULL,  10);
                        LOG::WriteLog("GetSapSymbolAddr got addr for symbol:%s addr:%ld.", pSymbol->_strFunName.c_str(), pSymbol->_addr);
                    } else {
                        bSuccess = false;
                    }
                #else
                   //for linux it is offset
                    char* pAddr= strtok(line, " ");
                    if (pAddr) {
                        pSymbol->_addr = strtoul(pAddr, NULL, 16);
                        LOG::WriteLog("GetSapSymbolAddr got addr for symbol:%s addr:%ld.", pSymbol->_strFunName.c_str(), pSymbol->_addr);
                    } else {
                        bSuccess = false;
                    }
                #endif 
            }       
        }   
    }
    
    pclose(fCmd);
    fCmd = nullptr;

    //write symbol addr informaiton to file
    const std::string strProgramDataDir = CommonFun::GetProgramDataFolder();
    if (strProgramDataDir.empty()){
        LOG::WriteLog("GetSapSymbolAddr program data folder is empty");
        return;
    }

    std::string systemDataDir = strProgramDataDir + strSapSystemID + "/";
    if (access(systemDataDir.c_str(), 0) != 0)  {
        if (mkdir(systemDataDir.c_str(), S_IRWXU|S_IRWXG |S_IRWXO) )  {
            LOG::WriteLog("GetSapSymbolAddr create system data folder failed, err=%d, path=%s", errno, systemDataDir.c_str() );
            return;
        }
    }

    std::string strSymbolAddr = systemDataDir + "symaddr";
    LOG::WriteLog("symbol path:%s", strSymbolAddr.c_str() );

    FILE *pFileSymbolAddr = fopen(strSymbolAddr.c_str(), "w");
    if (pFileSymbolAddr == NULL) {
        LOG::WriteLog("GetSapSymbolAddr open file failed. errno=%d, file=%s", errno, strSymbolAddr.c_str());
        return;
    }

    char szLine[10 * 1024];
    for (size_t iSymbol = 0; iSymbol < iSymbolCount; iSymbol++)  {
        SAP_SYMBOL *pSymbol = &symbos[iSymbol];
        if (pSymbol->_addr == 0)  {
            LOG::WriteLog("Get internal function address(offset):%s failed.", pSymbol->_strFunName.c_str());
            bSuccess = false;
        }
        else  {
            snprintf(szLine, sizeof(szLine), "%s,%s,%ld\n", pSymbol->_strFunName.c_str(), pSymbol->_symbolName.c_str(), pSymbol->_addr);
            size_t len = fwrite(szLine, 1, strlen(szLine), pFileSymbolAddr);
            if ( len != strlen(szLine) ) bSuccess = false;
        }
    }
    if(bSuccess) STEPS::SetStepStatus(STEP_GET_INTERNAL_SYMBOLS, true);

    fclose(pFileSymbolAddr);
    pFileSymbolAddr = NULL;
}

void GetInternalFunction(const char* szDispWork)
{
     //get symbol address
    LOG::WriteLog("\nBegin get symbol address...");
    GetSapSymbolAddr(szDispWork); 

}
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "log.h"
#include "CommonTools.h"

bool GetSapSystemIDFromPath(const char* szDispWork, std::string& refSapSystemID, std::string& refSapInstanceID);

std::string ParserSAPSID(const char* szDispWork)
{
    //get sap system name from disp+work path
    std::string strSapSystemID, strSapInstanceID;
    GetSapSystemIDFromPath(szDispWork, strSapSystemID, strSapInstanceID);
    if (strSapSystemID.empty()) {
        LOG::WriteLog("ParserSAPSID can't parser system ID");
    }

    return strSapSystemID;
}

bool GetSapSystemIDFromPath(const char* szDispWork, std::string& refSapSystemID, std::string& refSapInstanceID)
{
    #ifdef _WIN32
    const char path_sep='\\';
    const char szSapPathPrefix[] = "\\usr\\sap\\";
    #else
    const char path_sep='/';
    const char szSapPathPrefix[] = "/usr/sap/";
    #endif 


    bool bRet = false;
    do{
        const std::string strDispWork = szDispWork;
        size_t nPosSap = strDispWork.find(szSapPathPrefix);
        if (std::string::npos == nPosSap){
            LOG::WriteLog("GetSapSystemIDFromPath can't find SapPathPrefix from '%s'", szDispWork);
            break;
        }
        
        size_t nPosSlash0 = nPosSap + strlen(szSapPathPrefix);
        size_t nPosSlash1 = strDispWork.find(path_sep, nPosSlash0);
        if (std::string::npos!=nPosSlash1){
            refSapSystemID = strDispWork.substr(nPosSlash0, nPosSlash1-nPosSlash0 );
            size_t nPosSlash2 = strDispWork.find(path_sep , nPosSlash1+1);
            if (std::string::npos!=nPosSlash2) {
                refSapInstanceID = strDispWork.substr(nPosSlash1+1, nPosSlash2-nPosSlash1-1);
                bRet = true;
            }else{
                LOG::WriteLog("GetSapSystemIDFromPath can't find the rear of refSapInstanceID");
            }
           
        }else{
            LOG::WriteLog("GetSapSystemIDFromPath can't find the rear of refSapSystemID");
        }
      
    } while (false);
    
    return bRet;
}


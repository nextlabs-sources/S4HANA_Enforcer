#include <windows.h>
#include "MyDisp+Work.h"
#include "log.h"
#include "tool.h"
#include "PreStartLogProxy.h"
#include "detours.h"
#include <algorithm>

 MyDisp_Work* g_theMyDW=nullptr;

ThRqCheckQueuesFun MyDisp_Work::_ThRqCheckQueues_old=nullptr;
DpSesGetLogonNetworkAddressFun MyDisp_Work::_DpSesGetLogonNetworkAddress_old=nullptr;
DpSesGetLogonInfoFun           MyDisp_Work::_DpSesGetLogonInfo_old=nullptr;

MyDisp_Work::MyDisp_Work()
{
}

MyDisp_Work::~MyDisp_Work()
{

}

SAPRETURN MyDisp_Work::ThRqCheckQueues_new(int nArg,REQUEST_BUF** ppRequestBuf)
{
   SAPRETURN nRet =  _ThRqCheckQueues_old(nArg, ppRequestBuf);
 
   if (ppRequestBuf) {
       REQUEST_BUF*  pBuf = *ppRequestBuf;
       if (pBuf) {
            g_theMyDW->_currentSessionInfo = pBuf->receiver.sessionInfo;  
       }
   }
   
   return nRet;
}

bool MyDisp_Work::Init()
{
    std::map<std::string, SAP_SYMBOL> mapSymbols;
    ReadSymbolAddrFromFile(mapSymbols);

    unsigned long long baseAddrDP = 0;
    if ((baseAddrDP =(unsigned long long)LoadLibraryA("disp+work.exe")) >0)  {
        
         //prepare Detours
        DetourRestoreAfterWith();
	    DetourTransactionBegin();
	    DetourUpdateThread(GetCurrentThread());

        //ThRqCheckQueues
        auto itSymbol = mapSymbols.find("ThRqCheckQueues");
        if (itSymbol != mapSymbols.end())  {
            const unsigned long nOffsetThRqCheckQueues = itSymbol->second._addr;
            g_preLog->WriteLog( "get function offset, disp+work::ThRqCheckQueues=%lld", nOffsetThRqCheckQueues);

            if (nOffsetThRqCheckQueues) {
                _ThRqCheckQueues_old = (ThRqCheckQueuesFun)(baseAddrDP + nOffsetThRqCheckQueues);
                long lResult = DetourAttach((PVOID*)&_ThRqCheckQueues_old, (void*)&MyDisp_Work::ThRqCheckQueues_new); 
                if(0!=lResult){
                    g_preLog->WriteLog("MyDisp_Work::Init Hook ThRqCheckQueues failed"); 
                }  
            }
        }
        else  {
            g_preLog->WriteLog( "get function offset failed for disp+work::ThRqCheckQueues");
        }

         //commit hook
        long lResult = DetourTransactionCommit();
	    g_preLog->WriteLog("MyDisp_Work::Init DetourTransactionCommit result:%ld, %s", lResult,  lResult==0 ? "Success" : "Failed");

       
        //get function address DpSesGetLogonNetworkAddress
        itSymbol = mapSymbols.find("DpSesGetLogonNetworkAddress");
        if (itSymbol != mapSymbols.end()) {
            const unsigned long nOffsetGetNetworkAddress = itSymbol->second._addr;
            g_preLog->WriteLog( "get function offset, disp+work::DpSesGetLogonNetworkAddress=%lld", nOffsetGetNetworkAddress);
            if (nOffsetGetNetworkAddress) {
                _DpSesGetLogonNetworkAddress_old = (DpSesGetLogonNetworkAddressFun)(baseAddrDP + nOffsetGetNetworkAddress);
            }
        }
        else {
            g_preLog->WriteLog( "get function offset failed for disp+work::DpSesGetLogonNetworkAddress");
        }

        //get function address DpSesGetLogonInfo
        itSymbol = mapSymbols.find("DpSesGetLogonInfo");
        if (itSymbol != mapSymbols.end()) {
            const unsigned long nOffsetDpSesGetLogonInfo = itSymbol->second._addr;
            g_preLog->WriteLog( "get function offset, disp+work::DpSesGetLogonInfo=%lld", nOffsetDpSesGetLogonInfo);
            if (nOffsetDpSesGetLogonInfo)
            {
                _DpSesGetLogonInfo_old = (DpSesGetLogonInfoFun)(baseAddrDP + nOffsetDpSesGetLogonInfo);
            }
        }
        else  {
            g_preLog->WriteLog( "get function offset failed for disp+work::DpSesGetLogonInfo");
        }

    }else{
        g_preLog->WriteLog( "load disp+work failed. lasterror=%d", GetLastError() );
    }

    return _ThRqCheckQueues_old != nullptr;
}


void MyDisp_Work::PrintCurrentSession(const std::string& user, const DP_SESSION_INFO& sessionInfo)
{
    theLog->WriteLog(log_info, "PrintCurrentSession, _logonHdl:%d, _logonId:%d, _sessionHdl:%d, user:%s",
    sessionInfo._logonHdl, sessionInfo._logonId, sessionInfo._sessionHdl, user.c_str());
}

 

bool MyDisp_Work::GetCurrentLogonInfo(DAE_LOGON_INFO& myLogonInfo)
{
    bool bRet = false;
    if (MyDisp_Work::_DpSesGetLogonInfo_old) {
        DP_LOGON_INFO logonInfo = {0};
        MyDisp_Work::_DpSesGetLogonInfo_old(GetCurrentSession()._logonHdl, &logonInfo);

        myLogonInfo._client = CommonFun::ToUTF8(std::u16string(logonInfo.client, sizeof(logonInfo.client) / sizeof(logonInfo.client[0])));
         size_t nPos = myLogonInfo._client.find(" ");
        if (nPos!=std::string::npos){
            myLogonInfo._client = myLogonInfo._client.substr(0, nPos);
        }


        myLogonInfo._user = CommonFun::ToUTF8(std::u16string(logonInfo.user, sizeof(logonInfo.user) / sizeof(logonInfo.user[0])));
        nPos = myLogonInfo._user.find(" ");
        if (nPos!=std::string::npos){
            myLogonInfo._user = myLogonInfo._user.substr(0, nPos);
        }
        std::transform(myLogonInfo._user.begin(), myLogonInfo._user.end(), myLogonInfo._user.begin(), ::tolower);
         
        myLogonInfo._tcode = CommonFun::ToUTF8(std::u16string(logonInfo.tcod, sizeof(logonInfo.tcod) / sizeof(logonInfo.tcod[0])));
        nPos = myLogonInfo._tcode.find(" ");
        if (nPos!=std::string::npos){
            myLogonInfo._tcode = myLogonInfo._tcode.substr(0, nPos);
        }

       #if 0
        theLog->WriteLog(log_info, "testx _DpSesGetLogonInfo_old client:%s, name:%s, termin:%s, tcode:%s, program:%s, num_sess:%d"
                                   " logonId:%d, lastactivesession:%d, lastactiveworker:%d, type%d, prio:%d",
                         strU8Client.c_str(), strU8Name.c_str(), strU8Termin.c_str(), strU8Tcod.c_str(), strU8Prog.c_str(),
                         logonInfo.numberOfSessions, logonInfo.logonId, logonInfo.lastActiveSession,
                         logonInfo.lastActiveWorker, logonInfo.logonType, logonInfo.prio);
      #endif
      bRet = true;
    }

    if (MyDisp_Work::_DpSesGetLogonNetworkAddress_old)  {
        NI_NODEADDR *pAddr = MyDisp_Work::_DpSesGetLogonNetworkAddress_old(g_theMyDW->GetCurrentSession()._logonHdl);
        if (NULL == pAddr)  {
            theLog->WriteLog(log_fatal, " _DpSesGetLogonNetworkAddress_old return null");
        }
        else {
            unsigned char* pIP= (unsigned char*)(pAddr->addr64+1);
            char szIp[1024]={0};
            sprintf(szIp, "%d.%d.%d.%d", pIP[4],pIP[5],pIP[6],pIP[7]);

            myLogonInfo._ip = szIp;

            bRet = true;
        }    
    }

    return bRet;
}

const char* MyDisp_Work::GetInternalSessionKey()
{
    const DP_SESSION_INFO& currSessionInfo = GetCurrentSession();
    sprintf_s(_szInternalSessionkey, sizeof(_szInternalSessionkey)-1, "T%d_U%d_M%d", currSessionInfo._logonHdl, currSessionInfo._logonId, currSessionInfo._sessionHdl);
    return _szInternalSessionkey;
}

bool MyDisp_Work::ReadSymbolAddrFromFile(std::map<std::string, SAP_SYMBOL> &mapSymbols)
{
    //get SymbolAddr save file
    const std::string strProgDataFolder = CommonFun::GetProgramDataFolder();
    if (strProgDataFolder.empty())
    {
        g_preLog->WriteLog("ReadSymbolAddrFromFile program data folder is empty");
        return false;
    }

    const char *pSapSystemID = ::getenv("SAPSYSTEMNAME");
    if (NULL == pSapSystemID)
    {
        g_preLog->WriteLog("can get environment SAPSYSTEMNAME");
        return false;
    }

    std::string strSymbolAddr = strProgDataFolder + pSapSystemID + "\\symaddr";
    g_preLog->WriteLog("internal symbol file:%s", strSymbolAddr.c_str());

    FILE *pFileSymbol = fopen(strSymbolAddr.c_str(), "r");
    if (NULL == pFileSymbol)
    {
        g_preLog->WriteLog("ReadSymbolAddrFromFile open symbol file failed:%s, errno=%d", strSymbolAddr.c_str(), errno);
        return false;
    }

    //read
    const int nLineSize = 1024;
    char *pLine = new char[nLineSize];
    while (fgets(pLine, nLineSize - 1, pFileSymbol))
    {
        SAP_SYMBOL sapSym;

        char *pData = pLine;
        char *pTemp = strchr(pData, ',');
        if (pTemp)
        {
            *pTemp = '\0';
            sapSym._strFunName = pData;
            pTemp++;
            pData = pTemp;

            pTemp = strchr(pData, ',');
            if (pTemp)
            {
                *pTemp = '\0';
                sapSym._symbolName = pData;
                pTemp++;
                pData = pTemp;

                sapSym._addr = strtoul(pData, NULL, 10);

                mapSymbols[sapSym._strFunName] = sapSym;

                g_preLog->WriteLog("ReadSymbolAddrFromFile get symbol, fun=%s, addr=%lld", sapSym._strFunName.c_str(), sapSym._addr);
            }
        }
    }

    //free
    delete[] pLine;
    fclose(pFileSymbol);
    pFileSymbol = NULL;

    return true;
}
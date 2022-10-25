#include "MyDisp+Work.h"
#include "log.h"
#include "tool.h"
#
#include "funchook.h"
#include "dlfcn.h"
#include "PreStartLogProxy.h"

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
   
    #ifdef __powerpc64__
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
   #endif 

   return nRet;
}


bool MyDisp_Work::Init()
{
    std::map<std::string, SAP_SYMBOL> mapSymbols;
    ReadSymbolAddrFromFile(mapSymbols);

#ifdef _AIX
   funchook_t *funchook = funchook_create();

   _SetCurrentSessionProxy[0]=GetProcAddrSetCurrentSession();  //function address
   _SetCurrentSessionProxy[1]=GetTOCOfMyDispWork();     //TOC , in r2
   _SetCurrentSessionProxy[2]=0;     //in r11

    _ThWpSetCurrentSession_old = (ThWpSetCurrentSessionFun)(_SetCurrentSessionProxy);
    int rv = funchook_prepare(funchook, (void **)&_ThWpSetCurrentSession_old, (void *)&MyDisp_Work::ThWpSetCurrentSession_new);
    if (0 != rv) {
        _ThWpSetCurrentSession_old = nullptr;
        theLog->WriteLog(log_fatal, "Hook _Z21ThWpSetCurrentSession15DP_SESSION_INFO failed.");
        return false;
    }

    rv = funchook_install(funchook, 0);
    if (rv != 0) {
        _ThWpSetCurrentSession_old = nullptr;
        theLog->WriteLog(log_fatal, "install hook failed _Z21ThWpSetCurrentSession15DP_SESSION_INFO. rv=%d", rv);
        return false;
    }

#else 
    unsigned long baseAddrDP = 0;
    if ((baseAddrDP = CommonFun::get_module_base(-1, "disp+work")) >= 0)  {
        funchook_t *funchook = funchook_create();

        //ThRqCheckQueues
        auto itSymbol = mapSymbols.find("ThRqCheckQueues");
        if (itSymbol != mapSymbols.end())  {
            const unsigned long nOffsetThRqCheckQueues = itSymbol->second._addr;
            g_preLog->WriteLog( "get function offset, disp+work::ThRqCheckQueues=%ld", nOffsetThRqCheckQueues);

            if (nOffsetThRqCheckQueues) {
                _ThRqCheckQueues_old = (ThRqCheckQueuesFun)(baseAddrDP + nOffsetThRqCheckQueues);
                int rv = funchook_prepare(funchook, (void **)&_ThRqCheckQueues_old, (void *)&MyDisp_Work::ThRqCheckQueues_new);
                if (0 != rv) {
                    _ThRqCheckQueues_old = nullptr;
                    g_preLog->WriteLog( "Hook ThRqCheckQueues  failed.");
                    return false;
                }
            }
        }
        else  {
            g_preLog->WriteLog( "get function offset failed for disp+work::ThRqCheckQueues");
        }

        //get function address DpSesGetLogonNetworkAddress
        itSymbol = mapSymbols.find("DpSesGetLogonNetworkAddress");
        if (itSymbol != mapSymbols.end()) {
            const unsigned long nOffsetGetNetworkAddress = itSymbol->second._addr;
            g_preLog->WriteLog( "get function offset, disp+work::DpSesGetLogonNetworkAddress=%ld", nOffsetGetNetworkAddress);
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
            g_preLog->WriteLog( "get function offset, disp+work::DpSesGetLogonInfo=%ld", nOffsetDpSesGetLogonInfo);
            if (nOffsetDpSesGetLogonInfo)
            {
                _DpSesGetLogonInfo_old = (DpSesGetLogonInfoFun)(baseAddrDP + nOffsetDpSesGetLogonInfo);
            }
        }
        else  {
            g_preLog->WriteLog( "get function offset failed for disp+work::DpSesGetLogonInfo");
        }

        int rv = funchook_install(funchook, 0);
        if (rv != 0) {
            _ThRqCheckQueues_old = nullptr;
            g_preLog->WriteLog( "install hook failed _ThRqCheckQueues_old. rv=%d", rv);
            return false;
        }
    }

#endif 

    return _ThRqCheckQueues_old != nullptr;
}

unsigned long MyDisp_Work::GetSymbolOffsetByName(const char* szModuleName, const char* szSymbolName)
{
    unsigned long nOffset = 0;

    //get module file name
     std::string strModuleFullPath;
     FILE* fp = fopen("/proc/self/maps", "r");
     if(fp==nullptr){
         theLog->WriteLog(log_fatal, "MyDisp_Work::GetSymbolOffsetByName open /proc/self/maps failed.");
         return nOffset;
     }

    char line[1024] = {0};
    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, szModuleName))
        {
            const char* szPath = strstr(line, "/");
            if(szPath){
                strModuleFullPath = szPath;
            }
            break;
        }
    }
    fclose(fp);
    fp = nullptr;

    //get symbol offset
    if(!strModuleFullPath.empty()){
        std::string strCmd = "nm " + strModuleFullPath ;
        FILE* fCmd = popen(strCmd.c_str(), "r");
        if(fCmd==nullptr){
            theLog->WriteLog(log_fatal, "MyDisp_Work::GetSymbolOffsetByName popen %s failed.", strCmd.c_str() );
            return nOffset;
        }

        char line[256] = {0};
        while (fgets(line, sizeof(line), fCmd))
        {
            if(strstr(line, szSymbolName)){
                 char* pAddr= strtok(line, " ");
                nOffset = strtoul(pAddr, NULL, 16);
            }
        }
        
        pclose(fCmd);
        fCmd = nullptr;
    }
    
    return nOffset;
}

void MyDisp_Work::PrintCurrentSession(const std::string& user, const DP_SESSION_INFO& sessionInfo)
{
    theLog->WriteLog(log_info, "PrintCurrentSession, _logonHdl:%d, _logonId:%d, _sessionHdl:%d, user:%s",
    sessionInfo._logonHdl, sessionInfo._logonId, sessionInfo._sessionHdl, user.c_str());
}

#ifdef _AIX
unsigned long  MyDisp_Work::GetTOCOfMyDispWork()
{
    unsigned long lTOC = 0x0000000111563588; //the default one 
     void *pDispWorkMod = dlopen("disp+work", RTLD_NOW);
    if (NULL == pDispWorkMod) {
        theLog->WriteLog(log_fatal, "GetTOCOfMyDispWork dlopen disp+work failed. errno=%d", errno);
        return lTOC;
    }

    unsigned long* pFun = (unsigned long*)dlsym(pDispWorkMod, "MtxRwSharedUnlock");/*we use an export API to get the TOC, any export api can work*/
    if (pFun==NULL){
        theLog->WriteLog(log_fatal, "GetTOCOfMyDispWork ddlsym(pOCIModule, ThrCtime) failed. errno=%d", errno);
        return lTOC;
    }

    lTOC = pFun[1];
    theLog->WriteLog(log_info, "GetTOCOfMyDispWork TOC=%p, FunAddr=%p, pFun=%p, mod=%p", lTOC, pFun[0],pFun, pDispWorkMod);
    return lTOC;
}

unsigned long  MyDisp_Work::GetProcAddrSetCurrentSession()
{
    unsigned long lAddr = 0x0000000104961f00;   // the default value

    //get program full path
    std::string strProgName;
    char* pProgName = getenv("DIR_LIBRARY");
    if(NULL==pProgName){
         theLog->WriteLog(log_fatal, "MyDisp_Work::GetProcAddrSetCurrentSession can't get program name");
        return lAddr;
    }
    strProgName = pProgName;
    strProgName += "/disp+work";
    theLog->WriteLog(log_info, "MyDisp_Work::GetProcAddrSetCurrentSession programe name:%s", strProgName.c_str() );

    //get symbol offset
    const static std::string strCmd = "nm -X64 -C " + strProgName + " | grep \"ThWpSetCurrentSession__F15DP_SESSION_INFO T\"";
    FILE *fCmd = popen(strCmd.c_str(), "r");
    if (fCmd == nullptr) {
        theLog->WriteLog(log_fatal, "MyDisp_Work::GetProcAddrSetCurrentSession popen %s failed.", strCmd.c_str());
        return lAddr;
    }

    char line[512] = {0};
    if (fgets(line, sizeof(line) - 1, fCmd)) {
        theLog->WriteLog(log_fatal, "MyDisp_Work::GetProcAddrSetCurrentSession line:%s", line);
        char *pAddr = strrchr(line, ' ');
        if (NULL!=pAddr) {
            lAddr = strtoul(pAddr+1, NULL,  10);
            theLog->WriteLog(log_fatal, "MyDisp_Work::GetProcAddrSetCurrentSession get addr:%p.", lAddr);
        }
    }

    pclose(fCmd);
    fCmd = nullptr;

    return lAddr;
}
#endif 


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


bool MyDisp_Work::ReadSymbolAddrFromFile(std::map<std::string, SAP_SYMBOL>& mapSymbols)
{
    //get policy save file
    const std::string strProgDataFolder = CommonFun::GetProgramDataFolder();
    if (strProgDataFolder.empty()){
        g_preLog->WriteLog("ReadSymbolAddrFromFile program data folder is empty");
        return false;
    }

     const char *pSapSystemID = getenv("SAPSYSTEMNAME");
    if (NULL == pSapSystemID)  {
        g_preLog->WriteLog("can get environment SAPSYSTEMNAME");
        return false;
    }

    std::string strSymbolAddr = strProgDataFolder +  pSapSystemID + "/symaddr";
    g_preLog->WriteLog("internal symbol file:%s",  strSymbolAddr.c_str() );

    FILE *pFileSymbol = fopen(strSymbolAddr.c_str(), "r");
    if (NULL == pFileSymbol)  {
        g_preLog->WriteLog("ReadSymbolAddrFromFile open symbol file failed:%s, errno=%d", strSymbolAddr.c_str(), errno);
        return false;
    }

    //read
    const int nLineSize = 1024;
    char *pLine = new char[nLineSize];
    while (fgets(pLine, nLineSize - 1, pFileSymbol))  {
        SAP_SYMBOL sapSym;

        char *pData = pLine;
        char *pTemp = strchr(pData, ',');
        if (pTemp)  {
            *pTemp = '\0';
            sapSym._strFunName = pData;
            pTemp++;
            pData = pTemp;

            pTemp = strchr(pData, ',');
            if (pTemp)  {
                *pTemp = '\0';
                sapSym._symbolName = pData;
                pTemp++;
                pData = pTemp;

                sapSym._addr = strtoul(pData, NULL, 10);

                mapSymbols[sapSym._strFunName] = sapSym;

                g_preLog->WriteLog("ReadSymbolAddrFromFile get symbol, fun=%s, addr=%ld", sapSym._strFunName.c_str(), sapSym._addr);
            }
        }
    }

    //free
    delete[] pLine;
    fclose(pFileSymbol);
    pFileSymbol = NULL;

    return true;
}

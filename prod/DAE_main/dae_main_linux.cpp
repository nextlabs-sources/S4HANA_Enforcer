#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include "PreStartLog.h"
#include "DAE_main_utili.h"
#ifdef _AIX
#include <sys/procfs.h>
#endif

const char* g_kpchFuncName_DAE_init = "DAE_init";

std::string GetProcNameByPID(int npid)
{
    std::string strProgName;

    #ifdef _AIX
    FILE *pPSInfoFile = NULL;
    do  {
        struct psinfo info;
        char filename[64];
        sprintf(filename, "/proc/%d/psinfo", npid);
        if ((pPSInfoFile = fopen(filename, "r")) == NULL) {
            g_LogDAEMain->WriteLog( "GetProcNameByPID failed to open file:%s", filename);
            break;
        }
        if (fread(&info, sizeof(info), 1, pPSInfoFile) <= 0) {
            g_LogDAEMain->WriteLog( "GetProcNameByPID failed to fread. error=%d", errno);
            break;
        }

        strProgName = info.pr_fname;
    } while (false);

    if (NULL != pPSInfoFile) {
        fclose(pPSInfoFile);
        pPSInfoFile = NULL;
    }
     
    #else
    do
    {
        const char *szlink = "/proc/self/exe";
        char linkname[1024];
        ssize_t r = readlink(szlink, linkname, sizeof(linkname) - 1);
        if (r < 0) {
            break;
        }

        linkname[r] = '\0';
        
        char* pName = strrchr(linkname, '/');
        if (pName) {
            strProgName = pName+1;
        }
        else{
            strProgName = linkname;
        }   
    } while (false);

#endif

      g_LogDAEMain->WriteLog("GetProcNameByPID program name=%s", strProgName.c_str());

    return strProgName;
}


bool IsLoadBySAP()
{
    int nPID = ::getpid();
    std::string strProcName = GetProcNameByPID(nPID);

    if (strcasecmp(strProcName.c_str(), "disp+work")==0){
        return true;
    }
    return false;
}

__attribute__((constructor))
void DAE_main_init()
{
    //init pre-start log
    g_LogDAEMain = new CPreStartLog();

    //get install folder
    const std::string strInstallPath = "/usr/NextLabs/DAE";
     
    //prelog, log before readconfig file
	std::string strProxyLogDir = strInstallPath + "/log/PreStart";
    if (access(strProxyLogDir.c_str(), 0) != 0)  {
        if (mkdir(strProxyLogDir.c_str(), S_IRWXU|S_IRWXG|S_IRWXO))  {
            strProxyLogDir.clear();
        }
    }

    if(!strProxyLogDir.empty()){
        std::string strProxyLogfile = strProxyLogDir + "/";

         //get thread name
         char szCommName[32] = {0};
         int nName = pthread_getname_np(pthread_self(), szCommName, sizeof(szCommName));
         if (nName == 0) {
             strProxyLogfile +=  szCommName;
         }
         else {
             int dwProcID = ::getpid();
             strProxyLogfile += std::to_string(dwProcID);
         }
         strProxyLogfile += ".log";
         
        g_LogDAEMain->Init(strProxyLogfile.c_str());
    }
                      
    if(!IsLoadBySAP()){
        g_LogDAEMain->WriteLog("return for IsLoadBySAP==false");
        return;
    }
    else{
        //get install path
        g_LogDAEMain->WriteLog("Install path:%s", strInstallPath.c_str() );

         //get SID, and check if the folder strInstallPath/data/SID exist
        const char *pSapSystemID = getenv("SAPSYSTEMNAME");
        if (NULL == pSapSystemID)  {
            g_LogDAEMain->WriteLog("can get environment SAPSYSTEMNAME");
            return;
        }

        const std::string strSidFolder = strInstallPath + "/data/" + pSapSystemID + "/";
        g_LogDAEMain->WriteLog("DAE data folder:%s", strSidFolder.c_str() );
        if (access(strSidFolder.c_str(), 0) != 0)  {
            g_LogDAEMain->WriteLog("DAE data folder not exist, this SAP system is not enforced by DAE");
            return;
        }
        
        //load library
        const std::string szLibrarys[]={strInstallPath + "/bin/libtool.so",
                                        strInstallPath + "/bin/libhdb_policymgr.so",
                                        strInstallPath + "/bin/libQueryCloudAZSDKCpp.so"};

        for (int iLib = 0; iLib < sizeof(szLibrarys) / sizeof(szLibrarys[0]); iLib++)  {
            const char *pLib = szLibrarys[iLib].c_str();
            void* hDAE = dlopen(pLib, RTLD_NOW);
            if (NULL == hDAE) {
                g_LogDAEMain->WriteLog("Load %s failed. error=%s", pLib, dlerror() );
            }
            else {
                g_LogDAEMain->WriteLog("Load %s Success", pLib);
            }
        }

        //Load HDB_NEW
        const std::string kpchHdbNew = strInstallPath + "/bin/libhdb_new.so";
        void* hHdbNew = dlopen(kpchHdbNew.c_str(), RTLD_NOW);
        if (NULL == hHdbNew) {
            g_LogDAEMain->WriteLog("Load %s failed. error=%s", kpchHdbNew.c_str(), dlerror() );
        }
        else  {
            g_LogDAEMain->WriteLog("Load %s Success", kpchHdbNew.c_str());

            TyFuncDAE_init pFunc_DAE_init = (TyFuncDAE_init)dlsym(hHdbNew, g_kpchFuncName_DAE_init);
            if (NULL == pFunc_DAE_init){
                g_LogDAEMain->WriteLog("Load DAE function:[%s] failed. error=%s", g_kpchFuncName_DAE_init, dlerror());
            }
            else  {
                pFunc_DAE_init(CPreStartLog::WriteLogFile, strInstallPath.c_str());
            }
        }
    }
}
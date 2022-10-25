#include <windows.h>
#include <Shlobj.h>
#include <shlwapi.h>
#include <string>
#include "PreStartLog.h"
#include "../tool/tool.h"

struct SAP_Param{
    std::string _dispworkFile;
    std::string _dbFile;
    std::string _dispworkMD5;
};

const char* g_kpchFuncName_DAE_init = "DAE_init";
typedef size_t(__cdecl *TyFuncDAE_init)(const TyFuncWriteLog pFuncWriteLog, const char* InstallPath);

std::string GetProcName()
{
    char pProgName[MAX_PATH+1]={0};
    GetModuleFileNameA(NULL, pProgName, MAX_PATH);   
    g_LogDAEMain->WriteLog( "GetProcNameByPID program name=%s", pProgName);
    return pProgName;
}

bool IsLoadBySAP()
{
    std::string strProcName = GetProcName();
    //get file name
    std::string strFileName = strProcName;
    size_t nPos = strProcName.find_last_of('\\');
    if(nPos!=std::string::npos){
        strFileName = strProcName.substr(nPos+1);
    }

    if ( _stricmp(strFileName.c_str(), "disp+work.exe")==0){
        return true;
    }
    return false;
}


bool IsDispWorkChanged(const std::string& strSavedMD5)
{
    if(strSavedMD5.empty()){
        return true;
    }

    std::string strExeFile = GetProcName();
    std::string strMD5Now;
    int nRet = CommonFun::md5_file(strExeFile.c_str(), strMD5Now);
    if((0==nRet) && (!strMD5Now.empty())){
        return strMD5Now.compare(strSavedMD5)!=0;
    }
    else{
        g_LogDAEMain->WriteLog("IsDispWorkChanged calculate md5 failed. file:%s", strExeFile.c_str() );
    }
    return true;
}

void ReadBootstrapParameters(const std::string& strParamFile, SAP_Param& params)
{
    do  {
        //open
        FILE *pParamFile = fopen(strParamFile.c_str(), "r");
        if (NULL == pParamFile) {
            g_LogDAEMain->WriteLog("ReadParameters open file failed:%s, errno=%d", strParamFile.c_str(), errno);
            break;
        }

        //read
        const int nLineSize = 1024;
        char* pLine = new char[nLineSize];
        size_t idx = 0;
        while (fgets(pLine, nLineSize - 1, pParamFile))
        {
             char *find = strchr(pLine, '\n');
             if (find){
                 *find = '\0'; //on linux/aix, fgets will read br into line
             }

            if(0==idx){
                params._dispworkFile = pLine;
            }else if(1==idx){
                params._dbFile = pLine;
            }else if(2==idx){
                params._dispworkMD5 = pLine;
                break;
            }

            idx++;
        }

        //free
        delete[] pLine;
        fclose(pParamFile);
        pParamFile = NULL;

    } while (false);

    return;
}

std::string  GetInstallPath(HINSTANCE hDllHandle)
{
    char szPath[2*MAX_PATH]={0};
    DWORD dwLen = GetModuleFileNameA(hDllHandle, szPath, sizeof(szPath) );
    if(dwLen){
        char* pName = strrchr(szPath, '\\');
        if(pName){
            *pName='\0';  //remove the file name

            char* pBin = strrchr(szPath, '\\');
            if(pBin){
                *pBin= '\0'; //remove bin directory
            }
        }
    }
    return szPath;
}


BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD  nReason, LPVOID    /*Reserved*/ )
 {
  switch ( nReason )
   {
    case DLL_PROCESS_ATTACH:
      {
        //init pre-start log
        g_LogDAEMain = new CPreStartLog();

        //get programdata folder
        std::string strInstallPath = GetInstallPath(hDllHandle);
     
	   std::string strProxyLogDir = strInstallPath + "\\log\\PreStart\\";
        if(!PathFileExistsA(strProxyLogDir.c_str())){
            int nRes = SHCreateDirectoryExA(NULL, strProxyLogDir.c_str(), NULL);
            if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_ALREADY_EXISTS) && (nRes != ERROR_FILE_EXISTS))  {
                strProxyLogDir = "";
            }
        }

        if(!strProxyLogDir.empty()){
             DWORD lPid = GetCurrentProcessId();
             char szPid[32] = {0};
             sprintf_s(szPid, "%ld", lPid);
             std::string strProxyLogfile = strProxyLogDir + "/pre_" + szPid + ".log";
             g_LogDAEMain->Init(strProxyLogfile.c_str());
        }
                      
        if(!IsLoadBySAP()){
              g_LogDAEMain->WriteLog("return for IsLoadBySAP==false");
        }
        else{
            //get install path
            g_LogDAEMain->WriteLog("Install path:%s", strInstallPath.c_str() );

            //get SID, and check if the folder strInstallPath/data/SID exist
            const char *pSapSystemID = ::getenv("SAPSYSTEMNAME");
            if (NULL == pSapSystemID)  {
                g_LogDAEMain->WriteLog("can get environment SAPSYSTEMNAME");
                break;
            }

            const std::string strSidFolder = strInstallPath + "\\data\\" + pSapSystemID + "\\";
            g_LogDAEMain->WriteLog("DAE data folder:%s", strSidFolder.c_str() );
            if (!PathFileExistsA(strSidFolder.c_str()))  {
                g_LogDAEMain->WriteLog("DAE data folder not exist, this SAP system is not enforced by DAE");
                break;
            }

            //read paramfile
            std::string strParamFile = strSidFolder + "param";
            SAP_Param params;
            ReadBootstrapParameters(strParamFile, params);
            if(params._dispworkMD5.empty()){
                g_LogDAEMain->WriteLog("return for can't get md5 content of disp+work from:%s", strParamFile.c_str());
                break;
            }

            //check the md5 of disp+work
            if(IsDispWorkChanged(params._dispworkMD5)){
                g_LogDAEMain->WriteLog("return for check md5 of disp+work failed. disp+work is changed after DAE installed.");
                break;
            }

            //load library
            const std::string szLibrarys[]={ strInstallPath + "\\bin\\boost_date_time-vc140-mt-x64-1_67.dll",
                                        strInstallPath +  "\\bin\\boost_system-vc140-mt-x64-1_67.dll",
                                        strInstallPath + "\\bin\\boost_thread-vc140-mt-x64-1_67.dll",
                                        strInstallPath + "\\bin\\boost_filesystem-vc140-mt-x64-1_67.dll",
                                        strInstallPath + "\\bin\\boost_log-vc140-mt-x64-1_67.dll",
                                        strInstallPath + "\\bin\\libeay32.dll",
                                        strInstallPath + "\\bin\\ssleay32.dll",
                                        strInstallPath + "\\bin\\tool.dll",
                                        strInstallPath + "\\bin\\hdb_policymgr.dll",
                                        strInstallPath + "\\bin\\QueryCloudAZSDKcpp.dll"};
            
            for(int iLib=0; iLib<sizeof(szLibrarys)/sizeof(szLibrarys[0]); iLib++){
                const char* pLib = szLibrarys[iLib].c_str();
                HMODULE hDAE= LoadLibraryA(pLib);
                if(NULL==hDAE){
                        g_LogDAEMain->WriteLog("Load DAE %s failed. error=%d", pLib, GetLastError() );
                    }
                    else{
                        g_LogDAEMain->WriteLog("Load DAE %s Success", pLib );
                    }
            }

            // Load HDB_NEW
            const std::string kpchHdbNew =  strInstallPath + "\\bin\\hdb_new.dll";
            HMODULE hHdbNew= LoadLibraryA(kpchHdbNew.c_str());
            if(NULL==hHdbNew){
                g_LogDAEMain->WriteLog("Load DAE %s failed. error=%d", kpchHdbNew.c_str(), GetLastError() );
            }
            else{
                g_LogDAEMain->WriteLog("Load DAE %s Success", kpchHdbNew.c_str() );

                TyFuncDAE_init pFunc_DAE_init = (TyFuncDAE_init)GetProcAddress(hHdbNew, g_kpchFuncName_DAE_init);
                if (NULL == pFunc_DAE_init)
                {
                    g_LogDAEMain->WriteLog("Load DAE function:[%s] failed. error=%d", g_kpchFuncName_DAE_init, GetLastError() );
                }
                else
                {
                    pFunc_DAE_init(CPreStartLog::WriteLogFile,  strInstallPath.c_str() );
                }
            }
             
        }
          
      }
     break;

    case DLL_PROCESS_DETACH:
      break;
   }

   return TRUE;
 }

 extern "C"{
     __declspec(dllexport)  void NxlInit(){
         return;
     }
 }

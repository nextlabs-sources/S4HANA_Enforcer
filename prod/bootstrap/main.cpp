#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#ifdef _WIN32
    #include <windows.h>
    #include <Shlwapi.h>
    #include <Shlobj.h>
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif 
#include "log.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "tool.h"
#include "CommonTools.h"

#define PARAM_START "--start"
#define PARAM_STOP  "--stop"
#define PARAM_CHECK  "--check"

bool CheckAttach(const char* szDBLib);
void VerifyConfig(const char* szCfg);
void GetInternalFunction(const char* szDispWork);
void AddedDependence(const char* szDispWork, const char* szPatchExe, const char* szDependence);
void RemoveDependence(const char* szDispWork);

void ExecuteCommandWithPopen(const char* szCmd);

void PrintUsage()
{
    printf("\nusage:bootstrap --start {path of disp+work file} {path of db library}\n");
    printf("{path of disp+work file}:  <drive>:\\usr\\sap\\<SID>\\<InstanceID>\\exe\\disp+work.exe\n");
    printf("{path of db library}:   <drive>:\\usr\\sap\\<SID>\\hdbclient\\libSQLDBCHDB.dll\n");

    printf("\nusage:bootstrap --stop {SID}\n");
    printf("{SID}:  <SID>\n");

    printf("\nusage:bootstrap --check\n");
}

#ifdef _WIN32
int strcasecmp(const char* str1, const char* str2)
{
    return _stricmp(str1, str2);
}

int access(const char* filepath, int /*mode*/)
{
    return PathFileExistsA(filepath) ? 0 : -1 ;
}
#endif 

bool CheckParameters(int argc, char** argv)
{
     if(argc<2){
        return false;
    }

    if (strcasecmp(argv[1], PARAM_START)!=0 && strcasecmp(argv[1], PARAM_STOP)!=0 &&
        strcasecmp(argv[1], PARAM_CHECK)!=0 ){
        printf("invalid parameter:%s\n", argv[1]);
        return false;
    }

    if (strcasecmp(argv[1], PARAM_START)==0){
        if (argc<4){
            return false;
        }

        if (access(argv[2], 0)!=0)
        {
            printf("can't find the file name:%s\n", argv[2]);
            return false;
        }

        if (access(argv[3], 0)!=0){
            printf("can't find the file name:%s\n", argv[3]);
            return false;
        }
    }

    if (strcasecmp(argv[1], PARAM_STOP)==0){
        if (argc<3){
            return false;
        }

        const std::string strDAEInstallPath = CommonFun::GetInstallPath();
#ifdef _WIN32
        std::string strSIDFolder = strDAEInstallPath + "\\data\\" + argv[2] + "\\";
#else
        std::string strSIDFolder = strDAEInstallPath + "/data/" + argv[2] + "/";
#endif
        if (access(strSIDFolder.c_str(), 0)!=0)
        {
            printf("can't find the folder, path:%s\n", strSIDFolder.c_str());
            return false;
        }
    }
    
    return true;
}

void SaveParameters(const char* szDispWork, const char* szDBLib)
{
    std::string strSID = ParserSAPSID(szDispWork);
    if(strSID.empty()){
        LOG::WriteLog("SaveParameters error, SID isnot existed!\n");
        return;
    }

    LOG::WriteLog("\nBegin to save parameters for:%s", strSID.c_str());

    const std::string strDAEInstallPath = CommonFun::GetInstallPath();
    #ifdef _WIN32
    std::string strParamFolder = strDAEInstallPath + "\\data\\" + strSID;
	std::string strParamFile = strParamFolder + "\\param";
    #else
    std::string strParamFolder = strDAEInstallPath + "/data/" + strSID;
    std::string strParamFile = strParamFolder + "/param";
    #endif

    if(access(strParamFolder.c_str(), 0)!=0){

        #ifdef _WIN32
            int nRes = SHCreateDirectoryExA(NULL, strParamFolder.c_str(), NULL);
            if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_ALREADY_EXISTS) && (nRes != ERROR_FILE_EXISTS))  {
                    strParamFile.clear();
            }
        #else
            if (mkdir(strParamFolder.c_str(), S_IRWXU|S_IRWXG|S_IRWXO))  {
                strParamFile.clear();
            }
        #endif 
    }

    if(strParamFile.empty()){
        LOG::WriteLog("SaveCmdParameters can't access program data folder:%s",  strParamFolder.c_str() );
        return;
    }

    if (!strParamFile.empty()){
        FILE* pFileParam = fopen(strParamFile.c_str(), "w");
        if (pFileParam==NULL){
            LOG::WriteLog("SaveParameter open file failed. errno=%d, file=%s", errno, strParamFile.c_str() );
            return;
        }

        std::string strDispWork = szDispWork;
        strDispWork.append("\n");   //br in file
        size_t len = fwrite(strDispWork.c_str(), 1, strDispWork.size(), pFileParam);
        if ( len != strDispWork.size() ) LOG::WriteLog("SaveParameter write line failed. errno=%d, file=%s, text=%s", errno, strParamFile.c_str(), szDispWork );
        std::string strDBLib = szDBLib;
        strDBLib.append("\n");  //br in file
        len = fwrite(strDBLib.c_str(), 1, strDBLib.size(), pFileParam);
        if ( len != strDBLib.size() ) LOG::WriteLog("SaveParameter write line failed. errno=%d, file=%s, text=%s", errno, strParamFile.c_str(), szDispWork );
 
        //save the md5 of disp+work
        std::string strmd5;
        int nmd5 = CommonFun::md5_file(szDispWork, strmd5);
        if((0==nmd5) && (!strmd5.empty())){
            strmd5.append("\n");
             len = fwrite(strmd5.c_str(), 1, strmd5.size(), pFileParam);
             if ( len != strmd5.size() ){
                 LOG::WriteLog("SaveParameter write line failed. errno=%d, file=%s, text=%s", errno, strParamFile.c_str(), strmd5.c_str() );
             } 
        }else{
            LOG::WriteLog("get md5 of disp+work failed. nret:%d, md5:%s", nmd5, strmd5.c_str() );
        }

        fclose(pFileParam);
        pFileParam = NULL;
    }
}

std::string LoadParameters(const std::string& refSID)
{
    std::string strRet;
    do{
        if(refSID.empty()){
            LOG::WriteLog("LoadParameters error, SID isnot existed!\n");
            break;
        }

        LOG::WriteLog("\nBegin to load parameters for:%s", refSID.c_str());

        const std::string strDAEInstallPath = CommonFun::GetInstallPath(); 
        #ifdef _WIN32
        std::string strParamFolder = strDAEInstallPath + "\\data\\" + refSID;
        std::string strParamFile = strParamFolder + "\\param";
        #else
        std::string strParamFolder = strDAEInstallPath + "/data/" + refSID;
        std::string strParamFile = strParamFolder + "/param";
        #endif

        //folder isnot existed
        if(access(strParamFolder.c_str(), 0)!=0 ){
            LOG::WriteLog("LoadParameters error, param folder isnot existed, path=%s\n", strParamFolder.c_str());
            break;
        }

        //file isnot existed
        if(access(strParamFile.c_str(), 0)!=0 ){
            LOG::WriteLog("LoadParameters error, param file isnot existed, path=%s\n", strParamFile.c_str());
            break;
        }

        //open
        FILE *pParamFile = fopen(strParamFile.c_str(), "r");
        if (NULL == pParamFile)  {
            LOG::WriteLog("LoadParameters open file failed:%s, errno=%d", strParamFile.c_str(), errno);
            break;
        }

        //read
        const int nLineSize = 1024;
        char *pLine = new char[nLineSize];
        size_t idx = 0;
        while (fgets(pLine, nLineSize - 1, pParamFile))  {
            if(idx==1) {
                char* find = strchr(pLine, '\n');
                if(find) *find = '\0';  //on linux/aix, fgets will read br into line
                strRet = pLine;
                LOG::WriteLog("LoadParameters, got:%s\n", strRet.c_str());
            }
            if(++idx==2) break;   //break after read two lines
        }

        //free
        delete[] pLine;
        fclose(pParamFile);
        pParamFile = NULL;

    } while(false);

    if(strRet.empty()) LOG::WriteLog("LoadParameters get DBLib error");

    return strRet;
}

void ExecuteStart(const char* szCfg, const char* szDispWork, const char* szLibclntsh, const char* szPatchExe, const char* szDependence)
{
    VerifyConfig(szCfg);
    GetInternalFunction(szDispWork);
    AddedDependence(szLibclntsh, szPatchExe, szDependence);
}

void ExecuteStop(const char* szDBLib)
{
    RemoveDependence(szDBLib);
}

void ExecuteCheck(const char* szCfg)
{
    VerifyConfig(szCfg);
}

int main(int argc, char** argv)
{
#ifndef _WIN32
    //be ready for creating folder and file then
    umask(0);
#endif

    if (!CheckParameters(argc, argv)){
        PrintUsage();
        return 1;
    }

    const std::string strAction = argv[1];
    // const std::string strDispWork = argv[2];
    // const std::string strDBLib = argv[3];
	

    const std::string strDAEInstallPath = CommonFun::GetInstallPath(); 
    printf("DAE install path: %s\n",strDAEInstallPath.c_str() );
	
    #ifdef _WIN32
	std::string strCfgFile = strDAEInstallPath + "\\config\\DAE.conf";
    #else
    std::string strCfgFile = strDAEInstallPath + "/config/DAE.conf";
    #endif 


    #ifdef _AIX
    const std::string strPatchExe = strDAEInstallPath + "/bin/patchxcoff";
    #elif defined _WIN32
	std::string strPatchExe = strDAEInstallPath + "\\bin\\patchpe.exe";
    #else
    const std::string strPatchExe =  strDAEInstallPath +  "/bin/patchelf";
    #endif 

    #ifdef _WIN32
    const std::string strDependence = "dae_main.dll";
    #else
    const std::string strDependence = strDAEInstallPath + "/bin/libdae_main.so";
    #endif
    
    //get login name
    #if 0
    char szCurrentUser[LOGIN_NAME_MAX+1];
    int nError;
    if((nError=getlogin_r(szCurrentUser, LOGIN_NAME_MAX))==0){
        printf("Current user:%s\n", szCurrentUser);
    }
    else{
         printf("can't get the login user. error:%d,%s\n", nError, strerror(nError) );
         return 1;
    }
    #endif 


    //printf("disp+work path:%s\n", strDispWork.c_str());
    //printf("DB library path:%s\n", strDBLib.c_str());
    printf("Press Enter key to start...");
    getchar();

    if (strcasecmp(strAction.c_str(), PARAM_START)==0){

        const std::string strDispWork = argv[2];
        const std::string strDBLib = argv[3];
        printf("disp+work path:%s\n", strDispWork.c_str());
        printf("DB library path:%s\n", strDBLib.c_str());

        ExecuteStart(strCfgFile.c_str(),
                     strDispWork.c_str(), 
                     strDBLib.c_str(), 
                     strPatchExe.c_str(),  
                     strDependence.c_str() );

        SaveParameters(strDispWork.c_str(), strDBLib.c_str());
    }
    else if(strcasecmp(strAction.c_str(), PARAM_STOP)==0){
        
		const std::string strSid = argv[2];
        printf("SAP System ID:%s\n", strSid.c_str());
		
        std::string strDBLib = LoadParameters(strSid);
        if(strDBLib.empty())
        {
            LOG::WriteLog("load parameters failed, please check input/paramfile");
            return 1;
        }

        ExecuteStop(strDBLib.c_str());
    }
    else if(strcasecmp(strAction.c_str(), PARAM_CHECK)==0){
          ExecuteCheck(strCfgFile.c_str());
    }
    else{
        PrintUsage();
    }

    STEPS::PrintSummaryResults();
 
    return 0;
    
}
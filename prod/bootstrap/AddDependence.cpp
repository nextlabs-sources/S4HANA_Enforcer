#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#ifndef _AIX
#include <sys/sendfile.h>
#endif 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "log.h"
#include <errno.h>
#include <vector>

#define BACKUP_FILE_APPEND ".pre_NextLabs"

#ifdef _AIX  //AIX doens't implement sendfile
ssize_t sendfile (int __out_fd, int __in_fd, off_t *__offset,  size_t __count)
{
    char buf[4096];
    ssize_t nread;
    ssize_t nSend=0;
    while ((nread = read(__in_fd, buf, sizeof buf)) > 0) {

        ssize_t nwritten = write(__out_fd, buf, nread);
        if (nwritten!=nread){
            LOG::WriteLog("sendfile failed. nread=%ld, nwrite:%ld", nread, nwritten);
            nSend = -1;
            break;
        }   
        nSend += nwritten;
    }
    return nSend;
}
#endif 

bool CopyFile(const char* szOriDispWork, const char* szDestDispWork)
{
    int ori_fd= -1;
    int back_fd= -1;
    bool bBackup = false;  
    do
    {
        //first back up file
        int ori_fd = open(szOriDispWork, O_RDONLY);
        if (-1 == ori_fd)  {
            LOG::WriteLog("CopyFile open source file failed. file:%s err:%d,%s",szOriDispWork,  errno, strerror(errno));
            break;
        }
        //get file permisson
        struct stat ori_stat = {0};
        int nStat = fstat(ori_fd, &ori_stat);

        int back_fd = open(szDestDispWork, O_CREAT | O_WRONLY | O_TRUNC, ori_stat.st_mode/*keep the file permisson as the original one*/);
        if (-1 == back_fd)  {
            LOG::WriteLog("CopyFile open dest file failed. file:%s err:%d,%s",szDestDispWork,  errno, strerror(errno));
            break;
        }

        //copy file
        int nSend = sendfile (back_fd, ori_fd, NULL, ori_stat.st_size);
        if (-1 == nSend)  {
            LOG::WriteLog("CopyFile failed when call sendfile err:%d,%s",  errno, strerror(errno));
            break;
        }
        bBackup = true;

    } while (false);
    
  
    //free
    if (-1!=ori_fd){
        close(ori_fd);
        ori_fd = -1;
    }
    if (-1!=back_fd){
        close(back_fd);
        back_fd = -1;
    }

   // LOG::WriteLog("CopyFile result:%s", bBackup ? "Success" : "Failed" ); 
    return bBackup;

}

#if 0
void ExecuteCommandWithPopen(const char* szCmd)
{
    FILE *fCmd = popen(szCmd, "r");
    if (fCmd == nullptr) {
        LOG::WriteLog("popen failed.cmd=%s error=%d",szCmd, errno);
        return;
    }
    char line[1024] = {0};
    while (fgets(line, sizeof(line) - 1, fCmd)) {
        LOG::WriteLog(line);
    }
    fclose(fCmd);
    fCmd = NULL;
}
#endif

void ExecuteCommandWithPopen(const char* szCmd, std::string& strOutput)
{
    FILE *fCmd = popen(szCmd, "r");
    if (fCmd == nullptr) {
        LOG::WriteLog("popen failed.cmd=%s error=%d",szCmd, errno);
        return;
    }
    char line[10240] = {0};
    while (fgets(line, sizeof(line) - 1, fCmd)) {
        strOutput.append(line);
    }
    //LOG::WriteLog(strOutput.c_str());
    fclose(fCmd);
    fCmd = NULL;
}

std::string GetLinkName(const char* szSymName)
{
    std::string strLinkName;
    char linkname[1024];
    ssize_t r = readlink(szSymName, linkname, sizeof(linkname) - 1);
    if (r > 0) {
        linkname[r] = '\0';
        strLinkName = linkname;
        
        if (linkname[0]!='/'){
            const char* pFileName = strrchr(szSymName, '/');
            if (NULL!=pFileName){
                std::string strDir(szSymName, strlen(szSymName)-strlen(pFileName) + 1);
                strLinkName = strDir + linkname;
            }
        }
    }
    return strLinkName;
}

bool CheckDependence(const char* szDBLib)
{
    bool bRet = false;

    std::string strDBLib = szDBLib;
    std::string strCmd = "ldd " + strDBLib;

    std::string strPopenResult;
    ExecuteCommandWithPopen(strCmd.c_str(), strPopenResult);
    if(!strPopenResult.empty()) {
        bRet = strPopenResult.find("libdae_main.so") != std::string::npos;
        LOG::WriteLog("CheckDependence%s found SAP DAE dependence!", bRet ? "" : " NOT");
        //LOG::WriteLog("CheckDependence executes command:%s, output:\n%s", strCmd.c_str(), strPopenResult.c_str());
    } else {
        LOG::WriteLog("CheckDependence executes command:%s, failed", strCmd.c_str());
    }

    return bRet;
}

void AddedDependence(const char* szDBLib, const char* szPatchExe, const char* szDependence)
{
    LOG::WriteLog("\nBegin added dependence to: %s", szDBLib);

    STEPS::SetStepStatus(STEP_ATTACH_DAE, false);

    std::string strDBLib = szDBLib;

    //check the file exist
    if (access(strDBLib.c_str(), 0)!=0){ 
        LOG::WriteLog("can't find the file:%s", strDBLib.c_str() );
        return;
    }

    //check if it is a symbol link, if yes. get the really file name
    std::string strLinkName = GetLinkName(szDBLib);
    if (!strLinkName.empty()){
          LOG::WriteLog("get link target name:%s", strLinkName.c_str() );
          strDBLib = strLinkName;
    }

    bool bAttached = CheckDependence(szDBLib);
    if(bAttached){
        LOG::WriteLog("AddedDependence found DBLib has been added dependence");
        STEPS::SetStepStatus(STEP_ATTACH_DAE, true);
        return;
    } //if atteched before, do nothing with success status

     //check RW permission.
    if(access(strDBLib.c_str(), R_OK|W_OK)){
        LOG::WriteLog("%s didn't have R/W permissoin on:%s", __func__, strDBLib.c_str() );
        STEPS::SetStepStatus(STEP_ATTACH_DAE, false);
        return;
    }
    
    //backup the file
    const std::string strDBLibBack = strDBLib + BACKUP_FILE_APPEND;
    if (access(strDBLibBack.c_str(), 0)==0){ //backup file already exist, delete firstly
		int nRM = remove(strDBLibBack.c_str());
        LOG::WriteLog("deleted backup file:%s, result:%s", strDBLibBack.c_str(), nRM==0 ? "Success" : "Failed");
    }

    bool bBackup  = CopyFile(strDBLib.c_str(), strDBLibBack.c_str() );
    LOG::WriteLog("backup dblib to:%s result:%s", strDBLibBack.c_str(), bBackup ? "Success" : "Failed");    

    //modify file
    if (bBackup){
        std::string strCmd = szPatchExe;
        strCmd += " --add-needed ";
        strCmd += szDependence;
        strCmd += " ";
        strCmd += strDBLib;

        FILE *fCmd = popen(strCmd.c_str(), "r");
        if (fCmd == nullptr)  {
            LOG::WriteLog("popen command failed. cmd:%s, error=%d,%s", strCmd.c_str(), errno, strerror(errno) );
            return;
        }

        char line[1024] = {0};
        while (fgets(line, sizeof(line) - 1, fCmd)) {
            line[strlen(line)-1] = '\0'; //remove end '\n'
            LOG::WriteLog(line);
        }
        STEPS::SetStepStatus(STEP_ATTACH_DAE, true);
        pclose(fCmd);
        fCmd = nullptr;
    }
}


void RemoveDependence(const char* szDBLib)
{
    LOG::WriteLog("\nBegin remove dependence from: %s", szDBLib );
    STEPS::SetStepStatus(STEP_DETACH_DAE, false);

    std::string strDBLib = szDBLib;
    //check if it is a symbol link, if yes. get the really file name
    std::string strLinkName = GetLinkName(szDBLib);
    if (!strLinkName.empty()){
          LOG::WriteLog("get link target name:%s", strLinkName.c_str() );
          strDBLib = strLinkName;
    }
    const std::string strDBLibBack = strDBLib + BACKUP_FILE_APPEND;

    bool bAttached = CheckDependence(szDBLib);
    if(!bAttached){ //if atteched before, delete backup file with success status
        LOG::WriteLog("DBLib hasn't been added dependence");
        if(access(strDBLibBack.c_str(), 0)==0){
            int nRM = remove(strDBLibBack.c_str());
            LOG::WriteLog("delete %s, result:%s", strDBLibBack.c_str(),  nRM==0 ? "Success" : "Failed");
        }
        STEPS::SetStepStatus(STEP_DETACH_DAE, true);
        return;
    }

    //rollback original dblib, delete backup file
    bool bCopy  = CopyFile( strDBLibBack.c_str(), strDBLib.c_str() );
    LOG::WriteLog("Remove dependence from result:%s",   bCopy ? "Success" : "Failed");
    if (bCopy){
        int nRM = remove(strDBLibBack.c_str());
        LOG::WriteLog("delete %s, result:%s", strDBLibBack.c_str(),  nRM==0 ? "Success" : "Failed");
        if(nRM==0) STEPS::SetStepStatus(STEP_DETACH_DAE, true);
    }
}
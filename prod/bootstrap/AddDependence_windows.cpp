#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <windows.h>
#include <Shlwapi.h>

#include "log.h"
#include "CommonTools.h"
#include "CommonTools_windows.h"
#include "tool.h"

#pragma comment(lib, "Shlwapi.lib")

#define BACKUP_FILE_APPEND ".pre_NextLabs"
const std::string daemain = "dae_main.dll";

/*Convert VA to File Offset */
DWORD Rva2Offset(DWORD rva, PIMAGE_SECTION_HEADER psh, PIMAGE_NT_HEADERS pnt, DWORD bs)
{
    if (rva == 0) return rva;

    size_t i = 0;
    PIMAGE_SECTION_HEADER pSeh = psh;
    for (i = 0; i < pnt->FileHeader.NumberOfSections; i++)
    {
        if (rva >= pSeh->VirtualAddress && rva < pSeh->VirtualAddress +
            ((pSeh->Misc.VirtualSize - 1) / bs + 1) * bs)
        {
            break;
        }
        pSeh++;
    }
    return (rva - pSeh->VirtualAddress + pSeh->PointerToRawData);
}

bool CheckDependence(const char* szDBLib)
{
    bool result = false;

    HANDLE handle = CreateFileA(szDBLib, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD byteread, size = GetFileSize(handle, NULL);
    PVOID virtualpointer = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
    ReadFile(handle, virtualpointer, size, &byteread, NULL);
    CloseHandle(handle);
    // Get pointer to NT header
    PIMAGE_NT_HEADERS           ntheaders = (PIMAGE_NT_HEADERS)(PCHAR(virtualpointer) + PIMAGE_DOS_HEADER(virtualpointer)->e_lfanew);
    PIMAGE_SECTION_HEADER       pSech = IMAGE_FIRST_SECTION(ntheaders);//Pointer to first section header
    PIMAGE_IMPORT_DESCRIPTOR    pImportDescriptor; //Pointer to import descriptor 
    DWORD alignment = ntheaders->OptionalHeader.SectionAlignment;

    if (ntheaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0)/*if size of the table is 0 - Import Table does not exist */
    {
        //PIMAGE_IMPORT_DESCRIPTOR point to first element of Import Descriptor array 
        pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)virtualpointer + 
            Rva2Offset(ntheaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, pSech, ntheaders, alignment));
        while (pImportDescriptor->Name != NULL)
        {
            LPSTR libname = (PCHAR)((DWORD_PTR)virtualpointer + Rva2Offset(pImportDescriptor->Name, pSech, ntheaders, alignment));
            LOG::WriteLog("%s", libname);
            if (!daemain.compare(libname)) {
                LOG::WriteLog("found SAP DAE dependence!");
                result = true;
                break;
            }
            pImportDescriptor++;
        }
    }
    else
    {
        LOG::WriteLog("Not found any Import Table!");
    }

    if (virtualpointer)
    {
        VirtualFree(virtualpointer, size, MEM_DECOMMIT);
    }

    return result;
}

void AddedDependence(const char* szDBLib, const char* szPatchExe, const char* szDependence)
{
    STEPS::SetStepStatus(STEP_ATTACH_DAE, false);
    LOG::WriteLog("\nBegin added dependence to: %s, patchExe:[%s], szDependence:[%s]", szDBLib, szPatchExe, szDependence);

    std::string strDBLib = szDBLib;
    // Check if it is a symbol link, if yes. get the really file name
    // For windows, ignore link file, if we want to get link file real path, we need use interface IShellLink
    // Ignore link file in windows platform

    bool bAttached = CheckDependence(szDBLib);
    if(bAttached){
        LOG::WriteLog("DBLib has been added dependence");
        STEPS::SetStepStatus(STEP_ATTACH_DAE, true);
        return;
    } //if atteched before, do nothing with success status

    // check and delete backup file firstly
    const std::string strDBLibBack = strDBLib + BACKUP_FILE_APPEND;
    if (PathFileExistsA(strDBLibBack.c_str())) { //backup file already exist
        if(remove(strDBLibBack.c_str())){
            LOG::WriteLog("delete %s result:Failed", strDBLibBack.c_str());
            return;
        }
        LOG::WriteLog("delete %s result:Success", strDBLibBack.c_str());
    }

    //backup the file
    BOOL bBackup = ::CopyFileA(strDBLib.c_str(), strDBLibBack.c_str(), true);
    LOG::WriteLog("backup database library to:%s result:%s", strDBLibBack.c_str(), bBackup ? "Success" : "Failed");

    //modify file
    if (bBackup) {
        std::string strPatchCmd = GetFormatString("\"%s\" --add-needed \"%s\" \"%s\"", szPatchExe, szDependence, szDBLib);
#pragma warning(push)
#pragma warning(disable: 4245)  // conversion from 'int' to 'const DWORD', signed/unsigned mismatch
        DWORD dwExitCode = ExecuteCommand(strPatchCmd, -1, INFINITE);
#pragma warning(pop)        
        if (0 == dwExitCode)
        {
            std::string strDependenceScript = CommonFun::GetInstallPath() + "\\Bin\\Init.ps1";
            bool bExecPS = ExecutePSScript(strDependenceScript, "");
            if(bExecPS) STEPS::SetStepStatus(STEP_ATTACH_DAE, true);
        }
        else
        {
            LOG::WriteLog("execute command failed. cmd:%s, error=0x%x", strPatchCmd.c_str(), ::GetLastError());
            int nRM = remove(strDBLibBack.c_str());
            LOG::WriteLog("remove backup file %s", strDBLibBack.c_str(), nRM == 0 ? "Success" : "Failed");
            return;
        }
    }
}

void RemoveDependence(const char* szDBLib)
{
    STEPS::SetStepStatus(STEP_DETACH_DAE, false);
    LOG::WriteLog("\nBegin remove dependence from: %s", szDBLib);

    std::string strDBLib = szDBLib;

    // Check if it is a symbol link, if yes. get the really file name
    // For windows, ignore link file, if we want to get link file real path, we need use interface IShellLink
    // Ignore link file in windows platform

    const std::string strDBLibBack = strDBLib + BACKUP_FILE_APPEND;

    bool bAttached = CheckDependence(szDBLib);
    if(!bAttached){ //if NOT atteched before, delete backup file with success status
        LOG::WriteLog("DBLib hasn't been added dependence");
        if(PathFileExistsA(strDBLibBack.c_str())){
            if(remove(strDBLibBack.c_str())!=0){
                LOG::WriteLog("delete %s result:Failed", strDBLibBack.c_str());
                return;
            }
            LOG::WriteLog("delete %s result:Success", strDBLibBack.c_str());
        }
        STEPS::SetStepStatus(STEP_DETACH_DAE, true);
        return;
    }

    bool bCopy = CopyFileA(strDBLibBack.c_str(), strDBLib.c_str(), false);
    LOG::WriteLog("Remove dependence result:%s", bCopy ? "Success" : "Failed");
    if (bCopy) {
        int nRM = remove(strDBLibBack.c_str());
        LOG::WriteLog("delete %s, result:%s", strDBLibBack.c_str(), nRM == 0 ? "Success" : "Failed");
        if(nRM==0) STEPS::SetStepStatus(STEP_DETACH_DAE, true);
    }
}
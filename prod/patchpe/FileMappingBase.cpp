#include "FileMappingBase.h"
#include "MyLog.h"
#include <windows.h>
#include <Shlwapi.h>
#include <string>
#include "CommonTools.h"
#include "PETools.h"

#pragma comment(lib, "Shlwapi.lib")

namespace PEAssistant
{

FileMappingBase::FileMappingBase(_In_ const std::string& kstrPEFilePath) : m_kstrPEFilePath(kstrPEFilePath)
{
    ResetMappingInfo();
}
FileMappingBase::~FileMappingBase()
{
    StopMapping(false);
}

bool FileMappingBase::StartMapping(_In_ const DWORD kdwExtendSize)
{
    bool bStartMapping = GetMappingFlag();
    if (bStartMapping)
    {
        HZPrintDebugLogA("Current file:[%s] already mapped, no need mapping again. If you want remapping, you need stop the current mapping first\n", m_kstrPEFilePath.c_str());
    }
    else
    {
        ResetMappingInfo();
        HZPrintDebugLogA("Begin mapping file:[%s]\n", m_kstrPEFilePath.c_str());

        if (PathFileExistsA(m_kstrPEFilePath.c_str()))
        {
            // Open file with file mapping
            ::SetLastError(ERROR_SUCCESS);
            HANDLE hFile = CreateFileA(m_kstrPEFilePath.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
            );
            if (HZIsValidHandle(hFile))
            {
                // Ignore large file which above 2G
                DWORD dwFileSize = GetFileSize(hFile, nullptr);
                DWORD dwMappingSize = dwFileSize + kdwExtendSize;
                ::SetLastError(ERROR_SUCCESS);
                HANDLE hMapFile = CreateFileMappingW(hFile, nullptr, PAGE_READWRITE, 0, dwMappingSize, nullptr);
                if (HZIsValidHandle(hMapFile))
                {
                    // Analysis PE headers
                    ::SetLastError(ERROR_SUCCESS);
                    BYTE* pFileBuf = (BYTE*)MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
                    if (nullptr == pFileBuf)
                    {
                        HZPrintDebugLogA("Map file:[%s] view failed with last error:[0x%x]", m_kstrPEFilePath.c_str(), ::GetLastError());
                        HZCloseHandle(&hMapFile);
                        HZCloseHandle(&hFile);
                    }
                    else
                    {
                        m_dwFileSize = dwFileSize;
                        m_dwMappingSize = dwMappingSize;
                        m_hFile = hFile;
                        m_hMapFile = hMapFile;
                        m_pbyFileBase = pFileBuf;
                        bStartMapping = true;
                        SetMappingFlag(bStartMapping);
                    }
                }
                else
                {
                    HZPrintDebugLogA("Try to open file:[%p:%s] with file mapping failed with last error:[0x%x]\n", hFile, m_kstrPEFilePath.c_str(), ::GetLastError());
                    HZCloseHandle(&hFile);
                }
            }
            else
            {
                HZPrintDebugLogA("Try to open file:[%s] failed with last error:[0x%x]\n", m_kstrPEFilePath.c_str(), ::GetLastError());
                HZCloseHandle(&hFile);
            }
        }
        else
        {
            HZPrintDebugLogA("Try init file:[%s] in PE manager but it do not exist\n", m_kstrPEFilePath.c_str());
        }
        HZPrintDebugLogA("End init file:[%s] with result:[%s]\n", m_kstrPEFilePath.c_str(), HZBSuccessA(bStartMapping));
    }
    return bStartMapping;
}
bool FileMappingBase::StopMapping(_In_ const bool kbSubmit)
{
    bool bRet = true;
    if (GetMappingFlag())
    {
        DWORD dwOrgFileAttr = ::GetFileAttributesA(m_kstrPEFilePath.c_str());
        if (INVALID_FILE_ATTRIBUTES == dwOrgFileAttr)
        {
            bRet = false;
            HZPrintDebugLogA("Try get file:[%s] attributes:[0x%x] failed with error:[0x%x]\n",
                m_kstrPEFilePath.c_str(), dwOrgFileAttr, ::GetLastError());
        }
        else
        {
            DWORD dwNewFileAttr = kbSubmit ? (dwOrgFileAttr & (~FILE_ATTRIBUTE_READONLY)) : (dwOrgFileAttr | FILE_ATTRIBUTE_READONLY);
            bRet = ::SetFileAttributesA(m_kstrPEFilePath.c_str(), dwNewFileAttr);
            if (bRet)
            {
                ::SetLastError(ERROR_SUCCESS);
                bRet = UnmapViewOfFile(m_pbyFileBase);
                m_pbyFileBase = nullptr;
                HZPrintDebugLogA("Unmap view to submit flag:[%s]  for file:[%s] [%s] with last error:[0x%x]\n",
                    HZBStrA(kbSubmit), m_kstrPEFilePath.c_str(), HZBSuccessA(bRet), ::GetLastError());
                // Revert attributes
                ::SetFileAttributesA(m_kstrPEFilePath.c_str(), dwOrgFileAttr);
                if (!bRet && !kbSubmit)
                {
                    // No good way to make UnmapViewOfFile free resource without submit changes
                    // If no need submit, the file will set as read only and UnmapViewOfFile will failed at this time.
                    // But this failed is not logic error.
                    // Unmap failed without submit, retur ture.
                    bRet = true;
                }
            }
            else
            {
                HZPrintDebugLogA("Try set file:[%s] attributes:[0x%x] failed with error:[0x%x]\n",
                    m_kstrPEFilePath.c_str(), dwNewFileAttr,::GetLastError());
            }
        }
        HZCloseHandle(&m_hFile);
        HZCloseHandle(&m_hMapFile);

        ResetMappingInfo();
    }
    return bRet;
}

void FileMappingBase::ResetMappingInfo()
{
    m_bStartMapping = false;

    m_hFile = nullptr;
    m_hMapFile = nullptr;
    m_pbyFileBase = nullptr;
    m_dwMappingSize = 0;
    m_dwFileSize = 0;
}

} /*< Namespace PEAssistant */
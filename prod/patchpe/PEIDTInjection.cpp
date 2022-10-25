#include "PEIDTInjection.h"
#include "MyLog.h"
#include <windows.h>
#include <Shlwapi.h>
#include <string>

#include "CommonTools.h"
#include "PETools.h"

#pragma comment(lib, "Shlwapi.lib")

#define BYTE_POINT(kdwAddr) ((BYTE*)(kdwAddr))
#define MVAToOffset(kdwAddr) (MappingAddrToOffset((DADDR)(kdwAddr)))
#define MVAToOffsetPoint(kdwAddr) (BYTE_POINT(MVAToOffset(kdwAddr)))

// For some PE structures, it only define DWORD to save a pointer address
// For these structure values, it is save to do convert
// Provider follow macro to avoid warnings
#define DWORD_TO_DADDR_S(kdwAddr) ((DADDR)(kdwAddr))
#define DADDR_TO_DWORD_S(kdwAddr) ((DWORD)(kdwAddr))

namespace PEAssistant
{

PEIDTInjection::PEIDTInjection() 
{

}
PEIDTInjection::~PEIDTInjection()
{
    Submit();
}

bool PEIDTInjection::AddIDTItem(_In_ const std::string& kstrTargetFilePath, _In_ const std::string& kstrExtendFileName, _In_ const WORD kdwHint, _In_ const std::string& kstrExtendFuncName)
{
    PEIDTInjection obPEMgr;
    bool bRet = obPEMgr.BeginTransaction(kstrTargetFilePath);
    if (bRet)
    {
        bRet = obPEMgr.InnerAddIDTItem(kstrExtendFileName, kdwHint, kstrExtendFuncName);
        if (bRet)
        {
            bRet = obPEMgr.Submit();
            HZPrintDebugLogA("Submit IDT:[%s, %d, %s] Item to file:[%s] with result:[%s]\n", kstrExtendFileName.c_str(), kdwHint,
                kstrExtendFuncName.c_str(), kstrTargetFilePath.c_str(), HZBSuccessA(bRet));
        }
        else
        {
            HZPrintDebugLogA("Add IDT:[%s, %d, %s] Item to file:[%s] failed\n", kstrExtendFileName.c_str(), kdwHint,
                kstrExtendFuncName.c_str(), kstrTargetFilePath.c_str());
        }
    }
    else
    {
        HZPrintDebugLogA("BeginTransaction PE file:[%s] failed\n", kstrTargetFilePath.c_str());
    }
    return bRet;
}

bool PEIDTInjection::BeginTransaction(_In_ const std::string& kstrPEFilePath)
{
    bool bInit = GetMappingFlag();
    if (bInit)
    {
        HZPrintDebugLogA("Already init\n");
    }
    else
    {
        HZPrintDebugLogA("Begin init file:[%s]\n", kstrPEFilePath.c_str());

        m_strPEFilePath = kstrPEFilePath;
        if (PathFileExistsA(m_strPEFilePath.c_str()))
        {
            // Open file with file mapping
            ::SetLastError(ERROR_SUCCESS);
            HANDLE hFile = CreateFileA(m_strPEFilePath.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
            );
            if (HZIsValidHandle(hFile))
            {
                ::SetLastError(ERROR_SUCCESS);
                HANDLE hMapFile = CreateFileMappingW(hFile, nullptr, PAGE_READWRITE, 0, 0, nullptr);
                if (HZIsValidHandle(hMapFile))
                {
                    // Analysis PE headers
                    ::SetLastError(ERROR_SUCCESS);
                    BYTE* pFileBuf = (BYTE*)MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
                    if (nullptr == pFileBuf)
                    {
                        HZPrintDebugLogA("Map file:[%s] view failed with last error:[0x%x]", m_strPEFilePath.c_str(), ::GetLastError());
                        HZCloseHandle(&hMapFile);
                        HZCloseHandle(&hFile);
                    }
                    else
                    {
                        // Analysis PE headers
                        m_dwFileSize = GetFileSize(hFile, nullptr);
                        bInit = InitPEHeaders(pFileBuf, m_dwFileSize);
                        if (bInit)
                        {
                            m_hFile = hFile;
                            m_hMapFile = hMapFile;
                            m_pbyFileBase = pFileBuf;
                        }
                    }
                }
                else
                {
                    HZPrintDebugLogA("Try to open file:[%p:%s] with file mapping failed with last error:[0x%x]\n", hFile, m_strPEFilePath.c_str(), ::GetLastError());
                    HZCloseHandle(&hFile);
                }
            }
            else
            {
                HZPrintDebugLogA("Try to open file:[%s] failed with last error:[0x%x]\n", m_strPEFilePath.c_str(), ::GetLastError());
                HZCloseHandle(&hFile);
            }
        }
        else
        {
            HZPrintDebugLogA("Try init file:[%s] in PE manager but it do not exist\n", m_strPEFilePath.c_str());
        }

        HZPrintDebugLogA("End init file:[%s] with result:[%s]\n", m_strPEFilePath.c_str(), HZBSuccessA(bInit));
        if (bInit)
        {
            PrintPEInfo();
            SetMappingFlag(bInit);
        }
    }
    return bInit;
}
void PEIDTInjection::PrintPEInfo() const throw()
{
    HZPrintDebugLogA("Current file:[%s], buffer:[%p], size:[%ld]. PE info:\n", m_strPEFilePath.c_str(), m_pbyFileBase, m_dwFileSize);
    HZPrintDebugLogA("\tDOS_HEADER:[%p], Offset:[%p]\n", m_pstuDosHeader, MVAToOffsetPoint(m_pstuDosHeader));
    HZPrintDebugLogA("\tNT_HEADER:[%p], Offset:[%p]\n", m_pstuNTHeaders, MVAToOffsetPoint(m_pstuNTHeaders));
    HZPrintDebugLogA("\tFILE_HEADER:[%p], Offset:[%p]\n", m_pstuFileHeader, MVAToOffsetPoint(m_pstuFileHeader));
    HZPrintDebugLogA("\tOPTIONAL_HEADER:[%p], Offset:[%p]\n", m_pstuOptionalHeader, MVAToOffsetPoint(m_pstuOptionalHeader));
    HZPrintDebugLogA("\tSECTION_HEADER:[%p], Offset:[%p]\n", m_pszSectionHeader, MVAToOffsetPoint(m_pszSectionHeader));
    HZPrintDebugLogA("\tDATA_DIRECTORY Base:[%p], Offset:[%p]\n", m_pszStuDataDirectory, MVAToOffsetPoint(m_pszStuDataDirectory));
    HZPrintDebugLogA("\tDATA_DIRECTORY Import:[%p], Offset:[%p]\n", m_pstuEntryImport, MVAToOffsetPoint(m_pstuEntryImport));
    HZPrintDebugLogA("\tFileAlignment:[%ld], FirstSection:[%d], LastSection:[%d], NumbersOfSection:[%d]\n",
        m_dwFileAlignment, m_nFirstSection, m_nLastSection, m_pstuFileHeader->NumberOfSections);
    BYTE szSectionName[IMAGE_SIZEOF_SHORT_NAME + 1] = { 0 };
    for (WORD i = 0; i < m_pstuFileHeader->NumberOfSections; ++i)
    {
        // Note, if m_pszSectionHeader[i].Name length is equals IMAGE_SIZEOF_SHORT_NAME, no end '\0'
        memcpy(szSectionName, m_pszSectionHeader[i].Name, IMAGE_SIZEOF_SHORT_NAME);
        PSTUSectionInfo pszSectionInfo = m_pszSectionInfo + i;
        HZPrintDebugLogA("\t\tSection info, Name:[%s] Index:[%d,%d], FreeSize:[%p], StartFreePoint:[%p]\n",
            szSectionName, i, pszSectionInfo->m_nSectionIndex, BYTE_POINT(pszSectionInfo->m_dwFreeSize), BYTE_POINT(pszSectionInfo->dwStartFreeOffset));
    }
}
bool PEIDTInjection::Submit()
{
    bool bRet = true;
    if (nullptr != m_pbyFileBase)
    {
        bRet = UnmapViewOfFile(m_pbyFileBase);
        m_pbyFileBase = nullptr;
    }
    HZCloseHandle(&m_hFile);
    HZCloseHandle(&m_hMapFile);

    Reset();
    return bRet;
}
bool PEIDTInjection::InnerAddIDTItem(_In_ const std::string& kstrExtendFileName, _In_ const WORD kwHint, _In_ const std::string& kstrExtendFuncName) const
{
    HZPrintDebugLogA("Begin add IDT item:[%s] with function:[%d, %s] to:[%s]\n", kstrExtendFileName.c_str(), kwHint, kstrExtendFuncName.c_str(), m_strPEFilePath.c_str());

    bool bRet = false;
    if (!GetMappingFlag())
    {
        HZPrintDebugLogA("Try to add IDT item but do not init. extend file name:[%s] or function name:[%s]\n", kstrExtendFileName.c_str(), kstrExtendFuncName.c_str());
    }
    else if ((kstrExtendFileName.length() >= g_knMaxImportDllNameLength) || (kstrExtendFuncName.length() >= g_knMaxImportFuncNameLength))
    {
        HZPrintDebugLogA("The extend file name:[%s] or function name:[%s] it too long. Must be less than:[%d] and [%d]\n",
            kstrExtendFileName.c_str(), kstrExtendFuncName.c_str(),
            g_knMaxImportDllNameLength, g_knMaxImportFuncNameLength);
    }
    else
    {
        // Find place to move IDT and save extend data info
        std::pair<PIMAGE_IMPORT_DESCRIPTOR, int> pairNewIDT = {0, 0};
        std::pair<STUExtendIDTInfo*, int> pairExtendIDTInfo = { 0, 0 };
        DWORD dwNewIDTSize = m_pstuEntryImport->Size + sizeof(IMAGE_IMPORT_DESCRIPTOR);
        HZPrintDebugLogA("Try to find place to extend IDT, NewIDT size:[0x%lx], ExtendIDTInfo size:[0x%llx]\n", dwNewIDTSize, (ULONGLONG)(sizeof(STUExtendIDTInfo)));
        for (int i = 0; i < m_pstuFileHeader->NumberOfSections; ++i)
        {
            PSTUSectionInfo pCurSectionInfo = m_pszSectionInfo + i;
            if (0 == pairNewIDT.first)
            {
                if (dwNewIDTSize <= pCurSectionInfo->m_dwFreeSize)
                {
                    // Find place to move IDT
                    pairNewIDT.first = (PIMAGE_IMPORT_DESCRIPTOR)(m_pbyFileBase + pCurSectionInfo->dwStartFreeOffset);
                    pairNewIDT.second = i;

                    if (sizeof(STUExtendIDTInfo) <= m_pstuEntryImport->Size)
                    {
                        pairExtendIDTInfo.first = (STUExtendIDTInfo*)(m_pbyFileBase + RVAToOffset(m_pstuEntryImport->VirtualAddress, -1));
                        pairExtendIDTInfo.second = GetRVABelongSection(m_pstuEntryImport->VirtualAddress, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
                    }
                    else
                    {
                        if ((sizeof(STUExtendIDTInfo) + dwNewIDTSize) <= pCurSectionInfo->m_dwFreeSize)
                        {
                            pairExtendIDTInfo.first = (STUExtendIDTInfo*)(BYTE_POINT(pairNewIDT.first) + dwNewIDTSize);
                            pairExtendIDTInfo.second = i;
                        }
                    }
                    break;
                }
            }
        }
        if (0 == pairNewIDT.first)
        {
            // New IDT place do not find, continue
            HZPrintDebugLogA("Cannot find free place:[%ld] to move IDT, need try to create a new section to extend file", dwNewIDTSize);
        }
        else
        {
            if (0 == pairExtendIDTInfo.first)
            {
                for (int i = 0; i < m_pstuFileHeader->NumberOfSections; ++i)
                {
                    if (i == pairNewIDT.second)
                    {
                        // Skip
                        continue;
                    }
                    else
                    {
                        PSTUSectionInfo pCurSectionInfo = m_pszSectionInfo + i;
                        if (sizeof(STUExtendIDTInfo) <= pCurSectionInfo->m_dwFreeSize)
                        {
                            pairExtendIDTInfo.first = (STUExtendIDTInfo*)(m_pbyFileBase + pCurSectionInfo->dwStartFreeOffset);
                            pairExtendIDTInfo.second = i;
                        }
                        else
                        {
                            // Continue
                        }
                    }
                }
            }
            else
            {
                // OK
            }
        }
        
        if ((0 != pairNewIDT.first) && (0 != pairExtendIDTInfo.first))
        {
            // verify
            if (VerifyMVA((DADDR)(pairNewIDT.first), pairNewIDT.second) && VerifyMVA((DADDR)(pairExtendIDTInfo.first), pairExtendIDTInfo.second))
            {
                HZPrintDebugLogA("Begin to move IDT from:[%p] to:[%p] in section(index):[%d] and write extend IDT info into:[%p] in section(index):[%d]\n",
                    m_pstuEntryImport, pairNewIDT.first, pairNewIDT.second, pairExtendIDTInfo.first, pairExtendIDTInfo.second);

                // Move IDT
                memset((BYTE*)(pairNewIDT.first), 0, m_pstuEntryImport->Size + sizeof(IMAGE_IMPORT_DESCRIPTOR));
                if (0 != m_pstuEntryImport->VirtualAddress)
                {
                    memcpy((BYTE*)(pairNewIDT.first), m_pbyFileBase + RVAToOffset(m_pstuEntryImport->VirtualAddress, -1), m_pstuEntryImport->Size);
                }

                // Add new IDT item
                PIMAGE_IMPORT_DESCRIPTOR pNewImportDescriptor = PIMAGE_IMPORT_DESCRIPTOR((BYTE*)(pairNewIDT.first) + m_pstuEntryImport->Size - sizeof(IMAGE_IMPORT_DESCRIPTOR));
                pNewImportDescriptor->Name = 0;
                pNewImportDescriptor->FirstThunk = 0;
                pNewImportDescriptor->OriginalFirstThunk = 0;
                pNewImportDescriptor->ForwarderChain = 0;
                pNewImportDescriptor->TimeDateStamp = 0;

                // Fill extend IDT item info
                STUExtendIDTInfo stuExtendIDT = { 0 };
                memcpy(stuExtendIDT.szDllName, kstrExtendFileName.c_str(), kstrExtendFileName.length());
                stuExtendIDT.wHint = kwHint;
                memcpy(stuExtendIDT.szFuncName, kstrExtendFuncName.c_str(), kstrExtendFuncName.length());
                memcpy((BYTE*)(pairExtendIDTInfo.first), &stuExtendIDT, sizeof(stuExtendIDT));
                DADDR dwAddressOfData = MappingAddrToRVA((DADDR)(&(pairExtendIDTInfo.first->wHint)), pairExtendIDTInfo.second);
                pairExtendIDTInfo.first->pszThunkData[0].u1.AddressOfData = pairExtendIDTInfo.first->pszThunkData[2].u1.AddressOfData = dwAddressOfData;

                // Connect new IDT item and extend IDT item
                pNewImportDescriptor->Name = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)pairExtendIDTInfo.first->szDllName, pairExtendIDTInfo.second));
                pNewImportDescriptor->FirstThunk = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)pairExtendIDTInfo.first->pszThunkData, pairExtendIDTInfo.second));
                pNewImportDescriptor->OriginalFirstThunk = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)(pairExtendIDTInfo.first->pszThunkData + 2), pairExtendIDTInfo.second));

                // Reset IDT address and size
                m_pstuEntryImport->VirtualAddress = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)pairNewIDT.first, pairNewIDT.second));
                m_pstuEntryImport->Size += sizeof(IMAGE_IMPORT_DESCRIPTOR);

                // Add write permission to the section which contains the extend IDT info
                m_pszSectionHeader[pairExtendIDTInfo.second].Characteristics |= IMAGE_SCN_MEM_WRITE;

                bRet = true;
                HZPrintDebugLogA("End to move IDT from:[%p] to:[%p] in section(index):[%d] and write extend IDT info into:[%p] in section(index):[%d] with result:[%s]\n",
                    m_pstuEntryImport, MVAToOffsetPoint(pairNewIDT.first), pairNewIDT.second, MVAToOffsetPoint(pairExtendIDTInfo.first), pairExtendIDTInfo.second, HZBSuccessA(bRet));

                // We move and add extend data into section but we do not modify the section VirtualSize info.
                // In fact, no need update it. The new PE file also works fine.
                // Another is we do not sure if the old VirtualSize is use full or not, no need update it.
            }
            else
            {
                HZPrintDebugLogA("Verify NewIDT and ExtendIDTInfo address and section failed. NewIDT:[%p, %d], ExtendIDTInfo:[%p,%d]\n",
                    pairNewIDT.first, pairNewIDT.second, pairExtendIDTInfo.first, pairExtendIDTInfo.second);
            }
        }
        else
        {
            HZPrintDebugLogA("Cannot find free place to move IDT and save extend IDT info, need try to create a new section to extend file");
        }
    }

    HZPrintDebugLogA("End add IDT item:[%s] with function:[%d, %s] to:[%s] with result:[%s]\n",
        kstrExtendFileName.c_str(), kwHint, kstrExtendFuncName.c_str(), m_strPEFilePath.c_str(), HZBSuccessA(bRet));
    return bRet;
}

bool PEIDTInjection::InitPEHeaders(_In_ const BYTE* kpPEBuf, _In_ const DWORD /*kdwSize*/)
{
    if (nullptr == kpPEBuf)
    {
        return false;
    }

    bool bRet = false;
    // Analysis Dos header
    if (IsPEFile(kpPEBuf))
    {
        m_pstuDosHeader = (PIMAGE_DOS_HEADER)kpPEBuf;
        // Analysis NT header
        m_pstuNTHeaders = (PIMAGE_NT_HEADERS)(kpPEBuf + m_pstuDosHeader->e_lfanew);
        
        m_pstuFileHeader = &(m_pstuNTHeaders->FileHeader);
        m_pstuOptionalHeader = &(m_pstuNTHeaders->OptionalHeader);
        m_dwFileAlignment = m_pstuOptionalHeader->FileAlignment;

        m_pszStuDataDirectory = m_pstuOptionalHeader->DataDirectory;
        m_pstuEntryImport = &(m_pstuOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);

        // Loop section, do not sure if the section is sort by address
        m_pszSectionInfo = new STUSectionInfo[m_pstuFileHeader->NumberOfSections];
        memset(m_pszSectionInfo, 0, sizeof(STUSectionInfo) * m_pstuFileHeader->NumberOfSections);
        m_pszSectionHeader = (PIMAGE_SECTION_HEADER)(((char*)m_pstuOptionalHeader) + sizeof(IMAGE_OPTIONAL_HEADER));
        m_nFirstSection = 0;
        m_nLastSection = 0;
        // Note: PointerToRawData can be null
        DWORD dwMinRawDataPointer = (0 == m_pszSectionHeader->PointerToRawData) ? (DWORD)(-1) : m_pszSectionHeader->PointerToRawData;
        DWORD dwMaxRawDataPointer = m_pszSectionHeader->PointerToRawData;
        for (int i = 1; i < m_pstuFileHeader->NumberOfSections; ++i)
        {
            STUSectionInfo stuSessioninfo = { 0 };
            stuSessioninfo.m_nSectionIndex = i;
            PIMAGE_SECTION_HEADER pCurSectionHeader = m_pszSectionHeader + i;
            if (0 == pCurSectionHeader->PointerToRawData)
            {
               // This section no section data, no free place
            }
            else
            {
                if (dwMinRawDataPointer > pCurSectionHeader->PointerToRawData)
                {
                    dwMinRawDataPointer = pCurSectionHeader->PointerToRawData;
                    m_nFirstSection = i;
                }
                if (dwMaxRawDataPointer < pCurSectionHeader->PointerToRawData)
                {
                    dwMaxRawDataPointer = pCurSectionHeader->PointerToRawData;
                    m_nLastSection = i;
                }
                // Calculate section free place
                DWORD dwCalFreeSize = 0;
                if (pCurSectionHeader->SizeOfRawData > pCurSectionHeader->Misc.VirtualSize)
                {
                    dwCalFreeSize = pCurSectionHeader->SizeOfRawData - pCurSectionHeader->Misc.VirtualSize;
                }
                // Loop and check the free place
                // The Misc.VirtualSize value not always right, need loop and check
                DWORD dwLoopFreeSize = 0;
                BYTE* pCurSectionDataEnd = ((BYTE*)(m_pstuDosHeader)) + (DWORD_TO_DADDR_S(pCurSectionHeader->PointerToRawData) + pCurSectionHeader->SizeOfRawData - 1);
                while (pCurSectionDataEnd[0] == 0)
                {
                    dwLoopFreeSize++;
                    pCurSectionDataEnd--;
                }
                // Decision free space size
                if ((0 < dwCalFreeSize) && (0 < dwLoopFreeSize) && (dwLoopFreeSize < pCurSectionHeader->SizeOfRawData))
                {
#if 1
                    stuSessioninfo.m_dwFreeSize = min(dwCalFreeSize, dwLoopFreeSize);
                    // Because of Misc.VirtualSize and loop free size both are not 100% safe
                    // Leave some spare to make sure it is save
                    DADDR dwRemainderOf16 = (stuSessioninfo.m_dwFreeSize % 16);
                    if (4 <= dwRemainderOf16)
                    {
                        stuSessioninfo.m_dwFreeSize -= dwRemainderOf16;
                    }
                    else
                    {
                        stuSessioninfo.m_dwFreeSize -= (dwRemainderOf16 + 16);
                    }
#else
                    if ((dwLoopFreeSize >= dwCalFreeSize) && (dwLoopFreeSize <= dwCalFreeSize + 4))
                    {
                        // Using the calculate size
                        stuSessioninfo.dwFreeSize = dwCalFreeSize;
                    }
                    else
                    {
                        stuSessioninfo.dwFreeSize = dwLoopFreeSize - 4;
                        stuSessioninfo.dwFreeSize -= (stuSessioninfo.dwFreeSize % 16);
                    }
#endif
                    stuSessioninfo.dwStartFreeOffset = DWORD_TO_DADDR_S(pCurSectionHeader->PointerToRawData) + pCurSectionHeader->SizeOfRawData - stuSessioninfo.m_dwFreeSize;
                }
            }
            m_pszSectionInfo[i] = stuSessioninfo;
        }
        bRet = true;
    }
    else
    {
        HZPrintDebugLogA("Current buffer is not PE buffer\n");
    }
    return bRet;
}

void PEIDTInjection::Reset()
{
    m_bStartMapping = false;
    m_strPEFilePath = "";

    m_hFile = nullptr;
    m_hMapFile = nullptr;
    m_pbyFileBase = nullptr;

    m_pstuDosHeader = nullptr;
    m_pstuNTHeaders = nullptr;

    m_pstuFileHeader = nullptr;
    m_pstuOptionalHeader = nullptr;
    m_dwFileAlignment = 0;

    m_pszStuDataDirectory = nullptr;
    m_pstuEntryImport = nullptr;

    m_pszSectionHeader = nullptr;
    m_nFirstSection = 0;
    m_nLastSection = 0;
    m_pszSectionInfo = nullptr;
}

int PEIDTInjection::GetOffsetBelongSection(_In_ const DADDR kdwOffset, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections)
{
    for (int i = 0; i < knNumberOfSections; i++)
    {
        DADDR dwCurPointerToRawData = pSectionHeader[i].PointerToRawData;
        if ((dwCurPointerToRawData <= kdwOffset) && (dwCurPointerToRawData + pSectionHeader[i].SizeOfRawData) >= kdwOffset)
        {
            return i;
        }
    }
    return -1;
}
int PEIDTInjection::GetRVABelongSection(_In_ const DADDR kdwVirtualAddr, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections)
{
    for (int i = 0; i < knNumberOfSections; i++)
    {
        DADDR dwCurVirtualAddress = pSectionHeader[i].VirtualAddress;
        if ((dwCurVirtualAddress <= kdwVirtualAddr) && (dwCurVirtualAddress + pSectionHeader[i].SizeOfRawData >= kdwVirtualAddr))
        {
            return i;
        }
    }
    return -1;
}

bool PEIDTInjection::VerifyMVA(_In_ const DADDR kdwMVA, _In_ const int knExpectSectionIndex) const throw()
{
    DADDR kdwOffset = MappingAddrToOffset(kdwMVA);
    int knCalSectionIndex = GetOffsetBelongSection(kdwOffset, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
    return (knCalSectionIndex == knExpectSectionIndex);
}

DADDR PEIDTInjection::MappingAddrToRVA(_In_ const DADDR kdwVA, _In_ const int knBelongSectionIndex) const throw()
{
    DADDR kdwOffset = MappingAddrToOffset(kdwVA);
    return OffsetToRVA(kdwOffset, knBelongSectionIndex);
}
DADDR PEIDTInjection::MappingAddrToOffset(_In_ const DADDR kdwVA) const throw()
{
    if (kdwVA >= (DADDR)(m_pbyFileBase))
    {
        return kdwVA - ((DADDR)(m_pbyFileBase));
    }
    else
    {
        return 0;
    }
}
bool PEIDTInjection::IsValidSectionIndex(_In_ const int knBelongSectionIndex) const throw()
{
    return (0 <= knBelongSectionIndex) && (m_pstuFileHeader->NumberOfSections > knBelongSectionIndex);
}
DADDR PEIDTInjection::OffsetToRVA(_In_ const DADDR kdwOffset, _In_ int nBelongSectionIndex) const throw()
{
    if (!IsValidSectionIndex(nBelongSectionIndex))
    {
        nBelongSectionIndex = GetOffsetBelongSection(kdwOffset, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
    }
    DADDR dwRVARet = 0;
    if (IsValidSectionIndex(nBelongSectionIndex))
    {
        PIMAGE_SECTION_HEADER pSectionBelong = m_pszSectionHeader + nBelongSectionIndex;
        if (nullptr == pSectionBelong)
        {
            HZPrintDebugLogA("The section header is null with section index:[%d] for offset:[%p], current base section header address is:[%p]\n", 
                nBelongSectionIndex, BYTE_POINT(kdwOffset), m_pszSectionHeader);
        }
        else
        {
            dwRVARet = pSectionBelong->VirtualAddress + kdwOffset - pSectionBelong->PointerToRawData;
        }
    }
    else
    {
        HZPrintDebugLogA("Cannot get valid section index for offset:[%p], current file base address is:[%p]\n", BYTE_POINT(kdwOffset), m_pbyFileBase);
    }
    return dwRVARet;
}
DADDR PEIDTInjection::RVAToOffset(_In_ const DADDR kdwRVA, _In_ int nBelongSectionIndex) const throw()
{
    if (!IsValidSectionIndex(nBelongSectionIndex))
    {
        nBelongSectionIndex = GetRVABelongSection(kdwRVA, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
    }
    DADDR dwRVARet = 0;
    if (IsValidSectionIndex(nBelongSectionIndex))
    {
        PIMAGE_SECTION_HEADER pSectionBelong = m_pszSectionHeader + nBelongSectionIndex;
        if (nullptr == pSectionBelong)
        {
            HZPrintDebugLogA("The section header is null with section index:[%d] for RVA:[%p], current base section header address is:[%p]\n",
                nBelongSectionIndex, BYTE_POINT(kdwRVA), m_pszSectionHeader);
        }
        else
        {
            dwRVARet = pSectionBelong->PointerToRawData + kdwRVA - pSectionBelong->VirtualAddress;
        }
    }
    else
    {
        HZPrintDebugLogA("Cannot get valid section index for RVA:[%p], current file base address is:[%p]\n", BYTE_POINT(kdwRVA), m_pbyFileBase);
    }
    return dwRVARet;
}

} /*< Namespace PEAssistant */
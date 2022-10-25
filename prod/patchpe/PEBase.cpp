#include "PEBase.h"
#include "MyLog.h"
#include <windows.h>
#include <Shlwapi.h>
#include <string>
#include "CommonTools.h"
#include "PETools.h"

#pragma comment(lib, "Shlwapi.lib")

namespace PEAssistant
{

PEBase::PEBase(_In_ const std::string& kstrPEFilePath) : FileMappingBase(kstrPEFilePath)
{
    ResetPEBaseInfo();
}
PEBase::~PEBase()
{
    ResetPEBaseInfo();
}

bool PEBase::InitPEBase(_In_ const bool kbAutoStartMapping, _In_ const DWORD kdwExtendSize)
{
    bool bRet = false;
    const std::string& kstrCurPEFile = GetMappingFilePath();
    if (GetPEBaseInitFlag())
    {
        HZPrintDebugLogA("Current PEBase already init for file:[%s], no need init again\n", kstrCurPEFile.c_str());
        bRet = true;
    }
    else
    {
        bool bInnerOpenMapping = false;
        bRet = GetMappingFlag();
        if (!bRet)
        {
            if (kbAutoStartMapping)
            {
                bRet = StartMapping(kdwExtendSize);
                bInnerOpenMapping = true;
            }
        }
        if (bRet)
        {
            bRet = false;
            const BYTE* kpPEFileBase = GetMappingFileBase();
            if (nullptr == kpPEFileBase)
            {
                HZPrintDebugLogA("!!!!!!Logic error. Current PE file:[%s] base pointer:[%p] is NULL but the mapping flag is true, please check code\n",
                    kstrCurPEFile.c_str(), kpPEFileBase);
            }
            else
            {
                if (IsPEFile(kpPEFileBase))
                {
                    if (IsImageSupport(kpPEFileBase))
                    {
                        // Analysis DOS header
                        m_pstuDosHeader = (PIMAGE_DOS_HEADER)(kpPEFileBase);
                        // Analysis NT header
                        m_pstuNTHeaders = (PIMAGE_NT_HEADERS)(kpPEFileBase + m_pstuDosHeader->e_lfanew);

                        m_pstuFileHeader = &(m_pstuNTHeaders->FileHeader);
                        m_pstuOptionalHeader = &(m_pstuNTHeaders->OptionalHeader);

                        m_pszStuDataDirectory = m_pstuOptionalHeader->DataDirectory;
                        m_pstuEntryImport = &(m_pstuOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);

                        // Loop section, do not sure if the section is sort by address
                        m_pszSectionHeader = (PIMAGE_SECTION_HEADER)(((char*)m_pstuOptionalHeader) + sizeof(IMAGE_OPTIONAL_HEADER));
                        m_pszSectionInfo = InitSectionInfo(m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);

                        bRet = true;
                        SetPEBaseInitFlag(bRet);
                    }
                    else
                    {
                        HZPrintDebugLogA("Init PE file:[%s] failed, it is not support image. Please make sure the x86 and x64 are running with right process\n", kstrCurPEFile.c_str());
                    }
                }
                else
                {
                    HZPrintDebugLogA("Init PE file:[%s] failed, it is not PE format\n", kstrCurPEFile.c_str());
                }
            }
            if (!bRet && bInnerOpenMapping)
            {
                StopMapping(false);
            }
        }
        else
        {
            HZPrintDebugLogA("Current PE file:[%s] do not start mapping success, cannot init PEBase info, AutoStartMapping:[%s], ExtendSize:[%ld]\n",
                kstrCurPEFile.c_str(), HZBSuccessA(kbAutoStartMapping), kdwExtendSize);
        }
    }
    return bRet;
}
bool PEBase::UninitPEBase(_In_ const bool kbStopMapping, _In_ const bool kbSubmit)
{
    bool bRet = true;
    ResetPEBaseInfo();
    if (kbStopMapping)
    {
        bRet = StopMapping(kbSubmit);
    }
    return bRet;
}
bool PEBase::ReInitPEBase(_In_ const bool kbReMapping, _In_ const bool kbSubmit, _In_ const DWORD kdwExtendSize)
{
    UninitPEBase(kbReMapping, kbSubmit);
    return InitPEBase(kbReMapping, kdwExtendSize);
}
PESectionInfo* PEBase::InitSectionInfo(_In_ const PIMAGE_SECTION_HEADER pszSectionHeader, _In_ const WORD kdwSectionNumber) const throw()
{
    PESectionInfo* pszSectionInfo = nullptr;
    if ((nullptr == pszSectionHeader) || (0 >= kdwSectionNumber))
    {
        HZPrintDebugLogA("Try to init section header into but pass in section header:[%p] is null\n", pszSectionHeader);
    }
    else
    {
        pszSectionInfo = PESectionInfo::GetPESectionInfoArray(kdwSectionNumber);
        for (WORD i = 0; i < kdwSectionNumber; ++i)
        {
            PESectionInfo* pcurSectionInfo = pszSectionInfo + i;
            pcurSectionInfo->Init(m_pszSectionHeader, i, PEFileAlign, m_pbyFileBase);
        }
    }
    return pszSectionInfo;
}
void PEBase::ResetPEBaseInfo()
{
    SetPEBaseInitFlag(false);
    m_pstuDosHeader = nullptr;
    m_pstuNTHeaders = nullptr;

    m_pstuFileHeader = nullptr;
    m_pstuOptionalHeader = nullptr;

    m_pszStuDataDirectory = nullptr;
    m_pstuEntryImport = nullptr;

    m_pszSectionHeader = nullptr;
    if (nullptr != m_pszSectionInfo)
    {
        delete[] m_pszSectionInfo;
        m_pszSectionInfo = nullptr;
    }
}

void PEBase::PrintPEInfo() const throw()
{
    HZPrintDebugLogA("Current file:[%s], buffer:[%p], mapping size:[%ld]. PE info:\n", m_kstrPEFilePath.c_str(), m_pbyFileBase, m_dwMappingSize);
    HZPrintDebugLogA("\tDOS_HEADER:[%p], Offset:[%p]\n", m_pstuDosHeader, MVAToOffsetPoint(m_pstuDosHeader));
    HZPrintDebugLogA("\tNT_HEADER:[%p], Offset:[%p]\n", m_pstuNTHeaders, MVAToOffsetPoint(m_pstuNTHeaders));
    HZPrintDebugLogA("\tFILE_HEADER:[%p], Offset:[%p]\n", m_pstuFileHeader, MVAToOffsetPoint(m_pstuFileHeader));
    HZPrintDebugLogA("\tOPTIONAL_HEADER:[%p], Offset:[%p]\n", m_pstuOptionalHeader, MVAToOffsetPoint(m_pstuOptionalHeader));
    HZPrintDebugLogA("\tSECTION_HEADER:[%p], Offset:[%p]\n", m_pszSectionHeader, MVAToOffsetPoint(m_pszSectionHeader));
    HZPrintDebugLogA("\tDATA_DIRECTORY Base:[%p], Offset:[%p]\n", m_pszStuDataDirectory, MVAToOffsetPoint(m_pszStuDataDirectory));
    HZPrintDebugLogA("\tDATA_DIRECTORY Import:[%p], Offset:[%p]\n", m_pstuEntryImport, MVAToOffsetPoint(m_pstuEntryImport));
    HZPrintDebugLogA("\tFileAlignment:[%ld], NumbersOfSection:[%d]\n", m_pstuOptionalHeader->FileAlignment, m_pstuFileHeader->NumberOfSections);
    BYTE szSectionName[IMAGE_SIZEOF_SHORT_NAME + 1] = { 0 };
    for (WORD i = 0; i < m_pstuFileHeader->NumberOfSections; ++i)
    {
        // Note, if m_pszSectionHeader[i].Name length is equals IMAGE_SIZEOF_SHORT_NAME, no end '\0'
        memcpy(szSectionName, m_pszSectionHeader[i].Name, IMAGE_SIZEOF_SHORT_NAME);
        PESectionInfo* pcurSectionInfo = m_pszSectionInfo + i;
        pcurSectionInfo->PrintInfo("\t\t");
    }
}
bool PEBase::IsImageSupport(_In_ const BYTE* kpbyPEBase) throw()
{
#ifdef _WIN64 
    const static bool s_kbIsX64 = true;
#else
    const static bool s_kbIsX64 = false;
#endif
    bool bRet = false;
    if (nullptr != kpbyPEBase)
    {
        PIMAGE_DOS_HEADER pstuDosHeader = (PIMAGE_DOS_HEADER)(kpbyPEBase);
        PIMAGE_NT_HEADERS pstuNTHeaders = (PIMAGE_NT_HEADERS)(kpbyPEBase + pstuDosHeader->e_lfanew);
        PIMAGE_OPTIONAL_HEADER pstuOptionalHeader = &(pstuNTHeaders->OptionalHeader);

        if (IMAGE_NT_OPTIONAL_HDR32_MAGIC == pstuOptionalHeader->Magic)
        {
            bRet = !s_kbIsX64;
        }
        else if (IMAGE_NT_OPTIONAL_HDR64_MAGIC == pstuOptionalHeader->Magic)
        {
            bRet = s_kbIsX64;
        }
        else
        {
            bRet = false;
        }
    }
    return bRet;
}
bool PEBase::SetDataToSection(_In_ const int knSectionIndexIn, _In_ int knStartSectionOffsetIn, _In_ const BYTE* pbyNewDataIn, _In_ const DADDR kdwDataSizeIn) const throw()
{
    bool bRet = false;
    if (IsValidSectionIndex(knSectionIndexIn))
    {
        PIMAGE_SECTION_HEADER pCurSectionHeader = m_pszSectionHeader + knSectionIndexIn;
        PESectionInfo* pCurSectionInfo = m_pszSectionInfo + knSectionIndexIn;
        DWORD dwSectionOffset = knStartSectionOffsetIn;
        if (0 > knStartSectionOffsetIn)
        {
            dwSectionOffset = pCurSectionHeader->SizeOfRawData;
        }

        // Check size
        DADDR dwSectionSize = pCurSectionInfo->GetSectionSize();
        if (dwSectionOffset < SubSmall<DADDR>(dwSectionSize, kdwDataSizeIn, 0))
        {
            // Copy data
            memcpy(m_pbyFileBase + pCurSectionHeader->PointerToRawData + dwSectionOffset, pbyNewDataIn, kdwDataSizeIn);
            bRet = true;
        }
        else
        {
            HZPrintDebugLogA("Try to set data to section with index:[%d] but size over, StartOffset:[%ld, %d], DataSize:[%p], SectionSize:[%p]\n", 
                knSectionIndexIn, dwSectionOffset, knStartSectionOffsetIn, BYTE_POINT(kdwDataSizeIn), BYTE_POINT(dwSectionSize));
        }
    }
    else
    {
        HZPrintDebugLogA("Try to set data to section with index:[%d] but the index is invalid\n", knSectionIndexIn);
    }
    return bRet;
}
int PEBase::GetOffsetBelongSection(_In_ const DADDR kdwOffset) const throw()
{
    return PEAssistant::GetOffsetBelongSection(kdwOffset, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
}
int PEBase::GetRVABelongSection(_In_ const DADDR kdwVirtualAddr) const throw()
{
    return PEAssistant::GetOffsetBelongSection(kdwVirtualAddr, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
}

DWORD PEBase::GetPEHeaderSize(_In_ const unsigned int knHeaderType /*EMHeaderType*/) const throw()
{
    DWORD dwPEHeaderSizeRet = 0;
    if (GetMappingFlag())
    {
        if (HZEmOpEq(EMHeaderType::emHeaderType_All, knHeaderType))
        {
            dwPEHeaderSizeRet = m_pstuOptionalHeader->SizeOfHeaders;
        }
        else
        {
            if (HZEmOpAnd(EMHeaderType::emHeaderType_DOS, knHeaderType))
            {
                dwPEHeaderSizeRet += sizeof(IMAGE_DOS_HEADER);
            }
            if (HZEmOpAnd(EMHeaderType::emHeaderType_DOSStub, knHeaderType))
            {
                dwPEHeaderSizeRet += (m_pstuDosHeader->e_lfanew - sizeof(IMAGE_DOS_HEADER));
            }
            if (HZEmOpAnd(EMHeaderType::emHeaderType_NTHeader, knHeaderType))
            {
                dwPEHeaderSizeRet += sizeof(IMAGE_NT_HEADERS);
            }
            if (HZEmOpAnd(EMHeaderType::emHeaderType_NTFileHeader, knHeaderType))
            {
                dwPEHeaderSizeRet += sizeof(IMAGE_FILE_HEADER);
            }
            if (HZEmOpAnd(EMHeaderType::emHeaderType_NTOptionHeader, knHeaderType))
            {
                dwPEHeaderSizeRet += sizeof(IMAGE_OPTIONAL_HEADER);
            }
            if (HZEmOpAnd(EMHeaderType::emHeaderType_SectionHeader, knHeaderType))
            {
                dwPEHeaderSizeRet += (sizeof(IMAGE_SECTION_HEADER) * m_pstuFileHeader->NumberOfSections);
            }
            if (HZEmOpAnd(EMHeaderType::emHeaderType_HeaderPadding, knHeaderType))
            {
                // Full header size sub Dos header and Dos stub (m_pstuDosHeader->e_lfanew), sub NTHeaders, sub section headers
                dwPEHeaderSizeRet += (m_pstuOptionalHeader->SizeOfHeaders - m_pstuDosHeader->e_lfanew - sizeof(IMAGE_NT_HEADERS) - (sizeof(IMAGE_SECTION_HEADER) * m_pstuFileHeader->NumberOfSections));
            }
        }
    }
    return dwPEHeaderSizeRet;
}
DWORD PEBase::GetPEHeaderAlignGap() const throw()
{
    return GetPEHeaderSize((unsigned int)(EMHeaderType::emHeaderType_HeaderPadding));
}

bool PEBase::VerifyMVA(_In_ const DADDR kdwMVA, _In_ const int knExpectSectionIndex) const throw()
{
    if ((0 != kdwMVA) && (IsValidSectionIndex(knExpectSectionIndex)))
    {
        DADDR kdwOffset = MappingAddrToOffset(kdwMVA);
        int knCalSectionIndex = GetOffsetBelongSection(kdwOffset);
        return (knCalSectionIndex == knExpectSectionIndex);
    }
    return false;
}

bool PEBase::IsDllExistInIDT(_In_ const std::string& kstrDllNameIn) const throw()
{
    bool bRet = false;
    int nIDTICount = ((m_pstuEntryImport->Size) / sizeof(IMAGE_IMPORT_DESCRIPTOR));
    PIMAGE_IMPORT_DESCRIPTOR pFirstImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(m_pbyFileBase + RVAToOffset(m_pstuEntryImport->VirtualAddress, -1));
    for (int i = 0; i < nIDTICount; ++i)
    {
        PIMAGE_IMPORT_DESCRIPTOR pCurImportDescriptor = pFirstImportDescriptor + i;
        char* pIDTItemName = (char*)(m_pbyFileBase + RVAToOffset(pCurImportDescriptor->Name, -1));
        if (0 == _strnicmp(pIDTItemName, kstrDllNameIn.c_str(), kstrDllNameIn.length()))
        {
            bRet = true;
            break;
        }
    }
    return bRet;
}

bool PEBase::IsSectionExistInIDT(_In_ const std::string& kstrSectionNameIn) const throw()
{
    int nSectionIndex = GetSectionIndexByName(kstrSectionNameIn);
    return IsValidSectionIndex(nSectionIndex);
}
int PEBase::GetSectionIndexByName(_In_ const std::string& kstrSectionNameIn) const throw()
{
    int nSectionIndex = -1;
    std::string strCurSectionName = "";
    for (int i = 0; i < m_pstuFileHeader->NumberOfSections; ++i)
    {
        PIMAGE_SECTION_HEADER pCurSectionHeader = m_pszSectionHeader + i;
        strCurSectionName = ConvertSectionNameToString((char*)(pCurSectionHeader->Name));
        if (0 == _stricmp(strCurSectionName.c_str(), kstrSectionNameIn.c_str()))
        {
            nSectionIndex = i;
        }
    }
    return nSectionIndex;
}

DADDR PEBase::MappingAddrToRVA(_In_ const DADDR kdwVA, _In_ const int knBelongSectionIndex) const throw()
{
    DADDR kdwOffset = MappingAddrToOffset(kdwVA);
    return OffsetToRVA(kdwOffset, knBelongSectionIndex);
}
DADDR PEBase::MappingAddrToOffset(_In_ const DADDR kdwVA) const throw()
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
bool PEBase::IsValidSectionIndex(_In_ const int knBelongSectionIndex) const throw()
{
    return (0 <= knBelongSectionIndex) && (m_pstuFileHeader->NumberOfSections > knBelongSectionIndex);
}
DADDR PEBase::OffsetToRVA(_In_ const DADDR kdwOffset, _In_ int nBelongSectionIndex) const throw()
{
    if (!IsValidSectionIndex(nBelongSectionIndex))
    {
        nBelongSectionIndex = PEAssistant::GetOffsetBelongSection(kdwOffset, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
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
DADDR PEBase::RVAToOffset(_In_ const DADDR kdwRVA, _In_ int nBelongSectionIndex) const throw()
{
    if (!IsValidSectionIndex(nBelongSectionIndex))
    {
        nBelongSectionIndex = PEAssistant::GetRVABelongSection(kdwRVA, m_pszSectionHeader, m_pstuFileHeader->NumberOfSections);
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
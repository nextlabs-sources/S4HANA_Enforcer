#include "PEMgr.h"
#include "MyLog.h"
#include "CommonTools.h"

namespace PEAssistant
{

PEMgr::PEMgr(_In_ const std::string& kstrPEFilePath) : PEBase(kstrPEFilePath)
{
}
PEMgr::~PEMgr()
{
}

// 添加 IDT
// 首先获取已有IDT的长度
// 计算新加的 IDT 需要的长度
// 重新 Mapping 文件, 添加新 Section 用于容纳 新 IDT 和 依赖的数据
// 内存拷贝 Org IDT, 新 IDT 和 新 IDT data
bool PEMgr::AddIDTItem(_In_ const std::string& kstrInjectFileName, _In_ const WORD kwHint, _In_ const std::string& kstrInjectFuncName, _In_ const std::string& kstrNewSectionName)
{
    bool bRet = false;

    // Check parameters
    if ((!IsValidExtendIDTDllName(kstrInjectFileName)) || (!IsValidExtendIDTFuncName(kstrInjectFuncName)))
    {
        HZPrintDebugLogA("Parameters error, try to add new IDT item but one of the inject file name:[%s] or function name:[%s] is invalid. Maybe it is empty or too long. Max dll name length:[%d], max function name length:[%d]\n",
            kstrInjectFileName.c_str(), kstrInjectFileName.c_str(), g_knMaxImportDllNameLength, g_knMaxImportFuncNameLength);
        return bRet;
    }

    // Get data size
    DWORD dwFullDataSize = 0;
    DWORD dwNewIDTSize = 0;
    const std::string& kstrCurPEFile = GetMappingFilePath();
    bRet = InitPEBase(true, 0);
    if (bRet)
    {
        dwNewIDTSize = m_pstuEntryImport->Size + sizeof(IMAGE_IMPORT_DESCRIPTOR);
        dwFullDataSize = dwNewIDTSize + sizeof(STUExtendIDTInfo);
    }
    else
    {
        HZPrintDebugLogA("Add IDT item failed, failed to init PEBase for file:[%s]\n", kstrCurPEFile.c_str());
        UninitPEBase(true, false);
    }

    if (bRet)
    {
        if (IsDllExistInIDT(kstrInjectFileName))
        {
            HZPrintDebugLogA("The inject file:[%s] already exist in IDT list, no need inject again\n", kstrInjectFileName.c_str());
            UninitPEBase(true, false);
            bRet = true;
        }
        else
        {
            if (kstrNewSectionName.empty())
            {
                // This way(find free address to inject new IDT item) it is not works on all PE file.
                // Not all PE files have enough free address for inject and the free address calculate method is not stability
                HZPrintDebugLogA("The pass in new section name:[%s] is empty, find PE empty space to inject IDT item without extend section\n", kstrNewSectionName.c_str());

                bRet = false;
                std::map<int, int> mapSectionFreeSizeUseInfo;
                std::pair<BYTE*, int> pairNewIDT = FindFreeAddress(dwNewIDTSize, &mapSectionFreeSizeUseInfo);
                if (VerifyMVA((DADDR)(pairNewIDT.first), pairNewIDT.second))
                {
                    mapSectionFreeSizeUseInfo[pairNewIDT.second] = dwNewIDTSize;
                    std::pair<BYTE*, int> pairExtendInfo = FindFreeAddress(sizeof(STUExtendIDTInfo), &mapSectionFreeSizeUseInfo);
                    if (VerifyMVA((DADDR)(pairExtendInfo.first), pairExtendInfo.second))
                    {
                        InnerAddIDTItem(pairNewIDT.first, pairNewIDT.second, pairExtendInfo.first, pairExtendInfo.second,
                            kstrInjectFileName, kwHint, kstrInjectFuncName);

                        bRet = true;
                    }
                    else
                    {
                        HZPrintDebugLogA("Cannot find free address to store extend IDT info with size:[%zd]\n", sizeof(STUExtendIDTInfo));
                    }
                }
                else
                {
                    HZPrintDebugLogA("Cannot find free address to move IDT with size:[%ld]\n", dwNewIDTSize);
                }
                UninitPEBase(true, bRet);
            }
            else
            {
                HZPrintDebugLogA("The pass in new section name:[%s] inject IDT item with extend section\n", kstrNewSectionName.c_str());

                // Using a new section to add new IDT item
                // Add new section need extend file mapping, so unmap PE first and it will extend and mapping again in ExtendSection
                UninitPEBase(true, false);
                bRet = ExtendSection(kstrNewSectionName, dwFullDataSize, IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE, false);
                if (bRet)
                {
                    // New section info
                    int nNewSectionIndex = m_pstuFileHeader->NumberOfSections - 1;
                    PIMAGE_SECTION_HEADER pNewSectionHeader = m_pszSectionHeader + nNewSectionIndex;
                    BYTE* pbyNewSectionBody = m_pbyFileBase + pNewSectionHeader->PointerToRawData;

                    // New IDT size
                    size_t stNewIDTSize = m_pstuEntryImport->Size + sizeof(IMAGE_IMPORT_DESCRIPTOR);
                    InnerAddIDTItem(pbyNewSectionBody, nNewSectionIndex, pbyNewSectionBody + stNewIDTSize, nNewSectionIndex,
                        kstrInjectFileName, kwHint, kstrInjectFuncName);

                    // Unit mapping and save changes to file.
                    bRet = UninitPEBase(true, true);
                }
                else
                {
                    HZPrintDebugLogA("Extend section with name:[%s] size:[0x%x] failed\n", kstrNewSectionName.c_str(), dwFullDataSize);
                }
            }
        }
    }
    return bRet;
}

bool PEMgr::ExtendSection(_In_ const std::string& kstrSectionNameIn, _In_ const DWORD kdwSectionBodySizeIn, _In_ const DWORD kdwCharacteristicsIn, _In_ const bool kbAutoSave)
{
    bool bRet = false;
    
    //TODO: Need check section name, if the pass in section name is valid or already exist
    const std::string& kstrCurPEFile = GetMappingFilePath();
    bRet = InitPEBase(true, 0);
    if (bRet)
    {
        bRet = false;
        if (IsSectionExistInIDT(kstrSectionNameIn))
        {
            HZPrintDebugLogA("The section:[%s] already exist PE file:[%s], cannot extend a new section with same section name. Please change the name and do again\n", kstrSectionNameIn.c_str(), kstrCurPEFile.c_str());
            UninitPEBase(true, false);
        }
        else
        {
            // Calculate needed section size and remapping
            DWORD dwExtendSize = GetExtendSize(kdwSectionBodySizeIn);
            bool bNeedMoveHeader = IsNeedMoveHeader();
            if (bNeedMoveHeader)
            {
                // Move header
                bRet = MoveAndReInitNTHeaderToReplaceDOSStub(dwExtendSize);
            }
            else
            {
                bRet = ReInitPEBase(true, false, dwExtendSize);
            }
            if (bRet)
            {
                HZPrintDebugLogA("Current file:[%s] size:[0x%x], mapping size:[0x%x],ExtendSize:[0x%x], Align:[0x%x]\n",
                                        kstrCurPEFile.c_str(), GetRawFileSize(), GetMappingSize(), dwExtendSize, PEFileAlign);
                // Add new section
                bRet = AddNewSection(kstrSectionNameIn, kdwSectionBodySizeIn, kdwCharacteristicsIn);
                // This image size must be extend
                // SizeOfImage: the last section RVA align value
                m_pstuOptionalHeader->SizeOfImage += DADDR_TO_DWORD_S(GetAlignAddr(dwExtendSize, PESectionAlign, false));
            }
            else
            {
                HZPrintDebugLogA("Extend section failed, failed to remapping and init PEBase for file:[%s] with extend size:[%ld]\n",
                    kstrCurPEFile.c_str(), dwExtendSize);
            }
            if (kbAutoSave)
            {
                UninitPEBase(true, bRet);
            }
        }
    }
    else
    {
        HZPrintDebugLogA("Extend section failed, failed to init PEBase for file:[%s]\n", kstrCurPEFile.c_str());
        UninitPEBase(true, false);
    }
    return bRet;
}

bool PEMgr::InnerAddIDTItemEx(_In_ BYTE* pbyNewIDT, _In_ const int knNewIDTBelongSection,
    _In_ BYTE* pbyExtendInfo, _In_ const int knExtendInfoBelongSection,
    _In_ const std::string& kstrInjectFileName, _In_ const WORD kwHint, _In_ const std::string& kstrInjectFuncName) throw()
{
    bool bRet = false;

    // Parameters check
    if ((!IsValidExtendIDTDllName(kstrInjectFileName)) || (!IsValidExtendIDTFuncName(kstrInjectFuncName)))
    {
        return bRet;
    }
    if (IsDllExistInIDT(kstrInjectFileName))
    {
        // No need add
        bRet = true;
        HZPrintDebugLogA("The inject file:[%s] already exist in IDT list, no need add again\n", kstrInjectFileName.c_str());
    }
    else
    {
        InnerAddIDTItem(pbyNewIDT, knNewIDTBelongSection, pbyExtendInfo, knExtendInfoBelongSection, kstrInjectFileName, kwHint, kstrInjectFuncName);
        bRet = true;
    }
    return bRet;
}
void PEMgr::InnerAddIDTItem(_In_ BYTE* pbyNewIDT, _In_ const int knNewIDTBelongSection, 
    _In_ BYTE* pbyExtendInfo, _In_ const int knExtendInfoBelongSection,
    _In_ const std::string& kstrInjectFileName, _In_ const WORD kwHint, _In_ const std::string& kstrInjectFuncName) throw()
{
    // Move IDT
    // The new section first address is the new IDT start address
    size_t stNewIDTSize = m_pstuEntryImport->Size + sizeof(IMAGE_IMPORT_DESCRIPTOR);
    memset((BYTE*)(pbyNewIDT), 0, stNewIDTSize);
    if (0 != m_pstuEntryImport->VirtualAddress)
    {
        memcpy((BYTE*)(pbyNewIDT), m_pbyFileBase + RVAToOffset(m_pstuEntryImport->VirtualAddress, -1), m_pstuEntryImport->Size);
    }

    // Add new IDT item, the last IDT is empty as the IDT table end flag
    PIMAGE_IMPORT_DESCRIPTOR pNewImportDescriptor = PIMAGE_IMPORT_DESCRIPTOR((BYTE*)(pbyNewIDT)+stNewIDTSize - (sizeof(IMAGE_IMPORT_DESCRIPTOR) * 2));
    pNewImportDescriptor->Name = 0;
    pNewImportDescriptor->FirstThunk = 0;
    pNewImportDescriptor->OriginalFirstThunk = 0;
    pNewImportDescriptor->ForwarderChain = 0;
    pNewImportDescriptor->TimeDateStamp = 0;

    // Fill extend IDT item info
    // The extend IDT item info is following the new IDT table
    STUExtendIDTInfo* pstuExtendIDTInfo = (STUExtendIDTInfo*)(pbyExtendInfo);
    memset(pstuExtendIDTInfo, 0, sizeof(STUExtendIDTInfo));
    memcpy(pstuExtendIDTInfo->szDllName, kstrInjectFileName.c_str(), kstrInjectFileName.length());
    pstuExtendIDTInfo->wHint = kwHint;
    memcpy(pstuExtendIDTInfo->szFuncName, kstrInjectFuncName.c_str(), kstrInjectFuncName.length());
    DADDR dwAddressOfData = MappingAddrToRVA((DADDR)(&(pstuExtendIDTInfo->wHint)), knExtendInfoBelongSection);
    pstuExtendIDTInfo->pszThunkData[0].u1.AddressOfData = pstuExtendIDTInfo->pszThunkData[2].u1.AddressOfData = dwAddressOfData;

    // Connect new IDT item and extend IDT item
    pNewImportDescriptor->Name = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)pstuExtendIDTInfo->szDllName, knExtendInfoBelongSection));
    pNewImportDescriptor->FirstThunk = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)pstuExtendIDTInfo->pszThunkData, knExtendInfoBelongSection));
    pNewImportDescriptor->OriginalFirstThunk = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)(pstuExtendIDTInfo->pszThunkData + 2), knExtendInfoBelongSection));

    // Reset IDT address and size
    m_pstuEntryImport->VirtualAddress = DADDR_TO_DWORD_S(MappingAddrToRVA((DADDR)pbyNewIDT, knNewIDTBelongSection));
    m_pstuEntryImport->Size += sizeof(IMAGE_IMPORT_DESCRIPTOR);

    // Add write permission to the section which contains the extend IDT info
    m_pszSectionHeader[knNewIDTBelongSection].Characteristics |= IMAGE_SCN_MEM_WRITE;
}

std::pair<BYTE*, int> PEMgr::FindFreeAddress(_In_ const size_t kstNeededSize, _In_opt_ const std::map<int, int>* kpmapSectionFreeSizeUseInfo) const throw()
{
    std::pair<BYTE*, int>  pairFreeAddress = { nullptr, -1 };
    for (int i = 0; i < m_pstuFileHeader->NumberOfSections; ++i)
    {
        PESectionInfo* pCurSectionInfo = m_pszSectionInfo + i;
        DADDR dwCurSectionFreeSize = pCurSectionInfo->GetFreeSize();
        if (0 < dwCurSectionFreeSize)
        {
            // Get section free size using info
            int nFreeSizeUsingCount = 0;
            if (nullptr != kpmapSectionFreeSizeUseInfo)
            {
                std::map<int, int>::const_iterator kitr = kpmapSectionFreeSizeUseInfo->find(i);
                if (kpmapSectionFreeSizeUseInfo->cend() != kitr)
                {
                    nFreeSizeUsingCount = kitr->second;
                }
            }

            if ((kstNeededSize + nFreeSizeUsingCount) <= dwCurSectionFreeSize)
            {
                // Find free address for size kstNeededSize
                pairFreeAddress.first = (m_pbyFileBase + pCurSectionInfo->GetFreeOffset() + nFreeSizeUsingCount);
                pairFreeAddress.second = i;
                break;
            }
        }
    }
    return pairFreeAddress;
}

bool PEMgr::IsNeedMoveHeader() const throw()
{
#if false
    return true;
#else
    bool bRet = false;
    const DWORD dwPEHeaderAlignGap = GetPEHeaderAlignGap();
    if (dwPEHeaderAlignGap < sizeof(IMAGE_SECTION_HEADER))
    {
        // The gap do not enough to add the new header, need move and extend header
        bRet = true;
    }
    return bRet;
#endif
}
DWORD PEMgr::GetExtendSize(_In_ const DWORD kdwSectionBodySizeIn) const throw()
{
    return DADDR_TO_DWORD_S(GetAlignAddr(kdwSectionBodySizeIn, PEFileAlign, false));
}
bool PEMgr::MoveAndReInitNTHeaderToReplaceDOSStub(_In_ const DWORD kdwReInitExtendSize) throw()
{
    bool bRet = false;
    if (GetPEBaseInitFlag())
    {
        // Move current NTHeader to replace DOS stub
        DWORD nOffset = sizeof(IMAGE_DOS_HEADER);

        // Back up current NTHeader and sections info
        DWORD dwHeaderSizeWithoutDOS = m_pstuOptionalHeader->SizeOfHeaders - m_pstuDosHeader->e_lfanew;
        BYTE* pbyHeaderSizeWithoutDOS = new BYTE[dwHeaderSizeWithoutDOS];
        memcpy(pbyHeaderSizeWithoutDOS, m_pstuNTHeaders, dwHeaderSizeWithoutDOS);

        // Copy NTHeader and section to replace DOS stub
        BYTE* pbyNewPEHeader = PEFileBase + nOffset;
        memset(pbyNewPEHeader, 0, m_pstuOptionalHeader->SizeOfHeaders - nOffset);
        memcpy(pbyNewPEHeader, pbyHeaderSizeWithoutDOS, dwHeaderSizeWithoutDOS);

        // Set new NTHeader address
        m_pstuDosHeader->e_lfanew = DADDR_TO_LONG_S(nOffset);

        bRet = ReInitPEBase(true, true, kdwReInitExtendSize);
        delete[] pbyHeaderSizeWithoutDOS;
        pbyHeaderSizeWithoutDOS = nullptr;

        HZPrintDebugLogA("Move header to replace DOS stub at:[%d] with result:[%s]\n", nOffset, bRet?"success":"false");
    }
    else
    {
        HZPrintDebugLogA("Do not initialized when try to move PEFile header\n");
    }
    return bRet;
}

bool PEMgr::RebasePENTHeader() throw()
{
    const std::string& kstrCurPEFile = GetMappingFilePath();
    bool bRet = InitPEBase(true, 0);
    if (bRet)
    {
        bRet = MoveAndReInitNTHeaderToReplaceDOSStub(0);
    }
    else
    {
        HZPrintDebugLogA("Test move header failed, failed to init PEBase for file:[%s]\n", kstrCurPEFile.c_str());
    }
    return bRet;
}

bool PEMgr::AddNewSection(_In_ const std::string& kstrSectionNameIn, _In_ const DWORD kdwSectionBodySizeIn, _In_ const DWORD kdwCharacteristicsIn)
{
    bool bRet = false;
    if (GetPEBaseInitFlag() && (IsValidSectionName(kstrSectionNameIn)))
    {
        // Calculate address place for section body
        PIMAGE_SECTION_HEADER pNewSectionHeader = m_pszSectionHeader + m_pstuFileHeader->NumberOfSections;
        const DADDR dwNewSectionHeaderBeginOffset = MappingAddrToOffset((DADDR)pNewSectionHeader);
        const DADDR dwNewSectionHeaderEndOffset = dwNewSectionHeaderBeginOffset + sizeof(IMAGE_SECTION_HEADER);
        const DADDR dwNewSectionHeaderFileAlignOffset = GetAlignAddr(dwNewSectionHeaderEndOffset, m_pstuOptionalHeader->FileAlignment, false);
        if (dwNewSectionHeaderFileAlignOffset <= PEMappingSize)
        {
            int nBeginOffsetBelongSection = GetOffsetBelongSection(dwNewSectionHeaderBeginOffset);
            int nEndOffsetBelongSection = GetOffsetBelongSection(dwNewSectionHeaderEndOffset);
            if ((!IsValidSectionIndex(nBeginOffsetBelongSection)) && (!IsValidSectionIndex(nEndOffsetBelongSection)))
            {
                PIMAGE_SECTION_HEADER pOrgLastSectionHeader = m_pszSectionHeader + (m_pstuFileHeader->NumberOfSections - 1);
                PESectionInfo* pOrgLastSectionInfo = m_pszSectionInfo + (m_pstuFileHeader->NumberOfSections - 1);
                DADDR dwOrgLastSectionSize = pOrgLastSectionInfo->GetSectionSize();
                const DADDR dwNewSectionBodyOffset = max((dwNewSectionHeaderFileAlignOffset), (pOrgLastSectionHeader->PointerToRawData + dwOrgLastSectionSize));
                const DADDR dwNewSectionBodySizeFileAlignOffset = GetAlignAddr(kdwSectionBodySizeIn, m_pstuOptionalHeader->FileAlignment, false);
                if (dwNewSectionBodySizeFileAlignOffset <= SubSmall<DADDR>(PEMappingSize, dwNewSectionBodyOffset, 0))
                {
                    // Add new section header
                    memset(pNewSectionHeader, 0, sizeof(IMAGE_SECTION_HEADER));
                    memcpy(pNewSectionHeader->Name, kstrSectionNameIn.c_str(), kstrSectionNameIn.length());
                    pNewSectionHeader->Misc.VirtualSize = DADDR_TO_DWORD_S(dwNewSectionBodySizeFileAlignOffset);
                    pNewSectionHeader->VirtualAddress = DADDR_TO_DWORD_S(GetAlignAddr(pOrgLastSectionHeader->VirtualAddress + pOrgLastSectionHeader->SizeOfRawData, m_pstuOptionalHeader->SectionAlignment, false));
                    pNewSectionHeader->SizeOfRawData = kdwSectionBodySizeIn;
                    pNewSectionHeader->PointerToRawData = DADDR_TO_DWORD_S(dwNewSectionBodyOffset);
                    pNewSectionHeader->PointerToRelocations = 0;
                    pNewSectionHeader->PointerToLinenumbers = 0;
                    pNewSectionHeader->NumberOfRelocations = 0;
                    pNewSectionHeader->NumberOfLinenumbers = 0;
                    pNewSectionHeader->Characteristics = kdwCharacteristicsIn;
                    // Init new section body
                    memset(PEFileBase + pNewSectionHeader->PointerToRawData, 0, dwNewSectionBodySizeFileAlignOffset);
                    // Modify relation data
                    // The SizeOfHeaders is an align number of FileAlign, cannot change it
                    // Before we add a new section the invoke must make sure there are enough padding place for add a  new section
                    // The invoke can mover and re-init the NTHeader to replace DOS stub by MoveAndReInitNTHeaderToReplaceDOSStub to get more padding place
                    // m_pstuOptionalHeader->SizeOfHeaders += sizeof(IMAGE_SECTION_HEADER);
                    m_pstuFileHeader->NumberOfSections += 1;
                    // Re-init
                    bRet = ReInitPEBase(false, false, 0);
                }
                else
                {
                    HZPrintDebugLogA("Failed, no enough address for new section body. MappingSize:[%lx], HeaderAlign:[%p], BodySize:[%lx, %p], LastHeader:[%lx, %p]\n",
                        PEMappingSize, BYTE_POINT(dwNewSectionHeaderFileAlignOffset), kdwSectionBodySizeIn,
                        BYTE_POINT(dwNewSectionBodySizeFileAlignOffset),
                        pOrgLastSectionHeader->PointerToRawData, BYTE_POINT(dwOrgLastSectionSize));
                }
            }
            else
            {
                HZPrintDebugLogA("Failed, the new section address start from:[%p] to:[%p] is in exist section:[%x],[%x]\n",
                    BYTE_POINT(dwNewSectionHeaderBeginOffset), BYTE_POINT(dwNewSectionHeaderEndOffset), nBeginOffsetBelongSection, nEndOffsetBelongSection);
            }
        }
        else
        {
            HZPrintDebugLogA("Failed, header align offset:[%p] is above mapping size:[%ld], no enough place for section header\n", BYTE_POINT(dwNewSectionHeaderFileAlignOffset), PEMappingSize);
        }
    }
    else
    {
        HZPrintDebugLogA("Failed, do not initialized or the section name:[%s] above than:[%d] when try to add new section with size:[%ld], characteristics:[%lx]\n", 
            kstrSectionNameIn.c_str(), IMAGE_SIZEOF_SHORT_NAME, kdwSectionBodySizeIn, kdwCharacteristicsIn);
    }
    return bRet;
}

} /*< Namespace PEAssistant */
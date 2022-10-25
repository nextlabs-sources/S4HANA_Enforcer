#include "PETools.h"

static const DWORD DOS_MAGIC_NUMBER = 0x5A4D;   // MZ (LE)

namespace PEAssistant
{

bool IsValidExtendIDTDllName(_In_ const std::string& kstrDllNameIn)
{
    return ((0 < kstrDllNameIn.size()) && (g_knMaxImportDllNameLength > kstrDllNameIn.size()));
}
bool IsValidExtendIDTFuncName(_In_ const std::string& kstrFuncNameIn)
{
    return ((0 < kstrFuncNameIn.size()) && (g_knMaxImportFuncNameLength > kstrFuncNameIn.size()));
}

bool IsPEFile(_In_ const BYTE* kpbyPEBase)
{
    bool bRet = false;
    if (nullptr != kpbyPEBase)
    {
        PIMAGE_DOS_HEADER pstuDosHeader = (PIMAGE_DOS_HEADER)kpbyPEBase;
        if (DOS_MAGIC_NUMBER == pstuDosHeader->e_magic)
        {
            bRet = true;
        }
    }
    return bRet;
}
DADDR GetAlignAddr(_In_ const DADDR kdwCurAddr, _In_ const DWORD kdwAlignIn, _In_ const bool kbForceNext)
{
    DWORD dwResidue = (kdwCurAddr % kdwAlignIn);
    if (0 == dwResidue)
    {
        if (kbForceNext)
        {
            return kdwCurAddr - dwResidue + kdwAlignIn;
        }
        else
        {
            return kdwCurAddr;
        }
    }
    else
    {
        return kdwCurAddr - dwResidue + kdwAlignIn;
    }
}
DWORD GetAlignGap(_In_ const DADDR kdwCurAddr, _In_ const DWORD kdwAlignIn)
{
    DWORD dwResidue = (kdwCurAddr % kdwAlignIn);
    if (0 != dwResidue)
    {
        return kdwAlignIn - dwResidue;
    }
    return 0;
}
int GetOffsetBelongSection(_In_ const DADDR kdwOffset, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections)
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
int GetRVABelongSection(_In_ const DADDR kdwVirtualAddr, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections)
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
bool IsValidSectionName(_In_ const std::string& kstrSectionName)
{
    size_t stLength = kstrSectionName.length();
    if ((0 < stLength) && ((IMAGE_SIZEOF_SHORT_NAME) >= stLength))
    {
        return true;
    }
    return false;
}
std::string ConvertSectionNameToString(_In_ const char* pbySectionName)
{
    if (nullptr != pbySectionName)
    {
        return std::string(pbySectionName, IMAGE_SIZEOF_SHORT_NAME);
    }
    return "";
}

} /*< Namespace PEAssistant */
#include "PESectionInfo.h"
#include "PETools.h"
#include "MyLog.h"

namespace PEAssistant
{

PESectionInfo::PESectionInfo() : PESectionInfo(0, "", 0, 0, 0, 0)
{

}
PESectionInfo::PESectionInfo(_In_ const int nSectionIndexIn, _In_ const std::string& kstrSectionNameIn, _In_ const DADDR dwSectionSizeIn,
    _In_ const DADDR dwFreeOffsetIn, _In_ const DADDR dwFreeSizeIn, _In_ const DADDR dwGapOfAlignIn)
    : m_nSectionIndex(nSectionIndexIn), m_strSectionName(kstrSectionNameIn), m_dwSectionSize(dwSectionSizeIn),
    m_dwFreeOffset(dwFreeOffsetIn), m_dwFreeSize(dwFreeSizeIn), m_dwGapOfAlign(dwGapOfAlignIn)
{

}
PESectionInfo::~PESectionInfo()
{

}

void PESectionInfo::Init(PIMAGE_SECTION_HEADER pszSectionHeaderIn, _In_ const WORD kdwSectionIndexIn, _In_ const DWORD kdwFileAlignment, _In_ const BYTE* pbyFileBase) throw()
{
    m_nSectionIndex = kdwSectionIndexIn;
    // Note: PointerToRawData can be null
    PIMAGE_SECTION_HEADER pCurSectionHeader = pszSectionHeaderIn + kdwSectionIndexIn;
    if ((nullptr != pCurSectionHeader) && (0 != pCurSectionHeader->PointerToRawData))
    {
        m_strSectionName = ConvertSectionNameToString((char*)(pCurSectionHeader->Name));
        m_dwSectionSize = GetAlignAddr(pCurSectionHeader->SizeOfRawData, kdwFileAlignment, false);
        m_dwGapOfAlign = m_dwSectionSize - pCurSectionHeader->SizeOfRawData;
        m_dwFreeSize = CalculateSectionFreePlace(pCurSectionHeader, pbyFileBase, &m_dwFreeOffset);
    }
}
void PESectionInfo::PrintInfo(_In_ const std::string& kstrPreFix) const throw()
{
    HZPrintDebugLogA("%sSection info, Name:[%s] Index:[%d,%d], FreeSize:[%p], StartFreePoint:[%p], SectionSize:[%p]\n",
        kstrPreFix.c_str(), m_strSectionName.c_str(), m_nSectionIndex, m_nSectionIndex, BYTE_POINT(m_dwFreeSize),
        BYTE_POINT(m_dwFreeOffset), BYTE_POINT(m_dwSectionSize));
}
PESectionInfo PESectionInfo::EstablishSectionInfoBySectionHeader(PIMAGE_SECTION_HEADER pszSectionHeaderIn, 
    _In_ const WORD kdwSectionIndexIn, _In_ const DWORD kdwFileAlignment, _In_ const BYTE* pbyFileBase) throw()
{
    PESectionInfo obPESectioninfo(kdwSectionIndexIn, "", 0, 0, 0, 0);
    obPESectioninfo.Init(pszSectionHeaderIn, kdwSectionIndexIn, kdwFileAlignment, pbyFileBase);
    return obPESectioninfo;
}
DADDR PESectionInfo::CalculateSectionFreePlace(_In_ const PIMAGE_SECTION_HEADER pCurSectionHeader, _In_ const BYTE* pbyFileBase, _Inout_opt_ DADDR* pdwOffsetOut) throw()
{
    DADDR m_dwFreeSize = 0;
    // Calculate section free place
    DADDR dwCalFreeSize = 0;
    if (pCurSectionHeader->SizeOfRawData > pCurSectionHeader->Misc.VirtualSize)
    {
        dwCalFreeSize = pCurSectionHeader->SizeOfRawData - pCurSectionHeader->Misc.VirtualSize;
    }
    // Loop and check the free place
    // The Misc.VirtualSize value not always right, need loop and check
    DADDR dwLoopFreeSize = 0;
    BYTE* pCurSectionDataEnd = ((BYTE*)(pbyFileBase)) + (DWORD_TO_DADDR_S(pCurSectionHeader->PointerToRawData) + pCurSectionHeader->SizeOfRawData - 1);
    while (pCurSectionDataEnd[0] == 0)
    {
        dwLoopFreeSize++;
        pCurSectionDataEnd--;
    }
    // Decision free space size
    DADDR dwStartFreeOffset = 0;
    if ((0 < dwCalFreeSize) && (0 < dwLoopFreeSize) && (dwLoopFreeSize < pCurSectionHeader->SizeOfRawData))
    {
#if 1
        m_dwFreeSize = min(dwCalFreeSize, dwLoopFreeSize);
        // Because of Misc.VirtualSize and loop free size both are not 100% safe
        // Leave some spare to make sure it is save
        DADDR dwRemainderOf16 = (m_dwFreeSize % 16);
        if (4 <= dwRemainderOf16)
        {
            m_dwFreeSize -= dwRemainderOf16;
        }
        else
        {
            m_dwFreeSize -= (dwRemainderOf16 + 16);
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
        dwStartFreeOffset = DWORD_TO_DADDR_S(pCurSectionHeader->PointerToRawData) + pCurSectionHeader->SizeOfRawData - m_dwFreeSize;
    }
    if (nullptr != pdwOffsetOut)
    {
        *pdwOffsetOut = dwStartFreeOffset;
    }
    return m_dwFreeSize;
}

} /*< Namespace PEAssistant */
#pragma once
#include "PETools.h"

namespace PEAssistant
{

// Thread unsafe
class PESectionInfo
{
public:
    PESectionInfo(_In_ const int nSectionIndexIn, _In_ const std::string& kstrSectionNameIn, _In_ const DADDR dwSectionSizeIn,
        _In_ const DADDR dwFreeOffsetIn, _In_ const DADDR dwFreeSizeIn, _In_ const DADDR dwGapOfAlignIn);
    ~PESectionInfo();
private:
    PESectionInfo();

public:
    static PESectionInfo* GetPESectionInfoArray(_In_ const unsigned int knCount) { return knCount > 0 ? new PESectionInfo[knCount] : nullptr; }
    void Init(PIMAGE_SECTION_HEADER pszSectionHeaderIn, _In_ const WORD kdwSectionIndexIn, _In_ const DWORD kdwFileAlignment, _In_ const BYTE* pbyFileBase) throw();
    void PrintInfo(_In_ const std::string& kstrPreFix) const throw();
    
    inline DADDR GetSectionSize() const throw() { return m_dwSectionSize; }
    inline DADDR GetFreeOffset() const throw() { return m_dwFreeOffset; }
    inline DADDR GetFreeSize() const throw() { return m_dwFreeSize; }

private:
    static PESectionInfo EstablishSectionInfoBySectionHeader(PIMAGE_SECTION_HEADER pszSectionHeaderIn,
        _In_ const WORD kdwSectionIndexIn, _In_ const DWORD kdwFileAlignment, _In_ const BYTE* pbyFileBase) throw();
    static DADDR CalculateSectionFreePlace(_In_ const PIMAGE_SECTION_HEADER pCurSectionHeader, _In_ const BYTE* pbyFileBase, _Inout_opt_ DADDR* pdwOffsetOut) throw();

private:
    int m_nSectionIndex = -1;
    std::string m_strSectionName = "";
    DADDR m_dwSectionSize = 0;
    DADDR m_dwGapOfAlign = 0;
    DADDR m_dwFreeOffset = 0;
    DADDR m_dwFreeSize = 0;
};

} /*< Namespace PEAssistant */
#pragma once
#include <string>
#include <sal.h>
#include <windows.h>
#include "PETools.h"

namespace PEAssistant
{

// Thread unsafe
class PEIDTInjection
{
private:
    typedef struct _STUSectionInfo
    {
        int m_nSectionIndex;
        DADDR dwStartFreeOffset;
        DADDR m_dwFreeSize;
        DADDR m_dwSectionSize;
    }STUSectionInfo, * PSTUSectionInfo;

public:
    PEIDTInjection();
    ~PEIDTInjection();

public:
    static bool AddIDTItem(_In_ const std::string& kstrTargetFilePath, _In_ const std::string& kstrExtendFileName, _In_ const WORD kdwHint, _In_ const std::string& kstrExtendFuncName);

private:
    inline bool GetMappingFlag() const { return m_bStartMapping; }
    bool BeginTransaction(const _In_ std::string& kstrPEFilePath);
    // The name size must less than 8(include '\0')
    bool InnerAddIDTItem(_In_ const std::string& kwstrExtendFileName, _In_ const WORD kdwHint, _In_ const std::string& kwstrExtendFuncName) const;
    bool Submit();
    void PrintPEInfo() const throw();

    inline void SetMappingFlag(_In_ const bool kbInit) { m_bStartMapping = kbInit; }
    bool InitPEHeaders(_In_ const BYTE* kpPEBuf, _In_ const DWORD kdwSize);

    void Reset();

    bool IsValidSectionIndex(_In_ const int knBelongSectionIndex) const throw();
    
    bool VerifyMVA(_In_ const DADDR kdwMVA, _In_ const int knExpectSectionIndex) const throw();
    DADDR MappingAddrToRVA(_In_ const DADDR kdwVA, _In_ const int knBelongSectionIndex) const throw();
    DADDR MappingAddrToOffset(_In_ const DADDR kdwVA) const throw();
    DADDR OffsetToRVA(_In_ const DADDR kdwOffset, _In_ int nBelongSectionIndex) const throw();
    DADDR RVAToOffset(_In_ const DADDR kdwRVA, _In_ int nBelongSectionIndex) const throw();

public:
    static int GetOffsetBelongSection(_In_ const DADDR kdwOffset, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections);
    static int GetRVABelongSection(_In_ const DADDR kdwVirtualAddr, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections);

private:
    bool m_bStartMapping = false;
    std::string m_strPEFilePath = "";

    HANDLE m_hFile = nullptr;
    HANDLE m_hMapFile = nullptr;
    BYTE* m_pbyFileBase = nullptr;
    DWORD m_dwFileSize = 0;

    PIMAGE_DOS_HEADER m_pstuDosHeader = nullptr;
    PIMAGE_NT_HEADERS m_pstuNTHeaders = nullptr;

    PIMAGE_FILE_HEADER m_pstuFileHeader = nullptr;
    PIMAGE_OPTIONAL_HEADER m_pstuOptionalHeader = nullptr;
    DWORD m_dwFileAlignment = 0;

    PIMAGE_DATA_DIRECTORY m_pszStuDataDirectory = nullptr;
    PIMAGE_DATA_DIRECTORY m_pstuEntryImport = nullptr;    // Import directory

    PIMAGE_SECTION_HEADER m_pszSectionHeader = nullptr;
    int m_nFirstSection = 0;
    int m_nLastSection = 0;
    PSTUSectionInfo m_pszSectionInfo = nullptr;
};

} /*< Namespace PEAssistant */
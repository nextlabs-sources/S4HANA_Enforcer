#pragma once
#include "PEBase.h"
#include <map>

namespace PEAssistant
{

class PEMgr : private PEBase
{
public:
    PEMgr(_In_ const std::string& kstrPEFilePath);
    ~PEMgr();

public:
    bool AddIDTItem(_In_ const std::string& kstrInjectFileName, _In_ const WORD kwHint, _In_ const std::string& kstrInjectFuncName, _In_ const std::string& kstrNewIDTSectionName);
    // Before extend section directly, no need invoke start mapping and init PEBase
    // If kbAutoSave is false, the extend info do not save to PEFile, the invoke need to invoke UninitPEBase to free resource and save the extend info into PE file
    bool ExtendSection(_In_ const std::string& kstrSectionNameIn, _In_ const DWORD kdwSectionBodySizeIn, _In_ const DWORD kdwCharacteristicsIn, _In_ const bool kbAutoSave);

    bool RebasePENTHeader() throw();

private:
    bool InnerAddIDTItemEx(_In_ BYTE* pbyNewIDT, _In_ const int knNewIDTBelongSection, _In_ BYTE* pbyExtendInfo, _In_ const int knExtendInfoBelongSection,
        _In_ const std::string& kstrInjectFileName, _In_ const WORD kwHint, _In_ const std::string& kstrInjectFuncName) throw();
    void InnerAddIDTItem(_In_ BYTE* pbyNewIDT, _In_ const int knNewIDTBelongSection, _In_ BYTE* pbyExtendInfo, _In_ const int knExtendInfoBelongSection,
        _In_ const std::string& kstrInjectFileName, _In_ const WORD kwHint, _In_ const std::string& kstrInjectFuncName) throw();
    std::pair<BYTE*, int> FindFreeAddress(_In_ const size_t kstNeededSize, _In_opt_ const std::map<int, int>* kpmapSectionFreeSizeUseInfo) const throw();

    bool IsNeedMoveHeader() const throw();
    DWORD GetExtendSize(_In_ const DWORD kdwSectionBodySizeIn) const throw();
    // The NTHeader is follow DOSHeader and DOS stub(a sub program)
    // The DOS stub is an option info and can removed
    // So we can move forward the NTHeader to replace the DOS stub but cannot move the NTHeader back to others place.
    // The move able site is the DOS stub size: m_pstuDosHeader->e_lfanew - sizeof(IMAGE_DOS_HEADER)
    bool MoveAndReInitNTHeaderToReplaceDOSStub(_In_ const DWORD kdwReInitExtendSize) throw();
    bool AddNewSection(_In_ const std::string& kstrSectionNameIn, _In_ const DWORD kdwSectionBodySizeIn, _In_ const DWORD kdwCharacteristicsIn);

};

} /*< Namespace PEAssistant */
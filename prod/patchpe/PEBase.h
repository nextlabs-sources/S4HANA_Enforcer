#pragma once
#include <string>
#include <sal.h>
#include <windows.h>
#include "PETools.h"
#include "PESectionInfo.h"
#include "FileMappingBase.h"

// Function helper
#define MVAToOffset(kdwAddr) (MappingAddrToOffset((DADDR)(kdwAddr)))
#define MVAToOffsetPoint(kdwAddr) (BYTE_POINT(MVAToOffset(kdwAddr)))
#define PEFileBase (GetMappingFileBase())
#define PEMappingSize (GetMappingSize())
#define PEFileAlign (m_pstuOptionalHeader->FileAlignment)
#define PESectionAlign (m_pstuOptionalHeader->SectionAlignment)

namespace PEAssistant
{
// PEBase 拆分位两部分: FileMappingBase 和 PEBase
// PEBase 继承 FileMappingBase
// 原则上 FileMappingBase 和 PEBase 一个对应一个文件, 构造函数保存文件名
// PEBase 的初始化拆分为, mapping init, PE header init, PE section init
// 在一次mapping 中 可以多次初始化 Header, 一次 headermapping 中, 可以多次初始化 section
// PEBase PE header 的 init 提供 reset, reinit 工具
// 所有函数中都应该执行 init flag 检查, 提供 宏 插入到每个函数最前端
// 原则上 FileMappingBase 成员 private, 经常需要用到 filebase, mapingsize, filesize 提供宏简化
// FileMappingBase 提供 校验相关工具函数
// 原则上 PEBase 成员 protected, 提供基本解析函数
// Thread unsafe
class PEBase : public FileMappingBase
{
protected:
    PEBase(_In_ const std::string& kstrPEFilePath);
    ~PEBase();

    // Init
public:
    bool InitPEBase(_In_ const bool kbAutoStartMapping, _In_ const DWORD kdwExtendSize);
    bool UninitPEBase(_In_ const bool kbStopMapping, _In_ const bool kbSubmit);
    bool ReInitPEBase(_In_ const bool kbAutoReMapping, _In_ const bool kbSubmit, _In_ const DWORD kdwExtendSize);
    inline bool GetPEBaseInitFlag() const { return m_bInitPEBaseFlag; }

private:
    PESectionInfo* InitSectionInfo(_In_ const PIMAGE_SECTION_HEADER pszSectionHeader, _In_ const WORD kdwSectionNumber) const throw();
    void ResetPEBaseInfo();
    inline void SetPEBaseInitFlag(_In_ const bool kbInitPEBaseFlag) { m_bInitPEBaseFlag = kbInitPEBaseFlag; }

    // Base tools
public:
    void PrintPEInfo() const throw();
    static bool IsImageSupport(_In_ const BYTE* kpbyPEBase) throw();

    DWORD GetPEHeaderSize(_In_ const unsigned int knHeaderType /*EMHeaderType*/) const throw();
    DWORD GetPEHeaderAlignGap() const throw();
    
    // If knStartSectionOffset is -1, auto set data to the end of original RawDataSize
    bool SetDataToSection(_In_ const int knSectionIndexIn, _In_ int knStartSectionOffsetIn, _In_ const BYTE* pbyNewDataIn, _In_ const DADDR kdwDataSizeIn) const throw();
    int GetOffsetBelongSection(_In_ const DADDR kdwOffset) const throw();
    int GetRVABelongSection(_In_ const DADDR kdwVirtualAddr) const throw();

    bool IsValidSectionIndex(_In_ const int knBelongSectionIndex) const throw();
    bool VerifyMVA(_In_ const DADDR kdwMVA, _In_ const int knExpectSectionIndex) const throw();
    bool IsDllExistInIDT(_In_ const std::string& kstrDllNameIn) const throw();
    bool IsSectionExistInIDT(_In_ const std::string& kstrSectionNameIn) const throw();
    int GetSectionIndexByName(_In_ const std::string& kstrSectionNameIn) const throw();

    DADDR MappingAddrToRVA(_In_ const DADDR kdwVA, _In_ const int knBelongSectionIndex) const throw();
    DADDR MappingAddrToOffset(_In_ const DADDR kdwVA) const throw();
    DADDR OffsetToRVA(_In_ const DADDR kdwOffset, _In_ int nBelongSectionIndex) const throw();
    DADDR RVAToOffset(_In_ const DADDR kdwRVA, _In_ int nBelongSectionIndex) const throw();

protected:
    PIMAGE_DOS_HEADER m_pstuDosHeader = nullptr;
    PIMAGE_NT_HEADERS m_pstuNTHeaders = nullptr;

    PIMAGE_FILE_HEADER m_pstuFileHeader = nullptr;
    PIMAGE_OPTIONAL_HEADER m_pstuOptionalHeader = nullptr;

    PIMAGE_DATA_DIRECTORY m_pszStuDataDirectory = nullptr;
    PIMAGE_DATA_DIRECTORY m_pstuEntryImport = nullptr;    // Import directory

    // Section default is in ordered
    PIMAGE_SECTION_HEADER m_pszSectionHeader = nullptr;
    PESectionInfo* m_pszSectionInfo = nullptr;

private:
    bool m_bInitPEBaseFlag = false;
};

} /*< Namespace PEAssistant */
#pragma once
#include <string>
#include <sal.h>
#include <windows.h>
#include "PETools.h"
#include "PESectionInfo.h"

namespace PEAssistant
{
// PEBase 拆分位两部分: FileMappingBase 和 PEBase
// PEBase 的初始化拆分为, mapping init 和 PE header init
// PEBase PE header 的 init 提供 reset, reinit 工具
// 所有函数中都应该执行 init flag 检查, 提供 宏 插入到每个函数最前端
// Thread unsafe
class FileMappingBase
{
protected:
    FileMappingBase(_In_ const std::string& kstrPEFilePath);
    ~FileMappingBase();

public:
    bool StartMapping(_In_ const DWORD kdwSize);
    bool StopMapping(_In_ const bool kbSubmit);

    inline bool GetMappingFlag() const { return m_bStartMapping; }
    inline DWORD GetMappingSize() const throw() { return m_dwMappingSize; }
    inline const std::string& GetMappingFilePath() const throw() { return m_kstrPEFilePath; }
    inline BYTE* GetMappingFileBase() const throw() { return m_pbyFileBase; }
    inline DWORD GetRawFileSize() const throw() { return m_dwFileSize; }

private:
    inline void SetMappingFlag(_In_ const bool kbStartMapping) { m_bStartMapping = kbStartMapping; }
    void ResetMappingInfo();

private:
    bool m_bStartMapping = false;

protected:
    const std::string m_kstrPEFilePath = "";
    
    HANDLE m_hFile = nullptr;
    HANDLE m_hMapFile = nullptr;
    BYTE* m_pbyFileBase = nullptr;
    DWORD m_dwMappingSize = 0;
    DWORD m_dwFileSize = 0;
};

} /*< Namespace PEAssistant */
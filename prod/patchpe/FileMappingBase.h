#pragma once
#include <string>
#include <sal.h>
#include <windows.h>
#include "PETools.h"
#include "PESectionInfo.h"

namespace PEAssistant
{
// PEBase ���λ������: FileMappingBase �� PEBase
// PEBase �ĳ�ʼ�����Ϊ, mapping init �� PE header init
// PEBase PE header �� init �ṩ reset, reinit ����
// ���к����ж�Ӧ��ִ�� init flag ���, �ṩ �� ���뵽ÿ��������ǰ��
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
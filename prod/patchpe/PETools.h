#pragma once

#include <string>
#include <windows.h>

#define BYTE_POINT(kdwAddr) ((BYTE*)(kdwAddr))

// For some PE structures, it only define DWORD to save a pointer address
// For these structure values, it is save to do convert
// Provider follow macro to avoid warnings
#define DWORD_TO_DADDR_S(kdwAddr) ((DADDR)(kdwAddr))
#define DADDR_TO_DWORD_S(kdwAddr) ((DWORD)(kdwAddr))
#define DADDR_TO_LONG_S(kdwAddr) ((LONG)(kdwAddr))

#ifdef _WIN64
typedef ULONGLONG DADDR;
#else
typedef DWORD DADDR;
#endif

namespace PEAssistant
{

static const int g_knMaxImportDllNameLength = 16;
static const int g_knMaxImportFuncNameLength = 8;
// Size: 16 + size(IMAGE_THUNK_DATA) * 2 + 4 +  8 = 28 + (8/16)
// 32bit: 36
// 64bit: 44
struct STUExtendIDTInfo
{
    CHAR szDllName[g_knMaxImportDllNameLength];  // include "\0"
    IMAGE_THUNK_DATA pszThunkData[4];

    // An IMAGE_IMPORT_BY_NAME
    WORD   wHint;
    CHAR   szFuncName[g_knMaxImportFuncNameLength];
};

enum class EMHeaderType
{
    emHeaderType_Unknwon = 0,

    emHeaderType_DOS = 0x1,
    emHeaderType_DOSStub = 0x2,
    emHeaderType_NTHeader = 0x4,
    emHeaderType_NTFileHeader = 0x8,
    emHeaderType_NTOptionHeader = 0x10,
    emHeaderType_SectionHeader = 0x20,
    emHeaderType_HeaderPadding = 0x40,
    emHeaderType_All = 0xFFFF
};

bool IsValidExtendIDTDllName(_In_ const std::string& kstrDllNameIn);
bool IsValidExtendIDTFuncName(_In_ const std::string& kstrFuncNameIn);

bool IsPEFile(_In_ const BYTE* kpbyPEBase);
DADDR GetAlignAddr(_In_ const DADDR kdwCurAddr, _In_ const DWORD kdwAlignIn, _In_ const bool kbForceNext);
DWORD GetAlignGap(_In_ const DADDR kdwCurAddr, _In_ const DWORD kdwAlignIn);
int GetOffsetBelongSection(_In_ const DADDR kdwOffset, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections);
int GetRVABelongSection(_In_ const DADDR kdwVirtualAddr, _In_ const PIMAGE_SECTION_HEADER pSectionHeader, _In_ const int knNumberOfSections);
bool IsValidSectionName(_In_ const std::string& kstrSectionName);
std::string ConvertSectionNameToString(_In_ const char* pbySectionName);

} /*< Namespace PEAssistant */
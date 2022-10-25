#pragma once
#include <windows.h>

#ifdef _WIN64
typedef ULONGLONG DADDR;
#else
typedef DWORD DADDR;
#endif

static const int g_knMaxImportDllNameLength = 16;
static const int g_knMaxImportFuncNameLength = 8;
// Size: 16 + (size(IMAGE_THUNK_DATA) * 4) + 4 +  8 = 28 + (4/8) * 4
// 32bit: 44 => 0x2c => Align with 4 => 0x2c
// 64bit: 60 => 0x3c => Align with 8 => 0x40
typedef struct _STUExtendIDTInfo
{
    CHAR szDllName[g_knMaxImportDllNameLength];  // include "\0"
    IMAGE_THUNK_DATA pszThunkData[4];

    // An IMAGE_IMPORT_BY_NAME
    WORD   wHint;
    CHAR   szFuncName[g_knMaxImportFuncNameLength];
}STUExtendIDTInfo, *PSTUExtendIDTInfo;

typedef struct _STUSectionInfo
{
    int nSectionIndex;
    DADDR dwStartFreeOffset;
    DADDR dwFreeSize;
}STUSectionInfo, *PSTUSectionInfo;
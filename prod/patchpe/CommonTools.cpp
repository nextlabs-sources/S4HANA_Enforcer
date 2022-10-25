#include "CommonTools.h"
#include "MyLog.h"

bool g_bOutputDebugLog = false;

void HZCloseHandle(_Inout_opt_ HANDLE* phHandleIn)
{
    if ((nullptr != phHandleIn))
    {
        if ((nullptr != *phHandleIn) && (INVALID_HANDLE_VALUE != *phHandleIn))
        {
            CloseHandle(*phHandleIn);
            *phHandleIn = nullptr;
        }
    }
}
void HZFreeResource(_Inout_opt_ void** ppResourceIn)
{
    if (nullptr != ppResourceIn)
    {
        if (nullptr != *ppResourceIn)
        {
            delete[] (*ppResourceIn);
            *ppResourceIn = nullptr;
        }
    }
}

DWORD VirtualAddrToOffSet(_In_ const DWORD dwVirtualAddr, _In_ const BYTE* pFileImage)
{
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileImage;
    PIMAGE_NT_HEADERS pstuNTHeaders = (PIMAGE_NT_HEADERS)(pFileImage + pDosHeader->e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = &(pstuNTHeaders->FileHeader);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = &(pstuNTHeaders->OptionalHeader);
    PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(((char*)pOptionalHeader) + sizeof(IMAGE_OPTIONAL_HEADER));

    PIMAGE_SECTION_HEADER pSectionBelong = NULL;
    for (int i = 0; i < pFileHeader->NumberOfSections; i++)
    {
        if (pSectionHeader[i].VirtualAddress <= dwVirtualAddr && pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData >= dwVirtualAddr)
        {
            pSectionBelong = &(pSectionHeader[i]);
            break;
        }
    }
    if (pSectionBelong == NULL)
    {
        return 0;
    }
    else
    {
        return pSectionBelong->PointerToRawData + dwVirtualAddr - pSectionBelong->VirtualAddress;
    }
}

DWORD OffsetToVirtuanAddr(_In_ const DWORD dwOffset, _In_ const BYTE* pFileImage)
{
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileImage;
    PIMAGE_NT_HEADERS pstuNTHeaders = (PIMAGE_NT_HEADERS)(pFileImage + pDosHeader->e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = &(pstuNTHeaders->FileHeader);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = &(pstuNTHeaders->OptionalHeader);
    PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(((char*)pOptionalHeader) + sizeof(IMAGE_OPTIONAL_HEADER));

    PIMAGE_SECTION_HEADER pSectionBelong = NULL;
    for (int i = 0; i < pFileHeader->NumberOfSections; i++)
    {
        if (pSectionHeader[i].PointerToRawData <= dwOffset && pSectionHeader[i].PointerToRawData + pSectionHeader[i].SizeOfRawData >= dwOffset)
        {
            pSectionBelong = &(pSectionHeader[i]);
            break;
        }
    }
    if (pSectionBelong == NULL)
    {
        return 0;
    }
    else
    {
        return pSectionBelong->VirtualAddress + dwOffset - pSectionBelong->PointerToRawData;
    }
}


DWORD AlignSize(_In_ const DWORD nSize, _In_ const DWORD nAlign)
{
    return ((nSize + nAlign - 1) / nAlign * nAlign);
}

#ifndef _WIN64
BOOL AddSection(PIMAGE_NT_HEADERS pNtHdrs, DWORD dwRawSize)
{
    PIMAGE_SECTION_HEADER pNewSec = NULL;
    PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNtHdrs);
    PIMAGE_SECTION_HEADER pLastSec = NULL;

    DWORD dwNewSecAlign;
    DWORD dwNewFileAlign;

    //
    pNewSec = (PIMAGE_SECTION_HEADER)
        ((DWORD)pSec + pNtHdrs->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));

    pLastSec = (PIMAGE_SECTION_HEADER)
        ((DWORD)pNewSec - sizeof(IMAGE_SECTION_HEADER));

    dwNewSecAlign = AlignSize(
        pLastSec->VirtualAddress + pLastSec->Misc.VirtualSize,
        pNtHdrs->OptionalHeader.SectionAlignment
    );
    HZPrintDebugLogA("dwNewSecAlign : 0x%08X.\n", dwNewSecAlign);

    dwNewFileAlign = AlignSize(
        pLastSec->PointerToRawData + pLastSec->SizeOfRawData,
        pNtHdrs->OptionalHeader.FileAlignment
    );
    HZPrintDebugLogA("dwNewFileAlign : 0x%08X.\n", dwNewFileAlign);

    //modify SizeOfImage
    pNtHdrs->OptionalHeader.SizeOfImage += AlignSize(
        dwRawSize, pNtHdrs->OptionalHeader.SectionAlignment);

    memcpy(pNewSec->Name, ".VirWizd", 8);
    pNewSec->VirtualAddress = dwNewSecAlign;
    pNewSec->Misc.VirtualSize = dwRawSize;
    pNewSec->PointerToRawData = dwNewFileAlign;
    pNewSec->Characteristics =
        IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_WRITE;

    pNewSec->SizeOfRawData = AlignSize(
        dwRawSize,
        pNtHdrs->OptionalHeader.FileAlignment
    );
    pNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
    pNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
    pNtHdrs->FileHeader.NumberOfSections++;
    return TRUE;
}
#endif
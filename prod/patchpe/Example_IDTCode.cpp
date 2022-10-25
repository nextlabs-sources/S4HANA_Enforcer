#pragma once
#include <Windows.h>
#include "MyLog.h"
#include "CommonTypes.h"
#include "CommonTools.h"

class CMapFile
{
public:
    CMapFile(LPCTSTR pPath, bool bWrite, PVOID& pMap, DWORD& dwFileSize);
    ~CMapFile(void);
private:
    HANDLE hFileHandle;
    HANDLE hFileMapHandle;
    PVOID& pImageView;
    DWORD& dwSize;
};
CMapFile::CMapFile(LPCTSTR pPath, bool bWrite, PVOID& pMap, DWORD& dwFileSize)
    :pImageView(pMap)
    , dwSize(dwFileSize)

{
    this->hFileHandle = INVALID_HANDLE_VALUE;
    this->hFileMapHandle = INVALID_HANDLE_VALUE;
    this->pImageView = NULL;
    this->dwSize = 0;
    try
    {
        if (bWrite)
        {
            this->hFileHandle = CreateFile(pPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        }
        else
        {
            this->hFileHandle = CreateFile(pPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        }
        if (this->hFileHandle == INVALID_HANDLE_VALUE)
        {
            throw (0);
        }
        this->dwSize = GetFileSize(this->hFileHandle, NULL);
        if (this->dwSize == 0)
        {
            throw (1);
        }
        if (bWrite)
        {
            this->hFileMapHandle = CreateFileMapping(this->hFileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
        }
        else
        {
            this->hFileMapHandle = CreateFileMapping(this->hFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
        }
        if (this->hFileMapHandle == INVALID_HANDLE_VALUE)
        {
            throw (2);
        }
        CloseHandle(this->hFileHandle);
        this->hFileHandle = INVALID_HANDLE_VALUE;
        if (bWrite)
        {
            this->pImageView = MapViewOfFile(this->hFileMapHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
        }
        else
        {
            this->pImageView = MapViewOfFile(this->hFileMapHandle, FILE_MAP_READ, 0, 0, 0);
        }
        if (this->pImageView == NULL)
        {
            throw (4);
        }
    }
    catch (...)
    {
        this->pImageView = NULL;
    }

}
CMapFile::~CMapFile(void)
{
    try
    {
        if (this->pImageView != NULL)
        {
            UnmapViewOfFile(this->pImageView);
        }
        if (this->hFileMapHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(this->hFileMapHandle);
        }
        if (this->hFileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(this->hFileHandle);
        }
    }
    catch (...)
    {
    }
}

int ImportNewDll(const wchar_t* kpwchTargetPath, const char* kpchExtendDllName)
{
    DWORD dwFileSize = 0;
    LPVOID pPeImage = NULL;
    CMapFile cMapFile(kpwchTargetPath, true, pPeImage, dwFileSize);
    if (pPeImage != NULL)
    {
        BYTE* pFileImage = (BYTE*)pPeImage;
        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileImage;
        PIMAGE_NT_HEADERS pstuNTHeaders = (PIMAGE_NT_HEADERS)(pFileImage + pDosHeader->e_lfanew);
        PIMAGE_OPTIONAL_HEADER pOptionalHeader = &(pstuNTHeaders->OptionalHeader);
        PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(((char*)pOptionalHeader) + sizeof(IMAGE_OPTIONAL_HEADER));

        // 查找 节点
        int nTextSectionIndex = -1;
        for (int i = 0; nullptr != pSectionHeader[i].Name && '\0' != pSectionHeader[i].Name[0]; ++i)
        {
            // if (0 == _stricmp(".text", (const char*)(pSectionHeader[i].Name)))
            if (0 == _stricmp(".rdata", (const char*)(pSectionHeader[i].Name)))
                // if (0 == _stricmp(".reloc", (const char*)(pSectionHeader[i].Name)))
            {
                nTextSectionIndex = i;
                break;
            }
        }
        if (-1 == nTextSectionIndex)
        {
            HZPrintLogW(L"Cannot find text session header\n");
            return -1;
        }

        //查找 节中的空余信息
        PIMAGE_SECTION_HEADER pIDTSectionHeader = pSectionHeader + nTextSectionIndex;
        // 系统加载时需要用INT的指针写入IAT
        // 原始PE此节就没有可写权限, 添加写权限
        pIDTSectionHeader->Characteristics |= IMAGE_SCN_MEM_WRITE;

        BYTE* pRDataEnd = pFileImage + pIDTSectionHeader->PointerToRawData + pIDTSectionHeader->SizeOfRawData - 1;
        UINT nPadSize = 0;
        while (pRDataEnd[0] == 0)
        {
            nPadSize++;
            pRDataEnd--;
        }
        nPadSize--;

        BYTE* pPadStart = ++++pRDataEnd + 4;
        PIMAGE_IMPORT_DESCRIPTOR pOriginalImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(pFileImage + VirtualAddrToOffSet(pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, pFileImage));
        int nImportDescriptor = 0;
        do
        {
            nImportDescriptor++;
        } while (pOriginalImportDescriptor[nImportDescriptor].Characteristics != 0);
        DWORD dwBufferSize = sizeof(IMAGE_IMPORT_DESCRIPTOR) * nImportDescriptor;
        if (nPadSize > dwBufferSize + sizeof(IMAGE_IMPORT_DESCRIPTOR))
        {
            // Copy original IDT
            memcpy(pPadStart, pOriginalImportDescriptor, dwBufferSize);
            memset(pOriginalImportDescriptor, 0, dwBufferSize);

            // New import item
            PIMAGE_IMPORT_DESCRIPTOR pImportDescriptorAdded = PIMAGE_IMPORT_DESCRIPTOR(pPadStart + dwBufferSize);

#if 1
            pImportDescriptorAdded->Name = 0;
            pImportDescriptorAdded->FirstThunk = 0; // OffsetToVirtuanAddr((BYTE*)(pOriginalImportDescriptor + 1) - pFileImage, pFileImage);
            pImportDescriptorAdded->OriginalFirstThunk = 0;// OffsetToVirtuanAddr((BYTE*)(pOriginalImportDescriptor + 1) - pFileImage, pFileImage);
            pImportDescriptorAdded->ForwarderChain = 0;
            pImportDescriptorAdded->TimeDateStamp = 0;

            BYTE* pBufHead = (BYTE*)(pPadStart + dwBufferSize + sizeof(IMAGE_IMPORT_DESCRIPTOR) * 3);

            // 将原始的IDT表空间用于存储数据
            // DLL 名称
            strcpy_s((char*)pBufHead, strlen(kpchExtendDllName) + 1, kpchExtendDllName);
            DWORD dwDLLNameRVA = OffsetToVirtuanAddr((BYTE*)pBufHead - pFileImage, pFileImage);
            pImportDescriptorAdded->Name = dwDLLNameRVA;
            pBufHead += strlen(kpchExtendDllName) + 1;

            // Thunk 数组, INT, IAT
            PIMAGE_THUNK_DATA pThunkDataINT = (PIMAGE_THUNK_DATA)(pBufHead);
            memset(pThunkDataINT, 0, sizeof(IMAGE_THUNK_DATA) * 2);
            pImportDescriptorAdded->OriginalFirstThunk = OffsetToVirtuanAddr((BYTE*)(pThunkDataINT)-pFileImage, pFileImage);
            pBufHead += sizeof(IMAGE_THUNK_DATA) * 2;

            PIMAGE_THUNK_DATA pThunkDataIAT = (PIMAGE_THUNK_DATA)(pBufHead);
            memset(pThunkDataIAT, 0, sizeof(IMAGE_THUNK_DATA) * 2);
            pImportDescriptorAdded->FirstThunk = OffsetToVirtuanAddr((BYTE*)(pThunkDataIAT)-pFileImage, pFileImage);
            pBufHead += sizeof(IMAGE_THUNK_DATA) * 2;

            // 填充 Thunk 数组,IMAGE_IMPORT_BY_NAME 的信息紧跟在 Thunk 数组之后 
            pThunkDataINT->u1.AddressOfData = pThunkDataIAT->u1.AddressOfData = (DADDR)OffsetToVirtuanAddr((BYTE*)(pBufHead)-pFileImage, pFileImage);

            PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)(pBufHead);
            const char* kpchTestFuncName = "Test";
            size_t stNameLen = strlen(kpchTestFuncName) + 1;
            pImportByName->Hint = 0;
            strcpy_s((char*)pImportByName->Name, stNameLen, kpchTestFuncName);

#if 0
            // Thunk array
            PIMAGE_THUNK_DATA pThunkData = (PIMAGE_THUNK_DATA)(pBufHead);
            memset(pThunkData, 0, sizeof(IMAGE_THUNK_DATA) * 2);
            pBufHead += sizeof(IMAGE_THUNK_DATA) * 2;
            PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)(pBufHead);
            memset(pImportByName, 0, sizeof(IMAGE_IMPORT_BY_NAME));

            // Save the pointer to first thunk
            DWORD pThunkDataRVA = OffsetToVirtuanAddr((BYTE*)pImportByName - pFileImage, pFileImage);
            pThunkData->u1.AddressOfData = (DADDR)pThunkDataRVA;
            pImportDescriptorAdded->FirstThunk = pImportDescriptorAdded->OriginalFirstThunk = OffsetToVirtuanAddr((BYTE*)(pThunkData)-pFileImage, pFileImage);

            // Fill import names
            pImportByName->Hint = 0;
            const char* kpchTestFuncName = "Test";
            strcpy_s((char*)pImportByName->Name, strlen(kpchTestFuncName) + 1, kpchTestFuncName);
#endif
#endif

#if 0
            // 将原始的IDT表空间用于存储数据
            // DLL 名称
            strcpy_s((char*)pOriginalImportDescriptor, strlen(kpchExtendDllName) + 1, kpchExtendDllName);

            // 导入表
            PIMAGE_IMPORT_BY_NAME  pImportByName = (PIMAGE_IMPORT_BY_NAME)((char*)(pOriginalImportDescriptor + strlen(kpchExtendDllName) + 1));
            // 清空
            // 32 位, 程序如果导入表位空, 不导入任何符号
            // 64 位, 导入表为空, DLL依旧加载
            memset(pImportByName, 0, sizeof(IMAGE_IMPORT_BY_NAME));
            // 填写导入表, 如果不填写, 导入表中就没有任何符号, DLL 也不会被加载
            DWORD pIMAGE_THUNK_DATA = OffsetToVirtuanAddr((BYTE*)pImportByName - pFileImage, pFileImage);
            memcpy((char*)(pOriginalImportDescriptor + 1), &pIMAGE_THUNK_DATA, sizeof(void*));
            pImportByName->Hint = 0;
            const char* kpchTestFuncName = "Test";
            strcpy_s((char*)pImportByName->Name, strlen(kpchTestFuncName) + 1, kpchTestFuncName);

            pImportDescriptorAdded->Name = OffsetToVirtuanAddr((BYTE*)pOriginalImportDescriptor - pFileImage, pFileImage);
            pImportDescriptorAdded->FirstThunk = OffsetToVirtuanAddr((BYTE*)(pOriginalImportDescriptor + 1) - pFileImage, pFileImage);
            pImportDescriptorAdded->OriginalFirstThunk = OffsetToVirtuanAddr((BYTE*)(pOriginalImportDescriptor + 1) - pFileImage, pFileImage);
            pImportDescriptorAdded->ForwarderChain = 0;
            pImportDescriptorAdded->TimeDateStamp = 0;
#endif

            pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = OffsetToVirtuanAddr(pPadStart - pFileImage, pFileImage);
            pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size += sizeof(IMAGE_IMPORT_DESCRIPTOR);
            printf("Success\n");
        }
        else
        {
            printf("No enough spare space\n");
        }
    }
    else
    {
        printf("Map File Error\n");//IMAGE_IMPORT_DIRECTORY
    }
    return 0;
}
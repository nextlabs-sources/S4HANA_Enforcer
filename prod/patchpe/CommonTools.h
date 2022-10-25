#pragma once

#include <windows.h>

#define HZEmOpEq(emA, emB) (((unsigned int)(emA)) == ((unsigned int)(emB)))
#define HZEmOpOr(emA, emB) (((unsigned int)(emA)) | ((unsigned int)(emB)))
#define HZEmOpAnd(emA, emB) (((unsigned int)(emA)) & ((unsigned int)(emB)))

#define HZIsValidHandle(hHandleIn) ((nullptr != (hHandleIn)) && (INVALID_HANDLE_VALUE != (hHandleIn)))


void HZCloseHandle(_Inout_opt_ HANDLE* phHandleIn);
void HZFreeResource(_Inout_opt_ void** ppResourceIn);

template<typename tyNum>
tyNum SubSmall(_In_ const tyNum& ktyNumBig, _In_ const tyNum& ktyNumSmall, _In_ const tyNum& ktyNumDefault) throw()
{
    if (ktyNumBig > ktyNumSmall)
    {
        return ktyNumBig - ktyNumSmall;
    }
    return ktyNumDefault;
}


DWORD VirtualAddrToOffSet(_In_ const DWORD dwVirtualAddr, _In_ const BYTE* pFileImage);

DWORD OffsetToVirtuanAddr(_In_ const DWORD dwOffset, _In_ const BYTE* pFileImage);

DWORD AlignSize(_In_ const DWORD nSize, _In_ const DWORD nAlign);

BOOL AddSection(PIMAGE_NT_HEADERS pNtHdrs, DWORD dwRawSize);
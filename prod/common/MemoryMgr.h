#pragma once
#include <string>
#include <map>
#include <list>
#include <shared_mutex>
#include <thread>
#include <sal.h>    // _In_, _Inout_, _Out_
#include <windows.h>

#define NLNew(type, size) ((type *)g_MemoryMgr.NewBufEx((sizeof(type) * (size)), __FILE__, __FUNCDNAME__, __LINE__))
#define NLDelete(pAddr) (g_MemoryMgr.DeleteBufEx((pAddr), __FILE__, __FUNCDNAME__, __LINE__))

typedef std::shared_mutex rwmutex;
typedef std::shared_lock<rwmutex> readLock;
typedef std::unique_lock<rwmutex> writeLock;

#ifndef DISABLE_OVERLOAD_NEW_DELETE
// overload new and delete
void* operator new(_In_ size_t stBufIn);
void operator delete(_In_ void* pBufIn);
#endif

struct STUMemAllocInfo
{
    const void* pBuf = 0;
    size_t stBufSize = 0;
    SYSTEMTIME stuAllocTimeMs = { 0 };
    SYSTEMTIME stuFreeTimeMs = { 0 };
    std::string strAllocInfo = "";
    std::string strFreeInfo = "";

    STUMemAllocInfo(_In_ const void* pBufIn, _In_ const size_t stBufSizeIn,
        _In_ const SYSTEMTIME stuAllocTimeMsIn, _In_ const SYSTEMTIME stuFreeTimeMsIn,
        _In_ const std::string& kstrAllocInfoIn, _In_ const std::string& kstrFreeInfoIn);
    void PrintMemAllocInfo(_In_ const std::string& kstrPreFix) const;
};

class MemoryMgr
{
public:
    MemoryMgr();
    ~MemoryMgr();
public:
    static MemoryMgr& GetInstance();
    inline void StartRecord() { m_bStartRecord = true; }
    inline void StopRecord() { m_bStartRecord = false; }

public:
    void* NewBufEx(size_t stBufSize, const std::string& kstrFile, const std::string& kstrFunction, const int knLine);
    void DeleteBufEx(void* pBuf, const std::string& kstrFile, const std::string& kstrFunction, const int knLine);
    void* NewBuf(size_t stBufSize);
    void DeleteBuf(void* pBuf);

public:
    void PrintMemoryInfo(_In_ const std::string& kstrFlag) const throw();

private:
    void RecordNewMemoryInfo(_In_ const void* pBufAddrIn, _In_ const size_t kstBufSizeIn, _In_ const std::string& kstrAllocInfo);
    void RecordDeleteMemoryInfo(_In_ const void* pBufAddrIn, _In_ const std::string& kstrFreeInfo);

    bool IsNeedRecord() const;
    void SetThreadRecordFlag(_In_ const bool kbRecordFlag);
    bool GetThreadTecordFlag(_In_ const bool kbDefaultFlag) const;

    static std::string EstablishCallerInfo(const std::string& kstrFile, const std::string& kstrFunction, const int knLine);
private:
    size_t m_stTotalBuf;
    size_t m_stUsingBuf;
    size_t m_stFreeBuf;
    
    bool m_bStartRecord;

#if 0
    // Current for mutiple thread is not support, cannot use std::map to record info
    mutable rwmutex m_rwThreadInfo;
    std::map<DWORD, bool> m_mapThreadInfo;
#endif

    // Current memory info
    mutable rwmutex m_rwMemoryInfo;
    std::map<size_t, STUMemAllocInfo> m_mapMemoryInfo;

    // Freed memory info
    mutable rwmutex m_rwFreeMemoryInfo;
    std::list<STUMemAllocInfo> m_lsFreeMemoryInfo;
};

extern MemoryMgr g_MemoryMgr;
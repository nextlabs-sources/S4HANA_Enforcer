#include "MemoryMgr.h"

#ifndef HZUSERLOGA
#define HZUSERLOGA(strFormat, ...) (printf_s(strFormat, ##__VA_ARGS__))
#endif
#ifndef HZUSERLOGW
#define HZUSERLOGW(strFormat, ...) (wprintf_s(strFormat, ##__VA_ARGS__))
#endif

#ifndef HZDEBUGLOGA
#define HZDEBUGLOGA(strFormat, ...) (printf_s("%s:%d, ", __FUNCTION__, __LINE__), printf_s(strFormat, ##__VA_ARGS__))
#endif
#ifndef HZDEBUGLOGW
#define HZDEBUGLOGW(strFormat, ...) (wprintf_s("%s:%d, ", __FUNCTION__, __LINE__), wprintf_s(strFormat, ##__VA_ARGS__))
#endif


MemoryMgr g_MemoryMgr;
// Thread flag
thread_local bool g_bThreadRecord = true;

#ifndef DISABLE_OVERLOAD_NEW_DELETE
void* operator new(_In_ size_t stBufIn)
{
    return g_MemoryMgr.NewBuf(stBufIn);
}
void operator delete(_In_ void* pBufIn)
{
    g_MemoryMgr.DeleteBuf(pBufIn);
}
#endif

STUMemAllocInfo::STUMemAllocInfo(_In_ const void* pBufIn, _In_ const size_t stBufSizeIn, 
    _In_ const SYSTEMTIME stuAllocTimeMsIn, _In_ const SYSTEMTIME stuFreeTimeMsIn, 
    _In_ const std::string& kstrAllocInfoIn, _In_ const std::string& kstrFreeInfoIn)
    : pBuf(pBufIn), stBufSize(stBufSizeIn), stuAllocTimeMs(stuAllocTimeMsIn), stuFreeTimeMs(stuFreeTimeMsIn),
    strAllocInfo(kstrAllocInfoIn), strFreeInfo(kstrFreeInfoIn)
{

}
void STUMemAllocInfo::PrintMemAllocInfo(_In_ const std::string& kstrPreFix) const
{
    FILETIME stuAllocFileTime = { 0 };
    SystemTimeToFileTime(&stuAllocTimeMs, &stuAllocFileTime);
    FILETIME stuFreeFileTime = { 0 };
    SystemTimeToFileTime(&stuFreeTimeMs, &stuFreeFileTime);
    ULONGLONG tmSurvivalTimeMs = (ULONGLONG)(-1);
    if ((stuFreeFileTime.dwHighDateTime > stuAllocFileTime.dwHighDateTime) || 
        (stuFreeFileTime.dwHighDateTime == stuAllocFileTime.dwHighDateTime && stuFreeFileTime.dwLowDateTime >= stuAllocFileTime.dwLowDateTime))
    {
        tmSurvivalTimeMs = ((ULONGLONG)(stuFreeFileTime.dwHighDateTime - stuAllocFileTime.dwHighDateTime)) << sizeof(DWORD);
        tmSurvivalTimeMs += stuFreeFileTime.dwLowDateTime - stuAllocFileTime.dwLowDateTime;
    }
    
    
    HZUSERLOGA("%sAddress:[%p], size:[%zd], Survival time:[%llu](100ns), alloc time:[%d-%d-%d-%d:%d:%d:%d], free time:[%d-%d-%d-%d:%d:%d:%d], " 
        "AllocInfo:[%s], FreeInfo:[%s]\n",
        kstrPreFix.c_str(),
        pBuf, stBufSize, tmSurvivalTimeMs,
        stuAllocTimeMs.wYear, stuAllocTimeMs.wMonth, stuAllocTimeMs.wDay, stuAllocTimeMs.wHour, stuAllocTimeMs.wMinute, stuAllocTimeMs.wSecond, stuAllocTimeMs.wMilliseconds,
        stuFreeTimeMs.wYear, stuFreeTimeMs.wMonth, stuFreeTimeMs.wDay, stuFreeTimeMs.wHour, stuFreeTimeMs.wMinute, stuFreeTimeMs.wSecond, stuFreeTimeMs.wMilliseconds,
        strAllocInfo.c_str(), strFreeInfo.c_str());
}

MemoryMgr::MemoryMgr() : m_stTotalBuf(0), m_stUsingBuf(0), m_stFreeBuf(0), m_bStartRecord(false)
{
}
MemoryMgr::~MemoryMgr()
{
    PrintMemoryInfo("Memory manager destructor");
}

MemoryMgr& MemoryMgr::GetInstance()
{
    static MemoryMgr theMemMgrIns;
    return theMemMgrIns;
}

void* MemoryMgr::NewBufEx(size_t stBufSize, const std::string& kstrFile, const std::string& kstrFunction, const int knLine)
{
    void* pBufOut = malloc(stBufSize);

    if (IsNeedRecord())
    {
        if (NULL != pBufOut)
        {
            SetThreadRecordFlag(false);
            std::string strAllocInfo = EstablishCallerInfo(kstrFile, kstrFunction, knLine);
            RecordNewMemoryInfo(pBufOut, stBufSize, strAllocInfo);
            SetThreadRecordFlag(true);
        }
    }

    return pBufOut;
}
void MemoryMgr::DeleteBufEx(void* pBuf, const std::string& kstrFile, const std::string& kstrFunction, const int knLine)
{
    free(pBuf);

    if (IsNeedRecord())
    {
        if (NULL != pBuf)
        {
            SetThreadRecordFlag(false);
            std::string strFreeInfo = EstablishCallerInfo(kstrFile, kstrFunction, knLine);
            RecordDeleteMemoryInfo(pBuf, strFreeInfo);
            SetThreadRecordFlag(true);
        }
    }
}

void* MemoryMgr::NewBuf(size_t stBufSize)
{
    void* pBufOut = malloc(stBufSize);
    if (IsNeedRecord())
    {
        if (NULL != pBufOut)
        {
            SetThreadRecordFlag(false);
            RecordNewMemoryInfo(pBufOut, stBufSize, "");
            SetThreadRecordFlag(true);
        }
    }
    return pBufOut;
}
void MemoryMgr::DeleteBuf(void* pBuf)
{
    free(pBuf);

    if (IsNeedRecord())
    {
        if (NULL != pBuf)
        {
            SetThreadRecordFlag(false);
            RecordDeleteMemoryInfo(pBuf, "");
            SetThreadRecordFlag(true);
        }
    }
}

void MemoryMgr::PrintMemoryInfo(_In_ const std::string& kstrFlag) const throw()
{
    HZUSERLOGA("[%s] Memory Info start:\n", kstrFlag.c_str());
    
    HZUSERLOGA("Heap memory, total size:[%zd], using size:[%zd], free size:[%zd]\n", m_stTotalBuf, m_stUsingBuf, m_stFreeBuf);
    {
        readLock lock(m_rwMemoryInfo);
        HZUSERLOGA("Current memory using info, count:[%zd]:\n", m_mapMemoryInfo.size());
        for (std::map<size_t, STUMemAllocInfo>::const_iterator kitr = m_mapMemoryInfo.cbegin(); kitr != m_mapMemoryInfo.cend(); ++kitr)
        {
            const STUMemAllocInfo& stuMemAllocInfo = kitr->second;
            stuMemAllocInfo.PrintMemAllocInfo("\t");
        }
    }
    {
        readLock lock(m_rwFreeMemoryInfo);
        HZUSERLOGA("Free memory using info, count:[%zd]:\n", m_lsFreeMemoryInfo.size());
        for (std::list<STUMemAllocInfo>::const_iterator kitr = m_lsFreeMemoryInfo.cbegin(); kitr != m_lsFreeMemoryInfo.cend(); ++kitr)
        {
            const STUMemAllocInfo& stuMemAllocInfo = *kitr;
            stuMemAllocInfo.PrintMemAllocInfo("\t");
        }
    }
    HZUSERLOGA("[%s] Memory Info end:\n", kstrFlag.c_str());
}
void MemoryMgr::RecordNewMemoryInfo(_In_ const void* pBufAddrIn, _In_ const size_t kstBufSizeIn, _In_ const std::string& kstrAllocInfo)
{
    HZDEBUGLOGA("Add memory info: address:[%p], size:[%zd]\n", pBufAddrIn, kstBufSizeIn);
    SYSTEMTIME stuSysTimeAlloc = { 0 };
    GetSystemTime(&stuSysTimeAlloc);
    STUMemAllocInfo stuMemAllocInfo(pBufAddrIn, kstBufSizeIn, stuSysTimeAlloc, { 0 }, kstrAllocInfo, "");
    m_stTotalBuf += kstBufSizeIn;
    m_stUsingBuf += kstBufSizeIn;
    size_t stBufAddr = (size_t)pBufAddrIn;
    {
        writeLock lock(m_rwMemoryInfo);
        std::map<size_t, STUMemAllocInfo>::const_iterator kitr = m_mapMemoryInfo.find(stBufAddr);
        if (m_mapMemoryInfo.cend() == kitr)
        {
            m_mapMemoryInfo.insert(std::make_pair(stBufAddr, stuMemAllocInfo));
        }
        else
        {
            HZDEBUGLOGA("Attention, the buffer:[%p] do not removed and alloc again in the same space, please check\n", pBufAddrIn);
        }
    }
}
void MemoryMgr::RecordDeleteMemoryInfo(_In_ const void* pBufAddrIn, _In_ const std::string& kstrFreeInfo)
{
    HZDEBUGLOGA("Remove memory info: address:[%p]\n", pBufAddrIn);
    SYSTEMTIME stuSysTimeFree = { 0 };
    GetSystemTime(&stuSysTimeFree);
    STUMemAllocInfo stuMemAllocInfo(0, 0, { 0 }, { 0 }, "", "");
    size_t stBufAddr = (size_t)pBufAddrIn;
    {
        writeLock lock(m_rwMemoryInfo);
        std::map<size_t, STUMemAllocInfo>::const_iterator kitr = m_mapMemoryInfo.find(stBufAddr);
        if (m_mapMemoryInfo.cend() == kitr)
        {
            HZDEBUGLOGA("Attention the buffer:[%p] do not recode in memory manager map but invoke remove to free it. This buffer maybe alloc before memory manager start record. please check\n", pBufAddrIn);
        }
        else
        {
            stuMemAllocInfo = kitr->second;
            m_mapMemoryInfo.erase(kitr);
        }
    }
    stuMemAllocInfo.stuFreeTimeMs = stuSysTimeFree;
    stuMemAllocInfo.strFreeInfo = kstrFreeInfo;
    m_stUsingBuf -= stuMemAllocInfo.stBufSize;
    m_stFreeBuf += stuMemAllocInfo.stBufSize;
    {
        // Record into free memory list
        writeLock lock(m_rwFreeMemoryInfo);
        m_lsFreeMemoryInfo.push_back(stuMemAllocInfo);
    }
}

bool MemoryMgr::IsNeedRecord() const
{
    if (m_bStartRecord)
    {
        return GetThreadTecordFlag(true);
    }
    return false;
}
void MemoryMgr::SetThreadRecordFlag(_In_ const bool kbRecordFlag)
{
    g_bThreadRecord = kbRecordFlag;
#if 0
    DWORD dwThreadID = GetCurrentThreadId();
    writeLock lock(m_rwThreadInfo);
    std::map<DWORD, bool>::iterator kitr = m_mapThreadInfo.find(dwThreadID);
    if (m_mapThreadInfo.cend() == kitr)
    {
        m_mapThreadInfo.insert(std::make_pair(dwThreadID, kbRecordFlag));
    }
    else
    {
        kitr->second = kbRecordFlag;
    }
#endif
}
bool MemoryMgr::GetThreadTecordFlag(_In_ const bool /*kbDefaultFlag*/) const
{
    return g_bThreadRecord;
#if 0
    DWORD dwThreadID = GetCurrentThreadId();
    readLock lock(m_rwThreadInfo);
    std::map<DWORD, bool>::const_iterator kitr = m_mapThreadInfo.find(dwThreadID);
    if (m_mapThreadInfo.cend() == kitr)
    {
        return kbDefaultFlag;
    }
    else
    {
        return kitr->second;
    }
#endif
}

std::string MemoryMgr::EstablishCallerInfo(const std::string& kstrFile, const std::string& kstrFunction, const int knLine)
{
    std::string strCallerInfo = kstrFile + ":" + kstrFunction + ":";
    char szLine[128];
    _itoa(knLine, szLine, 10);
    strCallerInfo += szLine;
    return strCallerInfo;
}
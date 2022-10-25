#ifndef DIAGTOOL_LOG_H
#define DIAGTOOL_LOG_H
#include <stdio.h>
#include <fcntl.h>

#ifndef _WIN32
  #define __cdecl
#endif 

typedef size_t(__cdecl *TyFuncWriteLog)(const char* kpchLogInfo, size_t stLength);

#define MAX_MESSAGE_SIZE_CHARS 1024*10

class CPreStartLog
{
public:
  CPreStartLog();
  void Init(const TyFuncWriteLog pFuncWriteLog);
  size_t WriteLog(const char* szFmt, ... );

private:
  TyFuncWriteLog m_pFuncWriteLog;
    char  _szLog[MAX_MESSAGE_SIZE_CHARS];
};

extern CPreStartLog* g_preLog;


#endif 

#ifndef DIAGTOOL_LOG_H
#define DIAGTOOL_LOG_H
#include <stdio.h>
#include <fcntl.h>
#include "DAE_main_utili.h"

#define MAX_MESSAGE_SIZE_CHARS 1024*10

class CPreStartLog
{
public:
  CPreStartLog();  
  bool Init(const char* szLogFile);
  size_t WriteLog(const char* szFmt, ... );

  static size_t WriteLogFile(const char* kpchLogInfo, size_t stLength);

private:
  size_t InnerWriteToLogFile( const char* kpchLogInfo, size_t stLength);

private:
  FILE* _logFile;
  char  _szLog[MAX_MESSAGE_SIZE_CHARS];
};

extern CPreStartLog* g_LogDAEMain;


#endif 

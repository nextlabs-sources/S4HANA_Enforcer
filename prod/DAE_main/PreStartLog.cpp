#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PreStartLog.h"
#include <errno.h>

CPreStartLog* g_LogDAEMain=NULL;


CPreStartLog::CPreStartLog()
{
	_logFile = NULL;
}

bool CPreStartLog::Init(const char* szLogFile)
{
	//
	if (NULL==_logFile){
#pragma warning(push)
#pragma warning(disable: 4996)	// 'fopen': This function or variable may be unsafe. Consider using fopen_s instead.
		_logFile = fopen(szLogFile, "w");
#pragma warning(push)
		if (NULL==_logFile){
			//printf("fopen proxy log file failed. file:%s, errno=%s\n", szLogFile, errno);
			return false;
		}	
	}
	return true;
}

size_t CPreStartLog::WriteLog( const char* szFmt, ... )
{
	if (NULL==_logFile){
		return 0;
	}
	
	//format log content
	va_list args;
	va_start(args, szFmt);
	size_t nLog = vsnprintf(_szLog, MAX_MESSAGE_SIZE_CHARS-1, szFmt, args);
	va_end(args);

	fwrite(_szLog, nLog, 1, _logFile);
	fwrite("\n", 1, 1, _logFile);
	fflush(_logFile);
	
	return nLog;
}
size_t CPreStartLog::InnerWriteToLogFile(const char* kpchLogInfo, size_t stLength)
{
	if (NULL==_logFile || NULL == kpchLogInfo){
		return 0;
	}
	
	fwrite(kpchLogInfo, stLength, 1, _logFile);
	fwrite("\n", 1, 1, _logFile);
	fflush(_logFile);
	
	return stLength;
}

size_t CPreStartLog::WriteLogFile(const char* kpchLogInfo, size_t stLength)
{
	if (nullptr != g_LogDAEMain)
	{
		return g_LogDAEMain->InnerWriteToLogFile(kpchLogInfo, stLength);
	}
	return 0;
}
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PreStartLogProxy.h"
#include <errno.h>
#include <string>


CPreStartLog* g_preLog=NULL;

CPreStartLog::CPreStartLog() : m_pFuncWriteLog(NULL)
{

}

void CPreStartLog::Init(const TyFuncWriteLog pFuncWriteLog)
{
	m_pFuncWriteLog = pFuncWriteLog;
}

size_t CPreStartLog::WriteLog( const char* szFmt, ... )
{
	if (NULL==m_pFuncWriteLog){
		return 0;
	}
	
	//format log content
	va_list args;
	va_start(args, szFmt);
	size_t nLog = vsnprintf(_szLog, MAX_MESSAGE_SIZE_CHARS-1, szFmt, args);
	va_end(args);

	return m_pFuncWriteLog(_szLog, nLog);
}
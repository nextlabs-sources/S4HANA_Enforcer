#include "stdafx.h"
#include "util.h"

void OutputTrace(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[1000];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	OutputDebugString(chMsg);
	va_end(pArg);
}
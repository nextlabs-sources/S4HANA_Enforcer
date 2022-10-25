#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "log.h"
#include <windows.h>

// Common tools for windows

HANDLE HZCreateProcess(_In_ const std::string& kwstrProcessFullName, _In_ const std::string& kwstrCmdLine, _In_opt_ const char* kpchWorkDirectory) throw();
DWORD HZGetProcessExitCode(_In_ const HANDLE hProcessIn, _In_ const DWORD kdwDefaultRet, _In_ const DWORD kdwMilliseconds) throw();
DWORD ExecuteCommand(_In_ const std::string& kstrFullCmd, _In_ const DWORD kdwDefaultRet, _In_ const DWORD kdwMilliseconds) throw();

bool ExecutePSScript(_In_ const std::string& kstrPSScript, _In_ const std::string& kstrLogFilePath);
std::string GetFormatString(const char* _Fmt, ... );
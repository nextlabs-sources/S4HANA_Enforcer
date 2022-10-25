#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "log.h"
#include "CommonTools_windows.h"
#include "CommonTools.h"

HANDLE HZCreateProcess(_In_ const std::string& kwstrProcessFullName, _In_ const std::string& kwstrCmdLine, _In_opt_ const char* kpchWorkDirectory) throw()
{
    HANDLE hNewProcess = NULL;

    std::string wstrCreateProcessFullCommand = "";
    if (!kwstrProcessFullName.empty())
    {
        wstrCreateProcessFullCommand += "\"" + kwstrProcessFullName + "\" ";
    }
    if (!kwstrCmdLine.empty())
    {
        wstrCreateProcessFullCommand += kwstrCmdLine;
    }
    LOG::WriteLog("Create new process command:[%s]", wstrCreateProcessFullCommand.c_str());
    const size_t kstCommandLength = wstrCreateProcessFullCommand.length() + 1;
    char* pwchCommandLine = new char[kstCommandLength];
    memset(pwchCommandLine, 0, kstCommandLength);
    _snprintf_s(pwchCommandLine, kstCommandLength, kstCommandLength - 1, "%s", wstrCreateProcessFullCommand.c_str());

    STARTUPINFOA stuStartupInfo = { 0 };
    stuStartupInfo.cb = sizeof(STARTUPINFO);
    stuStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    stuStartupInfo.wShowWindow = SW_HIDE;
    stuStartupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    stuStartupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    stuStartupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    PROCESS_INFORMATION pi;

    ::SetLastError(ERROR_SUCCESS);
    BOOL bCreateProcess = CreateProcessA(
        NULL, // Process name, using command line.
        pwchCommandLine, // Command line
        NULL, // Process attributes
        NULL, // Thread attributes
        TRUE, // Inherit handles
        0, // Creation flags
        NULL, // Environment
        kpchWorkDirectory, // Process work directory
        &stuStartupInfo, // Start info
        &pi // Process information
    );
    
    if (bCreateProcess)
    {
        CloseHandle(pi.hThread);
        hNewProcess = pi.hProcess;
    }
    else{
        LOG::WriteLog("Create process failed. GetLastError=%d\n",  ::GetLastError());
    }
    if (NULL != pwchCommandLine)
    {
        delete[] pwchCommandLine;
        pwchCommandLine = NULL;
    }
    return hNewProcess;
}
DWORD HZGetProcessExitCode(_In_ const HANDLE hProcessIn, _In_ const DWORD kdwDefaultRet, _In_ const DWORD kdwMilliseconds) throw()
{
    DWORD dwExitCode = kdwDefaultRet;
    if (NULL != hProcessIn)
    {
        ::SetLastError(ERROR_SUCCESS);
        WaitForSingleObject(hProcessIn, kdwMilliseconds /*INFINITE*/);
        if (GetExitCodeProcess(hProcessIn, &dwExitCode))
        {
            LOG::WriteLog("Get process:[0x%x] exit code, [0x%x]\n", hProcessIn, dwExitCode);
        }
        else
        {
            LOG::WriteLog("Can't get process:[0x%x] exit code. error:[0x%x]\n", hProcessIn, ::GetLastError());
        }
    }
    return dwExitCode;
}
DWORD ExecuteCommand(_In_ const std::string& kstrFullCmd, _In_ const DWORD kdwDefaultRet, _In_ const DWORD kdwMilliseconds) throw()
{
    DWORD dwRet = kdwDefaultRet;
    if (kstrFullCmd.empty())
    {
        LOG::WriteLog("Try to execute some command but the pass command line:[%s] is empty\n", kstrFullCmd.c_str());
    }
    else
    {
        HANDLE hProcess = HZCreateProcess("", kstrFullCmd, NULL);
        dwRet = HZGetProcessExitCode(hProcess, kdwDefaultRet, kdwMilliseconds);
    }
    return dwRet;
}

bool ExecutePSScript(_In_ const std::string& kstrPSScript, _In_ const std::string& kstrLogFilePath)
{
	bool bRet = false;
	if (kstrPSScript.empty())
	{
		LOG::WriteLog("Try to execute some PS script but the path script value is empty");
	}
	else
	{
		std::string strCmd = "";
        if (kstrLogFilePath.empty())
        {
			strCmd = GetFormatString("powerShell.exe -ExecutionPolicy Unrestricted -Command \"& '%s'\" 2>&1", kstrPSScript.c_str());
        }
		else
		{
			strCmd = GetFormatString("powerShell.exe -ExecutionPolicy Unrestricted -Command \"& '%s'\" >\"%s\" 2>&1", kstrPSScript.c_str(), kstrLogFilePath.c_str());
		}
		LOG::WriteLog("Begin execute command:[%s]", strCmd.c_str());
#if false
		bRet = (0 == system(strCmd.c_str()));
#else
#pragma warning(push)
#pragma warning(disable: 4245)  // conversion from 'int' to 'const DWORD', signed/unsigned mismatch
        DWORD dwExitCode = ExecuteCommand(strCmd, -1, INFINITE);
#pragma warning(push)
        bRet = (0 == dwExitCode);
#endif
		LOG::WriteLog("End execute command:[%s]", strCmd.c_str());
	}
	return bRet;
}

std::string GetFormatString(const char* _Fmt, ... )
{
    std::string strOut = "";

    // retrieve the variable arguments
    va_list  args;
    va_start( args, _Fmt );

    int len = _vscprintf_l( _Fmt, 0, args ) + 1; // _vscprintf doesn't count, terminating '\0' 
    char* pchBuffer = (char*)malloc( len * sizeof(char) );
    if( NULL != pchBuffer )
    {
        vsprintf_s( pchBuffer, len, _Fmt, args );
        strOut = pchBuffer;

        free(pchBuffer);
		pchBuffer = nullptr;
    }
    va_end(args);

    return strOut;
}

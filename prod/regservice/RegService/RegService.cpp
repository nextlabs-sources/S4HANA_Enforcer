// RegService.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "RegService_i.h"
#include <memory>
#include "MyProcessManager.h"

using namespace ATL;

#include <stdio.h>
#include "util.h"

TCHAR								g_szServiceName[] = _T("RegService");
SERVICE_STATUS_HANDLE				g_hServiceStatus;
SERVICE_STATUS						g_status;
DWORD								g_dwThreadID;

BOOL								EnablePrivilege();
void								Init();
BOOL								IsInstalled();
BOOL								Install(LPCTSTR displayName);
BOOL								Uninstall();
int									StartMyService();
void								LogEvent(LPCTSTR pFormat, ...);
void WINAPI							ServiceMain();
void WINAPI							ServiceCtrl(DWORD dwOpcode);

class CRegServiceModule : public ATL::CAtlServiceModuleT< CRegServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_RegServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_REGSERVICE, "{AD1BBB16-9190-4CD9-90FC-F6ED8936F2A8}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropriate Non NULL Security Descriptor.

		return S_OK;
	}
	};

CRegServiceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR lpCmdLine, int nShowCmd)
{
	Init();

	g_dwThreadID = ::GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ g_szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	TCHAR seps[] = _T(" ");
	TCHAR *token = 0, *next_token = 0, *cmd = 0, *displayName = 0;

	if (_tcslen(lpCmdLine) > 0)
	{
		cmd = token = _tcstok_s(lpCmdLine, seps, &next_token);
		while (token != 0)
		{
			if (_tcscmp(token, _T("-i")) == 0)
			{
				displayName = next_token;
				break;
			}
			token = _tcstok_s(0, seps, &next_token);
		}
		if (_tcscmp(cmd, _T("-i")) == 0)
		{
			Install(displayName);
		}
		else if (_tcscmp(cmd, _T("-u")) == 0)
		{
			Uninstall();
		}
		else if (_tcscmp(cmd, _T("-start")) == 0)
		{
			StartMyService();
		}
	}
	else
	{
		if (!StartServiceCtrlDispatcher(st))
		{
			OutputTrace(_T("RegService, Register Service Main Function error\n"));
			LogEvent(_T("RegService, Register Service Main Function error"));
		}
	}

	return _AtlModule.WinMain(nShowCmd);
}

BOOL EnablePrivilege()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;  // one privilege to set   
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	CloseHandle(hToken);
	return TRUE;
}

void Init()
{
	//CoInitializeEx (NULL,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	g_hServiceStatus = NULL;
	g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_status.dwCurrentState = SERVICE_STOPPED;
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_status.dwWin32ExitCode = 0;
	g_status.dwServiceSpecificExitCode = 0;
	g_status.dwCheckPoint = 0;
	g_status.dwWaitHint = 0;

	EnablePrivilege();
}

BOOL IsInstalled()
{
	OutputTrace(_T("RegService, Check service install\n"));
	BOOL bResult = FALSE;
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM != NULL)
	{
		SC_HANDLE hService = ::OpenService(hSCM, g_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}

	return bResult;
}

BOOL Install(LPCTSTR displayName)
{
	OutputTrace(_T("RegService, Install service\n"));

	if (IsInstalled())
		return TRUE;

	//Open service manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), g_szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//Create service
	SC_HANDLE hService = ::CreateService(
		hSCM, g_szServiceName, _tcslen(displayName)>0 ? displayName : g_szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), g_szServiceName, MB_OK);
		return FALSE;
	}

	SERVICE_DESCRIPTION sd;
	sd.lpDescription = L"Monitor SAP client and inject enforcer dll";
	ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);

	return TRUE;
}

BOOL Uninstall()
{
	OutputTrace(_T("RegService, Uninstall service\n"));
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), g_szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, g_szServiceName, SERVICE_STOP | DELETE);
	if (hService == NULL)
	{
		CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), g_szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	ControlService(hService, SERVICE_CONTROL_STOP, &status);

	BOOL bDelete = ::DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	if (bDelete)
	{
		OutputTrace(_T("RegService, Delete service\n"));
		return TRUE;
	}

	OutputTrace(_T("RegService, Delete service error\n"));
	LogEvent(_T("RegService, Delete service error"));

	return FALSE;
}

int StartMyService()
{
	OutputTrace(_T("RegService, StartMyService\n"));

	SC_HANDLE schManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (schManager == NULL)
		return 0;

	SC_HANDLE hService = OpenService(schManager, g_szServiceName, SERVICE_ALL_ACCESS);
	if (hService == NULL)
		return 0;

	StartService(hService, 0, 0);

	CloseServiceHandle(hService);
	CloseServiceHandle(schManager);
	return 1;
}

void WINAPI ServiceMain()
{
	OutputTrace(_T("RegService, ServiceMain\n"));

	// Register the control request handler
	g_status.dwCurrentState = SERVICE_START_PENDING;
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	g_hServiceStatus = RegisterServiceCtrlHandler(g_szServiceName, ServiceCtrl);
	if (g_hServiceStatus == NULL)
	{
		OutputTrace(_T("RegService, Handler not installed\n"));
		LogEvent(_T("RegService, Handler not installed"));
		return;
	}
	SetServiceStatus(g_hServiceStatus, &g_status);

	g_status.dwWin32ExitCode = S_OK;
	g_status.dwCheckPoint = 0;
	g_status.dwWaitHint = 0;
	g_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_hServiceStatus, &g_status);

	//TODO
	OutputTrace(_T("RegService, ServiceMain ProcessMonitor...\n"));
	std::unique_ptr<MyProcessManager> myPM(new MyProcessManager());
	myPM->ProcessMonitor();

	g_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(g_hServiceStatus, &g_status);
	OutputTrace(_T("RegService, Service stopped\n"));
	LogEvent(_T("RegService, Service stopped"));
}

void WINAPI ServiceCtrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		OutputTrace(_T("RegService, Service control stop\n"));
		g_status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_hServiceStatus, &g_status);
		PostThreadMessage(g_dwThreadID, WM_CLOSE, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		OutputTrace(_T("RegService, Bad service request\n"));
		LogEvent(_T("RegService, Bad service request"));
	}
}

void LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[1000];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, g_szServiceName);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}
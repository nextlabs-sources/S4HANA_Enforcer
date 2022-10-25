#include "stdafx.h"
#include "MyProcessManager.h"
#include <string>
#include <assert.h>
#include "util.h"

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

MyProcessManager::MyProcessManager()
{
	GetAppPath();
}

MyProcessManager::MyProcessManager(const TCHAR* pszWorkPath)
{
	assert(pszWorkPath);
	if (pszWorkPath)
	{
		wcscpy((wchar_t*)m_szWorkPath, (wchar_t*)pszWorkPath);
	}
	else
	{
		GetAppPath();
	}
}

MyProcessManager::~MyProcessManager()
{
	RemoveAllProcesses();
}

void MyProcessManager::GetAppPath()
{
	TCHAR szWorkPath[STR_LENGTH] = { 0 };
	//Get current workpath
	GetModuleFileName(NULL, szWorkPath, STR_LENGTH);

	std::wstring strTemp = szWorkPath;
	int iIndex = strTemp.find_last_of('\\');
	int iLen = strTemp.length();
	strTemp.erase(iIndex, iLen - iIndex);
	strTemp += '\\';

	wcscpy((wchar_t*)m_szWorkPath, (wchar_t*)strTemp.c_str());
	OutputTrace(L"MyProcessManager, workpath:%s\n", m_szWorkPath);
}

void MyProcessManager::RemoveAllProcesses()
{
	if (m_ProcessMap.size() > 0)
	{
		for (auto iter = m_ProcessMap.begin(); iter != m_ProcessMap.end(); iter++)
		{
			MonitoredProcess *pMP = iter->second;
			if (nullptr != pMP)
			{
				delete pMP;
				pMP = nullptr;
				iter->second = nullptr;
			}
		}
		m_ProcessMap.clear();
	}
}

BOOL MyProcessManager::ProcessMonitor()
{
	TCHAR szSAPClient[MAX_PATH] = TEXT("disp+work.exe");//TEXT("notepad++.exe");
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	while (true)
	{
		// reset process flag 'existed' as FALSE
		for (auto iter = m_ProcessMap.begin(); iter != m_ProcessMap.end(); iter++)
		{
			MonitoredProcess *pMP = iter->second;
			if (nullptr != pMP)
			{
				pMP->Existed = FALSE;
			}
		}

		if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
			return FALSE;

		cProcesses = cbNeeded / sizeof(DWORD);//num of processes
		TCHAR szProcessName[STR_LENGTH] = { 0 };

		for (unsigned int idx = 0; idx < cProcesses; idx++)
		{
			DWORD processID = aProcesses[idx];
			if (processID != 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
				if (NULL != hProcess)
				{
					HMODULE hMod;
					DWORD cbNeeded;
					if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
					{
						GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
						if (_tcsicmp(szProcessName, szSAPClient) == 0)
						{
							auto iter = m_ProcessMap.find(processID);
							if (iter == m_ProcessMap.end())
							{//new mount
								MonitoredProcess *pMP = new MonitoredProcess();
								pMP->ProcessID = processID;
								wcscpy((wchar_t*)pMP->ProcessName, (wchar_t*)szProcessName);
								pMP->Existed = TRUE;
								pMP->Mounted = MountEnforcer(processID);
								m_ProcessMap.insert(std::make_pair(processID, pMP));

								OutputTrace(L"MyProcessManager, ProcessMonitor new mount, pid:%d pname:%s mounted:%d\n", processID, szProcessName, pMP->Mounted);
							}
							else
							{
								MonitoredProcess *pMP = iter->second;
								if (nullptr != pMP)
								{
									pMP->Existed = TRUE;
									if (!pMP->Mounted)
									{//re-mount
										pMP->Mounted = MountEnforcer(processID);
										OutputTrace(L"MyProcessManager, ProcessMonitor re-mount, pid:%d pname:%s mounted:%d\n", processID, szProcessName, pMP->Mounted);
									}
								}
							}//if (iter == m_ProcessMap.end())
						}
					}
					CloseHandle(hProcess);
				}
			}
		}

		// remove process that its existed is FALSE
		for (auto iter = m_ProcessMap.begin(); iter != m_ProcessMap.end();)
		{
			MonitoredProcess *pMP = iter->second;
			if (nullptr != pMP)
			{
				if (!pMP->Existed)
				{
					OutputTrace(L"MyProcessManager, ProcessMonitor remove process, pid:%d pname:%s\n", pMP->ProcessID, pMP->ProcessName);
					m_ProcessMap.erase(iter++);
				}
				else
				{
					iter++;
				}
			}
		}

		Sleep(2000);
	}

	return TRUE;
}

BOOL MyProcessManager::MountEnforcer(const DWORD& processID)
{
	HANDLE hStdOutRead = NULL;
	HANDLE hStdOutWrite = NULL;

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &saAttr, 0))
	{
		return FALSE;
	}

	TCHAR szCmdLine[STR_LENGTH] = { 0 };
	TCHAR szNxTool[STR_LENGTH] = TEXT("NxTools.exe");
	TCHAR szEnforcerDll[STR_LENGTH] = TEXT("SQLDBCHDB_en.dll");
	wsprintf(szCmdLine, L"%s -pid %d %s", szNxTool, processID, szEnforcerDll);
	//wsprintf(szCmdLine, L"ping %s", L"www.baidu.com");
	OutputTrace(L"MyProcessManager, MountEnforcer, %s\n", szCmdLine);

	// Start the child process
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = hStdOutWrite;
	si.wShowWindow = SW_HIDE;//SW_SHOWNORMAL;
	BOOL ret = CreateProcess(NULL, szCmdLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	if (ret)
	{
		// Wait until child process exits
		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD dwRead = 0;
		CHAR chBuf[1024] = { 0 };
		BOOL bSuccess = FALSE;
		while (TRUE)
		{
			bSuccess = PeekNamedPipe(hStdOutRead, chBuf, 1024, &dwRead, NULL, NULL);
			if (bSuccess && dwRead > 0)
			{
				bSuccess = ReadFile(hStdOutRead, chBuf, 1024, &dwRead, NULL);
				if (bSuccess && dwRead > 0)
				{
					OutputDebugStringA(chBuf);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		OutputTrace(L"MyProcessManager, MountEnforcer CreateProcess OK, pid=%d\n", processID);
	}
	else
	{
		OutputTrace(L"MyProcessManager, MountEnforcer CreateProcess error:%d, pid=%d\n", GetLastError(), processID);
	}
	CloseHandle(hStdOutRead);
	CloseHandle(hStdOutWrite);
	return ret;
}
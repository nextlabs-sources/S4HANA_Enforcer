#pragma once
#include <map>

struct MonitoredProcess
{
	TCHAR	ProcessName[STR_LENGTH];
	DWORD	ProcessID;
	BOOL	Existed;
	BOOL	Mounted;
};

class MyProcessManager
{
public:
	MyProcessManager();
	MyProcessManager(const TCHAR* szWorkPath);
	~MyProcessManager();

public:
	BOOL ProcessMonitor();

private:
	void GetAppPath();
	void RemoveAllProcesses();
	BOOL MountEnforcer(const DWORD& processID);

private:
	std::map<DWORD, MonitoredProcess *> m_ProcessMap;
	TCHAR m_szWorkPath[STR_LENGTH];
};


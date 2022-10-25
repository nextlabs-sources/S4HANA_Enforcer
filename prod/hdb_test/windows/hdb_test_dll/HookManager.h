#pragma once
#include <list>
#include <mutex>

struct HookItem
{
	void** OldFunction;
	void*  NewFunction;
	const char* szDllName;
	const char* szFunName;
	long   lHookResult;
};

class HookManager
{
public:
	HookManager();
	~HookManager();

private:
	HookManager(const HookManager&);

public:
	bool AddHookItem(void** OldFunction, void* newFunction, const char* szDllName, const char* szFunName);

	void Hook();
	void Unhook();

private:
	const HookItem* FindHookItem(const char* szDllName, const char* szFunName);
	void InsertHookItem(HookItem* item);

private:
	bool m_bHook;
	std::mutex m_mutexHookItem;
	std::list<HookItem*>  m_lstHookItem;
};

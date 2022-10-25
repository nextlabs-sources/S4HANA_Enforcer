#include "HookManager.h"
#include <Shlobj.h>
#include <new>
#include "thirdpart\Detours\include\detours.h"
#ifdef _WIN64 
#pragma comment(lib, "thirdpart/Detours/lib.X64/detours.lib")
#else
#pragma comment(lib, "thirdpart/Detours/lib.X86/detours.lib")
#endif // _WIN64

HookManager::HookManager()
{
	m_bHook = false;
}

HookManager::HookManager(const HookManager&)
{
	m_bHook = false;
}

HookManager::~HookManager()
{
	if (m_bHook)
	{
		Unhook();
	}

	// clear list
	std::list<HookItem*>::iterator itHookItem = m_lstHookItem.begin();
	while (itHookItem != m_lstHookItem.end())
	{
		HookItem* item = *itHookItem;
		if (item != NULL)
		{
			delete item;
			item = NULL;
			m_lstHookItem.erase(itHookItem++);
		}
		else
		{
			itHookItem++;
		}
	}
}

bool HookManager::AddHookItem(void** OldFunction, void* newFunction, const char* szDllName, const char* szFunName)
{
	if (OldFunction == NULL || newFunction == NULL || szDllName == NULL || szFunName == NULL)
	{
		return false;
	}

	if (!FindHookItem(szDllName, szFunName))
	{
		HookItem* item = new HookItem();
		item->OldFunction = OldFunction;
		item->NewFunction = newFunction;
		item->szDllName = szDllName;
		item->szFunName = szFunName;
		item->lHookResult = -1;

		InsertHookItem(item);
	}

	return true;
}

const HookItem* HookManager::FindHookItem(const char* szDllName, const char* szFunName)
{
	try
	{
		std::lock_guard<std::mutex> lock(m_mutexHookItem);
		std::list<HookItem*>::const_iterator itHookItem = m_lstHookItem.begin();
		while (itHookItem != m_lstHookItem.end())
		{
			if (_stricmp(szDllName, (*itHookItem)->szDllName) == 0 &&
				_stricmp(szFunName, (*itHookItem)->szFunName) == 0)
			{
				return *itHookItem;
			}

			itHookItem++;
		}
	}
	catch (...)
	{
		OutputDebugStringA("Exception on HookManager::FindHookItem");
	}

	return NULL;
}

void HookManager::InsertHookItem(HookItem* item)
{
	std::lock_guard<std::mutex> lock(m_mutexHookItem);
	m_lstHookItem.push_back(item);
}

void HookManager::Hook()
{
	try
	{
		std::lock_guard<std::mutex> lock(m_mutexHookItem);

		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		//begin hook
		std::list<HookItem*>::iterator itHookItem = m_lstHookItem.begin();
		while (itHookItem != m_lstHookItem.end())
		{
			HookItem* item = *itHookItem;
			if (*(item->OldFunction))
			{
				item->lHookResult = DetourAttach(item->OldFunction, item->NewFunction);
				OutputDebugStringA("HookManager::Hook Attach item->OldFunction");
			}
			else
			{
				void* oldFunction = DetourFindFunction(item->szDllName, item->szFunName);
				if (oldFunction)
				{
					*(item->OldFunction) = oldFunction;
					item->lHookResult = DetourAttach(item->OldFunction, item->NewFunction);
					OutputDebugStringA("HookManager::Hook Attach item->szDllName, item->szFunName, item->OldFunction");
				}
			}


			itHookItem++;
		}

		DetourTransactionCommit();
		m_bHook = true;
	}
	catch (...)
	{
		OutputDebugStringA("Exception on HookManager::Hook");
	}

}

void HookManager::Unhook()
{
	try
	{
		std::lock_guard<std::mutex> lock(m_mutexHookItem);
		if (!m_bHook)
		{
			OutputDebugStringA("Warning on HookManager::Unhook: m_bHook=false");
			return;
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		//end hook
		std::list<HookItem*>::iterator itHookItem = m_lstHookItem.begin();
		while (itHookItem != m_lstHookItem.end())
		{
			HookItem* item = *itHookItem;
			if (!item->lHookResult)
			{
				DetourDetach(item->OldFunction, item->NewFunction);
				OutputDebugStringA("HookManager::Unhook Detach item->OldFunction");
			}
			itHookItem++;
		}

		DetourTransactionCommit();
		m_bHook = false;
	}
	catch (...)
	{
		OutputDebugStringA("Exception on HookManager::Unhook");
	}
}

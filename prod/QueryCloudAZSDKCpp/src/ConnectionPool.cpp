#include "ConnectionPool.h"
#include "QueryCloudAZManager.h"
//#include <atlbase.h>

ConnctionPool::ConnctionPool()
{
	memset(m_Connections, 0, sizeof(m_Connections));
	m_nConnCount = 0;

	m_nLastAssignConnectionIndex = -1;
}

void ConnctionPool::SetConnectCount(int Count)
{
	if (Count > MAX_CONNECTIONS_TO_CLOUDAZ)
	{
		Count = MAX_CONNECTIONS_TO_CLOUDAZ;
	}
	m_nConnCount = Count;

}

void ConnctionPool::SetTimeout(unsigned int msTimeout) 
{
	m_msTimeout = msTimeout;
	if(m_Connections[0]!=nullptr){
		m_Connections[0]->SetTimeout(m_msTimeout);
	}
}

CloudAZConnection* ConnctionPool::AssignConnection()
{
#if 1
	std::lock_guard<std::mutex> sbguard(m_mutex);
	if (m_Connections[0] == 0) {
		m_Connections[0] = CreateNewConnection();
	}

	return m_Connections[0];
#else
    std::lock_guard<std::mutex> sbguard(m_mutex);
	int nCurIndex = (m_nLastAssignConnectionIndex + 1) % m_nConnCount;
	if (m_Connections[nCurIndex] == 0)
	{
		m_Connections[nCurIndex] = CreateNewConnection();
	}
	m_nLastAssignConnectionIndex = nCurIndex;
	return m_Connections[nCurIndex];
#endif 
}

CloudAZConnection* ConnctionPool::CreateNewConnection()
{
	QueryCloudAZMgr* pMgr = QueryCloudAZMgr::Instance();
	CloudAZConnection* pNewConn = new CloudAZConnection(
		pMgr->GetPCHost(), pMgr->GetPCPort(),m_msTimeout
		);
	return pNewConn;
}
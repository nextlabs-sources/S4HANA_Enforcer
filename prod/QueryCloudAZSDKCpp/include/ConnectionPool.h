#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include "QueryCloudAZExport.h"
#include "Connection.h"
#include <mutex>
//#include "CriticalSectionLock.h"

class ConnctionPool
{

private:
	ConnctionPool(const ConnctionPool&) {}

public:
	ConnctionPool();

	void SetConnectCount(int Count);
    void SetTimeout(const unsigned int msTimeout);
	CloudAZConnection* AssignConnection();
	CloudAZConnection* CreateNewConnection();



private:
	CloudAZConnection* m_Connections[MAX_CONNECTIONS_TO_CLOUDAZ];
	int m_nConnCount;
    unsigned int m_msTimeout = 2000;

	int m_nLastAssignConnectionIndex;
	//CRITICAL_SECTION  m_csLastAssignIndex;
    std::mutex m_mutex;

};
#endif


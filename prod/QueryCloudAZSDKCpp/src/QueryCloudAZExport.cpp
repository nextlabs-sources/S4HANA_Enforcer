#define QUERYCLOUDAZ_EXPORT

#include "QueryCloudAZExport.h"
#include "QueryCloudAZManager.h"

bool QueryCloudAZInit(const char* wszPCHost, const char* wszPcPort,
					  const char* wszOAuthServiceHost, const char* wszOAuthPort,
	                  const char* wszClientId, const char* wszClientSecret,  const char* wszTokenPath, const char* wszPDPPath, const unsigned int timeout,
	                 QueryJPCLogFun logFun)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->Init(wszPCHost, wszPcPort, wszOAuthServiceHost, wszOAuthPort, wszClientId, wszClientSecret, wszTokenPath, wszPDPPath, timeout, logFun, false);
}

bool QueryCloudAZInit202205(const char* wszPCHost, const char* wszPcPort,
                      const char* wszClientId, const char* wszClientSecret, const char* wszTokenPath, const char* wszPDPPath,
                      const unsigned int timeout,
                      QueryJPCLogFun logFun)
{
    QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
    return theMgr->Init(wszPCHost, wszPcPort, wszPCHost, wszPcPort, wszClientId, wszClientSecret,wszTokenPath, wszPDPPath, timeout, logFun, true);
}

bool UpdateQueryPCTimeout(unsigned int nTimeout)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
    return theMgr->UpdateTimeout(nTimeout);
}


QueryStatus CheckSingleResource(const IPolicyRequest* pcRequest, IPolicyResult** pcResult)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->CheckSingleResource(pcRequest, pcResult);
}

QueryStatus CheckMultiResource(const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->CheckMultiResource(pcRequest, nRequestCount, pcResult);
}

IPolicyRequest* CreatePolicyRequest()
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->CreatePolicyRequest();
}

IAttributes* CreateCEAttr()
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->CreateCEAttr();
}

void FreePolicyRequest(IPolicyRequest* pRequest)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->FreePolicyRequest(pRequest);
}

void FreePolicyResult(IPolicyResult* pResult)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->FreePolicyResult(pResult);
}

void FreeCEAttr(IAttributes* pAttr)
{
	QueryCloudAZMgr* theMgr = QueryCloudAZMgr::Instance();
	return theMgr->FreeCEAttr(pAttr);
}


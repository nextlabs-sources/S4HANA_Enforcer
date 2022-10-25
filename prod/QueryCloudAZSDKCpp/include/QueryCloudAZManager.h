#ifndef QUERY_CLOUDAZ_MANAGER_H
#define QUERY_CLOUDAZ_MANAGER_H

#include "QueryCloudAZManager.h"
#include "ConnectionPool.h"
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>

enum emLogLevel
{
    log_trace = 0,
    log_debug,
    log_info,
    log_warning,
    log_error,
    log_fatal
};


class QueryCloudAZMgr
{
protected:
	QueryCloudAZMgr();
	QueryCloudAZMgr(const QueryCloudAZMgr&) {}


public:
	static QueryCloudAZMgr* Instance()
	{
		static QueryCloudAZMgr* theMgr = NULL;
		if (theMgr==NULL)
		{
			theMgr = new QueryCloudAZMgr();
		}
		return theMgr;
	}

	//get member
public:
	const std::string& GetPCHost() { return m_strPCHost; }
	const std::string& GetPCPort() { return m_strPCPort; }
	const std::string& GetOAuthHost() { return m_strOAuthHost ; }
	const std::string& GetOAuthPort() { return m_strOAuthPort; }
	const std::string& GetClientID() { return m_strClientID; }
	const std::string& GetClientSecret() { return m_strClientSecret; }
	const std::string& GetTokenPath() { return m_strTokenPath; }
	const std::string& GetPDPPath() { return m_strPDPPath; }

	void GetToken(std::string& token, std::string& tokenType) {
		{
			boost::shared_lock<boost::shared_mutex> lock(m_mutexToken);	// read lock
			token = m_Token;
			tokenType = m_TokenType;
		}
        
		//if empty request token again
		if(tokenType.empty() || token.empty()){
			RequestToken();
			{
				boost::shared_lock<boost::shared_mutex> lock(m_mutexToken);	// read lock
				token = m_Token;
				tokenType = m_TokenType;
			}
		}
	}
 

public:
	bool Init(const char* wszPCHost, const char* wszPcPort,
		const char* wszOAuthServiceHost, const char* wszOAuthPort,
		const char* strClientId, const char* wszClientSecret,  const char* wszTokenPath, const char* wszPDPPath, unsigned int timeout,
		QueryJPCLogFun logFun=nullptr, bool bVer202205=false);

	bool UpdateTimeout(unsigned int nTimeout);
	QueryStatus  CheckSingleResource(const IPolicyRequest* ceRequest, IPolicyResult** pcResult);
	QueryStatus  CheckMultiResource (const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult);
	IPolicyRequest*  CreatePolicyRequest();
	IAttributes* CreateCEAttr();

	void  FreePolicyRequest(IPolicyRequest* pRequest);
	void  FreePolicyResult(IPolicyResult* pResult);
	void  FreeCEAttr(IAttributes* pAttr);

        void  WriteLog(int level, const char*szLog){
			if(nullptr!=m_WriteLogFun){
				m_WriteLogFun(level, szLog);
			}
        }           

private:
	bool RequestToken();
	//static DWORD WINAPI RefreshTokenThread(LPVOID lpParam);

private:
	bool ErrCheck(CloudAZConnection* pConn, QueryStatus qs);

private:
	std::string m_strPCHost;
	std::string m_strPCPort;
	std::string m_strOAuthHost;
	std::string m_strOAuthPort;
	std::string m_strClientID;
	std::string m_strClientSecret;
	std::string m_strTokenPath;
	std::string m_strPDPPath;
    unsigned int m_timeout = 5000; //millisecond
	ConnctionPool m_connPool;

	std::string m_Token;
	std::string m_TokenType;
	int m_nTokenExpier;
	boost::shared_mutex m_mutexToken;


	static const int MaxTryTimes = 2;
    QueryJPCLogFun m_WriteLogFun;
};


#endif



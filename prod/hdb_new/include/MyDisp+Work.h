#ifndef MY_DISP_WORK_H
#define MY_DISP_WORK_H

#include "interface.h"
#include "MyDisp+Work_type.h"

class MyDisp_Work
{
public:
	~MyDisp_Work();

private:
	MyDisp_Work();
	MyDisp_Work(const MyDisp_Work&) {}

public:
	static MyDisp_Work* Instance() {
		static MyDisp_Work mydw;
		return &mydw;
	}
 
	bool Init();
    unsigned long GetSymbolOffsetByName(const char* szModuleName, const char* szSymbolName);
        const DP_SESSION_INFO& GetCurrentSession() const{ return _currentSessionInfo; }

	bool GetCurrentLogonInfo(DAE_LOGON_INFO& myLogonInfo);
	const char* GetInternalSessionKey();

	#ifdef _AIX
	unsigned long GetTOCOfMyDispWork();
	unsigned long GetProcAddrSetCurrentSession();
	#endif 
   
    void PrintCurrentSession(const std::string& user, const DP_SESSION_INFO& sessionInfo) ;
	//hook api
private:
	static void ThWpSetCurrentSession_new(DP_SESSION_INFO info);
	static SAPRETURN ThRqCheckQueues_new(int nArg,REQUEST_BUF** ppRequestBuf);
    bool ReadSymbolAddrFromFile(std::map<std::string, SAP_SYMBOL>& mapSymbols);
private:
	//static ThWpSetCurrentSessionFun _ThWpSetCurrentSession_old;
	static ThRqCheckQueuesFun _ThRqCheckQueues_old;



public:
	static DpSesGetLogonNetworkAddressFun _DpSesGetLogonNetworkAddress_old;
    static DpSesGetLogonInfoFun           _DpSesGetLogonInfo_old;

private:
    DP_SESSION_INFO _currentSessionInfo;
	char _szInternalSessionkey[64];

	#ifdef _AIX
	unsigned long _SetCurrentSessionProxy[3];
	#endif 
};

extern MyDisp_Work* g_theMyDW;

#endif 
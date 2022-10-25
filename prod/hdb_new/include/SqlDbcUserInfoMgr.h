#ifndef SQL_DBC_USER_INFO_H
#define SQL_DBC_USER_INFO_H

#include <map>
#include <string>
#include <set>
#include <vector>
#include "tool.h"
#include "NXS4HConfig.h"


#define USER_ATTR_NAME_USERNAME  "name"
#define USER_ATTR_NAME_TERMINAL  "terminal"
#define USER_ATTR_NAME_HOSTIP    "ip"
#define USER_ATTR_NAME_HOSTNAME  "host"
#define USER_ATTR_NAME_MANDT     "mandt"
#define USER_ATTR_NAME_TCODE     "tcode"


class EnforcerCtxImpl;
class BindParameters;
class EMS4HAst;

struct NxlDBCUserInfo
{
	//user login info
	std::string _strMANDT;
	std::string _strName;
	std::string _strIP; 
	std::string _tcode;
	//http://www.cs.columbia.edu/~nahum/w6998/papers/sosp87-timing-wheels.pdf
	time_t _time; //time

	NxlDBCUserInfo()
		: _time(time(NULL))
	{
	}

	//user attribute
	std::map<std::string, std::set<std::string>, CommonFun::IgnoreCaseCmp> _attribute;

    std::string Print() const;
	std::set<std::string> GetAttr(const std::string & name)const ;
	bool Compare(const NxlDBCUserInfo* src) const{
        if(src->_strMANDT != _strMANDT) return false;
        if(src->_strName != _strName) return false;
        if(src->_strIP != _strIP) return false;
        if(src->_tcode != _tcode) return false;
        for(auto it: _attribute) {
            auto it2 = src->_attribute.find(it.first);
            if(it2 == src->_attribute.end()){
                return false;
            } else {
                if(it2->second != it.second){
                    return false;
                }
            }
        }
        return true;
    }
};

class CDbcUserInfoMgr
{
public:

	~CDbcUserInfoMgr();

public:
	static CDbcUserInfoMgr* Instance()
	{
		static CDbcUserInfoMgr mgr;
		return &mgr;
	}


public:
	const NxlDBCUserInfo* GetCurrentUserInfo(void* pConn, EnforcerCtxImpl* pCtx, EMS4HAst* ast);
	void SyncChanges(NXS4HConfig* pConf);

private:
	CDbcUserInfoMgr();
	CDbcUserInfoMgr(const CDbcUserInfoMgr &) {}

	NxlDBCUserInfo* GetUserInfoFromUsr41ByTermID(void* pConn, int logonHdl);
	void AddedUserInfoToCache(const std::string& wstrUserName, NxlDBCUserInfo* pUserInfo);
	NxlDBCUserInfo* GetUserInfoFromCache(const std::string& wstrUserName);

	void GetUserAttributeFrom_V_ADDR_USR(void* pConn, NxlDBCUserInfo* pUserInfo);
	void GetUserAttributeFrom_CustomTable(void* pConn, NxlDBCUserInfo* pUserInfo);
	//void GetUserAttributeNameFor_V_ADDR_USR(const std::string& strSql);

	bool IsUserInfoExpired(const NxlDBCUserInfo* pUserInfo) const;
	void RemoveUserInfoFromCache(const NxlDBCUserInfo *pUserInfo);

	void GetSessionVariable(void* pConn, std::map<std::string,std::string>& mapValue);

	std::string GetWorkFlowRealUser(void* pConn, EnforcerCtxImpl* pCtx, EMS4HAst* ast);
	void SetExpireTime(double exp) { _expiry=exp; }

private:
	std::map<std::string, NxlDBCUserInfo*> _mapUserInfo;
	std::vector<std::string>  _vecUserAttrName_V_ADDR_USR;

	//std::deque<std::pair<std::map<Key, Value>::iterator, time_t>> deque;
	//to be executed periodically void Check() { }
	double _expiry; //in seconds. See also difftime
	std::string _V_ADDR_USR_SQL;
	bool _inited_vecUserAttrName;
};

#endif //SQL_DBC_USER_INFO_H


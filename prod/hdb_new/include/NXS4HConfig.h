//
// Created by jeff on 2019/12/24.
//

#ifndef NXS4H_NXS4HCONFIG_H
#define NXS4H_NXS4HCONFIG_H

//filename:DAE.conf
#include <string>
#include <map>
#include <vector>
#include <set>
#include "tool.h"

typedef std::map<std::string,std::string> ConfigMap;
typedef std::set<std::string, CommonFun::IgnoreCaseCmp> WhiteListUserSet;

class EnforcerCtxImpl;

class NXS4HConfig {
public:
    NXS4HConfig();
    ~NXS4HConfig();
    bool InitConfig(const std::string & path, void* pDBConn);
    bool CycleUpdate( void* pDBConn);
    const std::string Print();

    bool IsWhiteListUser(const std::string& strUser);
    bool IsInited() { return _bInit;}

    // COMMON PARAMS
    unsigned int GetFeatureLevel();
    const std::string GetCommonFeatureLevel();
    double GetViewClearCycle();
    const std::string GetCommonViewClearCycle();

    // POLICY PARAMS
    const std::string GetPolicyCCHost();
    const std::string GetPolicyCCPort();
    const std::string GetPolicyCCUser();
    std::string GetCCPwd();
    const std::string GetPolicyCCPassword();
    const std::string GetPolicyJPCHost();
    const std::string GetPolicyJPCPort();
    const std::string GetPolicyJPCUser();
    std::string GetJPCPwd();
    const unsigned int GetQueryPCTimeout();
    const std::string GetPolicyJPCPassword();
    const std::string GetPolicyModelName();
    const std::string GetPolicyClearCycle();
    double GetPCClearCycle();
    const std::string GetJPCTokenPath();
    const std::string GetJPCPDPPath();
    int GetJPCVersion();
    bool UseSecurityTableList();

    // USERATTRIBUTE PARAMS
    const std::string GetUSERATTRUserSQL();
    const std::string GetUSERATTRCustomSQL();
    const std::string GetUSERATTRClearCycle();
    double GetUAClearCycle();

    // WHITELIST PARAMS
    //const std::string GetWhiteListApplyLevel();
    const std::string GetWhitelistUsers();
    const std::string GetWhiteListSQL();

    // ERRORHAND PARAMS
    const std::string GetErrorHandler();

    // PRIDATA PARAMS
    std::string GetSchema();
    const std::string GetPRIDATASchema();
    const std::string GetPRIDATAKeymaskTable();
    const std::string GetPRIDATAKeymaskFunc();
    const std::string GetPRIDATAKeymaskPrefix();

    // LOG PARAMS
    const std::string GetLogLevel();
    const std::string GetLogLocation();
    const std::string GetLogFilesize();
    const std::string GetLogMaxfiles();
    const std::string GetLogMinfreespace();

    // OTHER PARAMS
    const std::string GetTableAliasClearCycle();
    double GetTBAliasClearCycle();
    
private:
   void ParseConfig(const std::string& strCfgData, ConfigMap& mapConf);
   std::string parse_title(const std::string & read,unsigned int & pos);
   std::string parse_value(const std::string & read,unsigned int & pos,bool iskey);

   void InitWhiteList(WhiteListUserSet& whitelistUsers);
   void InitConfigValue();
   void GetSystemUserToWhiteList(void* pDBConn, WhiteListUserSet& whitelistUsers);
   std::string PrintWhiteListUser();
   void UpdateWhiteListUser(void* pDBConn);
   bool ReadCommonCfgFromRemote(void* pDBConn, ConfigMap& mapConf);
private:
    std::string _configFile;
    ConfigMap _value_map;   //both conf and table k-v
    WhiteListUserSet _whitelistUsers;   //both direct-conf and query-table 

    double _update_cycle;//second
    time_t _time_last_update;//last time to read configure data

    double _USERATTR_clearcycle;

    double _POLICY_clearcycle; //second;, we use this for both policy sync and whitelist update
    time_t _time_last_update_whitelist; //last time to read whitelist
    
    double _view_clear_cycle;
    double _tablealias_clear_cycle;

    bool _bInit;
};

extern NXS4HConfig * g_conf;

//conf value and parameter
#define CONFVAL_TRUE  "true"
#define CONFVAL_FALSE "false"
#define DAE_FEATURE_QPC  1
#define DAE_FEATURE_AST  2
#define DAE_FEATURE_NOT_FREE_NXLSTMT 4
#define DAE_FEATURE_EVA_CACHE 8
//16 32 64 128 512 is reserved
#define DAE_FEATURE_DISABLE_MSG_NOTIF       16
#define DAE_FEATURE_NOT_APPLY_OBLIGATION    32

#define CONFVAL_ERROR_HANDLER_ALLOW "allow"
#define CONFVAL_ERROR_HANDLER_DENY  "deny"
#define CONFVAL_EMPTY_STRING ""
#define CONFVAL_USERATTRUserSQL "SELECT \"FUNCTION\", V_ADDR_USR.Department as department_sap, Country, Region FROM V_ADDR_USR,USR21 WHERE V_ADDR_USR.CLIENT=USR21.MANDT AND V_ADDR_USR.PERSNUMBER=USR21.PERSNUMBER AND USR21.MANDT=:A0 AND USR21.BNAME=:A1"
#define CONFVAL_USERATTRCustomSQL "SELECT \"ATTR_NAME\", \"ATTR_VALUE\" FROM \"/NXLDAE/USR_ATTR\" WHERE \"MANDT\"=:A0 AND \"USER_ID\"=:A1"
#define CONFVAL_WhiteListSQL "SELECT BNAME FROM USR02 WHERE USTYP NOT IN ('A', 'S') OR BNAME LIKE 'R_%'"
#define JPC_VERSION_202205 202205
#endif //NXS4H_NXS4HCONFIG_H

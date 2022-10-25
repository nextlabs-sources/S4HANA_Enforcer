//
// Created by jeff on 2019/12/24.
//

#ifndef NXS4H_NXS4HCONFIG_H
#define NXS4H_NXS4HCONFIG_H

//filename:nxs4h.conf
#include <string>
#include <map>
#include <vector>
#include <set>

typedef  std::map<std::string,std::string> ConfigMap;

class NXS4HConfig {
public:
    //enum InitResult { INIT_OK, INIT_OPEN_FAILED, INIT_PASSWORD_UNDECRYPT}; 

    NXS4HConfig();
    ~NXS4HConfig();
    bool InitConfig(const std::string & path);
    const std::string &  operator[](const std::string & key); //key = title.key eg:POLICY.JPCHOST
    const std::string Print();
    bool IsWhiteListUser(const std::string& strUser);
    const std::vector<std::set<std::string>>& GetColumnAlias();
    void CycleUpdate();
    bool IsDeactiveDAE() { return _bDeactiveDAE; }
    const std::string& GetViewClearCycle() { return _view_clear_cycle; }

    std::string GetSchema();

    const std::string GetJPCTokenPath();
    const std::string GetJPCPDPPath();
    const unsigned int GetQueryPCTimeout();
    int GetJPCVersion();
    bool UseSecurityTableList();

private:
   void ParseConfig(const std::string& strCfgData);
   std::string parse_title(const std::string & read,unsigned int & pos);
   std::string parse_value(const std::string & read,unsigned int & pos,bool iskey);

   void InitWhiteList();
   void InitColumnAlias();
   void InitConfigValue();

   std::string PrintColumnAlias();
   std::string PrintWhiteListUser();


private:
    std::string _configFile;
    ConfigMap _value_map;  //this is used for solid config item. its data will never change after inited. So that user can use reference/point to point it.
    ConfigMap _value_map_dynamic; //this is used for dynamic config item. its value will changed after an _update_cycle. 
  
    double _update_cycle;//second
    time_t _time_last_update;//
    
    std::vector<std::string>  _whitelistUsers;
    std::vector<std::set<std::string>> _ColumnAliasSet;
    bool _bDeactiveDAE;
    std::string _view_clear_cycle;
};

extern NXS4HConfig * g_conf;

#define CONF_TRUE  "true"
#define CONF_FALSE "false"

//common
#define DEACTIVE_DAE "COMMON.deactive_dae"
#define VIEW_CLEAR_CYClE "COMMON.view_clearcycle"

//policy
#define CONF_CCHOST (*g_conf)["POLICY.cchost"]
#define CONF_CCPORT (*g_conf)["POLICY.ccport"]

#define CONF_CCUSER (*g_conf)["POLICY.ccuser"]
#define CONF_CCPWD (*g_conf)["POLICY.ccpwd"]

#define CONF_PCTAG (*g_conf)["POLICY.policytag"]



#define CONF_JPCHOST (*g_conf)["POLICY.jpchost"]
#define CONF_JPCPORT (*g_conf)["POLICY.jpcport"]

#define CONF_JPCUSER (*g_conf)["POLICY.jpcuser"]
#define CONF_JPCPWD (*g_conf)["POLICY.jpcpwd"]

#define CONF_CLEAR_CYCLE (*g_conf)["POLICY.clearcycle"]
#define CONF_MODEL (*g_conf)["POLICY.modelname"]

#define CONF_LOG_LEVEL (*g_conf)["LOG.level"]
#define CONF_LOG_LOCATION (*g_conf)["LOG.location"]
#define CONF_LOG_RETENTION_TIME (*g_conf)["LOG.retentiontime"]
#define CONF_LOG_FILE_SIZE (*g_conf)["LOG.filesize"]

//File count upper limit
#define CONF_LOG_MAX_FILES (*g_conf)["LOG.maxfiles"]
//Free space lower limit
#define CONF_LOG_MIN_FREE_SPACE (*g_conf)["LOG.minfreespace"]

#define CONF_USER_ATTR_VADDRUSR_SQL (*g_conf)["USERATTRIBUTE.V_ADDR_USR_SQL"]
#define CONF_USER_ATTR_CUSTOM_SQL (*g_conf)["USERATTRIBUTE.CUSTOM_ATTR_SQL"]

#define CONF_USER_ATTR_CLEAR_CYCLE (*g_conf)["USERATTRIBUTE.clearcycle"]


//White List
#define WHITELIST_USER "WHITELIST.users"
#define CONF_WHITELIST_USER (*g_conf)[WHITELIST_USER]

//Error handler
#define ERROR_HANDLER_ALLOW "allow"
#define ERROR_HANDLER_DENY  "deny"
#define CONF_ERROR_HANDLER (*g_conf)["ERRORHAND.exception_handler"]


//private data in database
#define CONF_PRIDATA_SCHEMA (*g_conf)["PRIDATA.schemaname"]
#define CONF_PRIDATA_KEYMASK_TABLE (*g_conf)["PRIDATA.keymaskcachetablename"]
#define CONF_PRIDATA_KEYMASK_FUNCTION (*g_conf)["PRIDATA.keymaskfunction"]
#define CONF_PRIDATA_KEYMASK_PREFIX (*g_conf)["PRIDATA.keymaskprefix"]


#endif //NXS4H_NXS4HCONFIG_H

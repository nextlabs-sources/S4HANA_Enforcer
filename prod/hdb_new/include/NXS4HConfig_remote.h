//
// Created by jeff on 2019/12/24.
//
#if 0
#ifndef NXS4H_NXS4HCONFIG_REMOTE_H
#define NXS4H_NXS4HCONFIG_REMOTE_H

//filename:DAE.conf
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
    bool InitConfig(void* pDBConn);
    const std::string &  operator[](const std::string & key); //key = title.key eg:POLICY.JPCHOST
    const std::string Print();
    bool IsWhiteListUser(const std::string& strUser);
    void CycleUpdate(void* pDBConn);
    bool IsDeactiveDAE() { return _bDeactiveDAE; }
    const std::string& GetViewClearCycle() { return _view_clear_cycle; }
    bool IsInited() { return _bInit; }
    std::string GetJPCPwd();
    std::string GetCCPwd();
private:
    bool ReadConfigFromRemote(void* pDBConn);
    bool ReadCommonCfgFromRemote(void* pDBConn, ConfigMap& mapConf);
    bool ReadWhiteListUserFromRemote(void* pDBConn, std::vector<std::string>& listUsers);

   void InitConfigValue();

   std::string PrintWhiteListUser();

private:
    ConfigMap _value_map;  //this is used for solid config item. its data will never change after inited. So that user can use reference/point to point it.
    ConfigMap _value_map_dynamic; //this is used for dynamic config item. its value will changed after an _update_cycle. 
  
    double _update_cycle;//second
    time_t _time_last_update;//
    
    std::vector<std::string>  _whitelistUsers;
    bool _bDeactiveDAE;
    std::string _view_clear_cycle;

    bool _bInit;
};

extern NXS4HConfig * g_conf;

#define CONF_TRUE  "true"
#define CONF_FALSE "false"

//common
#define DEACTIVE_DAE "deactive_dae"
#define VIEW_CLEAR_CYClE "view_clearcycle"


//policy
#define CONF_CCHOST (*g_conf)["cchost"]
#define CONF_CCPORT (*g_conf)["ccport"]

#define CONF_CCUSER (*g_conf)["ccuser"]
#define CONF_CCPWD_KEY  "ccpwd"

#define CONF_JPCHOST (*g_conf)["jpchost"]
#define CONF_JPCPORT (*g_conf)["jpcport"]

#define CONF_JPCUSER (*g_conf)["jpcuser"]
#define CONF_JPCPWD_KEY "jpcpwd"

#define CONF_CLEAR_CYCLE (*g_conf)["policy_clearcycle"]
#define CONF_MODEL (*g_conf)["policy_modelname"]

#define CONF_LOG_LEVEL (*g_conf)["log_level"]
#define CONF_LOG_LOCATION (*g_conf)["log_location"]
//#define CONF_LOG_RETENTION_TIME (*g_conf)["LOG.retentiontime"]
#define CONF_LOG_FILE_SIZE (*g_conf)["log_filesize"]

//File count upper limit
#define CONF_LOG_MAX_FILES (*g_conf)["log_maxfiles"]
//Free space lower limit
#define CONF_LOG_MIN_FREE_SPACE (*g_conf)["log_minfreespace"]

#define CONF_USER_ATTR_VADDRUSR_SQL (*g_conf)["V_ADDR_USR_SQL"]
#define CONF_USER_ATTR_CUSTOM_SQL (*g_conf)["CUSTOM_ATTR_SQL"]

#define CONF_USER_ATTR_CLEAR_CYCLE (*g_conf)["userattr_clearcycle"]
#define CONF_TABLE_ALIAS_CLEAR_CYCLE (*g_conf)["tablealias_clearcycle"]

//Error handler
#define ERROR_HANDLER_ALLOW "allow"
#define ERROR_HANDLER_DENY  "deny"
#define CONF_ERROR_HANDLER (*g_conf)["exception_handler"]


//private data in database
#define CONF_PRIDATA_SCHEMA (*g_conf)["schemaname"]
#define CONF_PRIDATA_KEYMASK_TABLE (*g_conf)["keymaskcachetablename"]
#define CONF_PRIDATA_KEYMASK_FUNCTION (*g_conf)["keymaskfunction"]
#define CONF_PRIDATA_KEYMASK_PREFIX (*g_conf)["keymaskprefix"]


#endif //NXS4H_NXS4HCONFIG_H
#endif
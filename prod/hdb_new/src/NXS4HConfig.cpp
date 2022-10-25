//
// Created by jeff on 2019/12/24.
//

#include "NXS4HConfig.h"
#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4714)
#include <boost/algorithm/string.hpp>
#pragma warning(pop)

#include "log.h"
#include <AESEncrypt.h>
#include <algorithm>
#ifdef _WIN32
#else
#include <experimental/algorithm>
#endif
#include <tool.h>
#include "SQLDBC.h"
#include "talk_with_hdb.h"
#include "PolicyManager.h"
#include "SqlDbcUserInfoMgr.h"
#include "TableAliasMgr_remote.h"

std::string parse_value(std::string & read,unsigned int & pos,bool iskey);
std::string parse_title(std::string & read,unsigned int & pos);
bool read_from_file(std::string & ret, const std::string& file_path);

NXS4HConfig * g_conf = NULL;

NXS4HConfig::NXS4HConfig(){ 

	_update_cycle = 1 * 60;
    _bInit = false;
    _POLICY_clearcycle = 3600;
    _USERATTR_clearcycle = 3600;
    _view_clear_cycle = 3600;
    _tablealias_clear_cycle = 3600;
}
NXS4HConfig::~NXS4HConfig(){
}

bool NXS4HConfig::IsWhiteListUser(const std::string& strUser)
{
    auto itUser = _whitelistUsers.find(strUser);
    return itUser!=_whitelistUsers.end();
}

void NXS4HConfig::InitWhiteList(WhiteListUserSet& whitelistUsers)
{
    const std::string strWhiteListUser= this->GetWhitelistUsers();
    std::string delimiters = ", ";
    std::vector<std::string> listUsers;
    CommonFun::split(strWhiteListUser, listUsers, delimiters); 
    for(auto itListUser = listUsers.begin(); itListUser!=listUsers.end(); itListUser++){
        whitelistUsers.insert(*itListUser);
    }
}

void NXS4HConfig::InitConfigValue()
{
    //_view_clear_cycle
    double viewcycleHour = atof(this->GetCommonViewClearCycle().c_str());
	if (viewcycleHour < MIN_VIEW_CYCLE_HOUR){
		viewcycleHour = MIN_VIEW_CYCLE_HOUR;
	}
    _view_clear_cycle = viewcycleHour * 3600;

    //_POLICY_clearcycle
    double clearcycleHour = atof(this->GetPolicyClearCycle().c_str());
    if (clearcycleHour < MIN_POLICY_CYCLE_HOUR){
        clearcycleHour = MIN_POLICY_CYCLE_HOUR;
    }
    _POLICY_clearcycle = clearcycleHour * 3600;

    //_USERATTR_clearcycle
    double userattrcycleHour = atof(this->GetUSERATTRClearCycle().c_str());
    if ( userattrcycleHour < MIN_USER_ATTR_CYCLE_HOUR ){
         userattrcycleHour = MIN_USER_ATTR_CYCLE_HOUR;
    }
    _USERATTR_clearcycle = userattrcycleHour * 3600;
    
    //_tablealias_clear_cycle
    double tablealiascycleHour = atof(this->GetTableAliasClearCycle().c_str());
    if ( tablealiascycleHour < MIN_TABLE_ALIAS_CYCLE_HOUR ){
         tablealiascycleHour = MIN_TABLE_ALIAS_CYCLE_HOUR;
    }
    _tablealias_clear_cycle = tablealiascycleHour * 3600;
}

void NXS4HConfig::UpdateWhiteListUser(void* pDBConn)
{
    WhiteListUserSet whitelistUsers_temp;
    InitWhiteList(whitelistUsers_temp); // get white list from config file
    GetSystemUserToWhiteList(pDBConn, whitelistUsers_temp); //get white list from sql
    _whitelistUsers = whitelistUsers_temp;
    _time_last_update_whitelist = time(nullptr);
}

bool NXS4HConfig::InitConfig(const std::string & path , void* pDBConn) {
    //init param;
    _time_last_update = time(nullptr);
    _configFile = path;
    _bInit = true;
    
    ConfigMap value_map_temp;
    //read conf
    std::string read;
    if(!read_from_file(read, path)) {
        return false;
    }
    if (!read.empty()) {
        ParseConfig(read, value_map_temp);
    } else {
        return false;
    }
        
#ifndef DAE_LOCAL_CONFIG
    //read table
    ReadCommonCfgFromRemote(pDBConn, value_map_temp);
#endif //#ifndef DAE_LOCAL_CONFIG

    //init config
    if(!value_map_temp.empty()){
        _value_map = value_map_temp;
    }
    InitConfigValue();

    //init whitelist
    UpdateWhiteListUser(pDBConn);   //known issue,: inter calls map[key] method, will insert {key, ""} into map,  but NO any effect

    return true; 
}

void NXS4HConfig::GetSystemUserToWhiteList(void* pDBConn, WhiteListUserSet& whitelistUsers)
{
    //create sql
    const  std::string strSql = GetWhiteListSQL();     //"SELECT BNAME FROM USR02 WHERE USTYP NOT IN ('A', 'S') ";
    if (strSql.empty()){
        theLog->WriteLog(log_info, "GetSystemUserToWhiteList, sql not configured");
        return;
    }
    
    //create statement
    auto hdb_connection = (SQLDBC::SQLDBC_Connection *)pDBConn;
    SQLDBC::SQLDBC_PreparedStatement *pStatement = NULL;
    do
    {
        pStatement = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);
        if (pStatement == nullptr)  {
            theLog->WriteLog(log_error, "GetSystemUserToWhiteList, createPreparedStatement failed");
            break;
        }

        SQLDBC_Retcode rc = prepare_3_old(pStatement, (char *)strSql.c_str(), strSql.size(), SQLDBC_StringEncodingType::Ascii);
        if (SQLDBC_OK != rc)  {
            theLog->WriteLog(log_error, "GetSystemUserToWhiteList, prepare_3_old failed, rc=%d", rc);
            break;
        }

        rc = execute_old(pStatement);
        if (rc != SQLDBC_OK)  {
            theLog->WriteLog(log_error, "GetSystemUserToWhiteList, execute failed, rc=%d", rc);
            break;
        }

        auto pResultSet = (SQLDBC::SQLDBC_ResultSet *)get_result_set_old(pStatement);
        if (pResultSet == nullptr) {
            theLog->WriteLog(log_error, "GetSystemUserToWhiteList, get_result_set==null");
            break;
        }

        int nErrorCount = 0;
        while ((rc=SQLDBC_ResultSet_next_Old(pResultSet))==SQLDBC_OK)  {
             char szBNAME[255] = {0};
             SQLDBC_Length nLen = 0;
             rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szBNAME, &nLen, sizeof(szBNAME), SQLDBC_TRUE);
            if (rc != SQLDBC_OK) {
                nErrorCount++;
                theLog->WriteLog(log_info, "GetSystemUserToWhiteList getColumnValue failed. rc=%d,  msg:%s",  rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet))  );
                continue;
            }
            szBNAME[nLen] = 0;

            whitelistUsers.insert(szBNAME);
        }

         theLog->WriteLog(log_info, "GetSystemUserToWhiteList, nErrorCount:%d, nSuccessCount:%llu", nErrorCount, whitelistUsers.size() );
        
    } while (false);

    if (pStatement) {
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pStatement);
        pStatement = NULL;
    }
}

void NXS4HConfig::ParseConfig(const std::string& read, ConfigMap& mapConf)
{
    unsigned int pos = 0;
    size_t len = read.length();

    std::string key;
    std::string value;
    std::string title = "t";
    bool iskey = true;
    while(pos < len) {
        switch (read[pos]) {
            case ' ':
            case '\r':
            case '\t':     {
                pos++;
            } break;
            case ';':     {
                while(pos<len && read[pos] != '\n') {
                    pos++;
                }
            } break;
            case '[':{
                title = parse_title( read, pos);
            }
            case '\n': {
                if(!key.empty() && !value.empty()) {
                    std::string mapkey;
                    mapkey+=title;
                    mapkey+='.';
                    mapkey+=key;
                    //theLog->WriteLog(log_debug, "InitConfig|%s=%s", key.c_str(), value.c_str());
                    mapConf.insert(make_pair(mapkey, value));
                }
                key.clear();
                value.clear();
                iskey = true;
                pos++;
            } break;
            case '=': {
                iskey = false;
                pos++;
            } break;
            default:
                std::string ss = parse_value(read, pos, iskey);
                if(iskey){
                    key = ss;
                } else {
                    value = ss;
                }
        }

    }

    if(!key.empty() && !value.empty()){
        std::string mapkey;
        mapkey += title;
        mapkey += '.';
        mapkey += key;
        //theLog->WriteLog(log_debug, "InitConfig|%s=%s", key.c_str(), value.c_str());
        mapConf.insert(make_pair(mapkey, value));
    }

    return;
}

const std::string NXS4HConfig::Print() {
    std::string ret;
    ret+="(CONFIG: ";
    for(auto it:_value_map){
        ret+="( ";
        ret+=it.first;
        ret+=": ";
        ret+=it.second;
        ret+="), ";
    }
    ret+=")\nWhite list users:";
 
    for (auto itUser : _whitelistUsers){
        ret += itUser + ",";
    }
    
    return ret;
}

std::string NXS4HConfig::PrintWhiteListUser()
{
    std::string strWhiteListUser="User white list:";
    auto itUserWhite = _whitelistUsers.begin();
    while (itUserWhite != _whitelistUsers.end()){
        strWhiteListUser += *itUserWhite + ",";
        itUserWhite++;
    }
    return strWhiteListUser;    
}

std::string NXS4HConfig::GetSchema()
{
    std::string strSchema = this->GetPRIDATASchema();
    if(strSchema.empty()) {
        strSchema = "SAPSR3";
    }

    return strSchema;
}

bool  NXS4HConfig::CycleUpdate(void* pDBConn)
{
    //config data
    if(difftime(time(nullptr), _time_last_update) > _update_cycle){

        _time_last_update = time(nullptr);
        ConfigMap value_map_temp;

        //read config again
        std::string read;
        if(!read_from_file(read, _configFile)) {
            return false;
        }
        if (!read.empty()) {
            ParseConfig(read, value_map_temp);
        } else {
            theLog->WriteLog(log_error, "NXS4HConfig::CycleUpdate, read_from_file failed");
            return false;
        }

#ifndef DAE_LOCAL_CONFIG
        //read table again
        bool bSync = ReadCommonCfgFromRemote(pDBConn, value_map_temp);
        if (!bSync) {
            theLog->WriteLog(log_error, "NXS4HConfig::CycleUpdate, ReadCommonCfgFromRemote failed");
        }
#endif //#ifndef DAE_LOCAL_CONFIG

        //init conf
        if(!value_map_temp.empty()){
            _value_map = value_map_temp;
            //theLog->WriteLog(log_debug, "NXS4HConfig::CycleUpdate, updated config, count:%lld, content: %s", value_map_temp.size(), NXS4HConfig::Print().c_str());
        }
        InitConfigValue();

        //notify changes to other units
#ifndef DAE_LOCAL_CONFIG
        if (!read.empty() && bSync) {
#else
        if (!read.empty()) {            
#endif //#ifndef DAE_LOCAL_CONFIG
            theLog->UpdateLogLevel(this->GetLogLevel());

            PolicyManager::Instance()->UpdatePolicySyncInterval(_POLICY_clearcycle, this->GetQueryPCTimeout());

            CDbcUserInfoMgr::Instance()->SyncChanges(this);

#ifndef DAE_LOCAL_CONFIG
            TableAliasMgr::Instance().UpdateClearCycleInterval(_tablealias_clear_cycle);
#endif
        }

          theLog->WriteLog(log_info, "NXS4HConfig::CycleUpdate, update config, FeatureLevel:%d, QueryPC timeout:%ds", 
                         this->GetFeatureLevel(), this->GetQueryPCTimeout() );

        //white list, use policy sync time to update white list
        if (difftime(time(nullptr), _time_last_update_whitelist) > _POLICY_clearcycle)  {
            UpdateWhiteListUser(pDBConn);
            theLog->WriteLog(log_info, std::bind(&NXS4HConfig::PrintWhiteListUser, this));
        }

    } else {
        return false;
    }

    return true;
}

bool read_from_file(std::string & ret, const std::string& file_path)
{
    std::ifstream t(file_path);
    ret = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return true;
}

std::string NXS4HConfig::parse_value(const std::string & read,unsigned int & pos,bool iskey) {
    std::string ret;
    while(pos < read.length()) {
        if(read[pos]=='\0' || read[pos] == '\n' ||  read[pos] == '\r' )
            break;
        if(iskey ){
            if(read[pos] == '\0' || 
                read[pos] == ' ' || 
                read[pos] == '\n' || 
                read[pos] == '\r'|| 
                read[pos] == '\t' ||
                read[pos] == '=') 
                break;
        }
        ret+=read[pos];
        pos++;
    }
    return ret;
}

std::string NXS4HConfig::parse_title(const std::string & read,unsigned int & pos) {
    pos++;
    std::string ret;
    while(pos < read.length()) {
        if(read[pos]=='\0' || read[pos]==' ' || read[pos] == '\n' || read[pos] == '\r'|| read[pos] == '\t')
            break;
        if(read[pos] == ']') {
            pos++;
            break;
        }
        ret+=read[pos];
        pos++;
    }
    return ret;
}

std::string NXS4HConfig::GetJPCPwd()
{
    theLog->WriteLog(log_info, "GetJPCPwd with decrypt"); //when decrpt password failed, program may crash. so this log is usefull.
    std::string strPwd;
    const std::string& strCipher = this->GetPolicyJPCPassword();
    if (std::string::npos == strCipher.find_first_not_of("0123456789ABCDEF")) {
        CommonFun::AesEncryptor theAes;
        strPwd = theAes.DecryptString(strCipher);
    }else{
         theLog->WriteLog(log_info, "GetJPCPwd, incorrect password:%s", strCipher.c_str() );
    } 
    
    theLog->WriteLog(log_info, "GetJPCPwd with decrypt success");
    
    return strPwd;
}

const unsigned int NXS4HConfig::GetQueryPCTimeout(){
     std::string ret = _value_map["policy_timeout"]; //read from table first.
    if(ret.empty()){
        ret=_value_map["POLICY.timeout"]; //read frm DAE.conf
    }
    if (ret.empty()){
        return 5;
    }else{
        return atoi(ret.c_str());
    }
}


unsigned int NXS4HConfig::GetFeatureLevel()
{
    const std::string& strFeatureLev = this->GetCommonFeatureLevel();
    if (strFeatureLev.empty()){
        return (0xffffffff & (~DAE_FEATURE_EVA_CACHE));  //all feature
    }else{
          return atoi(strFeatureLev.c_str());
    }
    
}

std::string NXS4HConfig::GetCCPwd()
{
    theLog->WriteLog(log_info, "GetCCPwd with decrypt"); //when decrpt password failed, program may crash. so this log is usefull.
    std::string strPwd;
    const std::string& strCipher = this->GetPolicyCCPassword();
    if (std::string::npos == strCipher.find_first_not_of("0123456789ABCDEF")) {
        CommonFun::AesEncryptor theAes;
        strPwd = theAes.DecryptString(strCipher);
    }else{
         theLog->WriteLog(log_info, "GetCCPwd, incorrect password:%s", strCipher.c_str() );
    } 
    
    theLog->WriteLog(log_info, "GetCCPwd with decrypt success");
    
    return strPwd;

}

bool NXS4HConfig::ReadCommonCfgFromRemote(void* pDBConn, ConfigMap& mapConf)
{
    //create sql
	const char* szSql = "(select \"NAME\", \"VALUE\" from  \"/NXLDAE/CONF\")";
	bool bResult = false;
    //create statement
	SQLDBC::SQLDBC_Connection* hdb_connection = (SQLDBC::SQLDBC_Connection*)pDBConn;
    SQLDBC::SQLDBC_Statement* pStatement = NULL;
	do{
			pStatement = SQLDBC_Connection_createStatement_Old(hdb_connection);
	    	if(pStatement==nullptr){
				theLog->WriteLog(log_info, "ReadCommonCfgFromRemote, createStatement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
				break;
			}

			SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStatement, szSql);
			if(rc!=SQLDBC_OK){
				theLog->WriteLog(log_info, "ReadCommonCfgFromRemote, execute failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
				break;
			}
	
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				theLog->WriteLog(log_info, "ReadCommonCfgFromRemote, get_result_set==null");
				break;
			}

			while (true){

          		rc =  SQLDBC_ResultSet_next_Old(pResultSet);
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					if (rc!=SQLDBC_NO_DATA_FOUND){
						theLog->WriteLog(log_info, "ReadCommonCfgFromRemote, call next failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					} else
                        bResult = true;
					break;
				}

				char szAttrName[255]={0};
				SQLDBC_Length nLenAttrName = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szAttrName, &nLenAttrName, sizeof(szAttrName), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_info, "ReadCommonCfgFromRemote, get colomn 1 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				szAttrName[nLenAttrName] = 0;
                
				char szAttrValue[255]={0};
				SQLDBC_Length nLenAttrValue = 0;
				 rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, szAttrValue, &nLenAttrValue, sizeof(szAttrValue), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_info, "ReadCommonCfgFromRemote, get colomn 2 attribute value failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				szAttrValue[nLenAttrValue] = 0;

                std::string value = szAttrValue;
				if (nLenAttrName>0 && nLenAttrValue>0){
					mapConf[szAttrName] = value;
				}
			}
		} while (false);

		//release
		if (NULL!=pStatement){
			SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
			pStatement = NULL;
		}

    return bResult;

}

const std::string NXS4HConfig::GetCommonFeatureLevel() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["COMMON.feature_level"]; 
#else
    std::string ret = _value_map["feature_level"]; 
#endif
    return ret.empty() ? "15" : ret; 
}
const std::string NXS4HConfig::GetCommonViewClearCycle() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["COMMON.view_clearcycle"]; 
#else
    std::string ret = _value_map["view_clearcycle"]; 
#endif
    return ret.empty() ? "0.1" : ret;
}
double NXS4HConfig::GetViewClearCycle(){
    return _view_clear_cycle;
}

const std::string NXS4HConfig::GetPolicyCCHost() { 
    std::string ret = _value_map["POLICY.cchost"]; 
    return ret.empty() ? "https://edrm-cc.azure.cloudaz.net" : ret;
}
const std::string NXS4HConfig::GetPolicyCCPort() { 
    std::string ret = _value_map["POLICY.ccport"]; 
    return ret.empty() ? "443" : ret;
}
const std::string NXS4HConfig::GetPolicyCCUser() { 
    std::string ret = _value_map["POLICY.ccuser"]; 
    return ret.empty() ? CONFVAL_EMPTY_STRING : ret;
}
const std::string NXS4HConfig::GetPolicyCCPassword() { 
    std::string ret = _value_map["POLICY.ccpwd"]; 
    return ret.empty() ? CONFVAL_EMPTY_STRING : ret;
}
const std::string NXS4HConfig::GetPolicyJPCHost() { 
    std::string ret = _value_map["POLICY.jpchost"]; 
    return ret.empty() ? "https://edrm-jpc.azure.cloudaz.net" : ret;
}
const std::string NXS4HConfig::GetPolicyJPCPort() { 
    std::string ret = _value_map["POLICY.jpcport"]; 
    return ret.empty() ? "443" : ret;
}
const std::string NXS4HConfig::GetPolicyJPCUser() { 
    std::string ret = _value_map["POLICY.jpcuser"]; 
    return ret.empty() ? CONFVAL_EMPTY_STRING : ret;
}
const std::string NXS4HConfig::GetPolicyJPCPassword() { 
    std::string ret = _value_map["POLICY.jpcpwd"]; 
    return ret.empty() ? CONFVAL_EMPTY_STRING : ret;
}
const std::string NXS4HConfig::GetPolicyModelName() { 
    std::string ret = _value_map["POLICY.modelname"]; 
    return ret.empty() ? "em_sap_ecc" : ret;
}
const std::string NXS4HConfig::GetPolicyClearCycle() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["POLICY.clearcycle"]; 
#else
    std::string ret = _value_map["policy_clearcycle"]; 
#endif
    return ret.empty() ? "0.1" : ret;
}
double NXS4HConfig::GetPCClearCycle(){
    return _POLICY_clearcycle;
}

const std::string NXS4HConfig::GetUSERATTRUserSQL() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["USERATTRIBUTE.V_ADDR_USR_SQL"]; 
#else
    std::string ret = _value_map["V_ADDR_USR_SQL"]; 
#endif
    return /*ret.empty() ? CONFVAL_USERATTRUserSQL :*/ ret;
}
const std::string NXS4HConfig::GetUSERATTRCustomSQL() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["USERATTRIBUTE.CUSTOM_ATTR_SQL"]; 
#else
    std::string ret = _value_map["CUSTOM_ATTR_SQL"]; 
#endif
    return /*ret.empty() ? CONFVAL_USERATTRCustomSQL :*/ ret;
}
const std::string NXS4HConfig::GetUSERATTRClearCycle() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["USERATTRIBUTE.clearcycle"]; 
#else
    std::string ret = _value_map["userattr_clearcycle"]; 
#endif
    return ret.empty() ? "1" : ret;
}
double NXS4HConfig::GetUAClearCycle(){
    return _USERATTR_clearcycle;
}

// const std::string NXS4HConfig::GetWhiteListApplyLevel() { 
//     std::string ret = _value_map["WHITELIST.applylevel"]; 
//     return ret.empty() ? "0" : ret;
// }
const std::string NXS4HConfig::GetWhitelistUsers() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["WHITELIST.users"]; 
#else
    std::string ret = _value_map["whitelist_Users"]; 
#endif
    return ret.empty() ? "DDIC," : ret;
}
const std::string NXS4HConfig::GetWhiteListSQL() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["WHITELIST.sql_whitelist"]; 
#else
    std::string ret = _value_map["sql_whitelist"]; 
#endif
    return /*ret.empty() ? CONFVAL_WhiteListSQL :*/ ret;
}

const std::string NXS4HConfig::GetErrorHandler() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["ERRORHAND.exception_handler"]; 
#else
    std::string ret = _value_map["exception_handler"]; 
#endif
    return ret.empty() ? CONFVAL_ERROR_HANDLER_ALLOW : ret;
}

const std::string NXS4HConfig::GetPRIDATASchema() { 
    std::string ret = _value_map["PRIDATA.schemaname"]; 
    return ret.empty() ? "SCHEMA_NAME" : ret;
}
const std::string NXS4HConfig::GetPRIDATAKeymaskTable() { 
    std::string ret = _value_map["PRIDATA.keymaskcachetablename"]; 
    return ret.empty() ? "KEYMASK_CACHE" : ret;
}
const std::string NXS4HConfig::GetPRIDATAKeymaskFunc() { 
    std::string ret = _value_map["PRIDATA.keymaskfunction"]; 
    return ret.empty() ? "KEY_MASK" : ret;
}
const std::string NXS4HConfig::GetPRIDATAKeymaskPrefix() { 
    std::string ret = _value_map["PRIDATA.keymaskprefix"]; 
    return ret.empty() ? "*_" : ret;
}

const std::string NXS4HConfig::GetLogLevel() { 
#ifdef DAE_LOCAL_CONFIG
    std::string ret = _value_map["LOG.level"]; 
#else
    std::string ret = _value_map["log_level"]; 
#endif
    return ret.empty() ? "0" : ret;
}
const std::string NXS4HConfig::GetLogLocation() { 
    std::string ret = _value_map["LOG.location"]; 
    return ret.empty() ? "/usr/NextLabs/DAE/log" : ret;
}
const std::string NXS4HConfig::GetLogFilesize() { 
    std::string ret = _value_map["LOG.filesize"]; 
    return ret.empty() ? "10" : ret;
}
const std::string NXS4HConfig::GetLogMaxfiles() { 
    std::string ret = _value_map["LOG.maxfiles"]; 
    return ret.empty() ? "2" : ret;
}
const std::string NXS4HConfig::GetLogMinfreespace() { 
    std::string ret = _value_map["LOG.minfreespace"]; 
    return ret.empty() ? "10" : ret;
}

const std::string NXS4HConfig::GetTableAliasClearCycle() { 
    std::string ret = _value_map["tablealias_clearcycle"]; 
    return ret.empty() ? "1.0" : ret;
}
double NXS4HConfig::GetTBAliasClearCycle(){
    return _tablealias_clear_cycle;
}

int NXS4HConfig::GetJPCVersion(){
    const std::string& ret = _value_map["POLICY.jpc_version"];
    if(ret.empty()) return 0;
    return int(atoi(ret.c_str()));
}

bool NXS4HConfig::UseSecurityTableList(){
    const std::string& ret = _value_map["POLICY.use_security_table_policy"];
    return CommonFun::StrCaseCmp(ret, CONFVAL_TRUE)==0;
}

const std::string NXS4HConfig::GetJPCTokenPath(){
    std::string ret = _value_map["POLICY.token_path"];
    return ret;
}

const std::string NXS4HConfig::GetJPCPDPPath(){
    return _value_map["POLICY.pdp_path"];
}
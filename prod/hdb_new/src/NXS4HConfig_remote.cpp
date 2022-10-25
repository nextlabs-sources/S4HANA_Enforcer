//
// Created by jeff on 2019/12/24.
//
#if 0
#ifndef DAE_LOCAL_CONFIG

#include "NXS4HConfig_remote.h"
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "log.h"
#include <AESEncrypt.h>
#include <algorithm>
#ifndef _WIN32
#include <experimental/algorithm>
#endif 
#include <tool.h>
#include "SQLDBC.h"
#include "talk_with_hdb.h"

NXS4HConfig * g_conf = NULL;

NXS4HConfig::NXS4HConfig(){ 

	_update_cycle = 1 * 60;
    _bDeactiveDAE = false;
    _bInit = false;

}
NXS4HConfig::~NXS4HConfig(){
}

bool NXS4HConfig::IsWhiteListUser(const std::string& strUser)
{
    bool bWhite = false;
    std::vector<std::string>::iterator itUser = _whitelistUsers.begin();
    while (itUser != _whitelistUsers.end() ){
        if (CommonFun::StrCaseCmp(strUser, *itUser)==0){
            bWhite = true;
            break;
        }
        itUser++;
    }
    return bWhite;
}

void NXS4HConfig::InitConfigValue()
{
    //deactive dae
    _bDeactiveDAE = (CommonFun::StrCaseCmp(CONF_TRUE, _value_map_dynamic[DEACTIVE_DAE])==0);

    _view_clear_cycle = _value_map_dynamic[VIEW_CLEAR_CYClE];
}

bool NXS4HConfig::InitConfig(void* pDBConn) {

    if(!ReadConfigFromRemote(pDBConn)) {
        return false;
    }

    if (!_value_map_dynamic.empty()) {
        _value_map = _value_map_dynamic;

       InitConfigValue();
    }

    _time_last_update = time(nullptr);

    _bInit = true;

    return true; 
}

const std::string & NXS4HConfig::operator[](const std::string & key) {
    return _value_map[key];
}

const std::string NXS4HConfig::Print() {
    std::string ret;
    ret+="(CONFIG: ";
    for(auto it:_value_map){
        ret+="(";
        ret+=it.first;
        ret+=":";
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


void NXS4HConfig::CycleUpdate(void* pDBConn)
{
    if(difftime(time(nullptr), _time_last_update) > _update_cycle){

        _time_last_update = time(nullptr);
        _whitelistUsers.clear();
        _value_map_dynamic.clear();

        //read config again
        if(!ReadConfigFromRemote(pDBConn)) {
            return ;
        }

        InitConfigValue();

        theLog->WriteLog(log_info, "NXS4HConfig::CycleUpdate, update config.");
        theLog->WriteLog(log_info, std::bind(&NXS4HConfig::PrintWhiteListUser, this));
        theLog->WriteLog(log_info, "DeactiveDAE:%s", (_bDeactiveDAE? "true" : "false") );
        theLog->WriteLog(log_info, "View clear cycle:%s", _view_clear_cycle.c_str() );

    }
}

bool NXS4HConfig::ReadCommonCfgFromRemote(void* pDBConn, ConfigMap& mapConf)
{
    //create sql
	const char* szSql = "(select \"NAME\", \"VALUE\" from  \"/NXLDAE/V_CONF\") UNION (select \"NAME\", \"VALUE\" from \"/NXLDAE/V_ENABLE\")";
	
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
					}	
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
    

    return true;

}
bool NXS4HConfig::ReadWhiteListUserFromRemote(void* pDBConn, std::vector<std::string>& listUsers)
{
    //create sql
	const char* szSql = "Select USER_ID from \"/NXLDAE/SUPERUSR\"";
	
    //create statement
	SQLDBC::SQLDBC_Connection* hdb_connection = (SQLDBC::SQLDBC_Connection*)pDBConn;
    SQLDBC::SQLDBC_Statement* pStatement = NULL;
	do{
			pStatement = SQLDBC_Connection_createStatement_Old(hdb_connection);
	    	if(pStatement==nullptr){
				theLog->WriteLog(log_info, "ReadWhiteListUserFromRemote, createStatement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
				break;
			}

			SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStatement, szSql);
			if(rc!=SQLDBC_OK){
				theLog->WriteLog(log_info, "ReadWhiteListUserFromRemote, execute failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
				break;
			}
	
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				theLog->WriteLog(log_info, "ReadWhiteListUserFromRemote, get_result_set==null");
				break;
			}

			while (true){

          		rc =  SQLDBC_ResultSet_next_Old(pResultSet);
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					if (rc!=SQLDBC_NO_DATA_FOUND){
						theLog->WriteLog(log_info, "ReadWhiteListUserFromRemote, call next failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					}	
					break;
				}
              
			    
				char userID[255]={0};
				SQLDBC_Length nLenUserID = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, userID, &nLenUserID, sizeof(userID), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_info, "ReadWhiteListUserFromRemote, get colomn 1 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				userID[nLenUserID] = 0;
                
				if (nLenUserID>0){
                   listUsers.push_back(userID);
				}		
			}
			
		} while (false);

		//release
		if (NULL!=pStatement){
			SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
			pStatement = NULL;
		}
    

    return true;
    
}

bool NXS4HConfig::ReadConfigFromRemote(void* pDBConn)
{
    if (NULL==pDBConn){
        return false;
    }
   
    bool bConfig = ReadCommonCfgFromRemote(pDBConn, _value_map_dynamic);
    if (bConfig){
        bConfig = ReadWhiteListUserFromRemote(pDBConn, _whitelistUsers);
    }
    
   return bConfig;
}

std::string NXS4HConfig::GetJPCPwd()
{
    theLog->WriteLog(log_info, "GetJPCPwd with decrypt"); //when decrpt password failed, program may crash. so this log is usefull.
    std::string strPwd;
    const std::string& strCipher = (*g_conf)[CONF_JPCPWD_KEY];
    if (std::string::npos == strCipher.find_first_not_of("0123456789ABCDEF")) {
        CommonFun::AesEncryptor theAes;
        strPwd = theAes.DecryptString(strCipher);
    }else{
         theLog->WriteLog(log_info, "GetJPCPwd, incorrect password:%s", strCipher.c_str() );
    } 
    
    theLog->WriteLog(log_info, "GetJPCPwd with decrypt success");
    
    return strPwd;
}

std::string NXS4HConfig::GetCCPwd()
{
    theLog->WriteLog(log_info, "GetCCPwd with decrypt"); //when decrpt password failed, program may crash. so this log is usefull.
    std::string strPwd;
    const std::string& strCipher = (*g_conf)[CONF_CCPWD_KEY];
    if (std::string::npos == strCipher.find_first_not_of("0123456789ABCDEF")) {
        CommonFun::AesEncryptor theAes;
        strPwd = theAes.DecryptString(strCipher);
    }else{
         theLog->WriteLog(log_info, "GetCCPwd, incorrect password:%s", strCipher.c_str() );
    } 
    
    theLog->WriteLog(log_info, "GetCCPwd with decrypt success");
    
    return strPwd;

}

#endif // DAE_LOCAL_CONFIG
#endif
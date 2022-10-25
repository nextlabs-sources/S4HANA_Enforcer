#include "SqlDbcUserInfoMgr.h"
#include "log.h"
#include "talk_with_hdb.h"
#include "../parser_yacc/EMS4HSelectAst.h"
#include "../parser_yacc/EMS4HUpdateAst.h"
#include "../parser_yacc/EMS4HUpsertAst.h"
#include "../parser_yacc/EMS4HInsertAst.h"
#include "../parser_yacc/sqlparser_hana_bison.h"
#include "../parser_yacc/parse_tool.h"
#include "interface.h"
#include "NXS4HConfig.h"
#include "tool.h"
#include "EnforcerCtxImpl.h"
#include  "MyDisp+Work.h"
#include "MyDisp+Work_type.h"


void DeletePraparedStatement(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt);
void parse_attributes(std::vector<std::string> & vec_attr, const std::string & strsql, S4HException & exc);


std::string NxlDBCUserInfo::Print() const {
	std::string strInfo = "NxlDBCUserInfo:";
	strInfo += _strName + ",";
	return strInfo;
}

std::set<std::string> NxlDBCUserInfo::GetAttr(const std::string &name) const
{
	std::set<std::string> setAttr;

	if (CommonFun::StrCaseCmp(name.c_str(), USER_ATTR_NAME_USERNAME) == 0) {
		setAttr.insert(_strName);
	}
	else if (CommonFun::StrCaseCmp(name.c_str(),USER_ATTR_NAME_HOSTIP) == 0) {
		setAttr.insert(_strIP);
	}
	else {
		auto itset = _attribute.find(name);
		if (itset != _attribute.end()) {
			setAttr = itset->second;
		}
	}
	
	return setAttr;
}

CDbcUserInfoMgr::CDbcUserInfoMgr()
{
	_expiry = g_conf->GetUAClearCycle();
	_V_ADDR_USR_SQL = g_conf->GetUSERATTRUserSQL();
	_inited_vecUserAttrName = false;
}

CDbcUserInfoMgr::~CDbcUserInfoMgr()
{
	for(auto el : _mapUserInfo){
		if(el.second) { 
			delete el.second;
			el.second = NULL;
		}
	}
}

#if 0
void CDbcUserInfoMgr::GetUserAttributeNameFor_V_ADDR_USR(const std::string& strUserAttrSql)
{
   #if 1
   S4HException exc;
   if(!_vecUserAttrName_V_ADDR_USR.empty()) _vecUserAttrName_V_ADDR_USR.clear();
   parse_attributes(_vecUserAttrName_V_ADDR_USR, strUserAttrSql, exc);
   std::string strLog= "user attribute name, ex:" + exc._detail + "name:";
   for(auto itAttrName: _vecUserAttrName_V_ADDR_USR){
	   strLog += itAttrName + ",";
   }
   theLog->WriteLog(log_info, strLog.c_str() );
   #else
   _vecUserAttrName_V_ADDR_USR.push_back("Function");
   _vecUserAttrName_V_ADDR_USR.push_back("Department");
   _vecUserAttrName_V_ADDR_USR.push_back("Country");
   _vecUserAttrName_V_ADDR_USR.push_back("Region");
   #endif 
   
}
#endif

void CDbcUserInfoMgr::GetSessionVariable(void* pConn, std::map<std::string,std::string>& mapValue)
{
	const static std::string strSql = "SELECT KEY, VALUE FROM M_SESSION_CONTEXT WHERE connection_id=current_connection AND KEY IN('APPLICATIONUSER','CLIENT');";

	auto hdb_connection = (SQLDBC::SQLDBC_Connection*)pConn;

		do
		{
			auto pStatement = SQLDBC_Connection_createStatement_Old(hdb_connection);
	    	if(pStatement==nullptr){
				theLog->WriteLog(log_error, "GetSessionVariable, createStatement failed");
				break;
			}

			SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStatement, strSql.c_str());
			if(rc!=SQLDBC_OK){
				SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
				pStatement = NULL;
				theLog->WriteLog(log_error, "GetSessionVariable, execute failed, rc=%d", rc);
				break;
			}
	
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
				pStatement = NULL;
				theLog->WriteLog(log_error, "GetSessionVariable, get_result_set==null");
				break;
			}


		    while ( SQLDBC_ResultSet_next_Old(pResultSet)==SQLDBC_OK)
			{
				char szColumnName[255]={0};
				SQLDBC_Length nNameLen = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szColumnName, &nNameLen, sizeof(szColumnName), SQLDBC_TRUE );
				if(rc!=SQLDBC_OK){
					theLog->WriteLog(log_error, "GetSessionVariable getColumnValue failed.iCol=%d,rc=%d",1, rc);
					continue;
				}
				szColumnName[nNameLen]=0;


				char szColumnValue[255]={0};
				SQLDBC_Length nValueLen = 0;
				rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, szColumnValue, &nValueLen, sizeof(szColumnValue), SQLDBC_TRUE );
				if(rc!=SQLDBC_OK){
					theLog->WriteLog(log_error, "GetSessionVariable getColumnValue failed.iCol=%d,rc=%d",2, rc);
					continue;
				}
			   szColumnValue[nValueLen] = 0;
     
	           theLog->WriteLog(log_info, "GetSessionVariable: %s=%s", szColumnName, szColumnValue);
			   mapValue[szColumnName]=szColumnValue;
			}
								
			SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
			pStatement = NULL;

		} while (false);
}

std::string CDbcUserInfoMgr::GetWorkFlowRealUser(void* pConn, EnforcerCtxImpl* pCtx, EMS4HAst* ast)
{
	 std::string strRealUser;

	if (!ast){
		return strRealUser;
	}
	
	EMStmtType emStmtType = ast->GetStatementType();
	if (emStmtType != E_STMT_TYPE_UPDATE /* && emStmtType != E_STMT_TYPE_UPSERT ||
				emStmtType != E_STMT_TYPE_INSERT && emStmtType != E_STMT_TYPE_DELETE*/
	) {
		return strRealUser;
	}

	EMS4HUpdateAst *pUpdateAst = dynamic_cast<EMS4HUpdateAst *>(ast);
	if (CommonFun::StrCaseCmp(pUpdateAst->GetTable()->_tb.c_str(), "EKPO") != 0) {
		return strRealUser;
	}
	theLog->WriteLog(log_info, "GetWorkFlowRealUser  replace work flow user with realy user");

	uint uParamIndex = 0;
	bool bGetparamID = pUpdateAst->GetWhereClauseConditionRightParamId("EBELN", uParamIndex);
	if ((!bGetparamID) || uParamIndex == 0) {
		theLog->WriteLog(log_info, "GetWorkFlowRealUser  get param  index failed");
		return strRealUser;
	}

	BindParameters *pParamWFID = pCtx->GetBindParamByPosition(uParamIndex);
	if (NULL == pParamWFID) {
		theLog->WriteLog(log_info, "GetWorkFlowRealUser  get bind param failed, index=%d", uParamIndex);
		return strRealUser;
	}

	//execute sql to get the realy user
   do {
	  const static std::string strSQL= "SELECT WI_CRUSER FROM SWWWIHEAD WHERE WI_ID IN (SELECT TOP 1 WI_ID FROM SWW_WI2OBJ WHERE INSTID = ? AND SWW_WI2OBJ.WI_ID=SWW_WI2OBJ.TOP_WI_ID ORDER BY WI_ID DESC)";

	  auto hdb_connection = (SQLDBC::SQLDBC_Connection *)pConn;
	  std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement, void (*)(SQLDBC::SQLDBC_PreparedStatement * pPreparedStamt)>
		  hdb_stmt(SQLDBC_Connection_createPreparedStatement_Old(hdb_connection), DeletePraparedStatement);
	  if (hdb_stmt == NULL)  {
		  theLog->WriteLog(log_info, "GetWorkFlowRealUser, create prepared statement failed.");
		  break;
	  }
     
	  SQLDBC_Retcode rc = prepare_3_old(hdb_stmt.get(), strSQL.c_str(), strSQL.length(), SQLDBC_StringEncodingType::Ascii);
      if (SQLDBC_OK != rc) {
    		theLog->WriteLog(log_info, "GetWorkFlowRealUser, call prepare_3_old failed:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
        	break;
      }

	  rc = bind_parameter_old(hdb_stmt.get(), 1, pParamWFID->_Type, pParamWFID->_paramAddr,
	  						 pParamWFID->_LengthIndicator, pParamWFID->_Size, pParamWFID->_Terminate);
	  if (SQLDBC_OK != rc) {
		  theLog->WriteLog(log_info, "GetWorkFlowRealUser, call bind_parameter_old failed. index:%d, msg:%s", 1, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
		  break;
	  }

	  //execute
	  rc = execute_old(hdb_stmt.get());
	  if (SQLDBC_OK != rc)  {
		  theLog->WriteLog(log_info, "GetWorkFlowRealUser, call execute_old failed. msg:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
		  break;
	  }

	  //get result
	  auto result_set = get_result_set_old(hdb_stmt.get());
	  if (result_set == nullptr)  {
		  theLog->WriteLog(log_info, "GetWorkFlowRealUser, call get_result_set_old failed. msg:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
		  break;
	  }

	  //get next
	  rc = SQLDBC_ResultSet_next_Old(result_set);
	  if (SQLDBC_OK != rc)  {
		  theLog->WriteLog(log_info, "GetWorkFlowRealUser, call next failed. rc=%d, msg:%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(result_set)));
		  break;
	  }

	  //get result
	  char szRealUser[64] = {0};
	  rc = SQLDBC_ResultSet_getObject_Old(result_set, 1, SQLDBC_HOSTTYPE_ASCII, szRealUser, nullptr, sizeof(szRealUser), SQLDBC_TRUE);
	  if (SQLDBC_OK != rc)  {
		  theLog->WriteLog(log_info, "GetWorkFlowRealUser, call getObject failed.index:%d, rc=%d, msg:%s", 1, rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(result_set)));
		  break;
	  }

	  theLog->WriteLog(log_info, "GetWorkFlowRealUser, success user:%s", szRealUser);
	  strRealUser = szRealUser;

   } while (false);
   
   return strRealUser;
}


//get user information.
//user informatoin contains two parts.  one is the basic attribute. e.g: department, title.
//the other one is login attribute, e.g: ip, login method(http, GUI)
const NxlDBCUserInfo* CDbcUserInfoMgr::GetCurrentUserInfo(void* pConn, EnforcerCtxImpl* pCtx, EMS4HAst* ast)
{
	DAE_LOGON_INFO logonInfo;
    g_theMyDW->GetCurrentLogonInfo(logonInfo);
    theLog->WriteLog(log_info, "GetCurrentUserInfo, termId:%d, logonInfo.client:%s, user:%s, ip:%s, tcode:%s", g_theMyDW->GetCurrentSession()._logonHdl,
                         logonInfo._client.c_str(), logonInfo._user.c_str(), logonInfo._ip.c_str(), logonInfo._tcode.c_str() );

	//get session variable
    //std::map<std::string, std::string> mapSessionVar;
	//GetSessionVariable(pConn, mapSessionVar);

	//get user name
	std::string strCurrentUserName  = logonInfo._user;
	std::string strClientID = logonInfo._client;
	if (strCurrentUserName.empty() || strClientID.empty() )
	{
		theLog->WriteLog(log_error, "GetCurrentUserInfo failed username:%s, clientid=%s", strCurrentUserName.c_str(), strClientID.c_str() );
		return NULL;
	}
	
	//if user name is SAP_WFRT, we need get the real user
	if (CommonFun::StrCaseCmp(strCurrentUserName.c_str(), "SAP_WFRT") == 0) {
		std::string strUserFromWF = GetWorkFlowRealUser(pConn, pCtx, ast);
		if (!strUserFromWF.empty()) {
			theLog->WriteLog(log_info, "GetCurrentUserInfo  GetWorkFlowRealUser:%s", strUserFromWF.c_str());
			strCurrentUserName = strUserFromWF;
		}
	}

	//get user information from cahce
	const std::string strUserCacheKey = strCurrentUserName+strClientID;
	NxlDBCUserInfo* pUserInfo = GetUserInfoFromCache(strUserCacheKey);


	//get user basic information, e.g: department, title
	if (NULL==pUserInfo)
	{
		//create user Info, added attribute from usr41
		pUserInfo = new NxlDBCUserInfo();
        pUserInfo->_strName = strCurrentUserName;
		pUserInfo->_strMANDT = strClientID;
		//Query user attribute from other tables
        GetUserAttributeFrom_V_ADDR_USR(pConn, pUserInfo);
		GetUserAttributeFrom_CustomTable(pConn, pUserInfo);

		//added UserInfo to cache
		AddedUserInfoToCache(strUserCacheKey, pUserInfo);
	}
	

	//update user session info, e.g: ip. the session information must be updated every time. 
	{
		pUserInfo->_strIP = logonInfo._ip;
		pUserInfo->_tcode = logonInfo._tcode;
	}

	 
	return pUserInfo;
}


NxlDBCUserInfo* CDbcUserInfoMgr::GetUserInfoFromCache(const std::string& strUserName)
{
	auto itUserInfo = _mapUserInfo.find(strUserName);
	if (itUserInfo != _mapUserInfo.end())
	{
		if (!IsUserInfoExpired(itUserInfo->second))
		{
			return itUserInfo->second;
		}
		theLog->WriteLog(log_trace, "GetUserInfoFromCache, expired for %s", strUserName.c_str()); //, itUserInfo->second->_time
		delete itUserInfo->second;
		itUserInfo->second = nullptr;
		_mapUserInfo.erase(itUserInfo);
	}
	return NULL;
}

void CDbcUserInfoMgr::AddedUserInfoToCache(const std::string& strUserName, NxlDBCUserInfo* pUserInfo) {
	_mapUserInfo[strUserName] = pUserInfo;
}


NxlDBCUserInfo* CDbcUserInfoMgr::GetUserInfoFromUsr41ByTermID(void* pConn, int logonHdl)
{
	NxlDBCUserInfo* pUserInfo = NULL;
	//create sql
	const char sqlFmt[] = "SELECT BNAME,TERMID,TERMINAL FROM USR41 WHERE TERMID=%d LIMIT 1;";
	char szSql[sizeof(sqlFmt) + 10];
	snprintf(szSql, sizeof(szSql),  sqlFmt, logonHdl);

	//theLog->WriteLog(log_info, "GetUserInfoFromUsr41ByTermID, sql=%s", szSql); too log for it, also it is info level, it will cover usefull log in info level

	//create statement
	auto hdb_connection = (SQLDBC::SQLDBC_Connection*)pConn;
    
	auto pStatement = SQLDBC_Connection_createStatement_Old(hdb_connection);
	if(pStatement){
		SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStatement, szSql);
		if (rc==SQLDBC_OK)
		{
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);

			rc =  SQLDBC_ResultSet_next_Old(pResultSet);
			if (rc == SQLDBC_OK)
			{
				pUserInfo = new NxlDBCUserInfo();

				//get user name
				{
					char szUserName[255] = { 0 };
					SQLDBC_Length ind;
					//CMySqldbLib::_SQLDBC_ResultSet_getObject_1_Old(pResultSet, 1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, szUserName,
					//	&ind, sizeof(szUserName), SQLDBC_TRUE);
					SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szUserName, 
							&ind, sizeof(szUserName), SQLDBC_TRUE );
					pUserInfo->_strName = szUserName;

					theLog->WriteLog(log_trace, "GetUserInfoFromUsr41ByTermID username=%s", szUserName);
				}

				//get terminal
				{
					char szTerminal[255] = { 0 };
					SQLDBC_Length ind;
					SQLDBC_ResultSet_getObject_Old(pResultSet, 3,  SQLDBC_HOSTTYPE_ASCII, szTerminal,
							&ind, sizeof(szTerminal), SQLDBC_TRUE);

					theLog->WriteLog(log_trace, "GetUserInfoFromUsr41ByTermID szTerminal=%s", szTerminal);
				}
			}
		}
		SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
		pStatement = NULL;
	}

	return pUserInfo;
}

void CDbcUserInfoMgr::GetUserAttributeFrom_CustomTable(void* pConn, NxlDBCUserInfo* pUserInfo)
{
	//create sql
	const std::string strSqlFmt = g_conf->GetUSERATTRCustomSQL();

	//create statement
	SQLDBC::SQLDBC_Connection* hdb_connection = (SQLDBC::SQLDBC_Connection*)pConn;
    SQLDBC::SQLDBC_PreparedStatement* pStatement = NULL;
		do
		{
			 pStatement = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);
	    	if(pStatement==nullptr){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, createStatement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
				break;
			}

		    SQLDBC_Retcode rc = prepare_3_old(pStatement, (char*)strSqlFmt.c_str(), strSqlFmt.size(), SQLDBC_StringEncodingType::Ascii);
           	if (SQLDBC_OK != rc) {
				theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, prepare_3_old failed, rc=%d", rc);
        		break;
   	 		}

			rc = bind_parameter_old(pStatement, 1, SQLDBC_HOSTTYPE_ASCII, (void *)pUserInfo->_strMANDT.c_str(), nullptr, 0, SQLDBC_TRUE);
			if (SQLDBC_OK != rc) {
				theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, bind_parameter_old 1 failed, rc=%d", rc);
        		break;
			}
			rc = bind_parameter_old(pStatement, 2, SQLDBC_HOSTTYPE_ASCII, (void *)pUserInfo->_strName.c_str(), nullptr, 0, SQLDBC_TRUE);
			if (SQLDBC_OK != rc) {
				theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, bind_parameter_old 2 failed, rc=%d", rc);
        		break;
			}

			rc = execute_old(pStatement);
			if(rc!=SQLDBC_OK){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, execute failed, rc=%d, error=%s", rc,SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
				break;
			}
	
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, get_result_set==null");
				break;
			}
            
			while (true){

          		rc =  SQLDBC_ResultSet_next_Old(pResultSet);
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					if (rc!=SQLDBC_NO_DATA_FOUND){
						theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, call next failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					}	
					break;
				}
                 
				char szAttrName[255]={0};
				SQLDBC_Length nLenAttrName = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szAttrName, &nLenAttrName, sizeof(szAttrName), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, get colomn 1 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				szAttrName[nLenAttrName] = 0;
                
				char szAttrValue[255]={0};
				SQLDBC_Length nLenAttrValue = 0;
				 rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, szAttrValue, &nLenAttrValue, sizeof(szAttrValue), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable, get colomn 2 attribute value failed, rc=%d, error=%s", rc,SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				szAttrValue[nLenAttrValue] = 0;

                theLog->WriteLog(log_info, "GetUserAttributeFrom_CustomTable %s=%s", szAttrName, szAttrValue );
				if (nLenAttrName>0 && nLenAttrValue>0){
					std::set<std::string>& attrValues = pUserInfo->_attribute[szAttrName]; //if not exist, here will create one
					attrValues.insert(szAttrValue);
				}		
			}
			
		} while (false);

		//release
		if (NULL!=pStatement){
			SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pStatement);
			pStatement = NULL;
		}
		
}

void CDbcUserInfoMgr::GetUserAttributeFrom_V_ADDR_USR(void* pConn, NxlDBCUserInfo* pUserInfo)
{
	//create sql
	const std::string strSql = g_conf->GetUSERATTRUserSQL();

	//create statement
	auto hdb_connection = (SQLDBC::SQLDBC_Connection*)pConn;
    SQLDBC::SQLDBC_PreparedStatement* pStatement = NULL;
		do
		{
			pStatement = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);
	    	if(pStatement==nullptr){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, createPreparedStatement failed");
				break;
			}

			SQLDBC_Retcode rc = prepare_3_old(pStatement, (char*)strSql.c_str(), strSql.size(), SQLDBC_StringEncodingType::Ascii);
           	if (SQLDBC_OK != rc) {
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, prepare_3_old failed, rc=%d", rc);
        		break;
   	 		}

			rc = bind_parameter_old(pStatement, 1, SQLDBC_HOSTTYPE_ASCII, (void *)pUserInfo->_strMANDT.c_str(), nullptr, 0, SQLDBC_TRUE);
			if (SQLDBC_OK != rc) {
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, bind_parameter_old 1 failed, rc=%d", rc);
        		break;
			}
			rc = bind_parameter_old(pStatement, 2, SQLDBC_HOSTTYPE_ASCII, (void *)pUserInfo->_strName.c_str(), nullptr, 0, SQLDBC_TRUE);
			if (SQLDBC_OK != rc) {
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, bind_parameter_old 2 failed, rc=%d", rc);
        		break;
			}

			rc = execute_old(pStatement);
			if(rc!=SQLDBC_OK){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, execute failed, rc=%d", rc);
				break;
			}
	
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, get_result_set==null");
				break;
			}

			//get resultset metadata
			auto pMetaData = SQLDBC_ResultSet_getResultSetMetaData_Old(pResultSet);
			if(nullptr==pMetaData){
				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, getResultSetMetaData failed");
				break;
			}
			const size_t nColumnCount = SQLDBC_ResultSetMetaData_getColumnCount_Old(pMetaData);
			theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, column count:%d", nColumnCount);

			//Get User Attribute Names
			if(!_inited_vecUserAttrName){
				_inited_vecUserAttrName = true;
				char szColumnLabel[256]={0};
				SQLDBC_Length outLength;
				std::vector<std::string>  vecUserAttrName_V_ADDR_USR_temp;
				for(int i = 1; i <= nColumnCount; ++i){
					outLength = 0;
					rc = SQLDBC_ResultSetMetaData_getColumnLabel_Old(pMetaData, i, szColumnLabel, SQLDBC_StringEncodingType::Ascii, 255, &outLength);
					if(rc!=SQLDBC_OK){
						theLog->WriteLog(log_error, "GetUserAttributeFrom_V_ADDR_USR, getColumnName error, index:%d, execute failed, rc=%d", i, rc);
						continue;
					}

					if(outLength>0){
						szColumnLabel[outLength] = '\0';
						vecUserAttrName_V_ADDR_USR_temp.emplace_back(szColumnLabel);
					}
				}
				if(vecUserAttrName_V_ADDR_USR_temp.size()) {
					_vecUserAttrName_V_ADDR_USR = vecUserAttrName_V_ADDR_USR_temp;
					std::string cch;
					for(auto elem : _vecUserAttrName_V_ADDR_USR){
						cch.append(elem).append(",");
					}
					theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, inited or updated UserAttrNames:%s size:%d", cch.c_str(), _vecUserAttrName_V_ADDR_USR.size());
				}
			}

			if(nColumnCount!=_vecUserAttrName_V_ADDR_USR.size()){
				theLog->WriteLog(log_error, "GetUserAttributeFrom_V_ADDR_USR, column count not equal to attribute name count. column=%d, attr=%d", nColumnCount, _vecUserAttrName_V_ADDR_USR.size() );
				break;
			}

			rc =  SQLDBC_ResultSet_next_Old(pResultSet);
			if(rc!=SQLDBC_OK){
				if(rc!=SQLDBC_NO_DATA_FOUND){
					theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR, call next failed, rc=%d", rc);
				}
				break;
			}

			//query data
			char szColumnValue[255]={0};
			SQLDBC_Length nLen = 0;
			for(unsigned int iCol=1; iCol<=nColumnCount; iCol++)
			{     
				rc = SQLDBC_ResultSet_getObject_Old(pResultSet, iCol, SQLDBC_HOSTTYPE_ASCII, szColumnValue, &nLen, sizeof(szColumnValue), SQLDBC_TRUE );
				if(rc!=SQLDBC_OK){
						theLog->WriteLog(log_error, "GetUserAttributeFrom_V_ADDR_USR getColumnValue failed.iCol=%d,rc=%d, user=%s, msg=%s",
						iCol, rc, pUserInfo->_strName.c_str(), SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				szColumnValue[nLen] = 0;

				theLog->WriteLog(log_info, "GetUserAttributeFrom_V_ADDR_USR %s=%s", _vecUserAttrName_V_ADDR_USR[iCol-1].c_str(), szColumnValue );
				if(nLen>0){
					std::set<std::string>& setValue = pUserInfo->_attribute[_vecUserAttrName_V_ADDR_USR[iCol-1]]; //if not exist, here will create one
					setValue.insert(szColumnValue);
				}
				
			}
		} while (false);

	if (pStatement) {
		SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pStatement);
		pStatement = NULL;
	}
	
}

bool CDbcUserInfoMgr::IsUserInfoExpired(const NxlDBCUserInfo* pUserInfo) const
{
	return difftime(time(nullptr), pUserInfo->_time) > _expiry;
}

void CDbcUserInfoMgr::RemoveUserInfoFromCache(const NxlDBCUserInfo *pUserInfo)
{
	auto itUserInfo = _mapUserInfo.find(pUserInfo->_strName);
	if (itUserInfo != _mapUserInfo.end())
	{
		delete pUserInfo;
		_mapUserInfo.erase(itUserInfo);
	}
}

void parse_attributes(std::vector<std::string> & vec_attr, const std::string & strsql, S4HException & exc) {
    EMS4HAst * past = create_ast(strsql, exc);
    if(!past) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "parse node is nullptr.");
        return;
    }
    if(past->GetStatementType() == E_STMT_TYPE_SELECT){
        const std::vector<EMColumnInfo*> & cols = dynamic_cast<EMS4HSelectAst*>(past)->GetColumns();
        for(auto it:cols){
            if(it->_alias){
                if(it->_node){
                    if(it->_node->getChild(1)){
                        vec_attr.push_back(it->_node->getChild(1)->serialize());
                    }
                }
            } else {
                vec_attr.push_back(it->_field);
            }
        }
    }
    delete(past);
}

//void parse_attributes(std::vector<std::string> & vec_attr, const std::string & strsql, S4HException & exc) {
//
//    ParseResult *r = new ParseResult();
//    if (!parse_hana(strsql, r)){
//        std::string strLog = "parse failed. detail:" + r->errDetail ;
//        strLog += + " sql=" + strsql;
//        exc.SetError(S4H_SQL_PARSE_FAIL,  strLog);//r->errDetail
//        delete(r);
//        return;
//    }
//
//    if (!r->result_tree_) {
//        // printf("parse failed !");
//        exc.SetError(S4H_SQL_PARSE_FAIL, "parse node is nullptr.");
//        delete(r);
//        return;
//    }
//
//    EMS4HAst * past = parser_stmt(r->result_tree_, exc);
//    if(past) {
//        past->SetResult(r);
//    } else {
//        exc.SetError(S4H_SQL_PARSE_FAIL, "parse node is nullptr.");
//        delete(r);
//        return;
//    }
//
//    if(past->GetStatementType() == E_STMT_TYPE_SELECT){
//        const std::vector<EMColumnInfo*> & cols = dynamic_cast<EMS4HSelectAst*>(past)->GetColumns();
//        for(auto it:cols){
//            if(it->_alias){
//                if(it->_node){
//                    if(it->_node->getChild(1)){
//                        vec_attr.push_back(it->_node->getChild(1)->serialize());
//                    }
//                }
//            } else {
//                vec_attr.push_back(it->_field);
//            }
//        }
//    }
//    delete(past);
////    NXS4H::Lex lex(strsql);
////    lex.Next();
////    const NXS4H::NxToken * ptk = lex.GetCurrent();
////    if(ptk->GetType() != NXS4H::NxToken::SELECT) {
////        exc.SetError(S4H_SQL_PARSE_FAIL,"config sql type is error");
////        return;
////    }
////    lex.Next();
////    int iparen = 0;
////    std::string strlast;
////    bool bbreak = false;
////    while(NXS4H::NxToken::TK_ERR != ptk->GetType() && NXS4H::NxToken::TK_END != ptk->GetType() ) {
////        switch (ptk->GetType()) {
////            case NXS4H::NxToken::FROM: {
////                if(iparen==0 && !strlast.empty()) {
////                    vec_attr.push_back(strlast);
////                    strlast.clear();
////                    bbreak = true;
////                }
////            } break;
////            case NXS4H::NxToken::COMMA:{
////                if(iparen==0 && !strlast.empty()) {
////                    vec_attr.push_back(strlast);
////                    strlast.clear();
////                }
////            }break;
////            case NXS4H::NxToken::LPAREN: {
////                iparen++;
////            } break;
////            case NXS4H::NxToken::RPAREN: {
////                iparen--;
////            } break;
////            case NXS4H::NxToken::IDENTIFIER:
////            case NXS4H::NxToken::D_LITERAL:
////            case NXS4H::NxToken::D_N_LITERAL:{
////                strlast = CommonFun::ToUTF8(ptk->GetContent());
////            } break;
////            default:
////                break;
////
////        }
////        if(bbreak) break;
////
////        lex.Next();
////    }
//}

void CDbcUserInfoMgr::SyncChanges(NXS4HConfig* pConf)
{
	this->SetExpireTime(pConf->GetUAClearCycle());

	std::string newSQL = pConf->GetUSERATTRUserSQL();
	if((newSQL.size() != this->_V_ADDR_USR_SQL.size()) || (newSQL != this->_V_ADDR_USR_SQL))
	{
		this->_V_ADDR_USR_SQL = newSQL;
		this->_inited_vecUserAttrName = false;
	}
}
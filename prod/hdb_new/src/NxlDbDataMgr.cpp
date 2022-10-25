#include "NxlDbDataMgr.h"
#include "log.h"
#include "NXS4HConfig.h"
#include "EnforcerCtxImpl.h"
#include "tool.h"
#include "talk_with_hdb.h"
#include "functional"

NxlDBDataMgr::NxlDBDataMgr(){
    _bInit = false;
    _strKeyMaskMaskFormat = "[KEY_MASK]";

    const std::string keymask_prefix = g_conf->GetPRIDATAKeymaskPrefix();
    _strKeyMaskPrefix =  keymask_prefix.empty() ? "*_" : keymask_prefix;

    const std::string pridata_schema = g_conf->GetPRIDATASchema();
    _strKeyMaskCacheTableFullName = pridata_schema + "." + g_conf->GetPRIDATAKeymaskTable();
    _strKeyMaskFunctionFullName = pridata_schema + "." + g_conf->GetPRIDATAKeymaskFunc();

 }

void NxlDBDataMgr::Init(SQLDBC::SQLDBC_Connection* pConn){
    SQLDBC_Retcode rc;

    //create statement
    SQLDBC::SQLDBC_Statement* pStmt = SQLDBC_Connection_createStatement_Old(pConn);
    if (NULL==pStmt){
        const char* pError = SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old( pConn) );
        theLog->WriteLog(log_error, "NxlDBDataMgr::Init create statement failed. error=%s", pError==NULL ? "null" : pError );
        return;
    }

    //create schema
    CreateSchema(pStmt);
 
    //create table
    CreateKeyMaskCacheTable(pStmt);
    
    //create key mask function
    CreateKeyMaskFunction(pStmt);

    #if RECORD_SQL_PARSE_ERROR
    CreateSQLParseErrorTable(pStmt);
    #endif 


    //release statement
    if (pStmt){
        SQLDBC_Connection_releaseStatement_Old(pConn, pStmt);
        pStmt = NULL;
    }
    
    _bInit = true;
}

bool NxlDBDataMgr::IsKeyMaskData(const std::string& strValue)
{
    return strValue.find( KeyMaskPrefix() )==0;
}

//get original value by mask value. e.g: maskprefix_1---> 'AUT'
//the result can be cached. and find cache first, if not in the cache, than find from db.
std::string NxlDBDataMgr::GetOriginalValuByMaskValue(SQLDBC::SQLDBC_Connection* pConn, const std::string& strValue)
{
    //extract ID from value
    const static std::string strPrefix = KeyMaskPrefix();
    if (strValue.length()<=strPrefix.length()){
        theLog->WriteLog(log_error, "GetOriginalValuByMaskValue value is less than prefix: strvalue=%s", strValue.c_str() );
        return "";
    }  
    std::string strID = strValue.substr(strPrefix.length());

    //construct SQL
    const static std::string strSQLFmt = "SELECT FIELD_VALUE FROM " +  KeyMaskCacheTableFullName()  + " WHERE ID=";
    std::string strSQL = strSQLFmt + strID;
    
    //create statement
    SQLDBC::SQLDBC_Statement* pStmt = SQLDBC_Connection_createStatement_Old(pConn);
    if (NULL==pStmt){
        theLog->WriteLog(log_error, "GetOriginalValuByMaskValue create statement failed: strvalue=%s", strValue.c_str() );
        return "";
    }
    
    std::string strOriginalValue;
    do
    {
        //execute
        SQLDBC_Retcode rc =  SQLDBC_Statement_execute_Old(pStmt, strSQL.c_str());
        if (rc!=SQLDBC_OK && rc!=SQLDBC_SUCCESS_WITH_INFO){
            theLog->WriteLog(log_error, "NxlDBDataMgr::GetOriginalValuByMaskValue execute sql failed. rc=%d, error=%s, sql=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)), strSQL.c_str() );
            break;
        }
        
        //get result set
        SQLDBC::SQLDBC_ResultSet* pResultSet =  get_result_set_old(pStmt);
        if (pResultSet==NULL){
            theLog->WriteLog(log_error, "NxlDBDataMgr::GetOriginalValuByMaskValue getResultSet failed." );
            break;
        }

        rc =  SQLDBC_ResultSet_next_Old(pResultSet);
		if(rc!=SQLDBC_OK){
			theLog->WriteLog(log_error, "NxlDBDataMgr::GetOriginalValuByMaskValue , call next failed, rc=%d", rc);
			break;
		}

        //get result
        char szColumnValue[255]={0};
        SQLDBC_Length nLen = 0;    
        rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szColumnValue, &nLen, sizeof(szColumnValue), SQLDBC_TRUE );
        if(rc!=SQLDBC_OK){
            theLog->WriteLog(log_error, "NxlDBDataMgr::GetOriginalValuByMaskValue getColumnValue rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
            break;
        }

        strOriginalValue = szColumnValue;

    } while (false);

    SQLDBC_Connection_releaseStatement_Old(pConn, pStmt);
    pStmt = NULL;
    
    return strOriginalValue;
}

bool NxlDBDataMgr::CreateKeyMaskFunction(SQLDBC::SQLDBC_Statement* pStmt)
{
     //check table exist
    const std::string sql_function_exist = "SELECT \"SCHEMA_NAME\" FROM SYS.OBJECTS  WHERE  \"OBJECT_TYPE\"='FUNCTION' AND \"SCHEMA_NAME\"='" + PrivateDataSchema() + "' AND \"OBJECT_NAME\"='" + KeyMaskFunctionName() + "';";
    SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStmt, sql_function_exist.c_str());
    if (rc!=SQLDBC_OK && rc!=SQLDBC_SUCCESS_WITH_INFO){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateKeyMaskFunction check function exist failed. rc=%d", rc );
        return false;
    }

    SQLDBC::SQLDBC_ResultSet* pResultSet = get_result_set_old(pStmt);
    if (pResultSet==NULL){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateKeyMaskFunction getResultSet failed" );
        return false;
    }

    bool bRet = true;
    SQLDBC_Int4 nCount= SQLDBC_ResultSet_getResultCount_Old(pResultSet);
    if (0==nCount){
        const std::string sql_function_create = "CREATE FUNCTION " + KeyMaskFunctionFullName() + "(in maskprefix varchar(64), in fieldname varchar(256), in fieldvalue varchar(256))\n"
                                                "RETURNS output1 VARCHAR(256) LANGUAGE SQLSCRIPT AS\n"
                                                "BEGIN\n"
                                                "declare currentID bigint;\n"
                                                "DECLARE EXIT HANDLER FOR SQLEXCEPTION output1='***';\n"
                                                "select ID into currentID from " + KeyMaskCacheTableFullName() + " where FIELD_VALUE=:fieldvalue limit 1;\n"
                                                "output1=CONCAT(:maskprefix, TO_VARCHAR(:currentID) );\n"
                                                "END;\n";

        rc =  SQLDBC_Statement_execute_Old(pStmt, sql_function_create.c_str());
        if (SQLDBC_OK!=rc && SQLDBC_SUCCESS_WITH_INFO!=rc){
            const char* pError = SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt));
            theLog->WriteLog(log_error, "NxlDBDataMgr::CreateKeyMaskFunction create function failed. rc=%d, msg=%s", rc, pError==NULL? "NULL" : pError );
            bRet = false;
        }
        else{
            theLog->WriteLog(log_info, "NxlDBDataMgr::CreateKeyMaskFunction create function success.");
        }
          
    }
    else{
         theLog->WriteLog(log_info, "NxlDBDataMgr::CreateKeyMaskFunction function exist" );
    } 

    return bRet;  
}

void NxlDBDataMgr::RecordSQLParseError(SQLDBC::SQLDBC_Connection* pConn, const std::string& sqlInfo)
{
    //construct SQL
    const static std::string strSQLFmt = "UPSERT  " + PrivateDataSchema() + ".\"" + RECORD_SQL_TABLE +  "\" values(?, ?,?)  WITH PRIMARY KEY";

    //create statement
    SQLDBC::SQLDBC_PreparedStatement* pStmt = SQLDBC_Connection_createPreparedStatement_Old(pConn);
    if (NULL==pStmt){
        theLog->WriteLog(log_error, "RecordSQLParseError create statement failed" );
        return;
    }
    
    std::string strOriginalValue;
    do
    {
        //prepare
        SQLDBC_Retcode rc= prepare_3_old(pStmt, (const char *)strSQLFmt.c_str(), strSQLFmt.size(), SQLDBC_StringEncodingType::Ascii);
        if (rc!=SQLDBC_OK){
            theLog->WriteLog(log_error, "RecordSQLParseError prepare sql failed.rc=%d, error=%s, sql=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)), strSQLFmt.c_str() );
            break;
        }

        //bind 1
        size_t sz = std::hash<std::string>()(sqlInfo);
        char szHash[33]={0};
        snprintf(szHash, sizeof(szHash), "%zu", sz);
        rc = bind_parameter_old(pStmt, 1, SQLDBC_HOSTTYPE_ASCII, (void*)szHash, nullptr, 0, SQLDBC_TRUE);
        if (SQLDBC_OK != rc) {
            theLog->WriteLog(log_error, "RecordSQLParseError bind 1 failed.rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)));
            break;
        }
        
        //bind 2
        rc = bind_parameter_old(pStmt, 2, SQLDBC_HOSTTYPE_ASCII, (void*)sqlInfo.c_str(), nullptr, 0, SQLDBC_TRUE);
        if (SQLDBC_OK != rc) {
            theLog->WriteLog(log_error, "RecordSQLParseError bind 2 failed.rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)));
            break;
        }

        //bind 3
        time_t timeNow=time(NULL);
        struct tm *tmNow=localtime(&timeNow);
        char strTime[100]={0};
        strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", tmNow);
        rc = bind_parameter_old(pStmt, 3, SQLDBC_HOSTTYPE_ASCII, (void*)strTime, nullptr, 0, SQLDBC_TRUE);
        if (SQLDBC_OK != rc) {
            theLog->WriteLog(log_error, "RecordSQLParseError bind 3 failed.rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)));
            break;
        }

        //execute
        rc =  execute_old(pStmt);
        if (rc!=SQLDBC_OK && rc!=SQLDBC_SUCCESS_WITH_INFO){
            theLog->WriteLog(log_error, "RecordSQLParseError execute sql failed. rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)) );
            break;
        }
    }
    while (false);

    //release
    if(NULL!=pStmt){
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(pConn, pStmt);
        pStmt=nullptr;
    }
}


bool NxlDBDataMgr::CreateSQLParseErrorTable(SQLDBC::SQLDBC_Statement* pStmt)
{
    //check table exist
    const std::string sql_table_exist = "SELECT \"TABLE_NAME\" FROM \"PUBLIC\".\"M_TABLES\" WHERE \"SCHEMA_NAME\"='" + PrivateDataSchema() + "' AND \"TABLE_NAME\"='" + RECORD_SQL_TABLE + "';";
    SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStmt, sql_table_exist.c_str());
    if (rc!=SQLDBC_OK){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSQLParseErrorTable check table exist failed. rc=%d", rc );
        return false;
    }

    SQLDBC::SQLDBC_ResultSet* pResultSet = get_result_set_old(pStmt);
    if (pResultSet==NULL){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSQLParseErrorTable getResultSet failed" );
        return false;
    }

    bool bRet = true;
    SQLDBC_Int4 nCount= SQLDBC_ResultSet_getResultCount_Old(pResultSet);
    if (0==nCount){
        const std::string sql_table_create = "CREATE TABLE "+ PrivateDataSchema() + ".\"" + RECORD_SQL_TABLE +  "\"(HASH_VALUE nvarchar(32), SQL_INFO varchar(5000), UPDATE_DATE nvarchar(32), PRIMARY KEY (\"HASH_VALUE\") );";
        rc = SQLDBC_Statement_execute_Old(pStmt, sql_table_create.c_str());
        if (SQLDBC_OK!=rc){
            const char* pError =SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt));
            theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSQLParseErrorTable create table failed. rc=%d, msg=%s", rc, pError==NULL? "NULL" : pError );
            bRet = false;
        }
        else{
            theLog->WriteLog(log_info, "NxlDBDataMgr::CreateSQLParseErrorTable create table success.");
        }
          
    }
    else{
       theLog->WriteLog(log_info, "NxlDBDataMgr::CreateSQLParseErrorTable table already exist.");
    } 

    return bRet;  

}



bool NxlDBDataMgr::CreateKeyMaskCacheTable(SQLDBC::SQLDBC_Statement* pStmt)
{
    //check table exist
    const std::string sql_table_exist = "SELECT \"TABLE_NAME\" FROM \"PUBLIC\".\"M_TABLES\" WHERE \"SCHEMA_NAME\"='" + PrivateDataSchema() + "' AND \"TABLE_NAME\"='" + KeyMaskCacheTableName() + "';";
    SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStmt, sql_table_exist.c_str());
    if (rc!=SQLDBC_OK){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSchema check table exist failed. rc=%d", rc );
        return false;
    }

    SQLDBC::SQLDBC_ResultSet* pResultSet = get_result_set_old(pStmt);
    if (pResultSet==NULL){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateKeyMaskCacheTable getResultSet failed" );
        return false;
    }

    bool bRet = true;
    SQLDBC_Int4 nCount= SQLDBC_ResultSet_getResultCount_Old(pResultSet);
    if (0==nCount){
        const std::string sql_table_create = "CREATE TABLE "+ KeyMaskCacheTableFullName() +  "(ID bigint not null generated by default as IDENTITY, FIELD_VALUE nvarchar(256) );";
        rc =  SQLDBC_Statement_execute_Old(pStmt, sql_table_create.c_str());
        if (SQLDBC_OK!=rc){
            const char* pError = SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt));
            theLog->WriteLog(log_error, "NxlDBDataMgr::CreateKeyMaskCacheTable create table failed. rc=%d, msg=%s", rc, pError==NULL? "NULL" : pError );
            bRet = false;
        }
        else{
            theLog->WriteLog(log_info, "NxlDBDataMgr::CreateKeyMaskCacheTable create table success.");
        }
          
    }
    else{
        /*can't clear the table becasue there are process create during SAP running. if clear, the data will cleard during SAP running
         const std::string sql_table_clear = "DELETE FROM " + KeyMaskCacheTableFullName() ;
         rc = pStmt->execute(sql_table_clear.c_str());
         theLog->WriteLog(log_info, "NxlDBDataMgr::CreateKeyMaskCacheTable table exist.  delete table data. rc=%d", rc );
         */
    } 

    return bRet;  
}


bool NxlDBDataMgr::CreateSchema(SQLDBC::SQLDBC_Statement* pStmt)
{
    //check schema exist
    const std::string sql_schema_exist = "SELECT SCHEMA_NAME FROM sys.schemas WHERE SCHEMA_NAME='" + PrivateDataSchema() +  "'";
    SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStmt, sql_schema_exist.c_str()); 
    if (rc!=SQLDBC_OK){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSchema check schema exist failed. rc=%d", rc );
        return false;
    }

    SQLDBC::SQLDBC_ResultSet* pResultSet = get_result_set_old(pStmt);
    if (pResultSet==NULL){
        theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSchema getResultSet failed" );
        return false;
    }

    bool bRet = true;
    SQLDBC_Int4 nCount= SQLDBC_ResultSet_getResultCount_Old(pResultSet);
    if (0==nCount){
        const std::string sql_schema = "CREATE SCHEMA " + PrivateDataSchema()  +  ";";
        rc = SQLDBC_Statement_execute_Old(pStmt, sql_schema.c_str());
        if (SQLDBC_OK!=rc){
            theLog->WriteLog(log_error, "NxlDBDataMgr::CreateSchema failed. rc=%d, msg=%s", rc,  SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)) );
            bRet = false;
        }
        else{
            theLog->WriteLog(log_info, "NxlDBDataMgr::CreateSchema create schema success.");
        }
          
    }
    else{
         theLog->WriteLog(log_info, "NxlDBDataMgr::CreateSchema schema exist." );
    } 

    return bRet;  
}

void NxlDBDataMgr::Execute(const std::string & sql, const std::vector<int> & indexs, void * conn, const EnforcerCtxImpl * pctx, S4HException & exc){
    auto hdb_connection = (SQLDBC::SQLDBC_Connection*)conn;
    auto hdb_stmt = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);

    std::u16string query = CommonFun::FromUTF8(sql);
    SQLDBC_Retcode rc = prepare_3_old(hdb_stmt, (char*)query.c_str(), query.size()*2, SQLDBC_StringEncodingType::UCS2Swapped);
    if (SQLDBC_OK != rc) {
        exc._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), exc._detail);
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, hdb_stmt);
        return ;
    }
    int param_index = 1;
    const EnforcerCtxImpl * pctxi = dynamic_cast<const EnforcerCtxImpl*>(pctx);
    for(int i =0; i < indexs.size(); ++i){
        int ifind = indexs[i];
        auto it = pctxi->_bind_parameters.find(ifind);
        if(it != pctxi->_bind_parameters.end()){
            rc = bind_parameter_old(hdb_stmt, param_index, it->second->_Type, it->second->_paramAddr, it->second->_LengthIndicator, it->second->_Size, it->second->_Terminate);
            if (rc != SQLDBC_OK) {
                exc._code = S4H_BIND_PARAM_ERROR;
                theLog->WriteLog(log_fatal, "NxlDBDataMgr::Execute, bind_parameter return false:%d", rc);
                CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), exc._detail);
                SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);
                return ;
            }
        } else {
            exc.SetError(S4H_BIND_PARAM_ERROR,"not find parameter.");
            SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, hdb_stmt);
            return;
        }
        param_index++;
    }

    rc = execute_old(hdb_stmt);
    if (SQLDBC_OK != rc) {
        exc._code = S4H_INVALID_CONNECTION;
        theLog->WriteLog(log_info, "NxlDBDataMgr::Execute,execute failed. rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt))  );
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection , hdb_stmt);
        return ;
    }

    rc = SQLDBC_Connection_Commit_Old(hdb_connection);
    if (SQLDBC_OK != rc) {
        exc._code = S4H_INVALID_CONNECTION;
        theLog->WriteLog(log_info, "NxlDBDataMgr::Execute,commit failed. rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection))  );
    }

     SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection,hdb_stmt);

}



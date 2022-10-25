//
// Created by jeff on 2020/3/26.
//

#include <interface.h>
#include "SqlDbcResourceInfoMgr.h"
#include "tool.h"
#include "talk_with_hdb.h"
#include "log.h"
#ifdef _WIN32
    #include <Winsock2.h>
#endif 

#define SQL_QUERY_DBINFO_HANA "SELECT CURRENT_USER,CURRENT_SCHEMA AS SCHEMA_NAME,(SELECT DATABASE_NAME FROM SYS.M_DATABASES) AS DB_NAME, 'TODU' AS SERVER_NAME FROM DUMMY"
#define SQL_QUERY_SERVERINFO_HANA "SELECT HOST as DB_SERVER,CLIENT_HOST as SAP_SERVER FROM M_CONNECTIONS WHERE CONNECTION_ID = CURRENT_CONNECTION"
void query_resource_info(NxlDBCResInfo & res, void* connection, S4HException& e);

SqlDbcResourceInfoMgr::~SqlDbcResourceInfoMgr(){
    
}

const NxlDBCResInfo * SqlDbcResourceInfoMgr::GetResourceInfo(void* connection, S4HException& e){
    if(_resource_info._database.empty()) {
        query_resource_info(_resource_info, connection, e);

        //get SAP SID
        const char *pSapSystemID = getenv("SAPSYSTEMNAME");
        if (pSapSystemID) {
            _resource_info._sapsid = pSapSystemID;
        }
    }

    

    return &_resource_info;
}

void query_resource_info(NxlDBCResInfo & res, void* connection, S4HException& e){
    if (nullptr == connection) {
        e.SetError(S4H_INVALID_CONNECTION, "INVALID CONNECTION!");
        return ;
    }

    auto hdb_connection = (SQLDBC::SQLDBC_Connection*)connection;

    auto hdb_stmt = SQLDBC_Connection_createStatement_Old(hdb_connection);

    //get schema name and database name
    SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(hdb_stmt, SQL_QUERY_DBINFO_HANA);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);
        return ;
    }
    auto result_set = get_result_set_old(hdb_stmt);
    if (result_set == nullptr) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);
        return ;
    }
    rc = SQLDBC_ResultSet_next_Old(result_set);
    if(SQLDBC_OK != rc) {
        e._code = S4H_INVALID_TABLE_NAME;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);
        return ;
    }

    char schema[64] = { 0 };
    char db[64] = { 0 };
    do {
        SQLDBC_ResultSet_getObject_Old(result_set, 2, SQLDBC_HOSTTYPE_ASCII, schema, nullptr, sizeof(schema), SQLDBC_TRUE);
        SQLDBC_ResultSet_getObject_Old(result_set, 3, SQLDBC_HOSTTYPE_ASCII, db, nullptr, sizeof(db), SQLDBC_TRUE);

    } while (0);

    res._database = db;
    res._schema = schema;


    //get db and sap server name
    rc = SQLDBC_Statement_execute_Old(hdb_stmt, SQL_QUERY_SERVERINFO_HANA);
    if (SQLDBC_OK != rc) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);
        return ;
    }
    result_set = get_result_set_old(hdb_stmt);
    if (result_set == nullptr) {
        e._code = S4H_INVALID_CONNECTION;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);
        return ;
    }
    rc = SQLDBC_ResultSet_next_Old(result_set);
    if(SQLDBC_OK != rc) {
        e._code = S4H_INVALID_TABLE_NAME;
        CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt)), e._detail);
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);
        return ;
    }

    char db_server[256] = { 0 };
    char app_server[256] = { 0 };
    do {
        SQLDBC_ResultSet_getObject_Old(result_set, 1, SQLDBC_HOSTTYPE_ASCII, db_server, nullptr, sizeof(db_server), SQLDBC_TRUE);
        SQLDBC_ResultSet_getObject_Old(result_set, 2, SQLDBC_HOSTTYPE_ASCII, app_server, nullptr, sizeof(app_server), SQLDBC_TRUE);

    } while (0);
  
    res._server = db_server;
    //res._appServer = app_server;

    //get app server name
    SqlDbcResourceInfoMgr::Instance()->GetAppServerName();

    e._code = S4H_SUCCESS;
    SQLDBC_Connection_releaseStatement_Old(hdb_connection, hdb_stmt);

    theLog->WriteLog(log_info, std::bind(&NxlDBCResInfo::Print, &res) );

    return ;
}

const std::string& SqlDbcResourceInfoMgr::GetAppServerName()
{
   if (_resource_info._appServer.empty()){
       const size_t max_host_name = 128;
       char szHost[max_host_name+1] = {0}; 
       /*int n=*/ gethostname(szHost, max_host_name);
       _resource_info._appServer = szHost;
       theLog->WriteLog(log_info, "get app server name:%s", szHost);
   }
   
   return  _resource_info._appServer;
}
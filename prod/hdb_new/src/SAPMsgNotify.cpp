#include "SAPMsgNotify.h"
#include "log.h"
#include "MyDisp+Work.h"
#include "talk_with_hdb.h"
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#define _getpid getpid
#endif

#define NOTIFY_MASK "MASKING"
#define NOTIFY_FILTER "FILTER"

void SAPMsgNotify::NotifyMask(void* pDBConn, EnforcerCtxImpl* pCtx,const NxlDBCUserInfo* pUserInfo, const std::vector<MaskItem>& vecMaskItem, const std::string &strMaskCountSql)
{
    if (NULL==pDBConn || NULL==pCtx || NULL==pUserInfo){
        theLog->WriteLog(log_error, "SAPMsgNotify::NotifyMask, the elements for notifying is existed, pDBConn:%p, pCtx:%p, pUserInfo:%p", pDBConn, pCtx, pUserInfo);
        return;
    }

    if (0==vecMaskItem.size() ){
        theLog->WriteLog(log_info, "SAPMsgNotify::NotifyMask, mask item not existed, ingored");
        return;
    }

    std::vector<MaskItem> vecEffectedMaskItem;
    CheckDoNotifyMask(pDBConn, pCtx, vecMaskItem, strMaskCountSql, vecEffectedMaskItem);

    if(!vecEffectedMaskItem.empty()){
        //get top 1 record
        const std::string strSessionKey = g_theMyDW->GetInternalSessionKey();
        memset(&_notifyData, 0, sizeof(_notifyData));
        SQLDBC_Retcode rc = GetTopRecord(pDBConn, pUserInfo->_strMANDT, pUserInfo->_strName, strSessionKey, _notifyData);
        if (SQLDBC_NOT_OK==rc)  {
            theLog->WriteLog(log_error, "SAPMsgNotify::NotifyMask get top record failed. mandt:%s, user:%s, session:%s", 
            pUserInfo->_strMANDT.c_str(), pUserInfo->_strName.c_str(), strSessionKey.c_str() );
            return;
        }
        int nCounter = SQLDBC_NO_DATA_FOUND==rc ? -1 : _notifyData.GetCounter();
        theLog->WriteLog(log_info,  "SAPMsgNotify::NotifyMask get top record success.rc=%d, mandt:%s, user:%s, session:%s, counter:%d", 
            rc, pUserInfo->_strMANDT.c_str(), pUserInfo->_strName.c_str(), strSessionKey.c_str(), nCounter );

        //insert new notify data 
        for (auto itMaskItem = vecEffectedMaskItem.begin(); itMaskItem != vecEffectedMaskItem.end(); itMaskItem++) {
            nCounter++;
            InsertNotifyDataForMask(pDBConn, pUserInfo, nCounter, &(*itMaskItem) );
        }
    }
}

/*
DAE_RELEVANT = ‘X’  (Fixed Value)
TABNAME = table name from masking obligation
FIELDNAME = mask field 
TYPE = ‘MASKING’ (Fixed Value)
MASKFORMAT = mask symbol
MASKCONDITION=mask condition;
UDATE = system data
UTIME = system time 
*/
#define BIND_NOTIFY_DATA(field) \
        rc = bind_parameter_old(pStatement, ++iParam, SQLDBC_HOSTTYPE_ASCII, (void *)field, nullptr, 0, SQLDBC_TRUE); \
        if (SQLDBC_OK != rc) { \
            theLog->WriteLog(log_error, "SAPMsgNotify::UpdateNotifyData, bind_parameter_old %d failed, rc=%d, msg=%s", iParam,  rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) ); \
            break; \
        }

#define GET_NOTIFY_DATA(field) \
        rc = SQLDBC_ResultSet_getObject_Old(pResultSet, ++iCol, SQLDBC_HOSTTYPE_ASCII, field, &nLen, sizeof(field), SQLDBC_TRUE); \
        if (rc != SQLDBC_OK) \
        { \
            theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord getColumnValue failed.iCol=%d,rc=%d", iCol, rc); \
            break; \
        }

void SAPMsgNotify::InsertNotifyDataForMask(void* pDBConn,const NxlDBCUserInfo* pUserInfo, int nCounter,const MaskItem* pMaskItem)
{
     const std::string strSessionKey = g_theMyDW->GetInternalSessionKey();
     const std::string& strField = pMaskItem->_strOriginalFiledForTable.empty() ? pMaskItem->_field : pMaskItem->_strOriginalFiledForTable;
     
     const std::string& strProgram =  pUserInfo->_tcode;
     const std::string strUIType = "";

     theLog->WriteLog(log_info, "SAPMsgNotify::InsertNotifyDataForMask, user:%s, session:%s, tcode:%s, table:%s, field:%s(%s), symbol:%s, cond:%s",
                      pUserInfo->_strName.c_str(), strSessionKey.c_str(), strProgram.c_str(), pMaskItem->_table.c_str(),
                      pMaskItem->_field.c_str(), pMaskItem->_strOriginalFiledForTable.c_str(),  pMaskItem->_symbols.c_str(), pMaskItem->_condition.c_str() );

     InsertNotifyData(pDBConn, pUserInfo->_strMANDT, strSessionKey, pUserInfo->_strName, nCounter, strProgram, strUIType,
                        pMaskItem->_table, strField, pMaskItem->_symbols,  pMaskItem->_condition, NOTIFY_MASK);
} 

void SAPMsgNotify::InsertNotifyData(void* pDBConn, const std::string& strMANDT, const std::string& strSessionKey, 
                                    const std::string& strUser, int nCounter,const std::string& strTcode, const std::string& strUIType,
                                    const std::string& strTable,  const std::string& strField, const std::string& maskSymbol, 
                                    const std::string& condition, const char* szType/*MASKING or FILTER*/)
{
    const char* szSql="INSERT INTO \"/NXLDAE/NOTIF\""
                      "(MANDT,USERID,SESSION_KEY,APP_NAME,COUNTER,PROCESS_ID,UI_TYPE,DAE_RELEVANT,TABNAME,FIELDNAME,TYPE,MASKFORMAT,CREATEDATE,CREATETIME) "
                      "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
  
    //create statement
    auto hdb_connection = (SQLDBC::SQLDBC_Connection *)pDBConn;
    SQLDBC::SQLDBC_PreparedStatement *pStatement = NULL;
    do
    {
        pStatement = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);
        if (pStatement == nullptr)  {
            theLog->WriteLog(log_error, "SAPMsgNotify::InsertNotifyData, createPreparedStatement failed");
            break;
        }

        SQLDBC_Retcode rc = prepare_3_old(pStatement, szSql, strlen(szSql), SQLDBC_StringEncodingType::Ascii);
        if (SQLDBC_OK != rc)  {
            theLog->WriteLog(log_error, "SAPMsgNotify::InsertNotifyData, prepare_3_old failed, rc=%d, msg=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
            break;
        }

        //bind param
        SQLDBC_UInt2 iParam=0;
        BIND_NOTIFY_DATA(strMANDT.c_str())  //MANDT
        BIND_NOTIFY_DATA(strUser.c_str()) //userid 
        BIND_NOTIFY_DATA(strSessionKey.c_str()); //key
 
        BIND_NOTIFY_DATA(strTcode.c_str());

        char szCounter[32]={0};
        sprintf_s(szCounter, 32, "%05d", nCounter);
        BIND_NOTIFY_DATA(szCounter);

        int pid = _getpid();
        char szPID[8]={0};
        sprintf_s(szPID, 8, "%d", pid);
        BIND_NOTIFY_DATA(szPID)  //PROCESS ID

        BIND_NOTIFY_DATA(strUIType.c_str()); 
        
        BIND_NOTIFY_DATA("X");  //DAE_RELEVANT

        BIND_NOTIFY_DATA(strTable.c_str()) //table name

        BIND_NOTIFY_DATA(strField.c_str())
        BIND_NOTIFY_DATA(szType)
        BIND_NOTIFY_DATA(maskSymbol.c_str())
        
 
        //CreateDATE, CrateTIME
        time_t tNow = time(NULL);
        struct tm* timeinfo = localtime (&tNow);
        char szDate[9], szTime[7];
        strftime(szDate, sizeof(szDate), "%Y%m%d", timeinfo );
        strftime(szTime, sizeof(szTime), "%H%M%S", timeinfo);
        BIND_NOTIFY_DATA(szDate)
        BIND_NOTIFY_DATA(szTime) 
 
        //execute
        rc = execute_old(pStatement);
        if (SQLDBC_OK != rc)  {
            theLog->WriteLog(log_error, "SAPMsgNotify::InsertNotifyData, execute_old failed, rc=%d, msg=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
            break;
        }

        SQLDBC_Connection_Commit_Old(hdb_connection);

        SQLDBC_Int4 nRow = get_rows_affected_old(pStatement);
        theLog->WriteLog(log_info, "SAPMsgNotify::InsertNotifyData, updated row:%d", nRow);

       
    }while (false);

    if (NULL!=pStatement){
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pStatement);
        pStatement = NULL;
    }
    
   return;

}


SQLDBC_Retcode SAPMsgNotify::GetTopRecord(void* pDBConn, const std::string& strMandt, const std::string& strUserID, 
                const std::string& strSessionKey, NotifyItemData& notifyData)
 {
     const char* szSql="SELECT TOP 1 MANDT, USERID, SESSION_KEY, COUNTER, PROCESS_ID, APP_NAME, UI_TYPE, DAE_RELEVANT, TABNAME, FIELDNAME, TYPE, PROCESSED  FROM \"/NXLDAE/NOTIF\" WHERE MANDT=? AND USERID=? AND SESSION_KEY=? ORDER BY COUNTER DESC";
     SQLDBC_Retcode rc = SQLDBC_NO_DATA_FOUND;
     //create statement
     auto hdb_connection = (SQLDBC::SQLDBC_Connection*)pDBConn;
     SQLDBC::SQLDBC_PreparedStatement* pStatement = NULL;
     do  {
         pStatement = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);
         if (pStatement == nullptr) {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, createPreparedStatement failed");
             break;
         }
 
         rc = prepare_3_old(pStatement, (char *)szSql, strlen(szSql), SQLDBC_StringEncodingType::Ascii);
         if (SQLDBC_OK != rc)  {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, prepare_3_old failed, rc=%d, msg=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
             break;
         }
 
         rc = bind_parameter_old(pStatement, 1, SQLDBC_HOSTTYPE_ASCII, (void *)strMandt.c_str(), nullptr, 0, SQLDBC_TRUE);
         if (SQLDBC_OK != rc) {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, bind_parameter_old 1 failed, rc=%d", rc);
             break;
         }
 
         rc = bind_parameter_old(pStatement, 2, SQLDBC_HOSTTYPE_ASCII, (void *)strUserID.c_str(), nullptr, 0, SQLDBC_TRUE);
         if (SQLDBC_OK != rc) {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, bind_parameter_old 3 failed, rc=%d", rc);
             break;
         }
           
         rc = bind_parameter_old(pStatement, 3, SQLDBC_HOSTTYPE_ASCII, (void *)strSessionKey.c_str(), nullptr, 0, SQLDBC_TRUE);
         if (SQLDBC_OK != rc) {
             theLog->WriteLog(log_info, "SAPMsgNotify::GetTopRecord, bind_parameter_old 3 failed, rc=%d", rc);
             break;
         }
 
         rc = execute_old(pStatement);
         if (rc != SQLDBC_OK) {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, execute failed, rc=%d", rc);
             break;
         }
 
         auto pResultSet = (SQLDBC::SQLDBC_ResultSet *)get_result_set_old(pStatement);
          if (pResultSet == nullptr)  {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, get_result_set==null");
             break;
         }
 
         rc = SQLDBC_ResultSet_next_Old(pResultSet);
         if (rc==SQLDBC_NOT_OK)  {
             theLog->WriteLog(log_error, "SAPMsgNotify::GetTopRecord, call next failed, rc=%d", rc);
             break;
         }

         if (rc != SQLDBC_NO_DATA_FOUND)  {
             SQLDBC_Length nLen = 0;
             SQLDBC_Int4 iCol = 0;

             GET_NOTIFY_DATA(notifyData._MANDT)
             GET_NOTIFY_DATA(notifyData._USERID)
             GET_NOTIFY_DATA(notifyData._SESSION_KEY)
             GET_NOTIFY_DATA(notifyData._COUNTER)
             GET_NOTIFY_DATA(notifyData._PROCESS_ID)
             GET_NOTIFY_DATA(notifyData._APP_NAME)
             GET_NOTIFY_DATA(notifyData._UI_TYPE)
             GET_NOTIFY_DATA(notifyData._DAE_RELEVANT)
             GET_NOTIFY_DATA(notifyData._TABNAME)
             GET_NOTIFY_DATA(notifyData._FIELDNAME)
             GET_NOTIFY_DATA(notifyData._TYPE)
             GET_NOTIFY_DATA(notifyData._PROCESSED)
         }
     }while (false);
 
     //free
     if (NULL!=pStatement){
         SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pStatement);
         pStatement = NULL;
     }
     
     return rc;
 }


void SAPMsgNotify::PrintNotifyData(const NotifyItemData& notifyData)
{
    theLog->WriteLog(log_info, "PrintNotifyData: MANDT:%s, userid:%s, sessionKey:%s, counter:%s, appname:%s,tabname:%s, processed:%s",
                           notifyData._MANDT, notifyData._USERID, notifyData._SESSION_KEY, notifyData._COUNTER, notifyData._APP_NAME,
                           notifyData._TABNAME, notifyData._PROCESSED);
}


void SAPMsgNotify::InsertNotifyDataForFilter(void* pDBConn,const NxlDBCUserInfo* pUserInfo, int nCounter,
                                 const PredicateItem* pPredicItem)
{
    if(pPredicItem->_tableInfo==NULL){
        theLog->WriteLog(log_info, "SAPMsgNotify::InsertNotifyDataForFilter return for _tableInfo==NULL, cond=%s", pPredicItem->_condition.c_str() );
        return;
    }

    std::string strFields;
    auto condiFields = pPredicItem->_cond_fields;
    for (auto itField=condiFields.begin(); itField!=condiFields.end(); itField++){
        if (!strFields.empty()){
            strFields += ",";
        }     
        strFields += *itField;
    }
    if(strFields.empty()) { 
        strFields += " ";
    }
    
    const std::string strTable =  pPredicItem->_tableName;// pPredicItem->_tableInfo->_tb; 
    const std::string strSessionkey = g_theMyDW->GetInternalSessionKey();

    const std::string& strProgram = pUserInfo->_tcode;
    const std::string strUIType = "";
    
    theLog->WriteLog(log_info, "SAPMsgNotify::InsertNotifyDataForFilter. user:%s, session:%s, tcode:%s, table:%s, field count:%lld, field:%s", 
                           pUserInfo->_strName.c_str(), strSessionkey.c_str(), strProgram.c_str(), strTable.c_str(), pPredicItem->_cond_fields.size(), strFields.c_str() );

    std::string strEmtpy;
    InsertNotifyData(pDBConn, pUserInfo->_strMANDT, strSessionkey, pUserInfo->_strName, nCounter, strProgram, strUIType, 
                                strTable, strFields, strEmtpy, strEmtpy, NOTIFY_FILTER );

}

void SAPMsgNotify::NotifyFilter(void* pDBConn, EnforcerCtxImpl* pCtx,const NxlDBCUserInfo* pUserInfo,
                                const std::list<PredicateItem>& lstPredicItems, const std::string& strNewSql, const std::string& strOldSql)
{
    if (NULL==pDBConn || NULL==pCtx || NULL==pUserInfo || 0==lstPredicItems.size() ){
        theLog->WriteLog(log_error, "SAPMsgNotify::NotifyFilter return: vecMaskItem.size:%d", lstPredicItems.size() );
        return;
    }

    bool bDoNotifyFilter = SAPMsgNotify::Instance().CheckDoNotifyFilter(pDBConn, pCtx, strNewSql, strOldSql);
    if(!bDoNotifyFilter){
        //theLog->WriteLog(log_info, "SAPMsgNotify::NotifyFilter return: bNotifyFilter false");
        return;
    }

     //get top 1 record
    const std::string strSessionKey = g_theMyDW->GetInternalSessionKey();
    memset(&_notifyData, 0, sizeof(_notifyData));
    SQLDBC_Retcode  rc = GetTopRecord(pDBConn, pUserInfo->_strMANDT, pUserInfo->_strName, strSessionKey, _notifyData);
    if (rc==SQLDBC_NOT_OK)  {
        theLog->WriteLog(log_error, "SAPMsgNotify::NotifyFilter get top record failed. mandt:%s, user:%s, session:%s", 
        pUserInfo->_strMANDT.c_str(), pUserInfo->_strName.c_str(), strSessionKey.c_str() );
        return;
    }
    int nCounter = rc==SQLDBC_NO_DATA_FOUND ?  -1 :  _notifyData.GetCounter();
    theLog->WriteLog(log_info,  "SAPMsgNotify::NotifyFilter get top record success.rc=%d, mandt:%s, user:%s, session:%s, counter:%d", 
        rc, pUserInfo->_strMANDT.c_str(), pUserInfo->_strName.c_str(), strSessionKey.c_str(), nCounter );

    for (auto itFilterItem = lstPredicItems.begin(); itFilterItem != lstPredicItems.end(); itFilterItem++) {
        nCounter++;
        InsertNotifyDataForFilter(pDBConn, pUserInfo, nCounter,  &(*itFilterItem) );
    }
}

void DeletePraparedStatement(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt);

bool SAPMsgNotify::GetRowCount(void* pDBConn, EnforcerCtxImpl *pctx, const std::string& strSql, int& outCount){
    bool bRet = false;
    SQLDBC::SQLDBC_Connection* hdb_connection = (SQLDBC::SQLDBC_Connection*)pDBConn;
    do{

        std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement, void(*)(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt)> 
                                        hdb_stmt( SQLDBC_Connection_createPreparedStatement_Old(hdb_connection), DeletePraparedStatement );
        if (hdb_stmt==NULL){         
            theLog->WriteLog(log_error, "GetRowCount, create prepared statement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
            break;
        }

		SQLDBC_Retcode rc = prepare_3_old(hdb_stmt.get(), (char*)strSql.c_str(), strSql.length(), SQLDBC_StringEncodingType::Ascii);
        if (SQLDBC_OK != rc) {
			theLog->WriteLog(log_error, "GetRowCount, prepare_3_old failed, rc=%d", rc);
        	break;
   	 	}

        //bind
        S4HException e;
        bool bBind = pctx->OriginStmtBindParameters(hdb_stmt.get(), e);
        if(!bBind){
            theLog->WriteLog(log_error, "GetRowCount, bind failed, error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
            break;
        }

        //execute
        rc = execute_old(hdb_stmt.get());
        if(rc!=SQLDBC_OK){
            theLog->WriteLog(log_error, "GetRowCount, execute failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
            break;
        }

        //get and check result
        auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(hdb_stmt.get());
        if (pResultSet==nullptr){
            theLog->WriteLog(log_error, "GetRowCount, get_result_set error");
            break;
        }

        //getResultCount
        int nResultRow = SQLDBC_ResultSet_getResultCount_Old(pResultSet);
        if(nResultRow != 1){
            theLog->WriteLog(log_error, "GetRowCount error, nResultRow=%d", nResultRow);
            break;
        }

        //get next
        rc = SQLDBC_ResultSet_next_Old(pResultSet);
        if (SQLDBC_OK != rc)
        {
            theLog->WriteLog(log_error, "GetRowCount, call next failed. rc=%d, msg:%s", rc, SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(pResultSet)));
            break;
        }

        const int nMaxLen = 32;
        char szCount[nMaxLen]={0};
        SQLDBC_Length nLenCount = 0;
        rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szCount, &nLenCount, sizeof(szCount), SQLDBC_TRUE );
        if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
            theLog->WriteLog(log_error, "GetRowCount, get colomn value failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
            break;
        }
        if(nLenCount > 0 && nLenCount < nMaxLen){   //if TABLE has no any record, nResultRow=1, but nLenCount=-1
            szCount[nLenCount] = 0;
            outCount = atoi(szCount);
            theLog->WriteLog(log_trace, "GetRowCount, nLenCount:%d, szCount:%s, nCount:%d", nLenCount, szCount, outCount);
            bRet = true;
        } else {
            theLog->WriteLog(log_error, "GetRowCount, nLenCount:%d", nLenCount);
        }
    } while (false);
    
    return bRet;
}

bool SAPMsgNotify::CheckDoNotifyFilter(void* pDBConn, EnforcerCtxImpl *pctx, const std::string& strNewSql, const std::string& strOldSql)
{
    std::string strNewSqlCount = "SELECT COUNT(*) FROM (" + strNewSql + ")";
    std::string strOldSqlCount = "SELECT COUNT(*) FROM (" + strOldSql +")";
    theLog->WriteLog(log_trace, "CheckDoNotifyFilter, new sql:%s, default sql:%s", strNewSqlCount.c_str(), strOldSqlCount.c_str() );

    int nNew = -1, nOld = -1;
    bool bNew = GetRowCount(pDBConn, pctx, strNewSqlCount, nNew);
    bool bOld = GetRowCount(pDBConn, pctx, strOldSqlCount, nOld);

    return bNew && bOld && nNew != nOld;
}

void SAPMsgNotify::CheckDoNotifyMask(void *pDBConn, EnforcerCtxImpl *pctx, const std::vector<MaskItem> &vecMaskItem, const std::string &strMaskCountSql, std::vector<MaskItem> &vecEffectedMaskItem)
{
    theLog->WriteLog(log_trace, "CheckDoNotifyMask, strMaskCountSql:%s", strMaskCountSql.c_str());
    do
    {
        S4HException e;
        SQLDBC::SQLDBC_Connection *hdb_connection = (SQLDBC::SQLDBC_Connection *)pDBConn;

        std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement, void (*)(SQLDBC::SQLDBC_PreparedStatement * pPreparedStamt)>
            hdb_stmt(SQLDBC_Connection_createPreparedStatement_Old(hdb_connection), DeletePraparedStatement);
        if (hdb_stmt == NULL)
        {
            theLog->WriteLog(log_error, "CheckDoNotifyMask, create prepared statement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(hdb_connection)));
            break;
        }

        SQLDBC_Retcode rc = prepare_3_old(hdb_stmt.get(), (char *)strMaskCountSql.c_str(), strMaskCountSql.length(), SQLDBC_StringEncodingType::Ascii);
        if (SQLDBC_OK != rc)
        {
            theLog->WriteLog(log_error, "CheckDoNotifyMask, prepare_3_old failed, rc=%d", rc);
            break;
        }

        //bind
        bool bBind = pctx->OriginStmtBindParameters(hdb_stmt.get(), e);
        if(!bBind){
            theLog->WriteLog(log_error, "CheckDoNotifyMask, bind failed, error=%s", SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
            break;
        }

        //execute
        rc = execute_old(hdb_stmt.get());
        if (rc != SQLDBC_OK)
        {
            theLog->WriteLog(log_error, "CheckDoNotifyMask, execute failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())));
            break;
        }

        //get and check result
        auto pResultSet = (SQLDBC::SQLDBC_ResultSet *)get_result_set_old(hdb_stmt.get());
        if (pResultSet == nullptr)
        {
            theLog->WriteLog(log_error, "CheckDoNotifyMask, get_result_set error");
            break;
        }

        //get next
        rc = SQLDBC_ResultSet_next_Old(pResultSet);
        if (SQLDBC_OK != rc)
        {
            theLog->WriteLog(log_error, "CheckDoNotifyMask, call next failed. rc=%d, msg:%s", rc, SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(pResultSet)));
            break;
        }

        //get result
        int nMaskIndex = 1;
        const int nMaxLen = 32;
        for (auto itMaskItem = vecMaskItem.begin(); itMaskItem != vecMaskItem.end(); ++itMaskItem, ++nMaskIndex)
        {
            char szCount[nMaxLen]={0};
            SQLDBC_Length nLenCount = 0;
            rc = SQLDBC_ResultSet_getObject_Old(pResultSet, nMaskIndex, SQLDBC_HOSTTYPE_ASCII, szCount, &nLenCount, sizeof(szCount), SQLDBC_TRUE );
            if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                theLog->WriteLog(log_error, "CheckDoNotifyMask, get colomn %d attribute value failed, rc=%d, error=%s", nMaskIndex, rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                break;
            }
            if(nLenCount > 0 && nLenCount < nMaxLen){   //if TABLE has no any record, nResultRow=1, but nLenCount=-1
                szCount[nLenCount] = 0;
                int nCount = atoi(szCount);
                theLog->WriteLog(log_info, "CheckDoNotifyMask, nMaskIndex:%d, nLenCount:%d, szCount:%s, nCount:%d", nMaskIndex, nLenCount, szCount, nCount);

                if(nCount > 0){
                    vecEffectedMaskItem.emplace_back(*itMaskItem);
                }
            } else {
                theLog->WriteLog(log_info, "CheckDoNotifyMask, nMaskIndex:%d, nLenCount:%d", nMaskIndex, nLenCount);
            }
        }
    } while (false);
}

int NotifyItemData::GetCounter()
{
    return atoi(_COUNTER);
}


//
// Created by jeff on 2020/3/10.
//

#include "EMS4HDeleteAst.h"
#include "PolicyManager.h"
#include "PolicyExport.h"
#include "log.h"
#include "EnforcerCtxImpl.h"
#include "TableMetaDataImpl.h"
#include "talk_with_hdb.h"
#include "TableDataCache.h"

TableDataCache EMS4HDeleteAst::_tableDataCacheForDelete;

void EMS4HDeleteAst::Parser(Node * proot, S4HException & exc) {
    if(!proot) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "delete statement root is nullptr");
        return;
    }
    // key = 2 is tb_ref
    Node * ptb = proot->getChild(2);
    if(ptb == nullptr) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "delete statement table is nullptr");
        return;
    }
    if(ptb->nodeType_ == E_TABLE_IDENT) {
        parser_name_table(ptb, _table, exc);
        _table._node = ptb;
    } else if(ptb->nodeType_ == E_ALIAS) {
        Node * pnode_tb = ptb->getChild(0);
        assert(pnode_tb);
        if( pnode_tb->GetType() == E_TABLE_IDENT){
            parser_name_table(pnode_tb, _table, exc);
            Node * palias = ptb->getChild(1);
            if(palias){
                _table._alias = palias->terminalToken_.str;
            }
        }
        _table._node = pnode_tb;
    }
    // key = 6 is whereclause
    ParseWhereClause(proot, exc);
    // key = 7 is hintclause
}


bool EMS4HDeleteAst::CheckSupport() {
    if(_table._node == NULL) return false;
    return true;
}

void EMS4HDeleteAst::ParseWhereClause(Node * pnode, S4HException & /*exc*/) {
    if(pnode)
    {
        if(pnode->nodeType_ == E_DELETE) {
            _whereclause._root = pnode;
            _whereclause._node = pnode->getChild(6);// key = 6 is whereclause
        }
    }
}

bool EMS4HDeleteAst::RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo){
    S4HException exc;
    std::vector<EMTableInfo*>  pctables;
    pctables.push_back(&_table);
    std::string action = EM_ACTION_DELETE;
    PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, pctables, action,exc);
    std::string cond =  PolicyManager::Instance()->GetFilterCondition(connection);

    bool bret = _whereclause.EvaluationSQL(cond);
    if(!bret) {
        theLog->WriteLog(log_error, "RewriteWithFilterCondition failed. EvaluationSQL failed.");
    }

    return bret;
}

void DeletePraparedStatement(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt);

bool EMS4HDeleteAst::RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * /*pctx*/){
#ifdef QUERY_POLICY_LOCAL_CC
    bool bret = false;
    assert(pUserInfo);
    assert(presinfo);
    //make subject and resource map
    EMDictionary user_dic;
    user_dic.insert(std::make_pair(USER_ATTR_NAME_USERNAME, pUserInfo->_strName));
    user_dic.insert(std::make_pair(USER_ATTR_NAME_TERMINAL, pUserInfo->_Terminal));
    user_dic.insert(std::make_pair(USER_ATTR_NAME_HOSTIP, pUserInfo->_strIP));
    user_dic.insert(std::make_pair(USER_ATTR_NAME_HOsTNAME, pUserInfo->_strHostName));
    for(auto it: pUserInfo->_attribute) {
        if(it.second.size() > 0) {
            user_dic.insert(std::make_pair(it.first, *(it.second.begin()) )); /* todo value is set */
        }
    }

    EMDictionary res_dic;
    if(_table._db.empty()) {
        res_dic.insert(std::make_pair(RES_DATABASE_NAME, presinfo->_database));
    } else {
        res_dic.insert(std::make_pair(RES_DATABASE_NAME, _table._db));
    }
    if(_table._schema.empty()) {
        res_dic.insert(std::make_pair(RES_SCHEMA_NAME, presinfo->_schema));
    } else {
        res_dic.insert(std::make_pair(RES_SCHEMA_NAME, _table._schema));
    }
    res_dic.insert(std::make_pair(RES_TABLE_NAME, _table._tb));

    //match policy
    S4HException emexc;
    bret = MatchPolicy(user_dic, res_dic, EM_ACTION_DELETE,  emexc);
    if(!bret) return  false;

    //get condition
    std::string cond;
    GetMatchCondition(cond, emexc);
    if(cond.empty()) return false;

    //rewrite sql
    bret = _whereclause.EvaluationSQL(cond);
    if(!bret) {
        theLog->WriteLog(log_error, "RewriteWithFilterCondition failed. EvaluationSQL failed.");
    }

    return bret;
#else
    return RewriteWithFilterCondition(connection,pUserInfo, presinfo);
#endif
}

void EMS4HDeleteAst::ResetSQL() {
    _whereclause.ResetNode();
}

void EMS4HDeleteAst::CacheDeleteData(void* connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo,
                         EnforcerCtxImpl* pctx)
{
    //remove expired cache data
    _tableDataCacheForDelete.RemoveExpiredData();

    //query policy to check if the table have mask obligation
     PolicyManager* policyMgr = PolicyManager::Instance();
    S4HException exc;
    std::vector<EMTableInfo *> vecTables;
    vecTables.push_back(&_table);
    policyMgr->QueryPC_Enforcer(pUserInfo, presinfo, vecTables, get_action_from_ast_type(E_STMT_TYPE_SELECT), exc);
    
    const MaskConditionMap* pMaskTableMap = policyMgr->GetMaskInfo();
    theLog->WriteLog(log_info,  std::bind(&PolicyManager::PrintMaskConditionMap, PolicyManager::Instance(), *pMaskTableMap) );

    auto itMaskInfo = pMaskTableMap->find(_table._tb);
    if (itMaskInfo==pMaskTableMap->end())  {
        return;
    }
    const MaskItemMap* pMaskItemMap = itMaskInfo->second;

    //get table metadata
    S4HException ex;
    const ITableMetaData* pTableMeta = policyMgr->GetTableMetaData(_table._tb, _table._type, connection, ex);
    if (NULL==pTableMeta) {
        theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, get table metadata failed" );
        return;
    }

    //construct SQL to get mask hint and the original data
    std::string strSQL= "SELECT ";
    const std::vector<std::string> vecItems = pTableMeta->GetAllKeyColumns();
    std::vector<std::string> vecKeys;
    for (auto itKey=vecItems.begin(); itKey!=vecItems.end(); itKey++){
        if(itKey!=vecItems.begin()){
            strSQL += ", ";
        }
        std::string value = *itKey;
        vecKeys.push_back(value);
        strSQL += "\"" + value  + "\"";
    } 
    for (auto itMaskItem=pMaskItemMap->begin(); itMaskItem!=pMaskItemMap->end(); itMaskItem++){
        strSQL += ", \"" + itMaskItem->first  + "\", ";

        //mask hint
        std::string strMaskCond = itMaskItem->second->_condition;
        if(strMaskCond.empty()){
            strMaskCond = " (1=1) ";
        }
        strSQL += "CASE WHEN " + strMaskCond + " THEN '"  MASK_HINT_MASKED  "' ELSE '" MASK_HINT_NOMASKED "' END MASK_HINT ";
    }
    strSQL += " FROM \"" + _table._tb + "\" ";
    strSQL += _whereclause._node->serialize();
    theLog->WriteLog(log_info, "EMS4HDeleteAst::CacheDeleteData, hint sql:%s", strSQL.c_str() );

    //execute sql and cache data
	do
	{
        auto hdb_connection = (SQLDBC::SQLDBC_Connection*)connection;
        std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement, void(*)(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt)> 
                                        hdb_stmt( SQLDBC_Connection_createPreparedStatement_Old(hdb_connection), DeletePraparedStatement );
        if (hdb_stmt==NULL){         
            theLog->WriteLog(log_info, "EMS4HDeleteAst::CacheDeleteData, create prepared statement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
            break;
        }

		SQLDBC_Retcode rc = prepare_3_old(hdb_stmt.get(), (char*)strSQL.c_str(), strSQL.length(), SQLDBC_StringEncodingType::Ascii);
        if (SQLDBC_OK != rc) {
			theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, prepare_3_old failed, rc=%d", rc);
        	break;
   	 	}

        //bind 
        S4HException e;
        bool bBind = pctx->OriginStmtBindParameters(hdb_stmt.get(), e);
        if(!bBind){
            break;
        }

        //execute
        rc = execute_old(hdb_stmt.get());
        if(rc!=SQLDBC_OK){
            theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, execute failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
            continue;
        }

        //get and check result
        auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(hdb_stmt.get());
        if (pResultSet==nullptr){
            theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, get_result_set==null");
            continue;
        }

        const int nMaxBuf=1024;
        while (true){

            rc =  SQLDBC_ResultSet_next_Old(pResultSet);
            if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                if (rc!=SQLDBC_NO_DATA_FOUND){
                    theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, call next failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                }
                break;
            }

            std::shared_ptr<ROW_DATA> pRowData(new ROW_DATA);
            pRowData->_table = _table._tb;
            pRowData->_time = time(0);
            pRowData->_maskedFields.reserve(pMaskItemMap->size());
  

           //get key
           std::vector<std::string> vecKeyValues;
           int nIndex= 0;
           for (; nIndex<vecKeys.size(); nIndex++) {

                char szFieldBuf[nMaxBuf]={0};
                SQLDBC_Length nLenField = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, nIndex+1, SQLDBC_HOSTTYPE_ASCII, szFieldBuf, &nLenField, sizeof(szFieldBuf), SQLDBC_TRUE );
                if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                    theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, get colomn %d attribute name failed, rc=%d, error=%s", nIndex+1, rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                    continue;
                }
                szFieldBuf[nLenField] = 0;

               std::string strFieldValue{szFieldBuf};
               vecKeyValues.push_back(strFieldValue);
               pRowData->_data[vecKeys[nIndex]]=strFieldValue;
           }
           std::string strRowKeyValue= TableDataCache::ConstructRowKey(_table._tb, vecKeyValues);
           pRowData->_key = strRowKeyValue;
           
           //get other data
           std::string strMaskedField;
           for (auto itMaskItem = pMaskItemMap->begin(); itMaskItem != pMaskItemMap->end(); itMaskItem++, nIndex+=2)  {     
                //field data
                char szFieldBuf[nMaxBuf]={0};
                SQLDBC_Length nLenField = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, nIndex+1, SQLDBC_HOSTTYPE_ASCII, szFieldBuf, &nLenField, sizeof(szFieldBuf), SQLDBC_TRUE );
                if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                    theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, get colomn %d attribute name failed, rc=%d, error=%s", nIndex+1, rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                    continue;
                }
                szFieldBuf[nLenField] = 0;
                pRowData->_data[itMaskItem->first]= szFieldBuf;

                //mask hint
                memset(szFieldBuf,'\0',nMaxBuf);
                nLenField = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, nIndex+2, SQLDBC_HOSTTYPE_ASCII, szFieldBuf, &nLenField, sizeof(szFieldBuf), SQLDBC_TRUE );
                if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                    theLog->WriteLog(log_error, "EMS4HDeleteAst::CacheDeleteData, get colomn %d attribute name failed, rc=%d, error=%s", nIndex+2, rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                    continue;
                }
                szFieldBuf[nLenField] = 0;

                std::string strMaskHint{szFieldBuf};
                if (strMaskHint.compare(MASK_HINT_MASKED) == 0){
                    pRowData->_maskedFields.push_back(itMaskItem->first);
                    strMaskedField += itMaskItem->first + ",";
                }
           }

           _tableDataCacheForDelete.InsertRowData(strRowKeyValue, pRowData);
           theLog->WriteLog(log_info, "EMS4HDeleteAst::CacheDeleteData, add row value, key:%s, maskedfields:%s", strRowKeyValue.c_str() , strMaskedField.c_str() );

        }
  		
	} while (false);
}
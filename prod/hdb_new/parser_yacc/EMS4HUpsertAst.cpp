//
// Created by jeff on 2020/7/6.
//
#include "tool.h"
#include "log.h"
#include "PolicyManager.h"
#include "EMS4HUpsertAst.h"
#include "TableAliasMgr.h"
#include "TableAliasItem.h"
#include "EnforcerCtxImpl.h"
#include "talk_with_hdb.h"
#include "StructDefine.h"

void EMS4HUpsertAst::Parser(Node * proot, S4HException & exc) {
    if(!proot) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "upsert statement root is nullptr");
        return;
    }
    // key = 0 is tb_ref
    Node * ptb = proot->getChild(0);
    if(ptb == nullptr) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "upsert statement table is nullptr");
        return;
    }
    if(ptb->nodeType_ == E_TABLE_IDENT) {
        parser_name_table(ptb, _table, exc);
        _table._node = ptb;
    }
    //key = 1 is E_INSERT_COLUMNS_AND_SOURCE
    Node * pcols = proot->getChild(1);
    if(pcols == nullptr) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "upsert statement table is nullptr");
        return;
    }
    if(pcols->nodeType_ == E_SIMPLE_IDENT_LIST_WITH_PARENS) {
        //parser colums
        Node * node = pcols->getChild(0);
        //E_SIMPLE_IDENT_LIST
        std::list<Node*> nodes;
        Node::ToList(node,nodes);
        for(auto it: nodes){
            UpCol col;
            col._deny = false;
            col._col = it->terminalToken_.str;
            col._node = it;
            _vec_col.push_back(col);
        }
    }
    //from_list  /   subquery
    Node * pvals = proot->getChild(2);
    if(pvals == nullptr) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "upsert statement table is nullptr");
        return;
    }
    if(pvals->nodeType_ == E_VALUES_CTOR) {
        //parser colums
        Node * node = pvals->getChild(0);
        //E_EXPR_LIST_WITH_PARENS
        if(node->GetType() == E_EXPR_LIST_WITH_PARENS){
            node = node->getChild(0);
        }
        //E_EXPR_LIST
        std::list<Node*> nodes;
        Node::ToList(node,nodes);
        for(auto it: nodes){
            UpCol col;
            col._deny = false;
            col._col = it->terminalToken_.str.empty()?it->serialize():it->terminalToken_.str;
            col._node = it;
            _vec_val.push_back(col);
        }
    } else { //subquery
        _subq = pvals;
    }
    //E_WHERE_CLAUSE
    Node * pwhere = proot->getChild(3);
    if(pwhere){
        _whereclause._node = pwhere;
        _whereclause._root = proot;
        _where = pwhere->serialize();
    }
    //e_string
    Node * pprimary = proot->getChild(4);
    if(pprimary){
        _primary = pprimary->serialize();
    }
}

bool EMS4HUpsertAst::CheckSupport() {
    if(_table._node == NULL) return false;
    return true;
}

bool EMS4HUpsertAst::InterceptSQL(void* /*connection*/, const NxlDBCUserInfo* /*pUserInfo*/, const NxlDBCResInfo * /*presinfo*/) {
    return false;
}

std::string output_vec_upcol(const std::vector<UpCol> & cols){
    std::string ret;
    for(size_t i = 0 ; i < cols.size(); ++i){
        if(!cols[i]._deny){
            if(!ret.empty()){
                ret += ", ";
            }
            ret += cols[i]._node->serialize();
        }
    }
    return ret;
}

std::string EMS4HUpsertAst::Output() const{
    std::string ret = "UPSERT ";
    ret += _table._node->serialize();
    if(_vec_col.size() > 0){
        ret+="(";
        ret += output_vec_upcol(_vec_col);
        ret+=")";
    }
    if(_subq){
        ret += _subq->serialize();
    } else {
        if(_vec_val.size() > 0){
            ret+="VALUES (";
            ret += output_vec_upcol(_vec_val);
            ret+=")";
        }
        ret += _where;
        ret += _primary;
    }

    return ret;
}


void EMS4HUpsertAst::ResetSQL(){
    for(size_t i = 0 ; i < _vec_col.size(); ++i){
        _vec_val[i]._deny = false;
        _vec_col[i]._deny = false;
    }
    _deny_parm_index.clear();
    _whereclause.ResetNode();
}


bool EMS4HUpsertAst::CollectParam(const int index, const std::string & strparam) {

    for(int i = 0 ; i < _vec_val.size();++i) {
        if(_vec_val[i]._node->questmarkid_ == index  )
        {
            _vec_val[i]._bind_data._type = SQLDBC_HOSTTYPE_ASCII;
            _vec_val[i]._bind_data._Data._strData = strparam;
            _vec_val[i]._bind_data._Data._strData.erase(_vec_val[i]._bind_data._Data._strData.find_last_not_of(" ") + 1);
            return true;
        }
    }
    return false;
}

bool EMS4HUpsertAst::CollectParam(const int index, int64_t param)
{
    for(int i = 0 ; i < _vec_val.size();++i) {
        if(_vec_val[i]._node->questmarkid_ == index  )
        {
            _vec_val[i]._bind_data._type =  SQLDBC_HOSTTYPE_INT8;
            _vec_val[i]._bind_data._Data._int64Data = param;
            return true;
        }
    }
    return false;
}

bool EMS4HUpsertAst::CollectParam(const int index, uint64_t param)
{
    for(int i = 0 ; i < _vec_val.size();++i) {
        if(_vec_val[i]._node->questmarkid_ == index  )
        {
            _vec_val[i]._bind_data._type = SQLDBC_HOSTTYPE_UINT8;
            _vec_val[i]._bind_data._Data._uint64Data = param;
            return true;
        }
    }
    return false;
}

bool EMS4HUpsertAst::IsDenyParamIndex(unsigned int index ){
    for(auto it:_deny_parm_index){
        if(it == index)
            return true;
    }
    return false;
}

void DeletePraparedStatement(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt);

bool EMS4HUpsertAst::RewriteSQL_UpsertToOtherTable(void *connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo, const EnforcerCtxImpl *pCtx)
{
    bool bRewriteForDenyMask = false;
    do
    {
        //check if it have table alias item for this table
        const TableAliasItem *pTableAlias = TableAliasMgr::Instance().GetAliasItemByAliasTable(_table._tb);
        if (NULL == pTableAlias)
        {
            theLog->WriteLog(log_info, "UpsertToOtherTable break for didn't find table alias. table:%s", _table._tb.c_str());
            break;
        }

        //here we find table alias, create EMTableInfo struct for original table
        theLog->WriteLog(log_info, "UpsertToOtherTable find table alias. original:%s, alias:%s", pTableAlias->OriginalTable().c_str(), pTableAlias->AliasTable().c_str());
        EMTableInfo originalTable(_table);
        originalTable._tb = pTableAlias->OriginalTable();
        originalTable._alias = "\"" + pTableAlias->OriginalTable() + "\"";
  
        //query policy with the original table
       S4HException exc;
       std::vector<EMTableInfo*> vecTables;
       vecTables.push_back(&originalTable);
       bool bQueryPC = PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, vecTables, EM_ACTION_SELECT, exc);
       if (!bQueryPC)  {
            theLog->WriteLog(log_error, "UpsertToOtherTable break for Query pc failed.");
            break;
       }
 
       //get mask obligation about the original table
       const MaskConditionMap *pmap_mask = PolicyManager::Instance()->GetMaskInfo();
       auto itMask = pmap_mask->find(originalTable._tb);
       if (itMask == pmap_mask->end()) {
           theLog->WriteLog(log_error, "UpsertToOtherTable break for no mask obligation. original table:%s.", originalTable._tb.c_str() );
           break;
       }

       const MaskItemMap *pMaskItemMap = itMask->second;
       if (NULL == pMaskItemMap)
       {
           theLog->WriteLog(log_info, "UpsertToOtherTable break for no mask item found. original table:%s.", originalTable._tb.c_str() );
           break;
       }


       //for each UPDATE filed. check if there is a mask obligation
       std::map<std::string, UpCol*> mapColumnHaveMaskObligation; //original field-> update field
       std::map<std::string, MaskItem*> mapColumnMaskInfo; // original field--> mask item  
       for (auto itField = _vec_col.begin(); itField != _vec_col.end(); itField++)
       {
           UpCol *pUpdateColInfo =&(*itField);

           //get the original filed
           std::string original_field = pTableAlias->GetOriginalField(pUpdateColInfo->_col);
           if (original_field.empty())
           {
                theLog->WriteLog(log_info, "UpsertToOtherTable continue for the original field is empty for %s, original_field:%s", pUpdateColInfo->_col.c_str(),  original_field.c_str() );
                continue; 
           }
           
           //get mask obligation for the original field
           auto itMaskItem = pMaskItemMap->find(original_field);
           if (itMaskItem == pMaskItemMap->end())
           {
               theLog->WriteLog(log_info, "UpsertToOtherTable continue for the original field have no mask obligation: %s, original_field:%s", pUpdateColInfo->_col.c_str(), original_field.c_str() );
               continue; 
           }
           
            if (itMaskItem->second->_symbols.empty())
            {
                theLog->WriteLog(log_info, "UpsertToOtherTable continue for mask symbol is empty filed:%s, original_field:%s", pUpdateColInfo->_col.c_str(), original_field.c_str() );
                continue;
            }

            //here there is mask obligation for that field.
            mapColumnHaveMaskObligation[pUpdateColInfo->_col] = pUpdateColInfo;
            mapColumnMaskInfo[pUpdateColInfo->_col] = itMaskItem->second;

            theLog->WriteLog(log_info, "UpsertToOtherTable find mask info: original field:%s, alias field:%s",  original_field.c_str() , pUpdateColInfo->_col.c_str()); 
       }

       //construct SELECT statment that get original record from original table
       std::string strSelectOriginalRecordSQL = "SELECT ";
       for(auto itColumnMask=mapColumnMaskInfo.begin(); itColumnMask!=mapColumnMaskInfo.end(); itColumnMask++)
       {
           MaskItem* pMaskItem =  itColumnMask->second;
           std::string strMaskCodition = pMaskItem->_condition;
           if (strMaskCodition.empty())
           {
               strMaskCodition="1=1";
           }
           
           strSelectOriginalRecordSQL += "(CASE WHEN " + strMaskCodition +" THEN '" +  MASK_HINT_MASKED  +"' ELSE  '" +  MASK_HINT_NOMASKED + "' END) as MASK_HINT, " + pMaskItem->_field + ",";
       }

       if (strSelectOriginalRecordSQL[strSelectOriginalRecordSQL.length()-1]==',')
       {
            strSelectOriginalRecordSQL = strSelectOriginalRecordSQL.substr(0, strSelectOriginalRecordSQL.length()-1 );
       }

       strSelectOriginalRecordSQL +=" FROM " + pTableAlias->OriginalTable() + " WHERE "; 

       //construct "update relation"
       std::string strUpdateRelation;
       std::list<int> listQuestionParamPosition;
       ConstructUpdateRelation(connection, pTableAlias->UpdateRelation(), strUpdateRelation, listQuestionParamPosition);
       
       strSelectOriginalRecordSQL += strUpdateRelation;

       theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, select sql:%s", strSelectOriginalRecordSQL.c_str() );
       
       //execute select SQL
       #if 1
       do
       {
            auto hdb_connection = (SQLDBC::SQLDBC_Connection*)connection;
            std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement, void(*)(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt)> 
                                            hdb_stmt(SQLDBC_Connection_createPreparedStatement_Old(hdb_connection), DeletePraparedStatement );
            if (hdb_stmt==NULL){         
                theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, create prepared statement failed." );
                break;
            }
            

           SQLDBC_Retcode rc = prepare_3_old(hdb_stmt.get(), strSelectOriginalRecordSQL.c_str(), strSelectOriginalRecordSQL.length(), SQLDBC_StringEncodingType::Ascii);
           if (SQLDBC_OK != rc) {
               theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call prepare_3_old failed:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
               break;
           }
      
           //bind
           int nParamIndex = 1;
           for (auto itQuestionParam = listQuestionParamPosition.begin(); itQuestionParam!=listQuestionParamPosition.end(); itQuestionParam++, nParamIndex++){
               
               //get paramter
               const BindParameters* pBindParam =  pCtx->GetBindParamByPosition(*itQuestionParam);
               if (NULL==pBindParam){
                    theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, can't find param for:%d", *itQuestionParam);
               }
               
               if (NULL!=pBindParam) {
                   rc = bind_parameter_old(hdb_stmt.get(), nParamIndex, pBindParam->_Type, pBindParam->_paramAddr, pBindParam->_LengthIndicator, pBindParam->_Size, pBindParam->_Terminate);
                   if (SQLDBC_OK != rc) {
                       theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call bind_parameter_old failed. index:%d, msg:%s", nParamIndex, SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
                       break;
                   }
               }
           }
           
           //execute
           rc = execute_old(hdb_stmt.get());
           if (SQLDBC_OK != rc){
                theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call execute_old failed. msg:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
                break;
           }

           //get result
           auto result_set = get_result_set_old(hdb_stmt.get());
           if (result_set == nullptr)  {
               theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call execute_old failed. msg:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
                break;
           }

           //get next
           rc = SQLDBC_ResultSet_next_Old(result_set);
           if (SQLDBC_OK != rc) {
              theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call next failed. rc=%d, msg:%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
              break;
           }

           //check mask or unmask
           int nResultIndex = 1;
           for (auto itColumnHaveMaskOB = mapColumnHaveMaskObligation.begin();
                itColumnHaveMaskOB != mapColumnHaveMaskObligation.end(); itColumnHaveMaskOB++, nResultIndex+=2) {

               UpCol *pUpdateColumnInfo = itColumnHaveMaskOB->second;

               //get mask hint result
               char szMaskHint[64] = { 0 };
               rc = SQLDBC_ResultSet_getObject_Old(result_set, nResultIndex, SQLDBC_HOSTTYPE_ASCII, szMaskHint, nullptr, sizeof(szMaskHint), SQLDBC_TRUE);
               if (SQLDBC_OK!=rc) {
                   theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call getObject failed.index:%d, rc=%d, msg:%s",nResultIndex,  rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(result_set)) );
                   continue;
               }
                
               theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, index:%d, mask hint:%s", nResultIndex, szMaskHint );

                  //if it masked, then get the real value from original table.
               if (CommonFun::StrCaseCmp(szMaskHint, MASK_HINT_MASKED) == 0)
               {
                   theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, get mask column:%s", pUpdateColumnInfo->_col.c_str() );

                   //get the bind param of the statement
                   uint uParamID = 0;
                   BindParameters *pBindParam = NULL;
                   GetUpsertColumnParamId(pUpdateColumnInfo->_col, connection, uParamID);
                   if (0==uParamID) {
                        theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable,GetUpsertColumnParamId failed for:%s", pUpdateColumnInfo->_col.c_str() );
                        continue;
                   }
                   pBindParam = pCtx->GetBindParamByPosition(uParamID);
                   if (NULL == pBindParam) {
                       theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, can't find param to replace for:%d", uParamID);
                       continue;
                   }

                    const int bufLen = 128;
                    unsigned char* pDataBuf= new unsigned char[bufLen];
                    SQLDBC_Length nLengthIndicate = *(pBindParam->_LengthIndicator); //get the indicator
                    rc = SQLDBC_ResultSet_getObject_Old(result_set, nResultIndex+1, pBindParam->_Type, pDataBuf, &nLengthIndicate, bufLen, SQLDBC_TRUE);
                    if (SQLDBC_OK!=rc) {
                        theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call getObject failed.index:%d, rc=%d, msg:%s",nResultIndex+1,  rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(result_set)) );
                        delete[] pDataBuf;
                        pDataBuf = NULL;
                        continue;
                    }
                    else  {
                        theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, get original  length:%d",  nLengthIndicate);
                        
                        pBindParam->_paramAddr = pDataBuf;
                        pBindParam->_Size = nLengthIndicate;
                       // *(pBindParam->_LengthIndicator) = nLengthIndicate;
                        pBindParam->_bBindWithNewValue = true;

                        bRewriteForDenyMask = true;
                    }
                    
               }
           }

       } while (false);

       #endif 

    } while (false);

    return bRewriteForDenyMask;
}

void EMS4HUpsertAst::ConstructUpdateRelation(void* connection, const std::string& strRelationTemplete, std::string& strRelation,
                                             std::list<int>& listQuestionParamPosition)
{
    strRelation = strRelationTemplete;
    size_t nEqualPos = std::string::npos;
    size_t nSpacePos = std::string::npos;
    size_t nBeginPos = 0;
    while (nBeginPos!=std::string::npos)
    {
        std::string strRelationRightValue;

        nEqualPos = strRelation.find('=', nBeginPos);
        if (nEqualPos==std::string::npos){
            break;
        }

        nBeginPos= nEqualPos;
        nSpacePos = strRelation.find(' ', nBeginPos);
        if (nSpacePos==std::string::npos)  {
            strRelationRightValue = strRelation.substr(nEqualPos+1);
        }
        else {
             strRelationRightValue = strRelation.substr(nEqualPos+1, nSpacePos-nEqualPos-1);
        }
        
        nBeginPos = nSpacePos; 

        std::string strColumnName;
        unsigned int nQuestionParamPosition=0;
        if (!strRelationRightValue.empty())  {
            strColumnName = strRelationRightValue;
            size_t nPointPos = strRelationRightValue.find('.');
            if (nPointPos!=std::string::npos){
                strColumnName = strRelationRightValue.substr(nPointPos+1);
            }
            
            //find place holder
            GetUpsertColumnParamId(strColumnName, connection,  nQuestionParamPosition);
            if (nQuestionParamPosition){
                listQuestionParamPosition.push_back(nQuestionParamPosition);
                strRelation.replace(nEqualPos+1, strRelationRightValue.length(), "?" );

                nBeginPos = nEqualPos + 1;
            } 
     
        }
        
        theLog->WriteLog(log_info, "ConstructUpdateRelation, RightValue:%s, column name:%s, question param:%d", 
                        strRelationRightValue.c_str(), strColumnName.c_str(), nQuestionParamPosition);    
    }

}


bool EMS4HUpsertAst::RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo){
    S4HException exc;
    std::vector<EMTableInfo*>  pctables;
    pctables.push_back(&_table);
    std::string action = EM_ACTION_UPSERT;
    PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, pctables, action,exc);
    std::string cond =  PolicyManager::Instance()->GetFilterCondition(connection);

    bool bret = false;  //false means didn't modify SQL
    if(!cond.empty()){
        bret = _whereclause.EvaluationSQL(cond);
        if(bret){
             Node * pwhere = _whereclause._root->getChild(3);
            if(pwhere){
                 _where = pwhere->serialize();
             }
         }
         if(!bret) {
            theLog->WriteLog(log_error, "RewriteWithFilterCondition failed. EvaluationSQL failed.");
        }
    }
    
    return bret;
}

bool EMS4HUpsertAst::RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * /*pctx*/){
    return RewriteWithFilterCondition(connection,pUserInfo, presinfo);
}

bool EMS4HUpsertAst::RewriteSQLForDenyUpsert(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx){
      bool bRewriteForDenyMask = false;
    
    //for UPSERT we only target for other table
    bRewriteForDenyMask = RewriteSQL_UpsertToOtherTable(connection, pUserInfo, presinfo, pctx);

    return bRewriteForDenyMask;
}

int EMS4HUpsertAst::GetValueParamId(const std::string & field) {
    assert(_node);
    //parser column
    Node * cols_p = _node->getChild(1);
    if(cols_p){
        Node * vals = _node->getChild(2);
        if(vals &&  vals->GetType() == E_VALUES_CTOR) {
            Node * cols = cols_p->getChild(0);
            std::vector<Node*> nodes;
            Node::ToList(cols, nodes);

            std::vector<Node*> values;
            Node * expr_list = vals->getChild(0);
            if(expr_list->GetType() == E_EXPR_LIST_WITH_PARENS){
                Node::ToList(expr_list->getChild(0), values);
            }
            if(values.size() != nodes.size()){
                return  -4;// column size not equal to value size
            }
            for(int i = 0 ; i < values.size(); ++i){
                if(CommonFun::StrCaseCmp(field, nodes[i]->terminalToken_.str) == 0){
                    Node * value_t = values[i];
                    if(value_t->GetType() == E_QUESTIONMARK){
                        return  value_t->questmarkid_;
                    } else {
                        return -2;
                    }
                }
            }

        } else {
            return -2;
        }

    } else { //no column def
        return  -1;
    }
    return -2;

}

int EMS4HUpsertAst::GetValueParamId(int index) {
    assert(_node);
    Node * vals = _node->getChild(2);

    if (vals && vals->GetType() == E_VALUES_CTOR) {

        std::vector<Node*> values;
        Node * expr_list = vals->getChild(0);
        if(expr_list->GetType() == E_EXPR_LIST_WITH_PARENS){
            Node::ToList(expr_list->getChild(0), values);
        }
        if(values.size() <= index){
            return  -1;
        }
        Node * value_t = values[index];
        if(value_t->GetType() == E_QUESTIONMARK){
            return  value_t->questmarkid_;
        } else {
            return -1;
        }
    }
    return -1;
}

bool EMS4HUpsertAst::GetUpsertColumnParamId( const std::string & column,   void *conn, unsigned int & markid) {
    int ret = GetValueParamId(column);
    if( ret >= 0 ){
        markid = ret;
        return true;
    } else {
        if(ret == -1){ // no column def
#ifdef JEFF_TEST
            std::vector<std::u16string> cols = {u"col1",u"col2",u"col3"};
#else
            const std::string strtb = GetTable()->_tb;
            S4HException exc;
            const ITableMetaData * meta = PolicyManager::Instance()->GetTableMetaData(strtb,EMTABLE_NATRUAL,conn,exc);
            if(NULL == meta) {
                return false;
            }
            std::vector<std::string> cols;
            meta->GetAllColumns(cols);
#endif
            int index = -1;
            for(int i = 0 ; i < cols.size(); ++i){
                if(CommonFun::StrCaseCmp(cols[i], column) == 0){
                    index = i;
                    break;
                }
            }
            if(index == -1){
                //exc.SetError(S4H_SQL_PARSE_FAIL,"field not in this table");
                return false;
            }
            ret = GetValueParamId(index);
            if( ret >= 0 ){
                markid = ret;
                return true;
            }
        }

    }
    //exc.SetError(S4H_SQL_PARSE_FAIL,"find field failed");
    return false;
}
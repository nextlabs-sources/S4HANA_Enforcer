//
// Created by jeff on 2020/2/23.
//

#include "EMS4HUpdateAst.h"
#include "PolicyManager.h"
#include "PolicyExport.h"
#include "log.h"
#include "tool.h"
#include "TableAliasItem.h"
#include "TableAliasMgr.h"
#include "talk_with_hdb.h"
#include "EnforcerCtxImpl.h"

void DeletePraparedStatement(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt);

EMS4HUpdateAst::~EMS4HUpdateAst(){
    for(auto it: _fields){
        delete(it);
    }
    _fields.clear();
}

void EMS4HUpdateAst::Parser(Node * proot, S4HException & exc) {
    if(!proot) return;
    switch (proot->nodeType_) {
        case E_STMT_LIST: {
            Parser(proot->getChild(0), exc);
        } break;
        case E_DIRECT_SELECT: {
            Node * ptemp = proot->getChild(1);
            Parser(ptemp, exc);
        } break;
        case E_UPDATE: {
            //
            Node * ptb = proot->getChild(2);
            if(ptb) ParserTable(ptb, exc);
            Node * plist = proot->getChild(4);
            if(plist) ParserColumns(plist, exc);
            ParseWhereClause(proot, exc);
        } break;
        default:
            break;
    }
}

void EMS4HUpdateAst::ParserTable(Node * pnode, S4HException & exc) {
    switch (pnode->nodeType_) {
        case E_TABLE_IDENT: {
            parser_name_table(pnode, _table, exc);
            if(!_table._tb.empty()) {
                _table._node = pnode;
            }
        }break;
        case E_ALIAS:{
            Node * pnode_tb = pnode->getChild(0);
            assert(pnode_tb);
            if( pnode_tb->GetType() == E_TABLE_IDENT){
                parser_name_table(pnode_tb, _table, exc);
                Node * palias = pnode->getChild(1);
                if(palias){
                    _table._alias = palias->terminalToken_.str;
                }
            }
            if(!_table._tb.empty()) {
                _table._node = pnode_tb;
            }
        } break;
        default:
            break;
    }
}
void EMS4HUpdateAst::ParserColumns(Node * pnode, S4HException & exc){
    switch (pnode->nodeType_) {
        case E_UPDATE_ELEM_LIST: {
            std::vector<Node * > list;
            Node::ToList(pnode, list);
            for(size_t i = 0; i < list.size();++i){
                ParserColumns( list[i], exc);
            }
        }break;
        case E_OP_EQ: {
            Node * pcol = pnode->getChild(0);
            Node * pval = pnode->getChild(1);
            if(pcol && pval) {
                if(pcol->nodeType_ == E_OP_NAME_FIELD){
                    EMColumnInfo info(pnode);
                    parser_name_field(pcol, info, exc);
                    if( pval->nodeType_ == E_QUESTIONMARK) {
    
                        EMUpdateColInfo * updateinfo = new EMUpdateColInfo() ;
                        updateinfo->_info = info;
                        updateinfo->_questmarkid = pval->questmarkid_;
                        updateinfo->_col = info._field;
                        _fields.push_back(updateinfo);
                    } else {
                        
                        EMUpdateColInfo * updateinfo = new EMUpdateColInfo() ;
                        updateinfo->_info = info;
                        updateinfo->_questmarkid = 0;
                        updateinfo->_col = info._field;
                        _fields.push_back(updateinfo);
                    }
                }
            }

        } break; // 0 , 1
//        case E_OP_NAME_FIELD: {
//
//
//        } break;
//        case E_QUESTIONMARK: {
//
//        } break;
        default:
            break;
    }
}


bool EMS4HUpdateAst::CollectParam(const int index, const std::string & strparam) {

    for(auto it: _fields) {
        if(it->_questmarkid == index  )
        {
            it->_UpdateData._type = SQLDBC_HOSTTYPE_ASCII;
            it->_UpdateData._Data._strData = strparam;
            it->_UpdateData._Data._strData.erase(it->_UpdateData._Data._strData.find_last_not_of(" ") + 1);  
            return true;
        }
    }
    return false;
}

bool EMS4HUpdateAst::CollectParam(const int index, int64_t param) 
{ 
     for(auto it: _fields) {
        if(it->_questmarkid == index  )
        {
            it->_UpdateData._type = SQLDBC_HOSTTYPE_INT8;
            it->_UpdateData._Data._int64Data = param; 
            return true;
        }
    }
    return false;
}
bool EMS4HUpdateAst::CollectParam(const int index, uint64_t param) 
{
    for(auto it: _fields) {
        if(it->_questmarkid == index  )
        {
            it->_UpdateData._type = SQLDBC_HOSTTYPE_UINT8;
            it->_UpdateData._Data._uint64Data = param; 
            return true;
        }
    }
    return false;
}

bool EMS4HUpdateAst::CheckSupport() {
    if(_fields.size() <= 0) return  false;
    return true;
}

void EMS4HUpdateAst::ResetSQL() {
    for(auto it: _fields) {
        if( it->_ismaskparam) {
            it->_ismaskparam = false;
            *(it->_info._node) = it->_old;
        }
    }
    _whereclause.ResetNode();
}

std::string EMS4HUpdateAst::Output() const {
    for(auto it: _fields)  {
        if( it->_ismaskparam) {
            *(it->_info._node) = it->_new;
        }
    }
    return EMS4HAst::Output();
}

void EMS4HUpdateAst::ParseWhereClause(Node * pnode, S4HException & /*exc*/) {
    if(pnode)
    {
        if(pnode->nodeType_ == E_UPDATE) 
        {
            _whereclause._root = pnode;
            _whereclause._node = pnode->getChild(7);// key = 7 is whereclause
        }
    }
}

bool EMS4HUpdateAst::RewriteSQLForDenyUpdate(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx) {
    bool bret = false;
    S4HException exc;
    std::vector<EMTableInfo*> vecTables;
    vecTables.push_back(&_table);

    //PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, vecTables, get_action_from_ast_type(E_STMT_TYPE_UPDATE), exc);/* becase filter arealdy queried pc*/
    DenyUpdateTableMap *pmap_dup = PolicyManager::Instance()->GetUpdateControlInfo();
    auto it_dup = pmap_dup->find(_table._tb);
    DenyUpdateItemMap  * p_dupitem = NULL;
    if(it_dup != pmap_dup->end()){
        p_dupitem = it_dup->second;
    }

    PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, vecTables, get_action_from_ast_type(E_STMT_TYPE_SELECT), exc);/*todo multi thread*/
    const MaskConditionMap *pmap_mask = PolicyManager::Instance()->GetMaskInfo();
    auto it_mask = pmap_mask->find(_table._tb);
    const MaskItemMap *p_maskitem = NULL;
    if(it_mask != pmap_mask->end()){
        p_maskitem = it_mask->second;
    } else {
        bret = RewriteSQLV2_UpdateToOtherTable(connection, pUserInfo, presinfo, pctx); /*todo*/
    }

    if(p_maskitem || p_dupitem ){
        for (size_t i = 0; i < _fields.size();++i)
        {
            auto p_upcol = _fields[i];
            if(p_upcol->_ismaskparam) {
                continue;
            }
            //field = value, check field.
            std::string col = p_upcol->_col;
            std::string cond_mask;
            if(p_maskitem){
                auto it_item = p_maskitem->find( col );
                if (it_item != p_maskitem->end())
                {
                    if (!it_item->second->_symbols.empty())
                    {
                        if (it_item->second->_condition.empty()) {
                            cond_mask="(1=1)";
                        }
                        else{
                            cond_mask = it_item->second->_condition;
                        }    
                    }
                }
            }

            std::string cond_updateDeny;
            if(p_dupitem){
                auto it_item2 = p_dupitem->find(col);
                if(it_item2 != p_dupitem->end()){
                    if(it_item2->second->_condition.empty()){
                        cond_updateDeny= "(1=1)";
                    } else {
                        cond_updateDeny = it_item2->second->_condition;
                    }
                }
            }

            std::string cond_all;
            if (cond_mask.empty() && cond_updateDeny.empty()){
                continue;
            }
            else{
                cond_all = cond_mask;
                if (!cond_all.empty()){
                    if (!cond_updateDeny.empty()) {
                        cond_all += "OR" + cond_updateDeny;
                    } 
                }
                else{
                    cond_all = cond_updateDeny;
                }       
            }
            
            
            {
                std::string s_col,s_value;
                if( p_upcol->_info._node->GetType() == E_OP_EQ ){
                    Node * pcol = p_upcol->_info._node->getChild(0);
                    if(pcol){
                        s_col = pcol->Serialize();
                    }
                    Node * pval = p_upcol->_info._node->getChild(1);
                    if(pval){
                        s_value = pval->Serialize();
                    }
                }
                if(s_col.empty() || s_value.empty()) {
                    theLog->WriteLog(log_info, "EMS4HUpdateAst::RewriteSQLForDenyUpdate continue for s_col.empty() || s_value.empty()");
                    continue;
                }
                //here we find the mask data
                create_old_node(p_upcol->_old, p_upcol->_info);
                //
                create_new_node(p_upcol->_new, p_upcol->_info, s_value, cond_all);
                p_upcol->_ismaskparam = true;
                bret = true;
                theLog->WriteLog(log_info, "EMS4HUpdateAst::RewriteSQLForDenyUpdate, matches mask symbol,Index:%d, filed:%s",
                                 p_upcol->_questmarkid, p_upcol->_col.c_str() );
            }
        }
    } 

    return bret;
}


bool EMS4HUpdateAst::RewriteSQLV2_UpdateToOtherTable(void *connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo, const EnforcerCtxImpl *pCtx)
{
    bool bRewriteForDenyMask = false;
    do
    {
        //check if it have table alias item for this table
        const TableAliasItem *pTableAlias = TableAliasMgr::Instance().GetAliasItemByAliasTable(_table._tb);
        if (NULL == pTableAlias)
        {
            theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable break for didn't find table alias. table:%s", _table._tb.c_str());
            break;
        }

        //here we find table alias, create EMTableInfo struct for original table
        theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable find table alias. original:%s, alias:%s", pTableAlias->OriginalTable().c_str(), pTableAlias->AliasTable().c_str());
        EMTableInfo originalTable(_table);
        originalTable._tb = pTableAlias->OriginalTable();
        originalTable._alias = "\"" + pTableAlias->OriginalTable() + "\"";
  
        //query policy with the original table
       S4HException exc;
       std::vector<EMTableInfo*> vecTables;
       vecTables.push_back(&originalTable);
       bool bQueryPC = PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, vecTables, EM_ACTION_SELECT, exc);
       if (!bQueryPC)  {
            theLog->WriteLog(log_error, "EMS4HUpdateAst::UpdateToOtherTable break for Query pc failed.");
            break;
       }
 
       //get mask obligation about the original table
       const MaskConditionMap *pmap_mask = PolicyManager::Instance()->GetMaskInfo();
       auto itMask = pmap_mask->find(originalTable._tb);
       if (itMask == pmap_mask->end()) {
           theLog->WriteLog(log_error, "EMS4HUpdateAst::UpdateToOtherTable break for no mask obligation. original table:%s.", originalTable._tb.c_str() );
           break;
       }

       const MaskItemMap *pMaskItemMap = itMask->second;
       if (NULL == pMaskItemMap)
       {
           theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable break for no mask item found. original table:%s.", originalTable._tb.c_str() );
           break;
       }


       //for each UPDATE filed. check if there is a mask obligation
       std::map<std::string, EMUpdateColInfo*> mapColumnHaveMaskObligation; //original field-> update field
       std::map<std::string, MaskItem*> mapColumnMaskInfo; // original field--> mask item  
       for (auto itField = _fields.begin(); itField != _fields.end(); itField++)
       {
           EMUpdateColInfo *pUpdateColInfo = *(itField);

           //get the original filed
           std::string original_field = pTableAlias->GetOriginalField(pUpdateColInfo->_col);
           if (original_field.empty())
           {
                theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable continue for the original field is empty for %s, original_field:%s", pUpdateColInfo->_col.c_str(),  original_field.c_str() );
                continue; 
           }
           
           //get mask obligation for the original field
           auto itMaskItem = pMaskItemMap->find(original_field);
           if (itMaskItem == pMaskItemMap->end())
           {
               theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable continue for the original field have no mask obligation: %s, original_field:%s", pUpdateColInfo->_col.c_str(), original_field.c_str() );
               continue; 
           }
           
            if (itMaskItem->second->_symbols.empty())
            {
                theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable continue for mask symbol is empty filed:%s, original_field:%s", pUpdateColInfo->_col.c_str(), original_field.c_str() );
                continue;
            }

            //here there is mask obligation for that field.
            mapColumnHaveMaskObligation[original_field] = pUpdateColInfo;
            mapColumnMaskInfo[original_field] = itMaskItem->second;

            theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, find mask info: original field:%s, alias field:%s",  original_field.c_str() , pUpdateColInfo->_col.c_str()); 
       }

       if (mapColumnMaskInfo.size()==0) {
           theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, break for mapColumnMaskInfo is 0");
           break;
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
           
           strSelectOriginalRecordSQL += "(CASE WHEN " + strMaskCodition +" THEN '" +  MASK_HINT_MASKED  +"' ELSE  '" +  MASK_HINT_NOMASKED + "' END) as MASK_HINT ,";
       }

       if (strSelectOriginalRecordSQL[strSelectOriginalRecordSQL.length()-1]==',')
       {
            strSelectOriginalRecordSQL = strSelectOriginalRecordSQL.substr(0, strSelectOriginalRecordSQL.length()-1 );
       }

       strSelectOriginalRecordSQL +=" FROM " + pTableAlias->OriginalTable() + " WHERE "; 

       //construct "update relation"
       std::string strUpdateRelation;
       std::list<int> listQuestionParamPosition;
       ConstructUpdateRelation(pTableAlias->UpdateRelation(), strUpdateRelation, listQuestionParamPosition);
       
       strSelectOriginalRecordSQL += strUpdateRelation;

       theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, select sql:%s", strSelectOriginalRecordSQL.c_str() );
       
       //execute select SQL
       #if 1
       do
       {
            auto hdb_connection = (SQLDBC::SQLDBC_Connection*)connection;
            std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement, void(*)(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt)> 
                                            hdb_stmt( SQLDBC_Connection_createPreparedStatement_Old(hdb_connection), DeletePraparedStatement );
            if (hdb_stmt==NULL){         
                theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, create prepared statement failed." );
                break;
            }
            

           SQLDBC_Retcode rc = prepare_3_old(hdb_stmt.get(), strSelectOriginalRecordSQL.c_str(), strSelectOriginalRecordSQL.length(), SQLDBC_StringEncodingType::Ascii);
           if (SQLDBC_OK != rc) {
               theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call prepare_3_old failed:%s", SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
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
                       theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call bind_parameter_old failed. index:%d, msg:%s", nParamIndex, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_stmt.get())) );
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
                itColumnHaveMaskOB != mapColumnHaveMaskObligation.end(); itColumnHaveMaskOB++, nResultIndex++) {

               EMUpdateColInfo *pUpdateColumnInfo = itColumnHaveMaskOB->second;

               //get result
               char szMaskHint[64] = { 0 };
               rc = SQLDBC_ResultSet_getObject_Old(result_set, nResultIndex, SQLDBC_HOSTTYPE_ASCII, szMaskHint, nullptr, sizeof(szMaskHint), SQLDBC_TRUE);
               if (SQLDBC_OK!=rc) {
                   theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, call getObject failed.index:%d, rc=%d, msg:%s",nResultIndex,  rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(result_set)) );
                   continue;
               }
                
               theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, index:%d, mask hint:%s", nResultIndex, szMaskHint );

               if (CommonFun::StrCaseCmp(szMaskHint, MASK_HINT_MASKED) == 0)
               {
                   theLog->WriteLog(log_info, "EMS4HUpdateAst::UpdateToOtherTable, get mask column:%s", pUpdateColumnInfo->_col.c_str() );

                   create_old_node(pUpdateColumnInfo->_old, pUpdateColumnInfo->_info);
                   create_new_node(pUpdateColumnInfo->_new, pUpdateColumnInfo->_info);
                   pUpdateColumnInfo->_ismaskparam = true;
                   bRewriteForDenyMask = true;
               }
           }

       } while (false);

       #endif 

    } while (false);

    return bRewriteForDenyMask;
}

void EMS4HUpdateAst::ConstructUpdateRelation(const std::string& strRelationTemplete, std::string& strRelation,
                                             std::list<int>& listQuestionParamPosition)
{
    strRelation = strRelationTemplete + "  ";
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
            GetWhereClauseConditionRightParamId(strColumnName, nQuestionParamPosition);
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

bool EMS4HUpdateAst::RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo){
    S4HException exc;
    std::vector<EMTableInfo*>  pctables;
    pctables.push_back(&_table);
    PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, pctables, get_action_from_ast_type(E_STMT_TYPE_UPDATE),exc);
    std::string cond =  PolicyManager::Instance()->GetFilterCondition(connection);

    bool bret = false;  //false means didn't modify sql
    if(!cond.empty()){
        bret = _whereclause.EvaluationSQL(cond);
        if(!bret) {
            theLog->WriteLog(log_error, "RewriteWithFilterCondition failed. EvaluationSQL failed.");
        }
    }
    

    return bret;
}

bool EMS4HUpdateAst::RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * /*pctx*/){
    return RewriteWithFilterCondition(connection,pUserInfo, presinfo);
}


bool EMS4HUpdateAst::GetWhereClauseConditionRightParamId(const std::string & column, unsigned int & markid) const{
    return  _whereclause.GetConditionRightParamId(column, markid);
}
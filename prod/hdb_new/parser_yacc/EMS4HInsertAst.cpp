//
// Created by jeff on 2020/3/10.
//

#include "EMS4HInsertAst.h"
#include "PolicyExport.h"
#include "PolicyManager.h"
#include "TableMetaDataImpl.h"
#include "log.h"
#include "IQueryCloudAZ.h"
#include "TableDataCache.h"
#include "EMS4HDeleteAst.h"
#include "EnforcerCtxImpl.h"

void EMS4HInsertAst::Parser(Node * proot, S4HException & exc) {
    if(!proot) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "insert statement root is nullptr");
        return;
    }
    // key = 0 is tb_ref
    Node * ptb = proot->getChild(0);
    if(ptb == nullptr) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "insert statement table is nullptr");
        return;
    }
    if(ptb->nodeType_ == E_TABLE_IDENT) {
        parser_name_table(ptb, _table, exc);
        _table._node = ptb;
    }
    //key = 1 is E_INSERT_COLUMNS_AND_SOURCE
}

bool EMS4HInsertAst::CheckSupport() {
    if(_table._node == NULL) return false;
    return true;
}

bool EMS4HInsertAst::InterceptSQL(void* /*connection*/, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo) {

#ifdef QUERY_POLICY_LOCAL_CC
    bool bdeny = false;
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
    bool bmatch = MatchPolicy(user_dic, res_dic, EM_ACTION_INSERT,  emexc);
    if(!bmatch) {
        theLog->WriteLog(log_info, "insert stmt InterceptSQL match policy return false.");
        return  false;
    }
    bool  bhav = GetMatchDenyValue(bdeny, emexc);
    if(!bhav){
        theLog->WriteLog(log_info, "insert stmt InterceptSQL no policy match.");
        return  false;
    }

    return bdeny;
#else
    S4HException exc;
    std::vector<EMTableInfo*>  pctables;
    pctables.push_back(&_table);
    std::string action = EM_ACTION_INSERT;
    PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, pctables, action,exc);
    return PolicyManager::Instance()->GetPCResult() == PolicyEnforcement::Deny;
#endif
}

int EMS4HInsertAst::GetValueParamId(const std::string & field) {
    assert(_node);
    Node * nodet = _node;
    if(nodet->GetType() == E_STMT_LIST){
        nodet = nodet->getChild(0);
    }
    if(nodet == NULL){
        return -4;
    }
    Node * sour = nodet->getChild(E_INSERT_INSERT_COLUMNS_AND_SOURCE);
    //Node * sour = _node->getChild(E_INSERT_INSERT_COLUMNS_AND_SOURCE);
    assert(sour);
    //parser column
    Node * cols_p = sour->getChild(0);
    if(cols_p){
        Node * vals = sour->getChild(1);
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

int EMS4HInsertAst::GetValueParamId(int index) {
    assert(_node);
    Node * nodet = _node;
    if(nodet->GetType() == E_STMT_LIST){
        nodet = nodet->getChild(0);
    }
    if(nodet == NULL){
        return -4;
    }
    Node * sour = nodet->getChild(E_INSERT_INSERT_COLUMNS_AND_SOURCE);
    assert(sour);
    //parser column
    Node *vals = sour->getChild(1);
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

bool EMS4HInsertAst::GetInsertColumnParamId( const std::string & column,  void *conn, unsigned int & markid) {
    int ret = GetValueParamId(column);
    if( ret >= 0 ){
        markid = ret;
        return true;
    } else {
        if(ret == -1){ // no column def
#ifdef JEFF_TEST
            std::vector<std::u16string> cols = {u"col1",u"col2",u"col3"};
#else
            const std::string& strtb = GetTable()->_tb;
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

void EMS4HInsertAst::RestoreFromDeleteData(EnforcerCtxImpl *pctx, void *connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo)
{
    S4HException ex;
    //get metadata, and get key fields
    PolicyManager* policyMgr= PolicyManager::Instance();
    const ITableMetaData* pMetaData = policyMgr->GetTableMetaData(_table._tb, _table._type, connection, ex);
    if(NULL==pMetaData){
        return;
    }
    const std::vector<std::string> vecField = pMetaData->GetAllKeyColumns();

    //get correspond parameter name
    std::vector<unsigned int> vecParamId;
    for(auto itField = vecField.begin(); itField<vecField.end(); ++itField){
        unsigned int markid = 0;
        bool bGetParam= GetInsertColumnParamId(*itField, pctx, markid);
        if(!bGetParam){
            theLog->WriteLog(log_error, "EMS4HInsertAst::RestoreFromDeleteData, get param failed for %s", itField->c_str() );
            return;
        }
        vecParamId.push_back(markid);
        theLog->WriteLog(log_info, "EMS4HInsertAst::RestoreFromDeleteData, get param for %s, paramId:%d", itField->c_str(), markid );
    }

    //process for each row of data
    auto& cachedDeleteData = EMS4HDeleteAst::GetCachedData();
    unsigned int nArraySize = pctx->GetBatchSize();
    for (int iRow = 0; iRow < nArraySize; iRow++){
        //get correspond parameter values
        std::vector<std::string> paramValuesForKey;
        for(auto itId=vecParamId.begin(); itId<vecParamId.end(); ++ itId){
            std::string strParamValue;
            pctx->GetParamValueByPosition(iRow, *itId, strParamValue);
            paramValuesForKey.push_back(strParamValue);
        }

        const std::string strRowKey = TableDataCache::ConstructRowKey(_table._tb, paramValuesForKey);
        //theLog->WriteLog(log_info, "EMS4HInsertAst::RestoreFromDeleteData, construct key:%s", strRowKey.c_str() );

        auto rowData = cachedDeleteData.GetRowData(strRowKey, true);
        if (NULL!=rowData) {
            theLog->WriteLog(log_info, "EMS4HInsertAst::RestoreFromDeleteData, get cached delete data from key:%s", strRowKey.c_str() );

            //replace eache masked data
            for (auto itMaskedField=rowData->_maskedFields.begin(); itMaskedField<rowData->_maskedFields.end(); itMaskedField++){
                unsigned int markid = 0;
                bool bGetParam= GetInsertColumnParamId(*itMaskedField, pctx, markid);
                if(bGetParam){
                    std::string strValue = rowData->_data[*itMaskedField];
                    pctx->ReplaceParamValueByPosition(iRow, markid, strValue);
                }
            }
        }
        else{
            theLog->WriteLog(log_error, "EMS4HInsertAst::RestoreFromDeleteData, not get data from key:%s", strRowKey.c_str() );
        }
    }
}
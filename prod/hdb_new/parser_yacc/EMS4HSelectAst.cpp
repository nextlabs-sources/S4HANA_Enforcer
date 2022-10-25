//
// Created by jeff on 2020/2/20.
//

#include "EMS4HSelectAst.h"
#include <set>
#include <PolicyManager.h>
#include "MaskStrategyImpl.h"
#include "tool.h"
#include "log.h"
#include "SqlDbcUserInfoMgr.h"
#include "PolicyExport.h"
#include "EMS4HMutliSelectAst.h"
#include "ViewManager.h"
#include <assert.h>

EMS4HSelectAst::~EMS4HSelectAst(){
    for(auto it:_fields) delete(it);
    _fields.clear();
    for(auto it:_tables) delete(it);
    _tables.clear();
    for(auto it:_groupby_list) delete(it);
    _groupby_list.clear();
}


void EMS4HSelectAst::Parser(Node * proot, S4HException & exc) {
    if(!proot) return;
    switch (proot->nodeType_) {
        case E_STMT_LIST: {
            Parser(proot->getChild(0), exc);
        } break;
        case E_DIRECT_SELECT: {
            Node * ptemp = proot->getChild(1);
            Parser(ptemp, exc);
        } break;
        case E_SELECT: {
            if(NULL != proot->getChild(6)){
                // union except ... set_op
                Parser(proot->getChild(8), exc);

            } else {
                //need match table_name
                Node * pcolumns = proot->getChild(1);
                Node * pfrom = proot->getChild(2);
                Node * pgroup = proot->getChild(4);
                ParserTable(pfrom, exc);
                if (NULL==pcolumns){
                    exc.SetError(0, "columns == null");
                }
                else{
                    _select_list_node = pcolumns;
                    ParserColumns(pcolumns, exc);
                    ParseWhereClause(proot,exc);
                }
                ParserGroupBy(pgroup, exc);
            }
        } break;
    }
}

void EMS4HSelectAst::ParserColumns(Node* pnode, S4HException & exc) {
    switch (pnode->nodeType_) {
        case E_SELECT_EXPR_LIST: {
            std::vector<Node * > list;
            Node::ToList(pnode, list);
            for(size_t i = 0; i < list.size();++i){
                ParserColumns( list[i], exc);
            }
        } break;
        case E_PROJECT_STRING: {
            ParserColumns( pnode->getChild(0), exc); // E_STAR / ...
        } break;
        case E_OP_NAME_FIELD: {
            EMColumnInfo *info = new EMColumnInfo(pnode);
            parser_name_field( pnode,  *info, exc);
            info->_alias = false;
            
            if(!info->_field.empty()) {
                _fields.push_back(info);
            } else {
                delete(info);
            }
        } break;
        case E_ALIAS: {//aaa AS bbb

            //parser_alias( pnode, *info, exc);
            EMColumnInfo *info = NULL;

            Node * pchild = pnode->getChild(0);
            if(pchild == nullptr) return;
            if (pchild->nodeType_ == E_OP_NAME_FIELD) {
                info = new EMColumnInfo(pnode);
                parser_name_field(pchild, *info, exc);
                if(!info->_field.empty()) {
                    info->_alias = true;
                    _fields.push_back(info);
                } else {
                    delete(info);
                }
            } else if(pchild->nodeType_ == E_FUN_CALL) {
                info = new EMFuncColumnInfo(pnode);
                bool bp = parser_function(pchild, *(EMFuncColumnInfo*)info, exc);
                if(bp){
                    info->_alias = true;
                    ((EMFuncColumnInfo*)info)->_alias_name = pnode->getChild(1)->serialize();
                    _fields.push_back(info);
                } else {
                    delete(info);
                }
            }
        } break;
        case E_STAR: {
            EMColumnInfo *info = new EMColumnInfo(pnode);
            info->_field = "*";
            _fields.push_back(info);
            return;
        }
        case E_FUN_CALL:{
            EMFuncColumnInfo * info = new EMFuncColumnInfo(pnode);
            bool bp = parser_function(pnode, *info, exc);
            if(bp){
                _fields.push_back(info);
            } else {
                delete(info);
            }
        } break;
//if input this case then does not to mask this children node
//        case E_OP_COLLATE:
//        case E_OP_CNN:
//        case E_OP_ADD:
//        case E_OP_MUL:
//        case E_OP_POW:
//        case E_OP_POS:
//        case E_OP_NEG:{
//        } break;
//            //factor 4
//        case E_STRING:
//        case E_BOOL:
//        case E_NULL:
//        case E_INT:
//        case E_DOUBLE:
//        case E_QUESTIONMARK: { //expr_const:
//
//        } break;
//        case E_SELECT_WITH_PARENS: {} break;//row_subquery
//        case E_EXPR_LIST_WITH_PARENS: {} break;//factor4:  	'(' row_expr_list ')'
//        case E_CASE: {} break;//factor4:  	case_expr
//        case E_FUN_CALL: {} break;//factor4:  	func_expr
//column-ref
//        case E_IDENTIFIER: {
//        } break;

        default: break;
    }
    return ;
}

void EMS4HSelectAst::ParserTable(Node* pnode, S4HException & exc){
    //locate from  table  name and node
    if (pnode == NULL) return;
    switch (pnode->nodeType_) {
        case E_FROM_CLAUSE: {
            ParserTable(pnode->getChild(0), exc);
        } break;
        case E_FROM_LIST: {
            std::vector<Node * > list;
            Node::ToList(pnode, list);
            for(size_t i = 0; i < list.size();++i){
                ParserTable( list[i], exc);
            }
        }break;
        case E_SELECT_WITH_PARENS:{
            _support_select_star = false;
        } break;
        case E_JOINED_TABLE_WITH_PARENS:{
            ParserTable(Node::remove_parens(pnode), exc);
        }break;
        case E_ALIAS: {
            Node * pnode_alias = pnode->getChild(0);
            Node * palias = pnode->getChild(1);
            assert(pnode_alias);
            if( pnode_alias->GetType() == E_TABLE_IDENT){
                EMTableInfo *info = new EMTableInfo(pnode_alias);
                parser_name_table(pnode_alias, *info, exc);
                if(_parent && ((EMS4HMutliSelectAst*)_parent)->FindCte(info->_tb)) { //cte dont care , cte = with 'cte' as (subquery)
                    delete(info);
                    info = NULL;
                    break;
                }
                if(palias){
                    info->_alias = palias->terminalToken_.str;
                }
                _tables.push_back(info);
            } else if(pnode_alias->GetType() == E_FUN_CALL) {
                //child 0 is name
                Node * funcname = pnode_alias->getChild(0);
                // child 1 is parameter, no used now
                // Node * parameter = pnode_alias->getChild(1);

                EMTableInfo *info = new EMTableInfo(funcname);
                info->_type = EMTABLE_FUNC;
                parser_name_table(funcname, *info, exc);
                if(_parent && ((EMS4HMutliSelectAst*)_parent)->FindCte(info->_tb)) { //cte dont care , cte = with 'cte' as (subquery)
                    delete(info);
                    break;
                }
                if(palias){
                    info->_alias = palias->terminalToken_.str;
                }
                _tables.push_back(info);
            }

        }break;
        case E_TABLE_IDENT: {
            EMTableInfo *info = new EMTableInfo(pnode);
            parser_name_table(pnode, *info, exc);
            if(_parent && ((EMS4HMutliSelectAst*)_parent)->FindCte(info->_tb)) { //cte dont care , cte = with 'cte' as (subquery)
                delete(info);
                info = NULL;
                break;
            }
            _tables.push_back(info);
        }break;
        case E_STRING: {
        }break;
        case E_IDENTIFIER: {
        }break;
        case E_JOINED_TABLE:
        {
            //table_reference // table_primary(E_ALIAS)/joined_table(E_JOINED_TABLE)
            ParserTable(pnode->getChild(1), exc); // join table_reference
            ParserTable(pnode->getChild(2), exc); //join table_reference
        }break;
        case E_SELECT: {
            _support_select_star = false;
            if(_parent){
                _parent->Parser(pnode, exc);
            }
        } break;
        case E_CONSTRUCT_FROM_TABLE:{
            _support_select_star = false;
            /*
            EMConstructTable * tb = new EMConstructTable();
            tb->Parser(pnode, exc);
            */
        } break;
        case E_FUN_CALL:{
            _support_select_star = false;
            //child 0 is name
            Node * funcname = pnode->getChild(0);
            // child 1 is parameter, no used now
            // Node * parameter = pnode->getChild(1);

            EMTableInfo *info = new EMTableInfo(funcname);
            info->_type = EMTABLE_FUNC;
            parser_name_table(funcname, *info, exc);
            if(_parent && ((EMS4HMutliSelectAst*)_parent)->FindCte(info->_tb)) { //cte dont care , cte = with 'cte' as (subquery)
                delete(info);
                break;
            }
            _tables.push_back(info);
        } break;

        default:
            break;
    }
}


void EMS4HSelectAst::ParserGroupBy(Node* node, S4HException & exc){
    if(node==NULL) return;
    Node * grouping_element_list = node->getChild(0);
    if(grouping_element_list==NULL) return;
    std::vector<Node * > list;
    Node::ToList(grouping_element_list, list);
    // grouping_element
    for(auto it: list) {
        if(it==NULL) continue;
        if(it->GetType() == E_OP_NAME_FIELD){
            //get group by fields info
            EMColumnInfo * info = new EMColumnInfo(it);
            parser_name_field(it, *info, exc);
            if(!info->_field.empty()) {
                _groupby_list.push_back(info);
            } else {
                delete(info);
            }
        }
    }

}

bool EMS4HSelectAst::CheckSupport() {
    if(_fields.size() <= 0) return false;
    if(_tables.size() <= 0) return false;
    if(_parent){

    }
    return true;
}

void EMS4HSelectAst::ResetSQL() {
    //reset node
    for(unsigned int i = 0 ; i < _fields.size(); ++i) {
        reset_columninfo(_fields[i]);
    }
    _whereclause.ResetNode();

    //reset table info
    for(std::vector<EMTableInfo*>::iterator itTable=_tables.begin(); itTable!=_tables.end(); itTable++){
        (*itTable)->_viewsource = NULL;
    }
}

bool get_valid_condition_fields( std::set<std::u16string> & settb, const ITableMetaData * pmeta, IMaskStrategy * pmask) {
    std::string str_mask = pmask->GetMaskedColumn();
    bool bhav = pmeta->HasColumn(str_mask);
    if(!bhav){
        theLog->WriteLog(log_error, "get_valid_condition_fields return false for HasColumn=false, col=%s", str_mask.c_str() );
        return false;
        //settb.insert(str_mask);
    }

    pmask->SetMaskSymbolsType(pmeta->GetColumnDataType(str_mask));

    std::map<std::string, int> col2op = pmask->GetConditionColumnAndOp();
    for(auto it:col2op) {
        if(!pmeta->HasColumn(it.first)) continue;
        if(pmeta->IsColumnCompatibleWithString(it.first) && it.second < STRING_OP ) continue;
        settb.insert(CommonFun::FromUTF8(it.first));
        auto columnType = pmeta->GetColumnDataType(it.first);
        theLog->WriteLog(log_info, "EMS4HSelectAst|SetMaskConditionColumnType(%s, %d)", it.first.c_str(), (int)columnType);
        pmask->SetMaskConditionColumnType(it.first, columnType);
    }
//        if(col2op.size() > 0 && settb.size() <= 0) {
//            return false;
//        }
    return true;
}

bool get_valid_condition_fieldsV2( std::set<std::u16string> & settb, const ITableMetaData * pmeta, MaskItem * pmask) {

    bool bhav = pmeta->HasColumn(pmask->_field);
    if(!bhav){
        theLog->WriteLog(log_error, "get_valid_condition_fields return false for HasColumn=false, col=%s", pmask->_field.c_str() );
        return false;
        //settb.insert(str_mask);
    }
    pmask->_field_type = (pmeta->GetColumnDataType(pmask->_field));

    for(auto it: pmask->_cond_fields){
        bool bhavTemp = pmeta->HasColumn(it);
        if(!bhavTemp){
            pmask->_condition.clear();/*if condition field is invalid we set condition empty*/
        } else {
            /*todo pmask condition field check type*/
            settb.insert(CommonFun::FromUTF8(it));
        }
    }
    return true;
}

//void  get_select_table_name(std::set<std::string> & tbs, const std::vector<EMColumnInfo*> & vec_columninfo) {
//    for(auto info:vec_columninfo) {
//        if(!info->_tb.empty()) tbs.insert(info->_tb);
//    }
//}

void collect_view_tables(std::vector<EMTableInfo*> & pctables, const std::vector<EMTableInfo*> & tables) {
    for(auto it:tables) {
        if(it->_viewsource != NULL) {

            const std::set<std::string>* pviewtables = it->_viewsource->GetSourceTables();
            for(auto tb : *pviewtables){
                EMTableInfo * temp = new EMTableInfo();
                temp->_viewTableInfo = it;

                size_t ipos = tb.find(".");
                if(ipos != std::string::npos){// if itSourceTable = schema.table
                    temp->_tb = tb.substr(ipos+1);
                    temp->_schema = tb.substr(0,ipos);
                } else {
                    temp->_tb = tb;
                }

                temp->_alias = it->_alias;
                if (temp->_alias.empty() && it->_node){
                    temp->_alias = it->_node->serialize();
                }

                pctables.push_back(temp);
            }
        } else {
            EMTableInfo * temp = new EMTableInfo();
            temp->_tb = it->_tb;
            temp->_schema = it->_schema;
            temp->_db = it->_db;
            temp->_node = it->_node;

            temp->_alias = it->_alias;
            if (temp->_alias.empty() && it->_node){
                temp->_alias = it->_node->serialize();
            }

            pctables.push_back(temp);
        }
    }
}

void release_view_tables(std::vector<EMTableInfo*> &  pctables){
    for(auto it: pctables) {
        delete (it);
    }
    pctables.clear();
}

bool EMS4HSelectAst::RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx) {
    S4HException exc;
    std::vector<EMTableInfo*>  pctables;
    collect_view_tables(pctables, _tables);

    ClearMatchedMaskItem();

    std::string action = EM_ACTION_SELECT;
    PolicyManager::Instance()->QueryPC_Enforcer(pUserInfo, presinfo, pctables,action, exc);/*todo multi thread*/
    bool bret1 = RewriteWithMaskConditionMap(connection, pUserInfo, presinfo, pctx);
    bool bret2 = RewriteWithFilterCondition(connection, pUserInfo, presinfo);
    release_view_tables(pctables);
    return bret1||bret2;
}


bool EMS4HSelectAst::RewriteWithMaskConditionMap(void* connection, const NxlDBCUserInfo* /*pUserInfo*/, const NxlDBCResInfo * /*presinfo*/, EnforcerCtxImpl * pctx){
    bool bret = false;
 
    const MaskConditionMap * pmap_mask = PolicyManager::Instance()->GetMaskInfo();

    if((NULL!=pmap_mask) && (!pmap_mask->empty()) ){
        theLog->WriteLog(log_info,  std::bind(&PolicyManager::PrintMaskConditionMap, PolicyManager::Instance(), *pmap_mask));
        S4HException exc;
        bret = mask_fields_v5(this, connection, pmap_mask, pctx, exc);
        if ((exc._code != 0 || false == bret) && (!exc._detail.empty())) {
            theLog->WriteLog(log_error, "RewriteWithMaskConditionMap:mask_fields_v5 return false at GetPolicyMaskStrategys, detail=%s",
                         exc._detail.c_str());
        }
    }
   
    return bret;
}
/*
bool EMS4HSelectAst::RewriteWithMaskStrategy(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo) {
    bool bret = false;

    S4HException exc;
    const std::vector<IMaskStrategy *> *mask_strategys = PolicyManager::Instance()->GetPolicyMaskStrategys(pUserInfo,exc);

    if (exc._code != 0 || NULL == mask_strategys) {
        theLog->WriteLog(log_error, "RewriteWithMaskStrategy return false at GetPolicyMaskStrategys, detail=%s",
                         exc._detail.c_str());
        return bret;
    }

    if (_isstar) {
        if(_tables.size() > 1) {
            theLog->WriteLog(log_error, "RewriteWithMaskStrategy only support one table");
            return false;
        }
        std::string stable = _tables[0]->_tb;

        std::set<std::u16string> vec_col;
        const ITableMetaData *ptb = PolicyManager::Instance()->GetTableMetaData(CommonFun::FromUTF8(stable), connection,
                                                                                exc);
        if (exc._code == S4H_INVALID_CONNECTION) {
            theLog->WriteLog(log_error, "RewriteWithMaskStrategy GetTableMetaData return with S4H_INVALID_CONNECTION");
            return bret;
        }
        //mask
        std::vector<std::u16string> vec = ptb->GetAllColumns();
        std::map<std::u16string, std::u16string> map_node;
        for (auto pmask: *mask_strategys) {

            if (!pmask->GetMaskedTable().empty()
                && CommonFun::StrCaseCmp(stable, pmask->GetMaskedTable()) != 0) {
                continue;
            }

            //add by jeff 20200402
            const std::set<std::string> & settables = pmask->GetMaskedTables();
            if(settables.size() > 0 ){
                if(settables.find("*") == settables.end() && settables.find(stable) == settables.end()) {
                    continue;
                }
            }


            //theLog->WriteLog(log_info, std::bind(&ITableMetaData::Print, ptb) );

            bool bhav = get_valid_condition_fields(vec_col, ptb, pmask);
            if (!bhav) {
                theLog->WriteLog(log_error, "RewriteWithMaskStrategy return for get_valid_condition_fields failed.");
                continue;
            }


            std::u16string str_mask = pmask->MakeExpressionV2(vec_col, u"", u"");
            if (str_mask.empty()) {
                continue;
            }

            std::u16string col = CommonFun::FromUTF8(pmask->GetMaskedColumn());
            map_node[col] = str_mask;

            //record masked info, 
           // PolicyManager::Instance()->InsertMaskInfo(pUserInfo, pmask->GetMaskedColumn(), pmask->GetMaskSymbols()); /
            bret = true;
        }

        std::string node_str;
        for (auto col:vec) {
            if (!node_str.empty()) node_str += ", ";
            auto it = map_node.find(col);
            if (it != map_node.end()) {
                node_str += CommonFun::ToUTF8(it->second);
            } else {
                node_str += "\"";
                node_str += CommonFun::ToUTF8(col);
                node_str += "\"";
            }
        }

        Node *pnode = Node::makeTerminalNode(E_SPECIAL_TERMINAL, node_str);
        *(_fields[0]->_node) = *pnode;
        delete (pnode);

    } else {
        for (auto pmask: *mask_strategys) {
            // printf("%s\n", pmask->Print().c_str());

            if (!pmask) continue;

            //mask column must be exist
            std::vector<EMColumnInfo *> vec_columninfo;
            {
                for (auto field:_fields) {
                    if (CommonFun::StrCaseCmp(field->_field, pmask->GetMaskedColumn()) == 0)
                        vec_columninfo.push_back(field);
                }

                if (vec_columninfo.size() <= 0) continue;
            }
            //get field modifier tbname ------bank.a
            std::set<std::string> tbs;
            {
                get_select_table_name(tbs, vec_columninfo);
            }
            // get from tables
            if (tbs.size() <= 0) {
                for (auto tb: _tables) {
                    tbs.insert(tb->_tb);
                }
            }

            std::set<std::u16string> vec_col;
            {
                for (auto tb:tbs) {
                    const ITableMetaData *ptb = PolicyManager::Instance()->GetTableMetaData(CommonFun::FromUTF8(tb),
                                                                                            connection, exc);
                    if (exc._code == S4H_INVALID_CONNECTION) {
                        continue;
                    }
                    if (pmask->GetMaskedTable().length() > 0
                        && CommonFun::StrCaseCmp(tb, pmask->GetMaskedTable()) != 0) {
                        continue;
                    }

                    //add by jeff 20200402
                    const std::set<std::string> & settables = pmask->GetMaskedTables();
                    if(settables.size() > 0 ){
                        if(settables.find("*") == settables.end() && settables.find(tb) == settables.end()) {
                            continue;
                        }
                    }
                    
                    bool bhav = get_valid_condition_fields(vec_col, ptb, pmask);
                    if (!bhav) continue;
                    if (vec_col.size() > 0) break;
                }
            }

            //mask
            for (auto info:vec_columninfo) {
                std::string str_alias;
                if (info->_alias) {
                    Node *pnode_alias = info->_node->getChild(1);
                    if (pnode_alias) {
                        str_alias = pnode_alias->text();
                    }
                }
                std::string stb = info->_tb;
                std::u16string str_mask = pmask->MakeExpressionV2(vec_col, CommonFun::FromUTF8(stb),
                                                                  CommonFun::FromUTF8(str_alias));
                if (str_mask.empty()) {
                    continue;
                }
                std::string node_str = CommonFun::ToUTF8(str_mask);
                Node *pnode = Node::makeTerminalNode(E_SPECIAL_TERMINAL, node_str);
                *(info->_node) = *pnode;
                delete (pnode);

                //record masked info
                PolicyManager::Instance()->InsertMaskInfo(pUserInfo, pmask->GetMaskedColumn(), pmask->GetMaskSymbols());
                bret = true;
            }
        }
    }
    return bret;
}
*/
bool EMS4HSelectAst::RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* /*pUserInfo*/, const NxlDBCResInfo * /*presinfo*/){

    bool bret = false;
#ifdef QUERY_POLICY_LOCAL_CC
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
    if(_tables.size() == 1) {
        if(_tables[0]->_db.empty()) {
            res_dic.insert(std::make_pair(RES_DATABASE_NAME, presinfo->_database));
        } else {
            res_dic.insert(std::make_pair(RES_DATABASE_NAME, _tables[0]->_db));
        }

        if(_tables[0]->_schema.empty()) {
            res_dic.insert(std::make_pair(RES_SCHEMA_NAME, presinfo->_schema));
        } else {
            res_dic.insert(std::make_pair(RES_SCHEMA_NAME, _tables[0]->_schema));
        }
        res_dic.insert(std::make_pair(RES_TABLE_NAME, _tables[0]->_tb));

    }
    //match policy
    S4HException emexc;
    bret = MatchPolicy(user_dic, res_dic, EM_ACTION_SELECT,  emexc);
    if(!bret) return  false;

    //get condition
    std::string cond1;
    GetMatchCondition(cond1, emexc);
    if(cond1.empty()) return false;

    //rewrite sql
    bret = _whereclause.EvaluationSQL(cond1);
    if(!bret) {
        theLog->WriteLog(log_error, "RewriteWithFilterCondition failed. EvaluationSQL failed.");
    }
#else
    std::string cond =  PolicyManager::Instance()->GetFilterCondition(connection);
    theLog->WriteLog(log_info, "RewriteWithFilterCondition  cond=%s", cond.c_str() );

    if (!cond.empty())
    {
        bret = _whereclause.EvaluationSQL(cond);
        if(!bret) {
            theLog->WriteLog(log_error, "RewriteWithFilterCondition failed. EvaluationSQL failed., cond=%s", cond.c_str() );
        }
    }
    
  
#endif
    return bret;
}



void EMS4HSelectAst::ParseWhereClause(Node * pnode, S4HException & /*exc*/) {
    assert(pnode);
    if(pnode->nodeType_ == E_SELECT) {
        _whereclause._root = pnode;
        _whereclause._node = pnode->getChild(3);
    }
}

bool EMS4HSelectAst::GetWhereClauseConditionRightParamId(const std::string & column, unsigned int & markid) const{
    return  _whereclause.GetConditionRightParamId(column, markid);
}


EMConstructTable::~EMConstructTable(){
    for(auto it:_columns){
        delete (it);
    }
}
void EMConstructTable::Parser(Node * node, S4HException & /*exc*/){
    if(NULL == node || NULL == node->getChild(0) || NULL == node->getChild(1)|| NULL == node->getChild(2)){
        return;
    }
    _questmarkid = node->getChild(0)->questmarkid_;
    _name = node->getChild(1)->terminalToken_.str;
    std::list<Node*> nodes;
    Node::ToList(node->getChild(2), nodes);
    for(auto itnode: nodes){
        EMColumnInfo * info = new EMColumnInfo();
        info->_node = itnode->getChild(0);
        info->_field = itnode->getChild(0)->terminalToken_.str;
        //PARSER COLUMN TYPE
        //itnode->getChild(1);
        _columns.push_back(info);
    }
}

void EMS4HSelectAst::CollectQuestMark_for_key_mask(Node * node,  std::vector<int> & questmarkids) {
    if (!node->isTerminalToken) {
        size_t  size_ = node->getChildrenCount();
        for (int i = 0 ; i < size_; ++i) {
            if(node->getChild(i) ){
                if(node->getChild(i) == _select_list_node){
                    continue;
                } else {
                    CollectQuestMark_for_key_mask(node->getChild(i), questmarkids);
                }
            }else{
                continue;
            }
            
        }
        if(node->GetType()  ==E_DIRECT_SELECT){ // delete for update
            if(node->getChild(E_DIRECT_SELECT_FOR_UPDATE)){
                delete(node->getChild(E_DIRECT_SELECT_FOR_UPDATE));
                node->setChild(E_DIRECT_SELECT_FOR_UPDATE, NULL);
            }
            if(node->getChild(E_DIRECT_HINT_CLAUSE)){ // delete with hint
                delete(node->getChild(E_DIRECT_HINT_CLAUSE));
                node->setChild(E_DIRECT_HINT_CLAUSE, NULL);
            }
        }else if(node->GetType()  ==E_SELECT) {
            if(!node->getChild(E_SELECT_DISTINCT)){/*todo subquery need continue*/
                Node * node_distinct = Node::makeTerminalNode(E_DISTINCT, "DISTINCT");
                node->setChild(E_SELECT_DISTINCT, node_distinct);
            } else {
                if(node->serialize().compare("DISTINCT") != 0){
                    delete(node->getChild(E_SELECT_DISTINCT));
                    Node * node_distinct = Node::makeTerminalNode(E_DISTINCT, "DISTINCT");
                    node->setChild(E_SELECT_DISTINCT, node_distinct);
                }
            }
        }
    }
    else {
        //ret = terminalToken_.yytex;
        if(node->GetType() == E_QUESTIONMARK){
            questmarkids.push_back(node->questmarkid_);
        }
    }
}

void EMS4HSelectAst::AddedMatchedMaskItem(const std::string& /*strTable*/, const MaskItem* pMask)
{
    _matchedMaskItem.push_back(*pMask);
}

std::string EMS4HSelectAst::ConstructQueryExprSQL(){

    std::string strRet;

    if(NULL != _node){
        switch (_node->nodeType_) {
            case E_STMT_LIST: {
                //todo
                theLog->WriteLog(log_error, "ConstructQueryExprSQL, go into E_STMT_LIST");
            } break;
            case E_DIRECT_SELECT: {
                Node * child1 = _node->getChild(1);
                strRet = child1->serialize();
            } break;
            case E_SELECT: {
                strRet = _node->serialize();
            } break;
        }
    }

    return strRet;
}

std::string EMS4HSelectAst::ConstructMaskCountSql(){

    Node *pBackupNode = Node::makeTerminalNode(E_SPECIAL_TERMINAL, "");
    *pBackupNode = *_select_list_node;

    std::string MASKED_ = "MASKED_", SUM_MASKED_ = "SUM_MASKED_";
    std::string strSumList, strMaskList;
    int nMaskIndex = 0;
    for (auto &maskItem : _matchedMaskItem)
    {
        std::string strIndex = std::to_string(++nMaskIndex);
        if (!strMaskList.empty()) strMaskList += ",";
        if (!strSumList.empty()) strSumList += ",";
        std::string strCond = maskItem._condition.empty() ? "1=1" : maskItem._condition;
        strMaskList += "CASE WHEN (" + strCond + ") THEN 1 ELSE 0 END AS " + MASKED_ + strIndex; //CASE WHEN Cond1 THEN 1 ELSE 0 END AS MASKED_1
        strSumList += "SUM(" + MASKED_ + strIndex + ") AS " + SUM_MASKED_ + strIndex;                       //SUM(MASKED_1) AS SUM_MASKED_1
    }

    Node *pnode = Node::makeTerminalNode(E_SPECIAL_TERMINAL, strMaskList);
    *_select_list_node = *pnode;
    delete (pnode); pnode = NULL;

    std::string strMaskCounSql = "SELECT " + strSumList + " FROM (" + ConstructQueryExprSQL() + ")";

    *_select_list_node = *pBackupNode;
    delete (pBackupNode); pBackupNode = NULL;

    return strMaskCounSql;
}
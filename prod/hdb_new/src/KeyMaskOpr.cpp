//
// Created by jeff on 2020/5/26.
//

#include "KeyMaskOpr.h"
#include "NxlDbDataMgr.h"
#include "../parser_yacc/EMS4HSelectAst.h"
#include "log.h"
#include <list>


void insert_into_mask_table(const std::string  & sql, void * conn, S4HException & exc);
std::string construct_insert_into_sql(const std::string & subsql, S4HException & exc);
std::string construct_insert_into_part(const MaskItem * pmask, S4HException & exc);
void modify_select_list(Node * node_list, const std::string & val, S4HException & exc);

std::string construct_insert_field_name(EMColumnInfo * field, const std::string & fieldname){
    assert(field && field->_node);
    std::string ret;
    if(field->_node && field->_node->GetType() == E_STAR){//select qualify.* from ...
        std::string star = field->_node->serialize();
        size_t pos = star.find("*");
        if(pos == 0){
            ret = fieldname;
        } else {
            ret = star.substr(0,pos) + fieldname;
        }
    } else {//select qualify.field, ... from ...
        ret = field->_node->serialize();
    }
    return  ret;
}

void insert_maskdata_into_cachetable(const std::string & sql, EMColumnInfo * field, const MaskItem * pmask, void * conn, const EnforcerCtxImpl* pctx, S4HException & exc){
    //parser sql
    EMS4HAst * ast = create_ast(sql,  exc);
    if(ast == NULL || ast->GetStatementType() != E_STMT_TYPE_SELECT) {
        exc.SetError(S4H_SQL_PARSE_FAIL, "insert_maskdata_into_cachetable parser subquery sql failed.");
        return;
    }
    EMS4HSelectAst* pselect =  dynamic_cast<EMS4HSelectAst*>(ast);
    
    //modify where clause
    std::string cond = construct_insert_into_part( pmask, exc);
    pselect->_whereclause.EvaluationSQL(cond);//EvaluationSQL(cond);

    //modify select list
    std::string col = construct_insert_field_name(field, pmask->_field);
    modify_select_list( pselect->_select_list_node, col, exc);
    if(exc._code != 0){
        exc.SetError(S4H_SQL_PARSE_FAIL, "insert_maskdata_into_cachetable modify_select_list failed.");
        delete(ast);
        return;
    }
    //collect questmark ids  and  delete some node for subquery
    std::vector<int>  questmarkids;
    pselect->CollectQuestMark_for_key_mask(pselect->_node, questmarkids );
    
    //create insert into sql
    std::string outsql = pselect->Output();
    std::string insert_sql =  construct_insert_into_sql(outsql, exc);
    if(insert_sql.empty()){
        exc.SetError(S4H_SQL_PARSE_FAIL, "insert_maskdata_into_cachetable modify subquery sql failed.");
        delete(ast);
        return;
    }
    
    //execute sql to add column value
    theLog->WriteLog(log_info, "insert_maskdata_into_cachetable, sql=%s", insert_sql.c_str() );
    NxlDBDataMgr::Instance()->Execute(insert_sql, questmarkids, conn, pctx, exc); 
    if(exc._code != 0){
        delete(ast);
        return;
    }
    delete(ast);

}


void modify_select_list(Node * node_list, const std::string & val, S4HException & /*exc*/){
    Node * new_node = Node::makeTerminalNode(E_SPECIAL_TERMINAL, val);
    (*node_list) = (*new_node);
    delete(new_node);
}

std::string construct_insert_into_part(const MaskItem * pmask, S4HException & /*exc*/){
    if(!pmask){
        return "";
    }

    std::string field ;
    if(!pmask->_qulify.empty()){
        field += pmask->_qulify;
        field += ".";
    }
    field += pmask->_field;

    std::string mask_cond;
    if(!pmask->_condition.empty()){
        mask_cond += " AND (";
        mask_cond += pmask->_condition;
        mask_cond += ")";
    }

    NxlDBDataMgr* pNxlDBDataMgr = NxlDBDataMgr::Instance();

    std::string cond = "(" + field + " NOT IN (SELECT \"FIELD_VALUE\" FROM " +  pNxlDBDataMgr->KeyMaskCacheTableFullName() + "))" + mask_cond;

    return cond;
}

std::string construct_insert_into_sql(const std::string & subsql, S4HException & /*exc*/){
    if(subsql.empty()) {
        return "";
    }
     NxlDBDataMgr* pNxlDBDataMgr = NxlDBDataMgr::Instance();
    //Insert into MASK_CACHE(FIELD_VALUE) (select DISTINCT "SCHEMA_NAME" from views where  "SCHEMA_NAME" not in (select "FIELD_VALUE" from MASK_CACHE));
    std::string ret;
    ret = "INSERT INTO " + pNxlDBDataMgr->KeyMaskCacheTableFullName() +  "(FIELD_VALUE) (" + subsql + ")";
    return ret;
}


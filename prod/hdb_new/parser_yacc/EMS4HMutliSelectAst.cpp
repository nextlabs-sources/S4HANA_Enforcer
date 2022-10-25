//
// Created by jeff on 2020/4/14.
//

#include "EMS4HMutliSelectAst.h"
#include "EMS4HSelectAst.h"

int resolve_cte( Node* node, EMS4HMutliSelectAst* parent, S4HException & exc);
EMS4HSelectAst * parser_one_select(Node * node, EMS4HMutliSelectAst * parent, S4HException & exc);



void EMS4HMutliSelectAst::Parser(Node * pnode, S4HException & exc) {
    assert(pnode);
    Node *with = NULL;
    Node *select_clause = pnode;
    Node *order_by = NULL;
    if (pnode->nodeType_ == E_DIRECT_SELECT) {
        with = pnode->getChild(E_DIRECT_SELECT_WITH);
        select_clause = pnode->getChild(E_DIRECT_SELECT_SELECT_CLAUSE);
        order_by = pnode->getChild(E_DIRECT_SELECT_ORDER);
    }

    if(with){
        //cte
        assert(with->nodeType_ == E_OPT_WITH_CLAUSE);
        Node* ctes = with->getChild(E_OPT_WITH_CLAUSE_CTE_LIST);
        std::list<Node*> ls;
        Node::ToList(ctes, ls);
        for (auto cte : ls)
        {
            resolve_cte(cte, this,exc );
        }

    }

    if(select_clause){
        if (select_clause->nodeType_ == E_SELECT_WITH_PARENS) {
            select_clause = Node::remove_parens(select_clause);
        }
        if(!select_clause){
            return ;
        }
        Node* set_op = select_clause->getChild(E_SELECT_SET_OPERATION);
        if (set_op != nullptr) {
            /*select with set operation*/
            Node* former = select_clause->getChild(E_SELECT_FORMER_SELECT_STMT);
            Parser(former, exc);

            Node* latter = select_clause->getChild(E_SELECT_LATER_SELECT_STMT);
            Parser( latter,exc);

        } else {
            EMS4HSelectAst * past = parser_one_select(select_clause, this, exc);
            if(past)  this->AddAst(past);
        }
    }
}


int resolve_cte( Node* node,  EMS4HMutliSelectAst* parent, S4HException & exc) {
    assert(node && node->nodeType_ == E_COMMON_TABLE_EXPR);
    Node* subquery = node->getChild(E_COMMON_TABLE_EXPR_SUBQUERY);
    assert(subquery && subquery->nodeType_ == E_SELECT_WITH_PARENS);
    subquery = Node::remove_parens(subquery);
    assert(subquery->nodeType_ == E_SELECT || subquery->nodeType_ == E_DIRECT_SELECT);
    uint64_t query_id = OB_INVALID_ID;

    Node* tb = node->getChild(E_COMMON_TABLE_EXPR_TABLE);
    assert(tb && tb->nodeType_ == E_IDENTIFIER);
    std::string table_name = tb->terminalToken_.str;

    //CteDef* ctedef = parent->AddCte(plan, query_id, table_name);
    //resolve_select_statement(plan, subquery, query_id, parent);
    parent->AddCte(table_name);
    if(subquery){
        EMS4HSelectAst * past = parser_one_select(subquery, parent, exc);
        if(past)  parent->AddAst(past);
    }


//
//      Since recursive common table expression is supported,
//      so this cte's definition query_id need to be set up
//
//    ctedef->ref_query_id_ = query_id;
//    Node* alias_list = node->getChild(E_COMMON_TABLE_EXPR_COLUMNS);
//    std::vector<std::string> aliass{};
//    if (alias_list) {
//        alias_list = Node::remove_parens(alias_list);
//        std::list<Node*> ls;
//        Node::ToList(alias_list, ls);
//        for (Node* it : ls)
//            aliass.push_back(it->terminalToken_.str);
//    }
//    ctedef->set_column_alias(plan, aliass);
    return 0;
}

EMS4HSelectAst * parser_one_select(Node * node, EMS4HMutliSelectAst * parent, S4HException & exc){
    EMS4HSelectAst * sub = new EMS4HSelectAst(parent);
    sub->Parser(node, exc);
    if(!sub->CheckSupport()){
        delete(sub);
        sub = NULL;
    }
    return sub;
}


 std::string EMS4HMutliSelectAst::Print(){
    return "";
};

 bool EMS4HMutliSelectAst::RewriteSQL(void* connection, const NxlDBCUserInfo* puserinfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx){
     bool ret;
     for(auto it:_selects){
         ret = it->RewriteSQL(connection, puserinfo, presinfo,pctx);
     }
     return ret;
 };
 void EMS4HMutliSelectAst::ResetSQL(){
     for(auto it:_selects){
         it->ResetSQL();
     }
 };
 bool EMS4HMutliSelectAst::CheckSupport(){
     if(_selects.size() == 0) return false;
//     for(auto it:_selects){  //parser_one_select already checked.
//         if(it->CheckSupport()) return true;
//     }
     return false;
 };
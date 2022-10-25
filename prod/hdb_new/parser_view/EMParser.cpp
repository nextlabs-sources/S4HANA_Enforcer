//
// Created by jeff on 2020/5/14.
//
#include "s4hexception.h"
#include "../parser_yacc/parser_interface.h"
#include "../parser_yacc/node.h"
#include "EMViewMgr.h"
#include "EMTableExpr.h"
#include "EMViewStmt.h"
#include <assert.h>
#include <vector>

class ParseMgr;

void resolve_select_statement(ParseMgr * mgr, Node * root,SId & id);
void resolve_table(ParseMgr * mgr, Node * node,EMViewStmt * st);

IViewStmt * parser_view_stmt(ParseMgr * mgr, Node * root){
    SId id = 999;
    switch (root->nodeType_) {
//        case E_STMT_LIST: {
//            resolve_multi_statements(plan, node);
//        } break;
        case E_DIRECT_SELECT: {
            resolve_select_statement(mgr, root, id);
        }
            break;
        default:
            break;
    }
    return mgr->GetStmt(id);
}

void resolve_cte(ParseMgr * mgr, Node * node, SId & id) {
    assert(node && node->nodeType_ == E_COMMON_TABLE_EXPR);
    Node* subquery = node->getChild(E_COMMON_TABLE_EXPR_SUBQUERY);
    assert(subquery && subquery->nodeType_ == E_SELECT_WITH_PARENS);
    subquery = Node::remove_parens(subquery);
    assert(subquery->nodeType_ == E_SELECT || subquery->nodeType_ == E_DIRECT_SELECT);


    Node* tb = node->getChild(E_COMMON_TABLE_EXPR_TABLE);
    assert(tb && tb->nodeType_ == E_IDENTIFIER);
    std::string table_name = tb->terminalToken_.str;
    if(table_name.empty()){
        return;
    }

    resolve_select_statement(mgr, subquery, id);
    EMViewStmt * sub = dynamic_cast<EMViewStmt*>(mgr->GetStmt(id));
    if(NULL == sub){
        return;
    }

    sub->_cte = table_name;
    //todo
    Node* alias_list = node->getChild(E_COMMON_TABLE_EXPR_COLUMNS);
    std::vector<std::string> aliass{};
    if (alias_list) {
        alias_list = Node::remove_parens(alias_list);
        std::list<Node*> ls;
        Node::ToList(alias_list, ls);
        for (Node* it : ls){
            aliass.push_back(it->terminalToken_.str);
        }
    }
}

void resolve_cte_clause(ParseMgr * mgr, Node * node, std::map<std::string,IViewStmt*>  & ctemap){
    if (!node)
        return ;
    assert(node->nodeType_ == E_OPT_WITH_CLAUSE);
    Node* ctes = node->getChild(E_OPT_WITH_CLAUSE_CTE_LIST);
    std::list<Node*> ls;
    Node::ToList(ctes, ls);
    for (auto cte : ls)
    {
        SId  id = 100;
        resolve_cte(mgr, cte, id);
        EMViewStmt * pcte = dynamic_cast<EMViewStmt*>(mgr->GetStmt(id));
        if(pcte !=NULL){
            ctemap.insert(std::make_pair(pcte->_cte, pcte));
        }
    }
    return ;
}

void resolve_joined_table(ParseMgr * mgr, Node * node, EMViewStmt * st){

    assert(node->nodeType_ == E_JOINED_TABLE);
    Node* table_node = nullptr;
    for (size_t i = 0; i < node->getChildrenCount(); ++i) {
        table_node = node->getChild(i);
        if (E_JOINED_TABLE_TABLE_FACTOR_L == i ||
            E_JOINED_TABLE_TABLE_FACTOR_R == i) {
            switch (table_node->nodeType_) {
                case E_SELECT:
                case E_ALIAS: {
                    resolve_table(mgr, table_node, st);

                } break;
                case E_JOINED_TABLE: {
                    resolve_joined_table(mgr, table_node, st);
                } break;
                case E_SELECT_WITH_PARENS: {
                    table_node = Node::remove_parens(table_node);
                    resolve_table(mgr, table_node, st);

                } break;
                case E_JOINED_TABLE_WITH_PARENS: {
                    table_node = Node::remove_parens(table_node);
                    resolve_joined_table(mgr, table_node, st);
                } break;
                default:
                    break;
            }
        }
    }
}

void resolve_table(ParseMgr * mgr, Node * node,EMViewStmt * st){
    assert(node != NULL);
    assert(NULL != st);

    Node* table_node = node;
    Node* alias_node = NULL;
    if (node->nodeType_ == E_ALIAS) {
        table_node = node->getChild(E_ALIAS_ORIGN);
        alias_node = node->getChild(E_ALIAS_ALIAS);
    }

    if (table_node->nodeType_ == E_SELECT_WITH_PARENS || table_node->nodeType_ == E_JOINED_TABLE_WITH_PARENS) {
        table_node = Node::remove_parens(table_node);
    }
    switch (table_node->nodeType_) {
        case E_TABLE_IDENT: {
            //parser table
            EMTableExpr_Ident * tb = new EMTableExpr_Ident();

            Node * ptb = table_node->getChild(0);
            if(ptb){
                tb->_table._name = ptb->terminalToken_.str;
            }
            Node * pschema = table_node->getChild(1);
            if(pschema){
                tb->_table._schema = pschema->terminalToken_.str;
            }
            Node * pdbname = table_node->getChild(2);
            if(pdbname){
                tb->_table._db = pdbname->terminalToken_.str;
            }
            if(alias_node){
                tb->_table._alias = alias_node->terminalToken_.str;
            }
            st->_tables.push_back(tb);
        } break;
        case E_DIRECT_SELECT:
        case E_SELECT: {
            //assert(alias_node != nullptr);  // The alias is actually not optional at all.
//            std::string df = table_node->serialize();
//            std::string qualify = "";
//            if (alias_node != nullptr) {
//                qualify = alias_node->serialize();
//            }

            SId id;
            resolve_select_statement(mgr, table_node, id);
            if(!mgr->GetStmt(id)){
                printf("sub query parser failed");
            }
            EMTableExpr_Select * tb = new EMTableExpr_Select();
            tb->_stmt = mgr->GetStmt(id);
            if(alias_node){
                tb->_alias = alias_node->terminalToken_.str;
            }
            st->_tables.push_back(tb);
//            Node* alias_list = node->getChild(2);
//            std::vector<std::string> aliass{};
//            if (alias_list) {
//                alias_list = Node::remove_parens(alias_list);
//                std::list<Node*> ls;
//                Node::ToList(alias_list, ls);
//                for (Node* it : ls)
//                    aliass.push_back(it->terminalToken_.str);
//            }
        } break;
        case E_JOINED_TABLE: {
            resolve_joined_table(mgr, table_node, st);
        } break;
        case E_TABLE_VALUE_CTOR_PARENS:
            break;
        case E_FUN_CALL:
            /*todo*/
            break;
        case E_PIVOT_TABLE: {
            /*todo*/
        } break;
        case E_UNPIVOT_TABLE: {
            /*todo*/
        } break;
        case E_TEMP_VAR_FUN_CALL:
            /*todo*/
            break;
        case E_TEMP_VARIABLE:
            /*todo*/
            break;
        default:
            /*unreachable*/
            assert(false);
    }
    return ;
}
void resolve_from_clause(ParseMgr * mgr, Node * root, EMViewStmt * st){
    if (!root)
        return ;

    assert(root->nodeType_ == E_FROM_CLAUSE);
    root = root->getChild(E_FROM_CLAUSE_FROM_LIST);
    std::list<Node*> ls{};
    Node::ToList(root, ls);
    for (auto child_node : ls) {

        resolve_table(mgr, child_node, st);

    }
    return ;
}

void parser_hana_field(Node* pnode, std::string & db, std::string & schema, std::string & tb, std::string & field){
    if (pnode == nullptr) return;
    if (pnode->nodeType_ == E_OP_NAME_FIELD) { ///E_OP_NAME_FIELD
        //ret._type = pnode->nodeType_;
        Node * pfield = pnode->getChild(0);
        Node * ptbname = pnode->getChild(1);
        Node * pschema = pnode->getChild(2);
        Node * pdbname = pnode->getChild(3);

        if(pdbname){
            //field qualify did't hav schema
            return;
        }
        if(pschema){
            //field qualify did't hav schema
            return;
        }
        if(ptbname){
            std::string table_name = ptbname->terminalToken_.str;
            std::size_t ifind = table_name.find('.');
            if(ifind != std::string::npos){
                std::string sub1 = table_name.substr(ifind+1);
                std::size_t ifind2 = sub1.find('.');
                if(ifind2 != std::string::npos){
                    db = table_name.substr(0, ifind);
                    schema = sub1.substr(0, ifind2);
                    tb = sub1.substr(ifind2+1);
                } else {
                    schema = table_name.substr(0, ifind);
                    tb = sub1;
                }
            } else {
                tb = table_name;
            }
        }
        if (pfield){
            if (pfield->nodeType_ == E_IDENTIFIER) {
                field = pfield->terminalToken_.str;
            } else if(pfield->nodeType_ == E_STAR){// E_STAR *
                field = "*";
            }
        } else{
            return;
        }

    }
}

int resolve_expr(ParseMgr* mgr, Node* node, EMColumn * col) { //this is for view parser and it is imperfect
    assert(node != nullptr);
    assert(col != nullptr);

    switch (node->nodeType_) {
        case E_NULL:
        case E_INT:
        case E_DOUBLE:
        case E_BOOL:
        case E_TEMP_VARIABLE:
        case E_QUESTIONMARK:
        case E_STRING: {
            //dont care
        } break;
        case E_OP_NAME_FIELD: {
            std::string db,schema,tb,field;
            parser_hana_field( node, db, schema, tb, field);
            if(field.compare("*") == 0){
                col->_star = true;
            }
            col->_name = field;
            col->_tb = tb;
        } break;
        case E_EXPR_LIST_WITH_PARENS: {
            Node* nd = Node::remove_parens(node);
            resolve_expr(mgr, nd, col);
        } break;
        case E_EXPR_LIST: {
            std::list<Node*> ls;
            Node::ToList(node, ls);
            for (auto it : ls) {
                resolve_expr(mgr, it, col);
            }
        } break;
        case E_SELECT_EXPR_LIST: {
            std::list<Node*> ls;
            Node::ToList(node, ls);
            for (auto it : ls) {
                resolve_expr(mgr, it, col);
            }
        } break;
        case E_PROJECT_STRING: {
            resolve_expr(mgr, node->getChild(0), col);
        } break;
        case E_ALIAS: {//aaa AS bbb
            resolve_expr(mgr, node->getChild(0), col);
            Node * alias = node->getChild(1);
            col->_alias = alias->terminalToken_.str;
        }  break;
        case E_STAR: {//aaa AS bbb
            /*todo*/
            col->_name = "*";
            col->_star = true;
        }  break;
        case E_SELECT_WITH_PARENS: {
            //don't care
//            Node* nd = Node::remove_parens(node);
//            resolve_expr(mgr, nd, st);
        } break;
        case E_DIRECT_SELECT:
        case E_SELECT: {
            //don't care
        } break;
        case E_OP_ADD:
        case E_OP_MINUS:
        case E_OP_MUL:
        case E_OP_DIV:
        case E_OP_REM:
        case E_OP_POW:
        case E_OP_MOD:
        case E_OP_LE:
        case E_OP_LT:
        case E_OP_EQ:
        case E_OP_GE:
        case E_OP_GT:
        case E_OP_NE:
        case E_OP_LIKE:
        case E_OP_NOT_LIKE:
        case E_OP_AND:
        case E_OP_OR:
        case E_OP_IS:
        case E_OP_IS_NOT:
        case E_OP_IN:
        case E_OP_NOT_IN:
        case E_OP_CNN:
        case E_OP_ASS:
        case E_OP_ASS_ADD:
        case E_OP_ASS_MINUS:
        case E_OP_ASS_MUL:
        case E_OP_ASS_DIV:
        case E_OP_ASS_REM:
        case E_OP_ASS_BIT_AND:
        case E_OP_ASS_BIT_OR:
        case E_OP_ASS_BIT_XOR:
        case E_OP_COLLATE: {
            resolve_expr(mgr, node->getChild(E_OP_BINARY_OPERAND_L), col);
            resolve_expr(mgr, node->getChild(E_OP_BINARY_OPERAND_R), col);
        } break;
        case E_OP_NOT:
        case E_OP_EXISTS:
        case E_OP_POS:
        case E_OP_NEG: {

        } break;
        case E_OP_BTW:
        case E_OP_NOT_BTW: {
            resolve_expr(mgr, node->getChild(E_OP_TERNARY_OPERAND_1), col);
            resolve_expr(mgr, node->getChild(E_OP_TERNARY_OPERAND_2), col);
            resolve_expr(mgr, node->getChild(E_OP_TERNARY_OPERAND_3), col);
        } break;
        case E_CASE: {
            //resolve_expr(mgr, node->getChild(E_CASE_ARG), col);
            Node* deft = node->getChild(E_CASE_ELSE);
            if (deft){
                resolve_expr(mgr, deft->getChild(E_CASE_DEFAULT_EXPR), col);
            }

            Node* when = node->getChild(E_CASE_WHEN_CLAUSE_LIST);
            std::list<Node*> ls;
            Node::ToList(when, ls);
            for (auto it : ls) {
                assert(it->nodeType_ == E_WHEN);
                resolve_expr(mgr, it->getChild(E_WHEN_WHEN_EXPR), col);
                resolve_expr(mgr, it->getChild(E_WHEN_THEN_EXPR), col);
            }
        } break;
        case E_FUN_CALL: {
            //Node* func = node->getChild(E_FUN_CALL_FUNC_NAME);
            Node* params = node->getChild(E_FUN_CALL_PARAMS);
            std::list<Node*> ls;
            Node::ToList(params, ls);
            for (auto it : ls) {
                resolve_expr(mgr, it, col);
            }
        } break;
        default:
            break;

    }
    return 0;
}
void resolve_select_items(ParseMgr * mgr, Node * node,EMViewStmt * st){
    switch (node->GetType()){
        case E_SELECT_EXPR_LIST: {
            std::list<Node*> ls;
            Node::ToList(node, ls);
            for (auto it : ls) {
                EMColumn * col = new EMColumn();
                resolve_expr(mgr, it, col);
                //
                std::string colname = col->_alias ;
                if(col->_alias.empty()){
                    colname = col->_name;
                }
                //printf("COL:(%s,%s,%s)\n",col->_tb.c_str(),col->_name.c_str(),col->_alias.c_str());
                st->_columns.push_back(col);
            }
        } break;
        case E_PROJECT_STRING: {
            EMColumn * col = new EMColumn();
            resolve_expr(mgr, node, col);
            //
            std::string colname = col->_alias ;
            if(col->_alias.empty()){
                colname = col->_name;
            }
            //printf("COL:(%s,%s,%s)\n",col->_tb.c_str(),col->_name.c_str(),col->_alias.c_str());
            st->_columns.push_back(col);
        } break;
        default:
            break;
    }

}
void resolve_select_statement(ParseMgr * mgr, Node * root,SId & id){
    Node *with = nullptr;
    Node *select_clause = root;
//    Node *order_by = nullptr;
    if (root->nodeType_ == E_DIRECT_SELECT) {
        with = root->getChild(E_DIRECT_SELECT_WITH);
        select_clause = root->getChild(E_DIRECT_SELECT_SELECT_CLAUSE);
       // order_by = root->getChild(E_DIRECT_SELECT_ORDER);
    }


    if (select_clause->nodeType_ == E_SELECT_WITH_PARENS) {
        select_clause = Node::remove_parens(select_clause);
    }

    std::map<std::string,IViewStmt*> ctemap;
    resolve_cte_clause(mgr, with, ctemap);

    Node* set_op = select_clause->getChild(E_SELECT_SET_OPERATION);
    if (set_op != nullptr) {
        /*select with set operation*/
        Node* former = select_clause->getChild(E_SELECT_FORMER_SELECT_STMT);
        resolve_select_statement(mgr, former, id);
        EMViewStmt * st = dynamic_cast<EMViewStmt*>(mgr->GetStmt(id));
        if(st != NULL && ctemap.size() > 0){
            st->_ctes = ctemap;
        }

        Node* latter = select_clause->getChild(E_SELECT_LATER_SELECT_STMT);
        SId id_latter = 999;
        resolve_select_statement(mgr, latter, id_latter);
//        mgr[id_latter]->set_parent_op_stmt(id);
    }
    else {
        /*simple select*/
        EMViewStmt *  st = new EMViewStmt();
        id = mgr->InsertStmt(st);
        resolve_from_clause(mgr, select_clause->getChild(E_SELECT_FROM_LIST), st);
        resolve_select_items(mgr, select_clause->getChild(E_SELECT_SELECT_EXPR_LIST), st);
//        resolve_where_clause(mgr, select_clause->getChild(E_SELECT_OPT_WHERE), idnew);
//        resolve_group_by_clause(mgr, select_clause->getChild(E_SELECT_GROUP_BY), idnew);
//        resolve_having_clause(mgr, select_clause->getChild(E_SELECT_HAVING), idnew);
        if( ctemap.size() > 0){
            st->_ctes = ctemap;
        }

    }
    //resolve_order_by_clause(mgr, order_by, id);
    return ;
}




//--------------------------------------------------------------------------
#include "../parser_yacc/sqlparser_hana_bison.h"
#include "../parser_yacc/sqlparser_hana_flex.h"
bool parse_view(const std::string& sql, ParseResult* result) {
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (hana_lex_init_extra(result, &scanner)) {
        //fprintf(stderr, "SQLParser2003: Error when initializing lexer!\n");
        return false;
    }

    const char* text = sql.c_str();
    state = hana__scan_string(text, scanner);

    // Parse the tokens.
    // If parsing fails, the result will contain an error object.
    hana_parse(result, scanner);
    hana__delete_buffer(state, scanner);
    hana_lex_destroy(scanner);

    return result->accept;
}

IViewStmt * parser_view(const std::string &sql, ParseMgr * mgr, S4HException & exc){
    ParseResult *r = new ParseResult();
    if (!parse_view(sql, r)){
        std::string strLog = "parse failed. detail:" + r->errDetail ;
        strLog += + " sql=" + sql;
        exc.SetError(S4H_SQL_PARSE_FAIL,  strLog);//r->errDetail
        delete(r);
        return NULL;
    }

    if (!r->result_tree_) {
        // printf("parse failed !");
        exc.SetError(S4H_SQL_PARSE_FAIL, "parse node is nullptr.");
        delete(r);
        return NULL;
    }
    IViewStmt * ret =  parser_view_stmt(mgr, r->result_tree_);
    mgr->SetParserResult(r);
    return ret;
}



std::string construct_key(const EMTable & tb){
    std::string ret;
    if(!tb._schema.empty()){
        ret+=tb._schema;
        ret+='.';
    }
    ret+=tb._name;
    return ret;
}
void resolve_key(std::string & schema, std::string & table, const std::string & src){
    unsigned  int ipos = src.find(".");
    if(ipos != std::string::npos){// if itSourceTable = schema.table
        table = src.substr(ipos+1);
        schema = src.substr(0, ipos);
    } else {
        table = src;
        schema.clear();
    }
}
std::string get_view_column(const EMColumn & col){
    if(!col._alias.empty()){
        return col._alias;
    } else {
        return col._name;
    }
}
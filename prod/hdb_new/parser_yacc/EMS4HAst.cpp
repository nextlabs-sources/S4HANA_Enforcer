//
// Created by jeff on 2020/2/23.
//
#include "EMS4HAst.h"
#include "EMS4HSelectAst.h"
#include "EMS4HUpdateAst.h"
#include "EMS4HDeleteAst.h"
#include "EMS4HInsertAst.h"
#include "EMS4HUpsertAst.h"
#include "PolicyManager.h"
#include "tool.h"
#include "parse_tool.h"
#include "NXS4HConfig.h"

std::string get_action_from_ast_type(EMStmtType ty){
    std::string ret;
    switch(ty){
        case E_STMT_TYPE_UPDATE:{
            ret = "UPDATE";
        } break;
        case E_STMT_TYPE_INSERT:{
            ret = "INSERT";
        } break;
        case E_STMT_TYPE_DELETE:{
            ret = "DELETE";
        } break;
        case E_STMT_TYPE_UPSERT:{
            ret = "UPSERT";
        } break;
        default:{
            ret = "SELECT";
        } break;

    }
    return ret;
}

bool is_support_func_name(const std::string & name){
    std::set<std::string> set_names{ "AVG", "CORR", "CORR_SPEARMAN", "MIN", "MEDIAN", "MAX",
                                     "STDDEV", "STDDEV_POP", "STDDEV_SAMP", "SUM", "VAR",  "VAR_POP", "VAR_SAMP"
    };
    if(set_names.find(name) != set_names.end()){
        return true;
    }
    return false;
}

 std::vector<EMTableInfo*>& EMS4HAst::GetAllTables(){
    if (_tempAllTables.size()==0){
        switch(GetStatementType()) {
            case E_STMT_TYPE_SELECT: {
                const std::vector<EMTableInfo*> & tables = ((EMS4HSelectAst*)this)->GetTables();
                for(auto it : tables) {
                    _tempAllTables.push_back(it);        
                }
            } break;
            case E_STMT_TYPE_UPDATE: {
                EMTableInfo * tb = ((EMS4HUpdateAst*)this)->GetTable();
                _tempAllTables.push_back(tb);
            } break;
            case E_STMT_TYPE_INSERT:{
                EMTableInfo * tb = ((EMS4HInsertAst*)this)->GetTable();
                _tempAllTables.push_back(tb);
            } break;
            case E_STMT_TYPE_DELETE:{
                EMTableInfo * tb = ((EMS4HDeleteAst*)this)->GetTable();
                _tempAllTables.push_back(tb);
            } break;
            case E_STMT_TYPE_UPSERT:{
                EMTableInfo * tb = ((EMS4HUpsertAst*)this)->GetTable();
                _tempAllTables.push_back(tb);
            } break;
            default: break;
        }
    }

    return _tempAllTables;
}

EMColumnInfo::EMColumnInfo(Node * node):_node(node),_alias(false),_questmarkid(0),_type(DIRECT_COL){
    if(node) {
        _old.set_text(node->serialize());
        _old.nodeType_ = node->nodeType_;
    }
}
EMColumnInfo::~EMColumnInfo(){
}

EMS4HAst * parser_stmt(Node * proot, S4HException & exc) {
    if(!proot) return NULL;
    EMS4HAst * past = NULL;
    switch (proot->nodeType_) {
        case E_STMT_LIST: {
            past = parser_stmt(proot->getChild(0), exc);
        } break;
        case E_SELECT_WITH_PARENS:{
            past = parser_stmt(proot->getChild(0), exc);
        } break;
        case E_DIRECT_SELECT: {
            Node * ptemp = proot->getChild(1);
            past = parser_stmt(ptemp, exc);
        } break;
        case E_SELECT: {
            //need match table_name
            past = new EMS4HSelectAst();
            past->Parser(proot, exc);
            if(!exc._detail.empty()){
                delete past;
                past = NULL;
            }
        } break;
        case E_UPDATE: {
            //need match table_name
            past = new EMS4HUpdateAst();
            past->Parser(proot, exc);
        } break;
        case E_DELETE: {
            //need match table_name
            past = new EMS4HDeleteAst();
            past->Parser(proot, exc);
        } break;
        case E_INSERT: {
            //need match table_name
            past = new EMS4HInsertAst();
            past->Parser(proot, exc);
        } break;
        case E_UPSERT:{
            past = new EMS4HUpsertAst();
            past->Parser(proot, exc);
        }  break;
        case E_REPLACE:{
            return NULL;
        }
        default:
            break;/*todo other notetype*/
    }
    if(past) past->SetNode(proot);
    return past;
}


void parser_name_field(Node* pnode, EMColumnInfo & info, S4HException & /*exc*/){
    if (pnode == nullptr) return;
    if (pnode->nodeType_ == E_OP_NAME_FIELD) { ///E_OP_NAME_FIELD
        //ret._type = pnode->nodeType_;
        Node * pfield = pnode->getChild(0);
        if(pfield){
            if (pfield->nodeType_ == E_IDENTIFIER) {
                info._field = pfield->terminalToken_.str;
            } else {// E_STAR *
                info._field = "*";
            }
        }
        Node * ptbname = pnode->getChild(1);
        if (ptbname) {
            info._tb = ptbname->terminalToken_.str;
        }
        Node * pschema = pnode->getChild(2);
        if (pschema) {
            info._schema = pschema->terminalToken_.str;
        }
        Node * pdbname = pnode->getChild(3);
        if (pdbname) {
            info._db = pdbname->terminalToken_.str;
        }
    }
}

void parser_name_table(Node* pnode, EMTableInfo & info, S4HException & /*exc*/){
    if (pnode == nullptr) return;
    if (pnode->nodeType_ == E_TABLE_IDENT) { ///E_OP_NAME_FIELD
        //ret._type = pnode->nodeType_;
        Node * pc = pnode->getChild(0);
        if(pc){
            if (pc->nodeType_ == E_IDENTIFIER) {
                info._tb = pc->terminalToken_.str;
            }
        }
        Node * pschema = pnode->getChild(1);
        if (pschema) {
            info._schema = pschema->terminalToken_.str;
        }
        Node * pdbname = pnode->getChild(2);
        if (pdbname) {
            info._db = pdbname->terminalToken_.str;
        }
    }
}


void parser_alias(Node* pnode, EMColumnInfo & info, S4HException & exc) {
    if (pnode == nullptr) return;
    Node * pchild = pnode->getChild(0);
    if(pchild == nullptr) return;
    if (pchild->nodeType_ == E_OP_NAME_FIELD) {
        parser_name_field(pchild, info, exc);
    }
}

int parser_func_params(EMFuncColumnInfo & col, Node* node) { //this is for view parser and it is imperfect
    assert(node != nullptr);

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
            EMColumnInfo * pinfo = new EMColumnInfo(node);
            S4HException exc;
            parser_name_field(node, *pinfo, exc);
            if(!pinfo->_field.empty()){
                pinfo->_type = EMColumnInfo::SUB_COL;
                col._params.push_back(pinfo);
            }
			else  // add code by Tonny at here. it may cause memory leak
			{
				delete pinfo;	pinfo = NULL;
			}
        } break;
        case E_EXPR_LIST_WITH_PARENS: {
            Node* nd = Node::remove_parens(node);
            parser_func_params(col, nd);
        } break;
        case E_EXPR_LIST: {
            std::list<Node*> ls;
            Node::ToList(node, ls);
            for (auto it : ls) {
                parser_func_params(col, it);
            }
        } break;
        case E_SELECT_EXPR_LIST: {
            std::list<Node*> ls;
            Node::ToList(node, ls);
            for (auto it : ls) {
                parser_func_params(col, it);
            }
        } break;

        case E_STAR: {//aaa AS bbb

        }  break;
        case E_SELECT_WITH_PARENS: {
            //don't care
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
            parser_func_params(col, node->getChild(E_OP_BINARY_OPERAND_L));
            parser_func_params(col, node->getChild(E_OP_BINARY_OPERAND_R));
        } break;
        case E_OP_NOT:
        case E_OP_EXISTS:
        case E_OP_POS:
        case E_OP_NEG: {

        } break;
        case E_OP_BTW:
        case E_OP_NOT_BTW: {
            parser_func_params(col,  node->getChild(E_OP_TERNARY_OPERAND_1));
            parser_func_params(col,  node->getChild(E_OP_TERNARY_OPERAND_2));
            parser_func_params(col,  node->getChild(E_OP_TERNARY_OPERAND_3));
        } break;
        case E_CASE: {
            //resolve_expr(mgr, node->getChild(E_CASE_ARG), col);
            Node* deft = node->getChild(E_CASE_ELSE);
            if (deft){
                parser_func_params(col,  deft->getChild(E_CASE_DEFAULT_EXPR));
            }

            Node* when = node->getChild(E_CASE_WHEN_CLAUSE_LIST);
            std::list<Node*> ls;
            Node::ToList(when, ls);
            for (auto it : ls) {
                assert(it->nodeType_ == E_WHEN);
                //parser_func_params(col,  it->getChild(E_WHEN_WHEN_EXPR));
                parser_func_params(col,  it->getChild(E_WHEN_THEN_EXPR));
            }
        } break;
        case E_FUN_CALL: {
            //Node* func = node->getChild(E_FUN_CALL_FUNC_NAME);
            Node* params = node->getChild(E_FUN_CALL_PARAMS);
            std::list<Node*> ls;
            Node::ToList(params, ls);
            for (auto it : ls) {
                parser_func_params(col,  it);
            }
        } break;
        default:
            break;

    }
    return 0;
}



bool parser_function(Node* pnode, EMFuncColumnInfo & info, S4HException & /*exc*/){
    assert(pnode && pnode->GetType() == E_FUN_CALL);
    Node * name = pnode->getChild(0);//name
    Node * param = pnode->getChild(1);//param
    if(param == NULL){
        return  false;
    }
    //check name
    info._func_name = name->Serialize();
    if( !is_support_func_name(info._func_name) ){
        return false;
    }

    parser_func_params( info, param);
    return info._params.size() > 0;
}



//bool parse_hana(const std::string& sql, ParseResult* result) {
//    yyscan_t scanner;
//    YY_BUFFER_STATE state;
//
//    if (hana_lex_init_extra(result, &scanner)) {
//        //fprintf(stderr, "SQLParser2003: Error when initializing lexer!\n");
//        return false;
//    }
//
//    const char* text = sql.c_str();
//    state = hana__scan_string(text, scanner);
//
//    // Parse the tokens.
//    // If parsing fails, the result will contain an error object.
//    hana_parse(result, scanner);
//    hana__delete_buffer(state, scanner);
//    hana_lex_destroy(scanner);
//
//    return result->accept;
//}


EMS4HAst * create_ast(const std::string & sql_in, S4HException & exc) {
    //const TableSet* tbset = PolicyManager::Instance()->GetEnforceTables("mask", exc);
    EMS4HAst * past = NULL;

     unsigned int uFeature = g_conf->GetFeatureLevel();
    if ((DAE_FEATURE_AST&uFeature)==0)  {
            return NULL;
    }

    ParseResult *r = new ParseResult();
    if (!parse_hana(sql_in, r)){
        std::string strLog = "parse failed. detail:" + r->errDetail ;
        strLog += + " sql=" + sql_in;
        exc.SetError(S4H_SQL_PARSE_FAIL,  strLog);//r->errDetail
        delete(r);
        return past;
    }

    if (!r->result_tree_) {
        // printf("parse failed !");
        exc.SetError(S4H_SQL_PARSE_FAIL, "parse node is nullptr.");
        delete(r);
        return past;
    }

    past = parser_stmt(r->result_tree_, exc);
    if(past) {
        past->SetResult(r);
    } else {
        std::string strLog = "sql statement type not support:" + sql_in;
        exc.SetError(S4H_SQL_PARSE_FAIL, strLog.c_str() );
        delete(r);
        r = NULL;
        return NULL;
    }

    return past;

}

std::string print_columninfo(EMColumnInfo & info){
    std::string ret;
    if(!info._db.empty()) {
        bool bDBHaveQuote = info._db[0]=='"';
        ret += (bDBHaveQuote ? "" : "\"") + info._db + (bDBHaveQuote ? "" : "\".");
    }
    if(!info._schema.empty()){
        bool bSchemaHaveQuote = info._schema[0]=='"';
        ret +=  (bSchemaHaveQuote ? "" : "\"") + info._schema +  (bSchemaHaveQuote ? "" : "\".");
    }
    if(!info._tb.empty()) {
        bool bTBHaveQuote = info._tb[0]=='"';
        ret += (bTBHaveQuote ? "" : "\"") + info._tb + (bTBHaveQuote ? "" :  "\".");
    }

    bool bFieldHaveQuote = info._field[0]=='"';
    ret += (bFieldHaveQuote ? "" : "\"") + info._field + (bFieldHaveQuote ? "" : "\"");
    return ret;

}


bool create_old_node(Node & old, EMColumnInfo & info) {
    old = *(info._node);
    return true;
}

bool create_new_node(Node & new_, EMColumnInfo & info) {
    std::string col = print_columninfo(info);
    if(col.empty())
        return false;
    std::string val = col ;
    val += " = CASE WHEN ";
    val += col;
    val += " = ? THEN ";
    val += col;
    val += " ELSE ";
    val += col;
    val += " END";
    new_.nodeType_ = E_SPECIAL_TERMINAL;
    new_.set_text(val);
    return true;
}

bool create_new_node(Node & new_, EMColumnInfo & info, const std::string& originalValueStr, const std::string& maskCondition) {
    std::string col = print_columninfo(info);
    if(col.empty())
        return false;

    //create mask condition
    std::string condition = maskCondition;
    if (condition.empty()){ //when condition is empty, means all rows of that field is masked.
        condition="1=1";
    }
    
    std::string val = col ;
    val += " =(CASE WHEN ";
    val += condition;
    val += " THEN ";
    val += col;
    val += " ELSE ";
    val += originalValueStr;
    val += " END)";
    
    new_.nodeType_ = E_SPECIAL_TERMINAL;
    new_.set_text(val);
    return true;
}

void reset_columninfo(EMColumnInfo * pinfo) {
    if(pinfo->_node->nodeType_ == E_SPECIAL_TERMINAL) {
        *(pinfo->_node) = pinfo->_old;
    }

    if (pinfo->_type == EMColumnInfo::FUNC_COL) {
        EMFuncColumnInfo *pFunCol = dynamic_cast<EMFuncColumnInfo *>(pinfo);
        for (auto itFunField : pFunCol->_params)  {
            if (itFunField->_node->nodeType_ == E_SPECIAL_TERMINAL) {
                *(itFunField->_node) = itFunField->_old;
            }
        }
    }
}

 bool UpdateData::Equal(const std::string& strMaskSymbol)
 {
    if(SQLDBC_HOSTTYPE_ASCII==_type){
        return CommonFun::StrCaseCmp(_Data._strData, strMaskSymbol)==0;
    }
    else if (SQLDBC_HOSTTYPE_INT8==_type){
        int64_t nMask = strtoll(strMaskSymbol.c_str(), NULL, 10); 
        return nMask==_Data._int64Data;
    }
    else if (SQLDBC_HOSTTYPE_UINT8==_type){
        uint64_t nMask = strtoull(strMaskSymbol.c_str(), NULL, 10); 
        return nMask==_Data._uint64Data;
    }
    return false;
}


Node * get_op_equal_condition_rightnode(Node * node,const std::string & col){
    switch(node->nodeType_){
        case E_OP_OR:{
            // 0,1
            Node * ret = get_op_equal_condition_rightnode(node->getChild(0), col);
            if(ret){
                return ret;
            }
            ret = get_op_equal_condition_rightnode(node->getChild(1), col);
            if(ret){
                return ret;
            }

        } break;
        case E_OP_AND:{
            // 0,1
            Node * ret = get_op_equal_condition_rightnode(node->getChild(0), col);
            if(ret){
                return ret;
            }
            ret = get_op_equal_condition_rightnode(node->getChild(1), col);
            if(ret){
                return ret;
            }
        } break;
        case E_OP_NOT:{
            // 0
            Node * ret = get_op_equal_condition_rightnode(node->getChild(0), col);
            if(ret){
                return ret;
            }
        } break;
        case E_OP_IS:
        case E_OP_IS_NOT:{
            // 0
            Node * ret = get_op_equal_condition_rightnode(node->getChild(0), col);
            if(ret){
                return ret;
            }
        } break;
        case E_EXPR_LIST_WITH_PARENS:{
            // 0
            Node * ret = get_op_equal_condition_rightnode(node->getChild(0), col);
            if(ret){
                return ret;
            }
        } break;
        case E_OP_EQ_ALL:
        case E_OP_EQ_SOME:
        case E_OP_EQ_ANY:
        case E_OP_EQ:{
            Node * node_col = node->getChild(0);//get_condition_rightnode(, col);
            if(node_col){
                EMColumnInfo info;
                S4HException exc;
                parser_name_field(node_col, info, exc);
                std::string ss =  info._field;
                if(CommonFun::StrCaseCmp(ss,col) == 0){
                    return node->getChild(1);
                }
            }
        } break;
        case E_OP_LE:
        case E_OP_LT:
        case E_OP_GE:
        case E_OP_GT:
        case E_OP_NE:
        case E_OP_LE_ALL:
        case E_OP_LT_ALL:
        case E_OP_GE_ALL:
        case E_OP_GT_ALL:
        case E_OP_NE_ALL:
        case E_OP_LE_SOME:
        case E_OP_LT_SOME:
        case E_OP_GE_SOME:
        case E_OP_GT_SOME:
        case E_OP_NE_SOME:
        case E_OP_LE_ANY:
        case E_OP_LT_ANY:
        case E_OP_GE_ANY:
        case E_OP_GT_ANY:
        case E_OP_NE_ANY://ignore
        {

        } break;
        case E_OP_BTW:
        case E_OP_NOT_BTW:{//ignore
            //012
        }
            break;
        case E_OP_LIKE:{//ignore
            //012
        } break;
        case E_OP_NOT_LIKE:{//ignore
            //01
        } break;
        case E_OP_IN:
        case E_OP_NOT_IN:{//ignore
            //01
        } break;
        case E_OP_EXISTS:{//ignore
            //0
        } break;
        default:
            return NULL;
    }
    return NULL;
}

bool EMWhereClauseInfo::GetConditionRightParamId(const std::string & column,  unsigned int & markid)const {
    if(NULL == _node){
        return false;
    }
    Node* and_expr = _node->getChild(E_WHERE_CLAUSE_EXPR);
    Node * node = get_op_equal_condition_rightnode(and_expr, column);
    if(NULL == node){
        return false;
    }
    if(node->nodeType_ != E_QUESTIONMARK){
        return false;
    }
    markid = node->questmarkid_;

    return true;
}

#if 0
bool EMUpdateColInfo::UpdateColumnControl(const std::string & condition){ //   Field = Value
    bool ret = false;
    assert(_info._node);
    Node * pcol = _info._node->getChild(0);
    Node * pval = _info._node->getChild(1);
    if(pcol && pval) {
        if(pcol->nodeType_ == E_OP_NAME_FIELD){
            ret = create_new_node(_new, _info, condition);
        }
    }
    return ret;
}
#endif 


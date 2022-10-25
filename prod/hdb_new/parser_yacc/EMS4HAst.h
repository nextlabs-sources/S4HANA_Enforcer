//
// Created by jeff on 2020/2/15.
//

#ifndef SQLPARSERTEST_EMS4HAST_H
#define SQLPARSERTEST_EMS4HAST_H

#include "keydef.h"
#include "node.h"
#include "interface.h"
#include "serialize_format.h"
#include <assert.h>
#include <memory>
#include <vector>

std::string get_action_from_ast_type(EMStmtType ty);

#define MASK_HINT_MASKED    "masked"
#define MASK_HINT_NOMASKED  "no_masked"

class EnforcerCtxImpl;
struct EMTableInfo;
class EMS4HAst {
public:
    EMS4HAst(EMStmtType t):_type(t), _presult(NULL), _node(NULL){}
    virtual ~EMS4HAst(){ delete(_presult); }

    virtual std::string Print(){ return ""; }
    void SetResult(ParseResult * presult) {
        _presult = presult;
    }
    void SetNode(Node * pnode) {
        _node = pnode;
    }
    virtual void Parser(Node * /*proot*/, S4HException & /*exc*/){ }
    EMStmtType GetStatementType()  { return _type; }
    virtual bool CheckSupport(){ return  false; }

    virtual std::string Output() const {
        if(_node) {
            return _node->Serialize();
        } else return "";
    }
//    virtual bool RewriteWithMaskStrategy(void* connection, const NxlDBCUserInfo* pUserInfo){
//        return false;
//    }
//    virtual bool RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo){
//        return false;
//    }
    virtual bool RewriteSQL(void* /*connection*/, const NxlDBCUserInfo* /*pUserInfo*/, const NxlDBCResInfo * /*presinfo*/, EnforcerCtxImpl * /*pctx*/) {
        return false;
    }
    virtual bool InterceptSQL(void* /*connection*/, const NxlDBCUserInfo* /*pUserInfo*/, const NxlDBCResInfo * /*presinfo*/) {
        return false;
    }
    virtual void ResetSQL() {}


    virtual bool CollectParam(const int /*index*/, const std::string & /*strparam*/) { return  false; }
    virtual bool CollectParam(const int /*index*/, int64_t /*param*/) { return  false; }
    virtual bool CollectParam(const int /*index*/, uint64_t /*param*/) { return  false; }
    virtual bool GetWhereClauseConditionRightParamId(const std::string & /*column*/, unsigned int & /*markid*/) const { return false;}

    std::vector<EMTableInfo*>& GetAllTables();

private:
    EMStmtType _type;
    ParseResult * _presult;
    std::vector<EMTableInfo*>  _tempAllTables; //cache all tables for performance conside
protected:
    Node * _node;
    
};

class EMColumnInfo {
public:
    EMColumnInfo():_node(NULL),_alias(false),_questmarkid(0),_type(DIRECT_COL){}
    EMColumnInfo(Node * node);
    virtual ~EMColumnInfo();
    Node * _node;
    std::string _db;
    std::string _schema;
    std::string _tb;
    std::string _field;
    bool _alias;
    unsigned  int _questmarkid;
    Node  _old;
    enum ColType{ DIRECT_COL, FUNC_COL ,SUB_COL} _type;
};
class EMFuncColumnInfo : public EMColumnInfo {
public:
    EMFuncColumnInfo(){ _type = FUNC_COL; }
    EMFuncColumnInfo(Node * node):EMColumnInfo(node){ _type = FUNC_COL; }
    virtual  ~EMFuncColumnInfo(){ for(auto it:_params) delete(it); _params.clear(); }
    std::string _func_name;
    std::string _alias_name;
    std::vector<EMColumnInfo*> _params;

};
void reset_columninfo(EMColumnInfo * pinfo) ;

class ViewInfo;
class ViewSourceInfo;

enum EMTableType{
    EMTABLE_NATRUAL = 0,
    EMTABLE_VIEW,
    EMTABLE_FUNC
};

struct EMTableInfo {
    EMTableInfo():_node(NULL),_viewsource(NULL),_type(EMTABLE_NATRUAL),_viewTableInfo(NULL){}
    EMTableInfo(Node * node):_node(node),_type(EMTABLE_NATRUAL),_viewTableInfo(NULL){}
    Node * _node;
    std::string _db;
    std::string _schema;
    std::string _tb;
    std::string _alias;
    std::shared_ptr<ViewSourceInfo> _viewsource;
    EMTableType _type;
    EMTableInfo* _viewTableInfo;  //when this is not NULL, this point its view object.  
};


    struct UpdateData
    {
      SQLDBC_HostType _type;
      struct Data{  //didn't use union!!!!!, it will crash . use struct as easy and it will not cost much memory
            std::string _strData;
            int64_t     _int64Data = 0;
            uint64_t    _uint64Data = 0;   
       }_Data;

       bool Equal(const std::string& strMaskSymbol);

    };

struct EMUpdateColInfo/*nonsupport copy*/ {
    EMUpdateColInfo():_questmarkid(0), _ismaskparam(false){}
    ~EMUpdateColInfo(){  }
    EMColumnInfo _info;
    std::string _col;
    unsigned int _questmarkid;
    std::map<std::string, MaskSymbols> _map_masksymbol;//< *** ,  (***/type)>
    bool _ismaskparam;// occur *** when bind param

    Node  _old;
    Node  _new;
  
    UpdateData _UpdateData;

    //bool UpdateColumnControl(const std::string & condition);
};

bool create_old_node(Node & old, EMColumnInfo & info);
bool create_new_node(Node & new_, EMColumnInfo & info);
bool create_new_node(Node & new_, EMColumnInfo & info, const std::string& originalValueStr, const std::string& strMaskCondition);

EMS4HAst * parser_stmt(Node * node, S4HException & exc) ;

void parser_name_field(Node* pnode, EMColumnInfo & info, S4HException & exc);
void parser_alias(Node* pnode, EMColumnInfo & info, S4HException & exc);
void parser_name_table(Node* pnode, EMTableInfo & info, S4HException & exc);
bool parser_function(Node* pnode, EMFuncColumnInfo & info, S4HException & exc);


EMS4HAst * create_ast(const std::string & sql, S4HException & exc);
EMS4HAst * create_ast(const std::u16string & sql, S4HException & exc);


struct EMWhereClauseInfo{
    EMWhereClauseInfo():_node(NULL),_root(NULL),_bevaluation(false){}
    Node * _node;
    Node * _root;
    bool _bevaluation;

    bool GetConditionRightParamId(const std::string & column, unsigned int & markid) const;

    bool ResetNode(){
        if(!_bevaluation)
            return false;
        assert(_root);
        if (!_node) {
            switch (_root->nodeType_) {
                case E_SELECT: {
                    delete(_root->getChild(E_SELECT_OPT_WHERE));
                    _root->setChild(E_SELECT_OPT_WHERE, NULL);
                } break;
                case E_UPDATE: {
                    delete(_root->getChild(E_UPDATE_OPT_WHERE));
                    _root->setChild(E_UPDATE_OPT_WHERE, NULL);
                } break;
                case E_DELETE: {
                    delete(_root->getChild(E_DELETE_OPT_WHERE));
                    _root->setChild(E_DELETE_OPT_WHERE, NULL);
                } break;
                default:
                    assert(false);  /* unreachable */
                    break;
            }
        } else {
            Node* and_expr = _node->getChild(E_WHERE_CLAUSE_EXPR);
            assert(and_expr);
            if(and_expr->GetType() != E_OP_AND)
                return false;
            Node * ori_node_with_p = and_expr->getChild(0);
            assert(ori_node_with_p);
            Node * ori = ori_node_with_p->getChild(0);
            assert(ori);
            ori_node_with_p->setChild(0, NULL);
            _node->setChild(E_WHERE_CLAUSE_EXPR, ori);
            //Node * node_condition = and_expr->getChild(1);
            delete(and_expr);

        }
        _bevaluation =  false;
        return  true;
    }
    bool EvaluationSQL(const std::string & condition){
        assert(_root);
        if(condition.empty()) return false;
        if (!_node) {
            Node* new_expr = Node::makeTerminalNode(E_STRING, condition);
            Node* where = Node::makeNonTerminalNode(E_WHERE_CLAUSE, E_WHERE_CLAUSE_PROPERTY_CNT, new_expr);
            where->serialize_format = &WHERE_SERIALIZE_FORMAT;
            switch (_root->nodeType_) {
                case E_SELECT: {
                    _root->setChild(E_SELECT_OPT_WHERE, where);
                } break;
                case E_UPDATE: {
                    _root->setChild(E_UPDATE_OPT_WHERE, where);
                } break;
                case E_DELETE: {
                    _root->setChild(E_DELETE_OPT_WHERE, where);
                } break;
                case E_UPSERT: {
                    _root->setChild(3, where);
                } break;
                default:
                    assert(false);  /* unreachable */
                    break;
            }
        } else {
            Node* ori_expr = _node->getChild(E_WHERE_CLAUSE_EXPR);
            Node* ori_expr_paren = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, ori_expr);
            ori_expr_paren->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
            Node* cond = Node::makeTerminalNode(E_STRING, condition);
            Node* new_expr = Node::makeNonTerminalNode(E_OP_AND, E_OP_BINARY_PROPERTY_CNT, ori_expr_paren, cond);
            new_expr->serialize_format = &OP_AND_SERIALIZE_FORMAT;
            _node->setChild(E_WHERE_CLAUSE_EXPR, new_expr);
        }
        _bevaluation = true;
        return true;
    }
};






#endif //SQLPARSERTEST_EMS4HAST_H

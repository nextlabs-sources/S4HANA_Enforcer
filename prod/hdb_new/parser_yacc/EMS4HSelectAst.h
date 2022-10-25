//
// Created by jeff on 2020/2/20.
//

#ifndef SQLPARSERTEST_EMS4HSELECTAST_H
#define SQLPARSERTEST_EMS4HSELECTAST_H

#include "EMS4HAst.h"
#include "EMMaskDef.h"

class EMS4HSelectAst : public EMS4HAst {
public:
    EMS4HSelectAst():EMS4HAst(E_STMT_TYPE_SELECT),_support_select_star(true),_parent(NULL),_select_list_node(NULL){}
    EMS4HSelectAst(EMS4HAst * parent):EMS4HAst(E_STMT_TYPE_SELECT),_support_select_star(true),_parent(parent){}
    virtual ~EMS4HSelectAst();

    virtual std::string Print(){return "";};

    //parameter questmarkid mean is first questmarkid from from...where.....
    void CollectQuestMark_for_key_mask(Node * node,  std::vector<int> & questmarkids);

    virtual bool RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    virtual void ResetSQL();

    virtual bool CheckSupport();

    void Parser(Node * pnode, S4HException & exc);
    void ParserColumns(Node* node, S4HException & exc);
    void ParserTable(Node* node, S4HException & exc);
    void ParserGroupBy(Node* node, S4HException & exc);
    //for where clause
    void ParseWhereClause(Node * pnode, S4HException & exc);
    virtual bool GetWhereClauseConditionRightParamId(const std::string & column, unsigned int & markid) const;
    std::string GetWhereClause() { return _whereclause._node->serialize(); }

    const std::vector<EMTableInfo*> & GetTables() const{ return _tables; }
    const std::vector<EMColumnInfo*> & GetColumns() const{ return _fields; }

    void AddedMatchedMaskItem(const std::string& strTable, const MaskItem* pMask);
    void ClearMatchedMaskItem() { _matchedMaskItem.clear(); }
    const std::vector<MaskItem>& GetMatchedMaskItem() const{ return _matchedMaskItem; } 
    std::string ConstructQueryExprSQL();
    std::string ConstructMaskCountSql();
private:
    bool RewriteWithMaskStrategy(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo = NULL);
    bool RewriteWithMaskConditionMap(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    bool RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);


    std::vector<EMTableInfo*>  _tables;// from tbname
    std::vector<EMColumnInfo*> _fields;// tbname.field
    EMWhereClauseInfo _whereclause;
    std::vector<EMColumnInfo*> _groupby_list; // group by
    bool _support_select_star;
    EMS4HAst * _parent;
    Node * _select_list_node;

    std::vector<MaskItem> _matchedMaskItem; // for notify

public:
    friend bool mask_fields_v5(EMS4HAst * ast, void* connection, const MaskConditionMap *pmap_mask, const EnforcerCtxImpl * pctx, S4HException & exc);
    friend void insert_maskdata_into_cachetable(const std::string & sql, EMColumnInfo * field, const MaskItem * pmask, void * conn, const EnforcerCtxImpl* pctx, S4HException & exc);
    friend bool mask_one_field(EMColumnInfo* field, EMS4HSelectAst * pselect, void* connection, const MaskConditionMap *pmap_mask, const EnforcerCtxImpl * pctx, S4HException & exc);
};

class EMConstructTable{
public:
    ~EMConstructTable();
    void Parser(Node * node, S4HException & exc);
    std::string _name;
    int _questmarkid;
    std::vector<EMColumnInfo*> _columns;
};

#endif //SQLPARSERTEST_EMS4HSELECTAST_H

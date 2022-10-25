//
// Created by jeff on 2020/2/23.
//

#ifndef SQLPARSERTEST_EMS4HUPDATEAST_H
#define SQLPARSERTEST_EMS4HUPDATEAST_H

#include <interface.h>
#include "node.h"
#include "EMS4HAst.h"

class EMS4HUpdateAst : public EMS4HAst {
public:
    EMS4HUpdateAst():EMS4HAst(E_STMT_TYPE_UPDATE){}
    virtual ~EMS4HUpdateAst();
    virtual std::string Print(){return "";};


    virtual bool CollectParam(const int index, const std::string & strparam) ;
    virtual bool CollectParam(const int index, int64_t param);
    virtual bool CollectParam(const int index, uint64_t param);

    virtual std::string Output() const;
    virtual bool CheckSupport();


    virtual void Parser(Node * proot, S4HException & exc);

    virtual bool RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    virtual bool RewriteSQLForDenyUpdate(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    virtual void ResetSQL();

    virtual bool GetWhereClauseConditionRightParamId(const std::string & column, unsigned int & markid) const;

    //virtual GetNode() = 0;
    EMTableInfo * GetTable(){ return &_table;}
    std::vector<EMUpdateColInfo*> & GetColumns(){ return _fields;}
private:
    void ParserTable(Node * proot, S4HException & exc);
    void ParserColumns(Node * proot, S4HException & exc);
    void ParseWhereClause(Node * pnode, S4HException & exc);

    //virtual bool RewriteSQLV1(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);
    
    bool RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);
    bool RewriteSQLV2_UpdateToOtherTable(void *connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo, const EnforcerCtxImpl *pCtx);
    void ConstructUpdateRelation(const std::string& strRelationTemplete, std::string& strRelation,
                                             std::list<int>& questionParamPosition);

    EMTableInfo _table;
    std::vector<EMUpdateColInfo*> _fields;// tbname.field
    EMWhereClauseInfo _whereclause;

};


#endif //SQLPARSERTEST_EMS4HUPDATEAST_H

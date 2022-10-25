//
// Created by jeff on 2020/7/6.
//

#ifndef PROD_EMS4HUPSERTAST_H
#define PROD_EMS4HUPSERTAST_H

#include <string>
#include <vector>
#include <interface.h>
#include "node.h"
#include "EMS4HAst.h"

struct UpCol{
    std::string _col;
    bool _deny;
    Node * _node;
    UpdateData _bind_data;
};

class EMS4HUpsertAst : public EMS4HAst {
public:
    EMS4HUpsertAst():EMS4HAst(E_STMT_TYPE_UPSERT),_subq(NULL){}
    virtual ~EMS4HUpsertAst(){  }

    virtual std::string Print(){ return ""; }
    virtual void Parser(Node * proot, S4HException & exc);
    virtual bool CheckSupport();
    virtual bool InterceptSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);

    virtual bool CollectParam(const int index, const std::string & strparam) ;
    virtual bool CollectParam(const int index, int64_t param);
    virtual bool CollectParam(const int index, uint64_t param);
    
    EMTableInfo *  GetTable(){ return &_table;}
    virtual std::string Output() const;

    virtual bool RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    bool RewriteSQLForDenyUpsert(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    virtual void ResetSQL();
    bool IsDenyParamIndex(unsigned int );

    bool GetUpsertColumnParamId( const std::string & column,  void *conn, unsigned int & markid) ;
private:
    int GetValueParamId(const std::string & field);
    int GetValueParamId(int index);
    bool RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);

    bool RewriteSQL_UpsertToOtherTable(void *connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo, const EnforcerCtxImpl *pCtx);
    void ConstructUpdateRelation(void* connection, const std::string& strRelationTemplete, std::string& strRelation,
                                             std::list<int>& questionParamPosition);

    EMTableInfo _table;
    std::vector<UpCol> _vec_col;
    std::vector<UpCol> _vec_val;
    std::vector<unsigned int> _deny_parm_index;
    std::string _primary;
    std::string _where;
    Node * _subq;
    EMWhereClauseInfo _whereclause;
};


#endif //PROD_EMS4HUPSERTAST_H

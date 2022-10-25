//
// Created by jeff on 2020/3/10.
//

#ifndef SQL_PARSE_EMS4HDELETEAST_H
#define SQL_PARSE_EMS4HDELETEAST_H

#include "EMS4HAst.h"
#include "TableDataCache.h"

class EMS4HDeleteAst :public EMS4HAst{
public:
    EMS4HDeleteAst():EMS4HAst(E_STMT_TYPE_DELETE){}
    virtual ~EMS4HDeleteAst(){  }

    virtual std::string Print(){ return ""; }
    virtual void Parser(Node * proot, S4HException & exc);
    virtual bool CheckSupport();
    virtual bool RewriteSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    virtual void ResetSQL();
    EMTableInfo *  GetTable(){ return &_table;}
    void CacheDeleteData(void* connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo,
                         EnforcerCtxImpl* pctx);

    static TableDataCache& GetCachedData() { return _tableDataCacheForDelete; }                    

private:
    void ParseWhereClause(Node * pnode, S4HException & exc);
    bool RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);

    EMTableInfo _table;
    EMWhereClauseInfo _whereclause;

    static TableDataCache _tableDataCacheForDelete; //cache the deleted item
};


#endif //SQL_PARSE_EMS4HDELETEAST_H

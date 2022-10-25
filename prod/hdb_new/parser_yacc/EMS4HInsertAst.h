//
// Created by jeff on 2020/3/10.
//

#ifndef SQL_PARSE_EMS4HINSERTAST_H
#define SQL_PARSE_EMS4HINSERTAST_H

#include "EMS4HAst.h"

class EMS4HInsertAst :public EMS4HAst{
public:
    EMS4HInsertAst():EMS4HAst(E_STMT_TYPE_INSERT){}
    virtual ~EMS4HInsertAst(){  }

    virtual std::string Print(){ return ""; }
    virtual void Parser(Node * proot, S4HException & exc);
    virtual bool CheckSupport();
    virtual bool InterceptSQL(void* connection, const NxlDBCUserInfo* pUserInfo, const NxlDBCResInfo * presinfo);
    void RestoreFromDeleteData(EnforcerCtxImpl *pctx, void *connection, const NxlDBCUserInfo *pUserInfo, const NxlDBCResInfo *presinfo);
    EMTableInfo *  GetTable(){ return &_table;}
    bool GetInsertColumnParamId( const std::string & column,  void *conn, unsigned int & markid) ;
private:
    int GetValueParamId(const std::string & field);
    int GetValueParamId(int index);

    EMTableInfo _table;
};


#endif //SQL_PARSE_EMS4HINSERTAST_H

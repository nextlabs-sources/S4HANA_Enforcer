//
// Created by jeff on 2020/4/14.
//

#ifndef PROD_EMS4HMUTLISELECTAST_H
#define PROD_EMS4HMUTLISELECTAST_H

#include "EMS4HAst.h"

class EMS4HMutliSelectAst : public EMS4HAst {
public:
    EMS4HMutliSelectAst():EMS4HAst(E_STMT_TYPE_SELECT),_node(NULL){}
    virtual ~EMS4HMutliSelectAst(){ for(auto it:_selects) delete(it); };

    virtual std::string Print();
    virtual bool RewriteSQL(void* connection, const NxlDBCUserInfo* puserinfo, const NxlDBCResInfo * presinfo, EnforcerCtxImpl * pctx);
    virtual void ResetSQL();
    virtual bool CheckSupport();

    void AddAst(EMS4HAst* ast){ _selects.push_back(ast); }
    void AddCte(std::string & cte){ _ctes.insert(cte); }
    bool FindCte(std::string & tb){  return _ctes.find(tb)!=_ctes.end();   }

    void Parser(Node * pnode, S4HException & exc);

private:
    EMS4HAst * _main_stmt;
    std::vector<EMS4HAst*> _selects;  // format: with col as (select1) select2 from select3 ....   but we only collect statment with 'from table' is exactly
    std::set<std::string> _ctes;    // cte not is table
    Node * _node;

};


#endif //PROD_EMS4HSELECTASTV2_H

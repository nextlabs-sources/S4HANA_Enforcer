//
// Created by jeff on 2020/5/14.
//

#ifndef SQL_PARSE_EMTABLEEXPR_H
#define SQL_PARSE_EMTABLEEXPR_H


#include "view_interface.h"

class EMTableExpr_Ident : public ITableExpr{
public:
    ITableExpr::Type GetType(){ return  ITableExpr::TB_IDENT;}

    EMTable _table;
};

class EMTableExpr_Select : public ITableExpr{
public:
    EMTableExpr_Select():_stmt(NULL){}
    ITableExpr::Type GetType(){ return  ITableExpr::TB_SUBQUERY;}

    IViewStmt * _stmt;
    std::string _alias;
};



#endif //SQL_PARSE_EMTABLEEXPR_H

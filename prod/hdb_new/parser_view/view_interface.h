//
// Created by jeff on 2020/5/13.
//

#ifndef SQL_PARSE_VIEW_INTERFACE_H
#define SQL_PARSE_VIEW_INTERFACE_H

#include <string>
#include <set>
#include <map>
#include "../parser_yacc/parser_interface.h"
#include "s4hexception.h"
#include <assert.h>
#include "ViewManager.h"

typedef unsigned int SId;


struct EMTable{
    std::string _name;
    std::string _schema;
    std::string _db;
    std::string _alias;
    bool _isview;
};
std::string construct_key(const EMTable & tb);
void resolve_key(std::string & schema, std::string & table, const std::string & src);
struct EMColumn{
    std::string _name;
    std::string _tb;
    std::string _alias;
    bool _star;
};
std::string get_view_column(const EMColumn & col);

class IColumnExpr{
public:
    virtual ~IColumnExpr(){}
public:
    enum ColType{COL_IDENT,COL_SUBQUERY,COL_CASEWHEN,COL_TODO};
    virtual IColumnExpr::ColType GetType()=0;
//    virtual bool HavAlias()=0;
//    virtual std::string GetAlias()=0;

};

class ITableExpr{
public:
    virtual ~ITableExpr(){}
public:
    enum Type{TB_IDENT,TB_SUBQUERY,TB_JOIN,TB_TODO};
    virtual ITableExpr::Type GetType()=0;
//    virtual bool HavAlias()=0;
//    virtual std::string GetAlias()=0;
};

class IViewStmt{
public:
    virtual ~IViewStmt(){}
public:
    enum StmtType{ST_SELECT,ST_CTE,ST_TODO};
    virtual StmtType GetType()=0;
    virtual void GetAllTableName(std::vector<EMTable> & tables) = 0;
    virtual const ViewSourceInfo& GetViewSources(void *pconn, S4HException & exc)=0;
};


class ParseMgr;
IViewStmt * parser_view(const std::string &sql, ParseMgr * mgr, S4HException & exc);





#endif //SQL_PARSE_VIEW_INTERFACE_H

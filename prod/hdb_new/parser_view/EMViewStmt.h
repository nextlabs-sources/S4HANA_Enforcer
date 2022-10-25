//
// Created by jeff on 2020/5/14.
//

#ifndef SQL_PARSE_EMSELECTSTMT_H
#define SQL_PARSE_EMSELECTSTMT_H

#include <vector>
#include "view_interface.h"



class EMViewStmt :public  IViewStmt{
public:
    ~EMViewStmt();
    IViewStmt::StmtType GetType(){ return _cte.empty()?IViewStmt::ST_SELECT:IViewStmt::ST_CTE; };
    virtual void GetAllTableName(std::vector<EMTable> & tables);
public:
    std::string _cte;
    std::map<std::string, IViewStmt*> _ctes;
    std::vector<EMColumn*> _columns;
    std::vector<ITableExpr*> _tables;
public:
    void ParserViewSources(void *pconn, S4HException & exc);
    const ViewSourceInfo& GetViewSources(void *pconn, S4HException & exc);
    ViewSourceInfo _info;
};


#endif //SQL_PARSE_EMSELECTSTMT_H

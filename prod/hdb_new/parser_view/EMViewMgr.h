//
// Created by jeff on 2020/5/14.
//

#ifndef SQL_PARSE_EMPARSERRESULTMGR_H
#define SQL_PARSE_EMPARSERRESULTMGR_H

#include "view_interface.h"
#include "../parser_yacc/node.h"

class ParseMgr{
public:
    ParseMgr():_p(NULL){}
    ~ParseMgr();
    SId InsertStmt(IViewStmt * st);
    IViewStmt* GetStmt(SId id);
    void SetParserResult(ParseResult * p);
private:
    ParseResult * _p;
    std::vector<IViewStmt*> _stmts;
};


#endif //SQL_PARSE_EMPARSERRESULTMGR_H

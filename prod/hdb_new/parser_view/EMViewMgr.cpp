//
// Created by jeff on 2020/5/14.
//

#include "EMViewMgr.h"


ParseMgr::~ParseMgr(){
    for(auto st:_stmts){
        delete(st);
    }
    if(_p){
        delete(_p);
    }
}
SId ParseMgr::InsertStmt(IViewStmt * st){
    _stmts.push_back(st);
    return _stmts.size()-1;
}
IViewStmt* ParseMgr::GetStmt(SId id){
    if(_stmts.size()<=id)
        return NULL;
    return _stmts[id];
}

void ParseMgr::SetParserResult(ParseResult * p){
    _p = p;
}
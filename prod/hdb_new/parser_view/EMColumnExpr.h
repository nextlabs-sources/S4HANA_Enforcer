//
// Created by jeff on 2020/5/14.
//

#ifndef SQL_PARSE_EMCOLUMNEXPR_H
#define SQL_PARSE_EMCOLUMNEXPR_H

#include "view_interface.h"
#include <vector>

class EMColExpr_Ident : public IColumnExpr{
public:
    ~EMColExpr_Ident();
    IColumnExpr::ColType GetType(){ return IColumnExpr::COL_IDENT; };
    bool HavAlias(){ return !_alias.empty();};
public:
    std::vector<EMColumn*> _cols;
    std::string _alias;



};

class EMColExpr_Select : public IColumnExpr{
public:
    IColumnExpr::ColType GetType(){ return IColumnExpr::COL_SUBQUERY; };
    bool HavAlias(){ return false;};
    std::string GetAlias(){ return "";};

};


#endif //SQL_PARSE_EMCOLUMNEXPR_H

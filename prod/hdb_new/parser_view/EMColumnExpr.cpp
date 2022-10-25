//
// Created by jeff on 2020/5/14.
//

#include "EMColumnExpr.h"

EMColExpr_Ident::~EMColExpr_Ident(){
    for(auto it: _cols){
        delete(it);
    }
    _cols.clear();
}
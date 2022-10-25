//
// Created by jeff on 2020/5/26.
//

#ifndef PROD_KEYMASKOPR_H
#define PROD_KEYMASKOPR_H


#include "s4hexception.h"
#include "EMMaskDef.h"
#include "EnforcerCtxImpl.h"
//limitation :  sql need is simple sql: select ... from ... where.. group....
void insert_maskdata_into_cachetable(const std::string & sql, EMColumnInfo * field, const MaskItem * pmask, void * conn, const EnforcerCtxImpl* pctx, S4HException & exc);


#endif //PROD_KEYMASKOPR_H

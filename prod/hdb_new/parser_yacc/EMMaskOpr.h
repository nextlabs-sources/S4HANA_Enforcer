//
// Created by jeff on 2020/3/18.
//

#ifndef EMDB_MASK_OPR_H
#define EMDB_MASK_OPR_H

#include "node.h"
#include "EMS4HAst.h"
#include "EMMaskDef.h"

bool mask_fields_v5(EMS4HAst * ast, void* connection, const MaskConditionMap *pmap_mask, const EnforcerCtxImpl * pctx, S4HException & exc);
#endif //EMDB_MASK_OPR_H

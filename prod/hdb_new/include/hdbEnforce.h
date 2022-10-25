#ifndef HDB_ENFORCE_H
#define HDB_ENFORCE_H
#include "SQLDBC.h"
#include "../parser_yacc/EMS4HUpdateAst.h"
#include "../parser_yacc/EMS4HUpsertAst.h"
#include "EnforcerCtxImpl.h"

class HdbEnforce
{
    public:
   static void ReplaceKeyMastValueWithOriginalValue(SQLDBC::SQLDBC_PreparedStatement* pPreparedStmt, EnforcerCtxImpl* pCtx);

   private:
   static void CollectParamToAST(EnforcerCtxImpl* pCtx, EMS4HAst* ast);


};

#endif

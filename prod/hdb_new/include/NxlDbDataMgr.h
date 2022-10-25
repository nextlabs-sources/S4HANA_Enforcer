#ifndef NXL_DB_DATA_MGR_H
#define NXL_DB_DATA_MGR_H
#include <stdio.h>
#include <string>
#include "SQLDBC.h"
#include <vector>
#include "s4hexception.h"
#include "NXS4HConfig.h"

//#define NXL_SCHEMA  "SAPHANADB"
//#define KEYMASK_CACHE_TABLE "KEYMASK_CACHE"
//#define KEYMASK_FUNCTION    "KEY_MASK"

#define RECORD_SQL_PARSE_ERROR 0
#define RECORD_SQL_TABLE "/NXLDAE/SQL_PARSE_ERROR"

class EnforcerCtxImpl;

class NxlDBDataMgr
{
    public:
    static NxlDBDataMgr* Instance(){
        static NxlDBDataMgr* pIns = NULL;
        if (pIns==NULL){
            pIns = new NxlDBDataMgr();
        }
        return pIns;
    }

    bool IsInit() { return _bInit;}
    void Init(SQLDBC::SQLDBC_Connection* pConn);

    bool IsKeyMaskData(const std::string& strValue);
    std::string GetOriginalValuByMaskValue(SQLDBC::SQLDBC_Connection* pConn, const std::string& strValue);

    const std::string  PrivateDataSchema() { return g_conf->GetPRIDATASchema(); }
    const std::string  KeyMaskCacheTableName() { return g_conf->GetPRIDATAKeymaskTable(); }
    const std::string  KeyMaskFunctionName() { return g_conf->GetPRIDATAKeymaskFunc(); }
    const std::string&  KeyMaskCacheTableFullName() { return _strKeyMaskCacheTableFullName; }
    const std::string&  KeyMaskFunctionFullName() { return _strKeyMaskFunctionFullName; }

    bool KeyMaskCacheTableExist() { return _bKeyMaskCacheTableExist; }
    bool KeyMaskFunctionExist() { return _bKeyMaskFunctionExist; }

    const std::string& KeyMaskMaskFormat() { return _strKeyMaskMaskFormat; }
    const std::string& KeyMaskPrefix() {  return _strKeyMaskPrefix; }
   
    void RecordSQLParseError(SQLDBC::SQLDBC_Connection* pConn, const std::string& sqlInfo);


   private:
   NxlDBDataMgr();


   private:
   bool CreateSchema(SQLDBC::SQLDBC_Statement* pStmt);
   bool CreateKeyMaskCacheTable(SQLDBC::SQLDBC_Statement* pStmt);
   bool CreateKeyMaskFunction(SQLDBC::SQLDBC_Statement* pStmt);
   bool CreateSQLParseErrorTable(SQLDBC::SQLDBC_Statement* pStmt);

public:
    void Execute(const std::string & sql, const std::vector<int> & indexs, void * conn, const EnforcerCtxImpl * pctx, S4HException & exc);
   

private:
   bool _bInit;
   
   std::string _strKeyMaskCacheTableFullName;  //schema.table
   std::string _strKeyMaskFunctionFullName; //schema.function

   bool _bKeyMaskCacheTableExist; //is key mask cache table exist
   bool _bKeyMaskFunctionExist; //is key mask cache function exist

   std::string _strKeyMaskMaskFormat;
   std::string _strKeyMaskPrefix;
};

#endif 
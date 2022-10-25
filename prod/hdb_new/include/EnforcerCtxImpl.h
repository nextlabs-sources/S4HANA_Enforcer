#ifndef ENFORCER_CTX_IMPL_H
#define ENFORCER_CTX_IMPL_H

#include "interface.h"
#include "../parser_yacc/EMS4HAst.h"
#include <map>
#include <memory>
class EMS4HAst;


EnforcerCtxImpl *register_enforcer_ctx(void *original_stmt, std::shared_ptr<EMS4HAst> ast);
EnforcerCtxImpl *get_enforcer_ctx(void *original_stmt);
void remove_enforcer_ctx(void *original_stmt);

    class EnforcerCtxImpl;

enum CALLED_API{
    API_NONE=0,
    API_BIND_PARAMETER_ADDR=1,
    API_SET_BIND_TYPE=2,
    API_SET_BATCH_SIZE=4,
};
    class BindParameters {
    public:
        BindParameters(const SQLDBC_UInt4 Index,
                       const SQLDBC_HostType Type,
                       void *paramAddr,
                       SQLDBC_Length *LengthIndicator,
                       const SQLDBC_Length Size,
                       const SQLDBC_Bool Terminate);

        ~BindParameters();

    public:
        const SQLDBC_UInt4 _Index;
        const SQLDBC_HostType _Type;
        void*_paramAddr;
        SQLDBC_Length *_LengthIndicator;
        SQLDBC_Length _Size;
        const SQLDBC_Bool _Terminate;
        bool _bBindWithNewValue; //if _paramAddr point to a value allocated by DAE. so need to free it.
        friend class EnforcerCtxImpl;
    };

    class EnforcerCtxImpl  {
    public:
        EnforcerCtxImpl(void *original_stmt, std::shared_ptr<EMS4HAst> ast);
        ~EnforcerCtxImpl();

        void RecordParameter(const SQLDBC_UInt4 Index,
                                     const SQLDBC_HostType Type,
                                     void *paramAddr,
                                     SQLDBC_Length *LengthIndicator,
                                     const SQLDBC_Length Size,
                                     const SQLDBC_Bool Terminate);

        bool EnforcedStmtBindParameters(S4HException &e);
        bool OriginStmtBindParameters(SQLDBC::SQLDBC_PreparedStatement* pStmt, S4HException& e);
        bool ReplaceParamValueByPosition(uint iRow, unsigned int position, const std::string& strParamValue);

        void *GetOriginalStmt()  { return _original_stmt; }

        void SetEnforcedStmt(void *s)  { _enforced_stmt = (SQLDBC::SQLDBC_PreparedStatement *) s; }

        void *GetEnforcedStmt()  { return _enforced_stmt; }

       // std::u16string GetOriginalSql();
       // void SetOriginalSql(const std::u16string & sql);

       // const std::string& GetOriginalSqlU8() const { return _original_sql_u8; }
       // void SetOriginalSqlU8(const std::string & sql) {  _original_sql_u8 = sql; }

        //void SetOriginalStmtReprepared(bool b) { _original_stmt_reprepared = b;}
       // bool IsOriginalStmtReprepared() { return _original_stmt_reprepared; }
        void AllocateOriginalValueBuf(const std::u16string& strOriginalValue, void** pBuf, long long* /*SQLDBC_Length*/ pLen);
        
        bool GetParamValueByPosition(uint iRow, uint position, std::string& strValue);
        BindParameters* GetBindParamByPosition(uint position) const ;

        void SetAst( std::shared_ptr<EMS4HAst> past) { _past = past; }
         std::shared_ptr<EMS4HAst> GetAst() { return _past; }

         const std::string& GetPreviousNewSql() { return _previous_new_sql; }
         void SetPreviousNewSql(const char* szNewSql) { _previous_new_sql = szNewSql;}

         bool IsEnforced() { return _bEnforced; }
         void SetEnforced(bool bEnforce) { _bEnforced=bEnforce; } 

        void SetBatchSize(SQLDBC_UInt4 unBatchSize  ) { _unBatchSize=unBatchSize; }
        const unsigned int GetBatchSize() const { return _unBatchSize==0 ? 1 : _unBatchSize; }

         bool CanNotSupported() { return (_uCalledAPI&API_BIND_PARAMETER_ADDR); }
         void SetCAlledAPI(CALLED_API calledAPI) {_uCalledAPI = _uCalledAPI | calledAPI ;  }
         unsigned long GetCalledAPI() { return _uCalledAPI; }

    private:
    void AddReplacedParameterBuf(char* pBuf);
    void FreeReplacedParameterBuf();

    public:
        std::map<SQLDBC_UInt4, BindParameters *> _bind_parameters;
       // static unsigned long long _uSizeAll;
    private:
        void *_original_stmt;
        //bool _original_stmt_reprepared;
        //std::u16string _original_sql; 
        //std::string _original_sql_u8;
        SQLDBC::SQLDBC_PreparedStatement *_enforced_stmt;
       // EMStmtType _sqltype;
       std::shared_ptr<EMS4HAst>  _past;
       std::string _previous_new_sql;
       bool _bEnforced;
       SQLDBC_UInt4 _unBatchSize=0; //init with 0, if not 0, then we need call SetBatchSize with Enforced Statement

       unsigned long _uCalledAPI;

        //in KEY_MASK feature, we will replace the parameter from *_XXXX with the original value.
        // the new parameter need to be freed, so cache here
        std::list<char*> _replacedParametersBuf; 
        
    };

    extern std::map<void*, EnforcerCtxImpl*> BINDINGS;

#endif
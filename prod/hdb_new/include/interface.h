#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include <set>
#include "SQLDBC.h"
#include <map>
#include "ems4h_datatype.h"
#include "SqlDbcUserInfoMgr.h"
#include "SqlDbcResourceInfoMgr.h"
#include "../parser_yacc/keydef.h"
#include "s4hexception.h"

//#define QUERY_POLICY_LOCAL_CC
/*ALL_TABLE_SYMBOLS instead all table in mask obligation*/
#define ALL_TABLE_SYMBOLS "*"

#ifdef _WIN32
    typedef unsigned int uint;
#endif


/*
 * x86_64  SUSE  OK
 * ppc
 * Currently, we can hook the 6 api of libSQLDBC on cpu x86_64 and os SUSE
 * */
class IHook {
public:
    enum HOOK_TYPE { HOOK_BY_SUBHOOK, HOOK_BY_FUNCHOOK /* todo extension */ };
    virtual ~IHook() { }
    virtual int Prepare(void **old_addr, void *new_addr){}
    virtual int Install(){};
};

IHook *create_hook(IHook::HOOK_TYPE);

/*
 * map the terminal user
 * Currently, we can't get the terminal user
 * */

/*
 * this module need to use the current connection and hana db sql to fetch the table or view's column name and type
 * need 1 days
 * */
class ITableMetaData {
public:
    virtual ~ITableMetaData() { }
    virtual std::string GetTableName() const = 0;
    virtual bool HasColumn(const std::string& column_name) const = 0;
    virtual bool IsColumnCompatibleWithString(const std::string& column_name) const = 0;
    virtual EM_SAP_DataType GetColumnDataType(const std::string& column_name) const = 0;
    virtual void GetAllColumns(std::vector<std::string>& vecCol) const = 0;
    virtual std::vector<std::string> GetAllKeyColumns() const = 0;
    virtual std::string Print()const = 0;
};


/*
 * this module need to run queryPC cpp sdk on linux
 * */

class MaskSymbols{
public:
    MaskSymbols():_type(EM_SAP_VARCHAR){
    }
    ~MaskSymbols(){}
    void SetSymbols(EM_SAP_DataType tp, std::string value){
        _type = tp;
        _symbols = value;
    }

    EM_SAP_DataType _type;
    std::string _symbols;
};


class IMaskStrategy {
public:
    virtual ~IMaskStrategy() {}
    virtual std::string GetMaskedColumn() const = 0;
    virtual std::map<std::string, int> GetConditionColumnAndOp() = 0;
    //virtual std::u16string MakeExpression(const std::u16string& scope, const std::u16string& alias) = 0;
    virtual std::u16string MakeExpressionV2(const std::set<std::u16string> & vec_col, const std::u16string& scope, const std::u16string& alias) = 0;
    virtual void SetMaskSymbolsType(EM_SAP_DataType type) = 0;

    /*support format: table.maskfield */
    virtual const std::string & GetMaskedTable() const = 0;
    virtual void  SetMaskedTable(const std::string & table ) = 0;

    /*support new obligation */
    virtual const std::set<std::string> & GetMaskedTables() const = 0;
    virtual void  SetMaskedTables(const std::set<std::string> & table ) = 0;

    virtual EM_SAP_DataType GetMaskConditionColumnType(const std::string & column) const = 0;
    virtual void SetMaskConditionColumnType(const std::string & column, EM_SAP_DataType type) = 0;

    //add by jeff
    virtual MaskSymbols GetMaskSymbols() = 0;
    virtual void SetMaskSymbols(MaskSymbols & sym) = 0;

    virtual std::string Print()const= 0;
};
typedef  std::vector<IMaskStrategy*> IMaskStrategyVec;

std::vector<IMaskStrategy*> * query_pc_masks( const std::string & action, const NxlDBCUserInfo* pUserInfo, S4HException & exc);
void query_pc_tables(std::map<std::string , std::set<std::string>*> & tbmap, S4HException & exc);
//void query_pc_tables(std::set<std::string> & tables, S4HException & exc);
void query_pc_fields(std::vector<std::set<std::string>> & fieldset, S4HException & exc);//mask fields and alias


/*
 * this module is the rewrite sql module
 * contains a simple hana sql lexer conponent, parser component, and apply mask strategy component
 * */
namespace NXS4H {
    
    
    class IAstStmt {
    public:
        virtual ~IAstStmt() { }
        /*
         *
         * */
        virtual bool CheckSupport() = 0;
        /*
         * return : is the ast really modified
         * */
        virtual bool RewriteWithMaskStrategy(void* connection, const NxlDBCUserInfo* pUserInfo) = 0;
        virtual bool RewriteWithFilterCondition(void* connection, const NxlDBCUserInfo* pUserInfo) = 0;
        virtual std::u16string Output() const = 0;
        virtual std::string Print()const = 0;

        //for update
        virtual void ResetSQL() = 0;
        virtual bool ModifyParam(const int index, const std::u16string & strparam) = 0;
    };

}
NXS4H::IAstStmt *parse_sql(const std::u16string& sql, S4HException& e);


 typedef  std::map<std::string, MaskSymbols> Col2MaskSymbols;
// typedef  std::map<std::u16string, Col2MaskSymbols> Table2MaskCols;
// typedef std::map<std::string, Table2MaskCols> User2MaskInfoMap;
 typedef std::map<std::string, Col2MaskSymbols> User2MaskColumnMap;

/*
* We need a sql prefilter
* use hana sql lexer analyze and blacklist, white list inplement it
* */
bool is_sql_need_enforce(EMStmtType & type, const std::u16string& sql);


/*for update solution interface
 * date:2020/1/13
 * author:jeff
 * */
//  in IAstStmt for update
struct EMUpdateColumnInfo{
#pragma warning(push)
#pragma warning(disable:4245)   // conversion from 'int' to 'unsigned int', signed/unsigned mismatch
    EMUpdateColumnInfo():_isfocused(false),_index(-1),_ismaskparam(false){}
#pragma warning(pop)

    std::u16string _colname;//column name
    //std::u16string _maskfield;//mask field
    std::map<std::string,MaskSymbols> _map_masksymbol;//< *** ,  (***/type)>
    bool _isfocused;// is masked field alias
    unsigned int _index;// '?'index
    bool _ismaskparam;// occur *** when bind param
} ;




NXS4H::IAstStmt *parse_update_sql(const std::u16string& sql, S4HException& e);








#endif 
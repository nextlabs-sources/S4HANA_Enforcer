#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H

//#ifndef WIN32
//#define POLICY_ENGINE_PUBLIC_API    __attribute__((visibility("default")))
//#define POLICY_ENGINE_LOCAL_API     __attribute__((visibility("hidden")))
//#else
//#ifdef POLICY_ENGINE_EXPORT
//#define POLICY_ENGINE_PUBLIC_API    __declspec(dllexport)
//#else
//#define POLICY_ENGINE_PUBLIC_API    __declspec(dllimport)
//#endif //POLICY_ENGINE_EXPORT
//#endif //WIN32

#include <string>
#include <map>
#include <vector>
#include <set>
#include "s4hexception.h"
#include "../../hdb_new/include/EMMaskDef.h"


typedef short PolicyEngineReturn;
typedef void* PolicyEngineHandle;

#define EM_ACTION_SELECT "SELECT"
#define EM_ACTION_UPDATE "UPDATE"
#define EM_ACTION_DELETE "DELETE"
#define EM_ACTION_INSERT "INSERT"
#define EM_ACTION_CONFIG "SECURITY_CONFIG"
#define EM_ACTION_UPSERT "UPSERT"

typedef int(*WriteLog_Fun)(int nLogLevel, const char* szMsg);

enum POLICY_ENGINE_HANDLE_TYPE { PE_STRING_LIST, PE_SUBJECT, PE_HOST, PE_APPLICATION, PE_RESOURCE };

enum POLICY_ENGINE_MATCH_RESULT { PE_NO_MATCHED = 0, PE_NEED_MORE_WORK /* todo */ };

#define STRING_OP 4 /*if op < STRING_OP then must  be no string type*/
class SQLCondition {
public:
    enum SQL_OP {SQL_GT = 0, SQL_GTEQ, SQL_LT, SQL_LTEQ, SQL_EQ, SQL_NEQ} ;
    static SQL_OP TransformOp(const std::string & sop, S4HException & e) {
        if(sop.length() == 1) {
            if(sop[0] == '=') {
                return SQL_EQ;
            } else if (sop[0] == '>') {
                return SQL_GT;
            } else if (sop[0] == '<') {
                return SQL_LT;
            }
        } else if (sop.length() == 2) {
            if (sop[0] == '>' && sop[1] == '=') {
                return SQL_GTEQ;
            } else if (sop[0] == '<' && sop[1] == '=') {
                return SQL_LTEQ;
            } else if (sop[0] == '!' && sop[1] == '=') {
                return  SQL_NEQ;
            }
        }
        if(!sop.empty())
            e._code = POLICY_ENGINE_FAIL;
        return SQL_GT;
    }
    //static SQL_OP TransformOp(const std::string & sop, S4HException & e) ;
    std::string                      _column1;
    SQL_OP                              _op1;
    std::string                      _constant_value1;
    std::string                      _column2;
    SQL_OP                              _op2;
    std::string                      _constant_value2;
    std::string                      _column3;
    SQL_OP                              _op3;
    std::string                      _constant_value3;
};

struct MaskInfo{
    std::set<std::string> _tables;
    std::string _table;
    std::string _field;
    std::string _symbols;
    std::string _format; /*todo mask_format*/
    std::vector<SQLCondition> _conditions;
};

//Tables-based policy information
enum EnumAction
{
    ACT_None=0,
    ACT_SELECT = 1, // 0001
    ACT_UPDATE = 2, // 0010
    ACT_DELETE = 4,   // 0100
    ACT_INSERT = 8, // 1000
    ACT_UPSERT = 16  //10000
};
class TablePolicyInfo{
public:
    TablePolicyInfo():_bfilter(false),_bmask(false),_bdup(false),_actions(ACT_None){}
    //std::string _table;
    bool _bfilter;
    bool _bmask;
    bool _bdup;
    unsigned char _actions;//enum value
    static EnumAction GetEnumActionFromString(const std::string & action){
        if(action==EM_ACTION_SELECT) return ACT_SELECT;
        if(action==EM_ACTION_UPDATE) return ACT_UPDATE;
        if(action==EM_ACTION_DELETE) return ACT_DELETE;
        if(action==EM_ACTION_INSERT) return ACT_INSERT;
        if(action==EM_ACTION_UPSERT) return ACT_UPSERT;
        return ACT_None;
    }
};


typedef std::map<std::string , std::string> EMDictionary;
typedef std::vector<std::string> EMStrVector;
typedef std::vector<EMStrVector*> EMVec2StrVector;
typedef  std::map<std::string, MaskInfo*> MaskInfoMap;
typedef std::set<std::string> EMStrSet;
typedef std::vector<EMStrSet> EMStrSetVec;
typedef std::map<std::string, TablePolicyInfo>  TablePolicyInfoMap;


#endif
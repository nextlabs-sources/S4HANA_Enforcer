#ifndef MASK_HEADER
#define MASK_HEADER

#include <map>
#include <set>
#include <string>
#include "ems4h_datatype.h"

#ifdef WIN32
#include <windows.h>
#include <Shlobj.h>
#else
#include <string.h>
#endif

#define SUPPORT_EXCLUDE_MASKTABLE  0


struct IgnoreCaseCmp : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const std::string& left, const std::string& right) const {
#ifndef WIN32
        return strcasecmp(left.c_str(), right.c_str()) < 0;
#else
        return _stricmp(left.c_str(), right.c_str()) < 0;
#endif
    }
};
enum EMMaskFormat{
    EMMASK_FULL,
    EMMASK_RANDNUMBER,
    EMMASK_PARTIAL,
    EMMASK_KEY
};
struct MaskItem {
    std::string _table;
    std::string _field;
    std::string _strOriginalFiledForTable; //when we apply mask obligation from table to view. _field will be prelace with field in view. then this member will be set to the original field of table.
    EM_SAP_DataType _field_type;
    std::string _condition;
    std::string _symbols;
    //std::string _format;
    EMMaskFormat _format;
    std::string _qulify;
    std::set<std::string> _cond_fields;

#if SUPPORT_EXCLUDE_MASKTABLE
    std::set<std::string> _excludeTable; //when this item is for all table "*". use this field to exclude some table
#endif

    static EMMaskFormat GetMaskFormat(const std::string & val){
        EMMaskFormat ret = EMMASK_FULL;
        if(val.compare("FullMask") == 0){
            ret = EMMASK_FULL;
        } else if(val.compare("RandNumber") == 0){
            ret = EMMASK_RANDNUMBER;
        }
        else if(val.compare("PartialMask") == 0){
            ret = EMMASK_PARTIAL;
        }
        else if(val.compare("KeyMask") == 0){
            ret = EMMASK_KEY;
        }
        return ret;
    }

};
typedef  std::map<std::string, MaskItem*, IgnoreCaseCmp> MaskItemMap; // field - mask item
typedef  std::map<std::string, MaskItemMap*> MaskConditionMap; // table - mask columns


struct UpdateItem {
    std::string _field;
    EM_SAP_DataType _field_type;
    std::string _condition;
    std::set<std::string> _cond_fields;
    std::set<std::string> _excludeTable;
};

struct EMTableInfo;
struct PredicateItem {
    PredicateItem():_tableInfo(NULL){}
    std::string _tableName; //some times _tableInfo->_tb is emmpty. we use this one for temp solution., need to be removed.
    std::string _condition;
    std::set<std::string> _cond_fields;
    const EMTableInfo* _tableInfo;
};

typedef  std::map<std::string, UpdateItem*, IgnoreCaseCmp> DenyUpdateItemMap; // field - UpdateItem
typedef  std::map<std::string, DenyUpdateItemMap*> DenyUpdateTableMap; // table - DenyUpdateItemMap



#endif

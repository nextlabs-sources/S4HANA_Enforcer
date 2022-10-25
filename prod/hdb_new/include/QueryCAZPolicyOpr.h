//
// Created by jeff on 2020/4/24.
//

#ifndef EMDB_QUERYCAZPOLICYOPR_H
#define EMDB_QUERYCAZPOLICYOPR_H

#include <string>
#include <vector>
#include "interface.h"
#include "IQueryCloudAZ.h"
#include "s4hexception.h"
#include "EMMaskDef.h"
#include "SqlDbcUserInfoMgr.h"
#include "SqlDbcResourceInfoMgr.h"

#define MIN_VIEW_CYCLE_HOUR 0.02
#define MIN_POLICY_CYCLE_HOUR 0.05
#define MIN_USER_ATTR_CYCLE_HOUR 0.05
#define MIN_TABLE_ALIAS_CYCLE_HOUR 0.05

#define PARSE_CONDITION_ERROR_IGNORE_OB  "ERROR_IGNORE_OBLIGATION" 
#define PARSE_CONDITION_SUCCESS "SUCCESS"  

struct ResourceAttrValue
{
    std::string strValue;
    CEAttributeType attrType;
};

typedef std::list<std::pair<std::string , ResourceAttrValue>> EMResDictionary;

enum EMOperator{
    EMOP_EQ,
    EMOP_NEQ,
    EMOP_LE,
    EMOP_LT,
    EMOP_GE,
    EMOP_GT,
    EMOP_LIKE,
    EMOP_NOT_LIKE,
    EMOP_IN

};

struct EMCondVal{
    enum EmvalType{
        EMVAL_STR,
        EMVAL_SUBQUERY,
        EMVAL_NUM,
        EMVAL_IDENT
    } _type;
    std::string _val;
};

struct ConditionInfo {
    ConditionInfo(const std::string & real,
                  const std::string & name,
                  const std::string & act,const NxlDBCUserInfo * usr,const NxlDBCResInfo * res, const EMTableInfo* ptb):
            real_tbname(real),
            tableName(name),
            _action(act),
            _puser(usr),
            _pres(res),
            _tbInfo(ptb){}

    std::string real_tbname;
    std::string tableName;
    std::string _action;
    const NxlDBCUserInfo * _puser;
    const NxlDBCResInfo * _pres;
    const EMTableInfo* _tbInfo;

};

class EMCondition {
public:
    static EMOperator TransformOp(const std::string & sop, S4HException & e) ;
    static std::string Serialize(std::vector<EMCondition*> &conds, ConditionInfo & condinfo); // OR LINK
    std::string                      _col;
    EMOperator                       _op;
    EMCondVal                        _const_val;
};

enum PCResultType{
    PCResult_Error,
    PCResult_Config,
    PCResult_Enforcer
};

class QueryPcResult{
public:
    virtual ~QueryPcResult(){}
    QueryPcResult(PCResultType type):_type(type), _deny(false){ }
    virtual bool IsDeny(){ return _deny; };
    virtual void SetDeny(bool b) {_deny = b; };
    virtual PCResultType GetType(){ return _type; }
private:
    PCResultType _type;
    bool _deny;

};

class EnforcerPcResult: public  QueryPcResult {
public:
    EnforcerPcResult():QueryPcResult(PCResult_Enforcer){}
    ~EnforcerPcResult();

    //std::vector<EMCondition*> _filter_cond;
    std::string _filter_cond;
    MaskConditionMap _mask_map;
    std::string _table_name;

};

class ConfigPcResult: public  QueryPcResult {
public:
    ConfigPcResult():QueryPcResult(PCResult_Config){}
    ~ConfigPcResult(){}

    std::vector<std::set<std::string>> _fields;
   // std::vector<std::string> _fields;
    std::set<std::string> _tables;
    std::vector<std::string> _schemas;


};



IPolicyRequest * create_request(const std::string & action, EMResDictionary & user, const std::string& resourceID,
                                EMResDictionary & resource, const std::string & user_id);
QueryStatus  single_query_pc(const IPolicyRequest* pcRequest, IPolicyResult** pcResult);
QueryStatus  multi_query_pc (const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult);
//QueryPcResult * parser_result_array(IPolicyResult * result, ConditionInfo & condinfo, S4HException & e);
void parser_result_2_mask_condition(DenyUpdateTableMap & map_dup,MaskConditionMap & map_in, std::list<PredicateItem>& lstPredicItems, IPolicyResult * result, ConditionInfo & condinfo, S4HException & e);
void parser_result_2_security_config(std::set<std::string> & _tables, std::vector<std::set<std::string>> & _fields,IPolicyResult * result, ConditionInfo & condinfo, S4HException & e);

void free_pc_request(IPolicyRequest* p);
void free_pc_result(IPolicyResult* p);
void free_pc_result(std::vector<QueryPcResult *> & vp);

std::string parser_condition(const std::string & src, ConditionInfo & condinfo, std::set<std::string> & fields);
void parse_fields(const std::string & ss, std::set<std::string> & set_fields);

//QueryPcResult*  query_pc(std::string & action, EMDictionary & user, EMDictionary & resource, ConditionInfo & condinfo, SqlException & e);


#endif //EMDB_QUERYCAZPOLICYOPR_H

//
// Created by jeff on 2019/12/21.
//

#ifndef NXS4H_POLICYMANAGER_H
#define NXS4H_POLICYMANAGER_H

#include "interface.h"
#ifdef _WIN32
#include <time.h>
#endif
#include <map>
#include <set>
#include <vector>
#include "SqlDbcUserInfoMgr.h"
#include "../parser_yacc/EMS4HAst.h"
#include "QueryCAZPolicyOpr.h"
#include "QueryCAZPolicyCache.h"
#include "StructDefine.h"
#include "IQueryCloudAZ.h"

typedef  std::set<std::string> TableSet;
typedef  TableSet ColumnSet;

class PolicyManager {
public:
    static PolicyManager* Instance();
    ~PolicyManager();
    void SetCycle(const int seconds) {_update_cycle = seconds; }
    bool init_query_pc(double policyClearCycle);
    bool IsPCInited() { return _bInitPC; }
    void CycleClear();

    IMaskStrategyVec* GetAllMaskStrategys();
    const ITableMetaData * GetTableMetaData(const std::string& table, EMTableType emTBType, void *connection, S4HException & exc);


    bool QueryPC_Enforcer(const NxlDBCUserInfo* usr,const NxlDBCResInfo* res, const std::vector<EMTableInfo*> tbs,const std::string& act, S4HException & exc);
    bool QueryPC_Enforcer_TableList();
   
    // CC MASK MAP
    const MaskConditionMap * GetMaskInfo(){ return &_allmaskmap; }
	// CC FILTER
    std::string GetFilterCondition(void* pDBConn);
    const std::list<PredicateItem>& GetMatchedPredicateItem() { return _lstMatchedPredicateItems; }

    //CC DENY UPDATE CONTROL
    DenyUpdateTableMap * GetUpdateControlInfo() { return &_all_dup_map; }

    //CC table->action| bool flter | bool mask | bool deny update
    void UpdateAllTablePolicyInfoMap();

    bool IsExistMaskOpr(const std::string & user);
    void InsertMaskInfo(const NxlDBCUserInfo* pUserInfo,const std::string & col,  const MaskSymbols & masksymbol);


    bool IsAliasColumn(const std::string& strCol1, const std::string& strCol2);
    bool IsAliasColumn(const std::string& strCol);

    void ClearPreviousPolicyResult();
    void ClearMaskConditionMap(MaskConditionMap& maskCondMap);
    void ClearUpdateCtrolMap(DenyUpdateTableMap& dup_map);

    std::string PrintMaskConditionMap(const MaskConditionMap& maskCondMap) const ;


    bool IsNeedEnforce(EMStmtType type, const std::vector<EMTableInfo*>& vec_tbs,void* p_conn);

    bool UpdatePolicySyncInterval(unsigned int iseconds, unsigned int nQueryPCTimeout);

    PolicyEnforcement GetPCResult() const { return _pc_result; }

private:
    bool IsNeedEnforceByMask(const std::vector<EMTableInfo*>& vecTbs,  void* pDBConnection);
    bool IsNeedEnforceByUpdateCtrol(const std::string & table, void* conn);
    bool IsNeedEnforceByFilter(EnumAction emAction, const std::vector<EMTableInfo*>& vec_tbs, void* p_conn );

    bool IsNeedEnforceByMaskByTalbe(const std::vector<EMTableInfo*>& vecTbs);
    bool IsNeedEnforcerByMaskByColumn(const std::vector<EMTableInfo*>& vecTbs, const MaskConditionMap * pMaskMap, void* pDBConnection);
    bool IsNeedEnforcerForView(const std::vector<EMTableInfo*>& vecTbs, void* pDBConnection, bool bMask/*check for mask or filter*/);
      
    void AddedMatchedPredicateItem(const PredicateItem& item) { _lstMatchedPredicateItems.push_back(item); }
    void ClearMatchedPredicateItem() { _lstMatchedPredicateItems.clear(); }

    void ClearPredicateItem() { _lstPredicateItems.clear(); }

    void ResetPCResult() { _pc_result = DontCare; }

private:
    bool QueryPC(const NxlDBCUserInfo* usr,const NxlDBCResInfo* res, const std::vector<EMTableInfo*> tbs,const std::string & act,  S4HException & exc);
    PolicyManager();
    void Clear();
    std::string PrintEnforceTable();
    void ClearMaskStrategyVec();
	double _update_cycle;//second
	time_t _time_last;//
    bool _bInitPC;
    PolicyEnforcement _pc_result;

    //std::map<std::string, IMaskStrategyVec *> _user_mask_map;
    std::map<std::string, ITableMetaData *> _table2meta_map;
    //IMaskStrategyVec _AllMaskStrategyVec;
    
    MaskConditionMap _allmaskmap;  //query pc
   // std::string _filter_cond; //query pc if deny then ="deny"
    std::list<PredicateItem> _lstPredicateItems; 
    std::list<PredicateItem> _lstMatchedPredicateItems; 

    DenyUpdateTableMap _all_dup_map;//query pc

    // if masked, we need record this info.
    User2MaskColumnMap _user2col_map;

    //MaskConditionMap _AllMaskMapFromSync; //mask info synced from CC.
    //std::set<std::string> _AllMaskTablesFromSync;  //for convenient, construct all tables form _AllMaskMapFromSync;

   // std::set<std::string> _AllFilterTables; // resource table from CC.
  //  DenyUpdateTableMap _AllDenyUpdateTables;//deny update info synced from CC.
    TablePolicyInfoMap _AllTablePolicyInfo;//table->action| bool flter | bool mask | bool deny update
    int _jpcversion = 202103;
    bool _use_table_list = false;
    QueryCAZCacheMap _cache_jpc_result;

};


#endif //NXS4H_POLICYMANAGER_H

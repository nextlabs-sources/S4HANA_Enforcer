//
// Created by jeff on 2021/5/31.
//

#ifndef SAP_HANA_WINDOWS_QUERYCAZPOLICYCACHE_H
#define SAP_HANA_WINDOWS_QUERYCAZPOLICYCACHE_H
#include "SqlDbcUserInfoMgr.h"
#include "SqlDbcResourceInfoMgr.h"
#include <list>
#include <vector>
#include <memory>
#include "EMMaskDef.h"
#include "IQueryCloudAZ.h"

class QueryCAZCache;
typedef std::map<std::string, std::shared_ptr<QueryCAZCache>> QueryCAZCacheMap;

class QueryCAZCache {
public:
    QueryCAZCache();
    virtual ~QueryCAZCache();

    static void ConstructKey(std::string & out_key, const std::vector<EMTableInfo*> & tbs, const NxlDBCUserInfo * puser, /*const NxlDBCResInfo * pres,*/ const std::string & act);

    static int Match(QueryCAZCacheMap & map_cache, const std::vector<EMTableInfo*>& tbs, const NxlDBCUserInfo * puser, /*const NxlDBCResInfo * pres,*/ const std::string & act,
            MaskConditionMap &out_maskmap, DenyUpdateTableMap &out_dupmap,  std::list<PredicateItem> & out_lcond, PolicyEnforcement &out_pc_result);

    static void Collect(QueryCAZCacheMap & map_cache, const std::vector<EMTableInfo*>& tbs, const NxlDBCUserInfo * puser, /*const NxlDBCResInfo * pres,*/ const std::string & act,
        const MaskConditionMap &maskmap, const DenyUpdateTableMap &dupmap,  const std::list<PredicateItem> & lcond, PolicyEnforcement &pc_result);


public:
    const std::string& GetTCode() { return _tcode; }
    void SetTcode(const std::string& t) { _tcode=t; }

    const std::string& GetInternalSessionKey() { return _internalSessionKey; }
    void SetInternalSessionKey(const char* szSession) { _internalSessionKey=szSession; }

private:
    static bool CopyMaskMap( MaskConditionMap & map, const MaskConditionMap & src);
    static bool CopyDupMap( DenyUpdateTableMap & map, const DenyUpdateTableMap & src);

    void Clear();

    static bool IsCacheDisabled();



private:
    //NxlDBCUserInfo  _puser;
    //NxlDBCResInfo  _pres;
    //std::string   _act;
    //std::vector<EMTableInfo> _tbs;
    //difftime(time(nullptr), _time_last_update)

    time_t _create_time;
    PolicyEnforcement _pc_result;

    //int _ret_code = 0;
    std::list<PredicateItem> _lst_cond;
    MaskConditionMap _mask_map;
    DenyUpdateTableMap _dup_map;

    std::string _tcode;
    std::string _internalSessionKey;

};


#endif //SAP_HANA_WINDOWS_QUERYCAZPOLICYCACHE_H

//
// Created by jeff on 2021/5/31.
//

#include "QueryCAZPolicyCache.h"
#include "QueryCAZPolicyOpr.h"
#include "NXS4HConfig.h"
#include "../parser_yacc/EMS4HAst.h"
#include "PolicyManager.h"
#include "log.h"
#include "MyDisp+Work.h"

const size_t  CACHE_COUNT = 50000;

QueryCAZCache::QueryCAZCache(){
    _create_time = time(nullptr);
}

QueryCAZCache::~QueryCAZCache(){
    Clear();
}

void QueryCAZCache::ConstructKey(std::string & out_key, const std::vector<EMTableInfo*> & tbs, const NxlDBCUserInfo * puser, /*const NxlDBCResInfo * pres,*/ const std::string & act){
    for(size_t i =0; i < tbs.size();++i){//tbs
        out_key+=tbs[i]->_tb;
        out_key+='.';
        out_key+=tbs[i]->_alias;
        if(tbs.size()-i>1){
            out_key+='+';
        }
    }
    out_key+=';';
    if(puser){//user
        out_key+=puser->_strMANDT;
        out_key+='.';
        out_key+=puser->_strName;

    }
    out_key+=';';
    {//action
        out_key+=act;
    }
}

bool QueryCAZCache::IsCacheDisabled()
{  
    return (g_conf->GetFeatureLevel()&DAE_FEATURE_EVA_CACHE)==0;
}

int QueryCAZCache::Match(QueryCAZCacheMap & map_cache, const std::vector<EMTableInfo*> & tbs, const NxlDBCUserInfo * puser, /*const NxlDBCResInfo * pres,*/ const std::string & act,
                         MaskConditionMap &out_maskmap, DenyUpdateTableMap &out_dupmap, std::list<PredicateItem> & out_lcond, PolicyEnforcement &out_pc_result){

    if(IsCacheDisabled()){
        map_cache.clear();
        return -1;
    }
    
    std::string key;
    ConstructKey(key,tbs, puser, act);
    theLog->WriteLog(log_trace,"QueryCAZCache::Match, build key for querying cache, key=%s; cache count=%d", key.c_str(), map_cache.size());

    //int len = list_cache.size();
    auto itcache = map_cache.find(key);
    if( itcache != map_cache.end() ){
        auto pcache = itcache->second;
        assert(pcache);
        
        const std::string sessionNow = g_theMyDW->GetInternalSessionKey();
        if (pcache->GetInternalSessionKey()!=sessionNow)  {
            map_cache.erase(itcache);
            theLog->WriteLog(log_info,"QueryCAZCache::Match, sessionkey didn't match, erased from cache. pcache.session=%s, user.session=%s, key=%s", pcache->GetInternalSessionKey().c_str(), sessionNow.c_str(),  key.c_str());
            return -2;
        }
        

        if (pcache->GetTCode()!=puser->_tcode) {
            map_cache.erase(itcache);
            theLog->WriteLog(log_info,"QueryCAZCache::Match, tcode didn't match, erased from cache. pcache.tcode=%s, user.tcode=%s, key=%s", pcache->GetTCode().c_str(), puser->_tcode.c_str(),  key.c_str());
            return -2;
        }
        
        double fClearCycleHour =  g_conf->GetPCClearCycle();
        time_t t_now = time(nullptr);

        if(difftime(t_now, pcache->_create_time) > fClearCycleHour){
            map_cache.erase(itcache);
            theLog->WriteLog(log_info,"QueryCAZCache::Match, cached policy has expired, erased from cache, key=%s", key.c_str());
            return -2;//expired
        }

        //check succeed  get result
        CopyMaskMap( out_maskmap, pcache->_mask_map);
        CopyDupMap( out_dupmap, pcache->_dup_map);
        out_lcond = pcache->_lst_cond;
        for(auto it =out_lcond.begin(); it != out_lcond.end(); ++it){
            if(it->_tableName.empty()){
                continue;
            }
            for(auto itt :tbs){
                if(it->_tableName == itt->_tb  ){
                    it->_tableInfo = itt; // modify _tbinfo
                    break;
                }
            }
        }
        out_pc_result = pcache->_pc_result;

        theLog->WriteLog(log_info,"QueryCAZCache::Match, hit the cache and got enforcement result directly! key=%s, t_code:%s, session:%s", key.c_str(), puser->_tcode.c_str(), pcache->GetInternalSessionKey().c_str() );
        return 0;
    }

    theLog->WriteLog(log_trace,"QueryCAZCache::Match, not found in cache! key=%s", key.c_str());
    return -1;
}



void QueryCAZCache::Collect(QueryCAZCacheMap & map_cache, const std::vector<EMTableInfo*>& tbs, const NxlDBCUserInfo * puser, /* const NxlDBCResInfo * pres,*/ const std::string & act,
        const MaskConditionMap &maskmap, const DenyUpdateTableMap &dupmap,  const std::list<PredicateItem> & lcond, PolicyEnforcement &pc_result){
 
    if(IsCacheDisabled()){
         map_cache.clear();
         return;
    }
    
    if(map_cache.size() >= CACHE_COUNT){
        map_cache.clear();
    }
    std::string key;
    ConstructKey(key, tbs, puser, act);

    std::shared_ptr<QueryCAZCache> ptr = std::make_shared<QueryCAZCache>();

    CopyMaskMap(  ptr->_mask_map, maskmap);
    CopyDupMap( ptr->_dup_map, dupmap);
    ptr->_lst_cond = lcond;
    ptr->SetTcode(puser->_tcode);
    ptr->SetInternalSessionKey(g_theMyDW->GetInternalSessionKey());
    ptr->_pc_result = pc_result;

    map_cache[key] = ptr;
    theLog->WriteLog(log_trace,"QueryCAZCache::Collect, cached enforcement result for new case, key=%s, tcode:%s, session:%s", key.c_str(), ptr->GetTCode().c_str(), ptr->GetInternalSessionKey().c_str() );
}

bool  QueryCAZCache::CopyMaskMap( MaskConditionMap & map, const MaskConditionMap & src){
    auto it = src.begin();
    for(;it != src.end();++it){
        auto map2 = it->second;
        MaskItemMap * map_new = new MaskItemMap();
        auto it2 = map2->begin();
        for(;it2 != map2->end();++it2){
            MaskItem * item_new = new MaskItem();
            *item_new = *(it2->second);
            map_new->insert(std::make_pair(it2->first, item_new));
        }
        map.insert(std::make_pair(it->first, map_new));
    }
    return true;
}

bool  QueryCAZCache::CopyDupMap( DenyUpdateTableMap & map, const DenyUpdateTableMap & src){
    auto it = src.begin();
    for(;it != src.end();++it){
        auto map2 = it->second;
        DenyUpdateItemMap * map_new = new DenyUpdateItemMap();
        auto it2 = map2->begin();
        for(;it2 != map2->end();++it2){
            UpdateItem * item_new = new UpdateItem();
            *item_new = *(it2->second);
            map_new->insert(std::make_pair(it2->first, item_new));
        }
        map.insert(std::make_pair(it->first, map_new));
    }
    return true;
}


void  QueryCAZCache::Clear(){
    PolicyManager::Instance()->ClearMaskConditionMap(_mask_map);
    PolicyManager::Instance()->ClearUpdateCtrolMap(_dup_map);
}
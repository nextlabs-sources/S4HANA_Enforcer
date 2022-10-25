//
// Created by jeff on 2019/12/21.
//
#include "PolicyManager.h"
#include <time.h>
#include "log.h"
#include "tool.h"
#include "PolicyExport.h"
#include "MaskStrategyImpl.h"
#include "NXS4HConfig.h"
#include "ViewManager.h"
#include <thread>
#include "TableAliasMgr.h"
#include "TableAliasItem.h"
#include "QueryCloudAZExport.h"
#include <chrono>
using namespace std::chrono;
#include "QueryCAZPolicyCache.h"

#define DENY_CONDITION "1=0"

static std::map<QueryStatus,std::string> g_QueryStatusStr={ {QueryStatus::QS_S_OK, "S_OK"},
                                                   {QueryStatus::QS_E_Failed, "E_Failed"},
                                                   {QueryStatus::QS_E_Unauthorized, "E_Unauthorized"},
                                                   {QueryStatus::QS_E_BadRequest, "E_BadRequest"},
                                                    {QueryStatus::QS_E_ServiceUnavailable, "E_ServiceUnavailable"},
                                                    {QueryStatus::QS_E_MissAttributes, "E_MissAttributes"},
                                                    {QueryStatus::QS_E_InternalError, "E_InternalError"},
                                                    {QueryStatus::QS_E_TooManyRequest, "E_TooManyRequest"},
                                                    {QueryStatus::QS_E_DisConnect, "E_DisConnect" },
                                                    {QueryStatus::QS_E_Timeout, "E_Timeout"},
                                                    {QueryStatus::QS_E_EmptyToken, "E_EmptyToken"},
                                                    {QueryStatus::QS_E_ResolveFailed, "E_ResolveFailed" },
                                                    {QueryStatus::QS_E_WriteFailed, "E_WriteFailed" },
                                                    {QueryStatus::QS_E_ReadFailed, "E_ReadFailed" } };


ITableMetaData *create_table_metadata(void *connection, const std::string& table_name, EMTableType emTbType,  S4HException& e);

PolicyManager::PolicyManager() :_update_cycle(3600), _pc_result(PolicyEnforcement::DontCare) {
    // time(&_time_last);
    _time_last =0;//update when sap start
    _bInitPC=false;
}

PolicyManager::~PolicyManager(){
    Clear();
}

PolicyManager* PolicyManager::Instance()
{
    static PolicyManager* pInstance = new PolicyManager();
    return pInstance;
}

int Policy_WriteLog(int nLogLevel, const char* szMsg)
{
  return  theLog->WriteLog(nLogLevel, szMsg);
}

bool PolicyManager::init_query_pc(double policyClearCycle)
{
     //set update cycle
    _update_cycle = policyClearCycle;
    _jpcversion = g_conf->GetJPCVersion();
    _use_table_list =g_conf->UseSecurityTableList();

    theLog->WriteLog(log_info, "PolicyManager::init_query_pc  _update_cycle=second:%lf, _jpcversion:%d, _use_table_list:%d", _update_cycle, _jpcversion, _use_table_list);

    //int sdk
    std::string strJPCHost = g_conf->GetPolicyJPCHost();
    std::string strJPCPort = g_conf->GetPolicyJPCPort();
    std::string strCCHost = g_conf->GetPolicyCCHost();
    std::string strCCPort = g_conf->GetPolicyCCPort();
    std::string strJPCUser = g_conf->GetPolicyJPCUser();
    std::string strJPCPwd = g_conf->GetJPCPwd();
    std::string strTokenPath = g_conf->GetJPCTokenPath();
    std::string strPDPPath = g_conf->GetJPCPDPPath();
    unsigned int sTimeout = g_conf->GetQueryPCTimeout();

    if(_jpcversion<JPC_VERSION_202205){

        if(strJPCHost.length()<8 || strJPCPort.length()<2 || strCCHost.length()<8 || strCCPort.length()<2 || strJPCUser.empty() || strJPCPwd.empty() ){
            theLog->WriteLog(log_error, "PolicyManager::init_query_pc CC/JPC information not correct");
            return false;
        }
        if (!QueryCloudAZInit(strJPCHost.c_str(), strJPCPort.c_str(), strCCHost.c_str(), strCCPort.c_str(), strJPCUser.c_str(), strJPCPwd.c_str(), strTokenPath.c_str(), strPDPPath.c_str(), sTimeout, Policy_WriteLog))
        {
            theLog->WriteLog(log_info, "%s QueryCloudAZInit failed.", __func__);
            //return false;
        }
    } else {
        if(strJPCHost.length()<8 || strJPCPort.length()<2  || strJPCUser.empty() || strJPCPwd.empty() ){
            theLog->WriteLog(log_error, "PolicyManager::init_query_pc CC/JPC information not correct");
            return false;
        }
        //
        if (!QueryCloudAZInit202205(strJPCHost.c_str(), strJPCPort.c_str(), strJPCUser.c_str(), strJPCPwd.c_str(),strTokenPath.c_str(), strPDPPath.c_str(),  sTimeout, Policy_WriteLog)){
            theLog->WriteLog(log_info, "%s QueryCloudAZInit202205 failed.", __func__);
            //return false;
        }
    }
    if(_use_table_list){
        QueryPC_Enforcer_TableList();
    } else {
        //init policy sync module to get security table list
        std::string strCCUser = g_conf->GetPolicyCCUser();
        std::string strCCPwd = g_conf->GetCCPwd();

        S4HException ex;
        PolicyInit(strCCHost, strCCPort, strCCUser, strCCPwd, g_conf->GetPolicyModelName(), policyClearCycle, ex, Policy_WriteLog);
    }
    
    _bInitPC = true;

    return true;
}


void PolicyManager::UpdateAllTablePolicyInfoMap(){
    S4HException ex;
    GetTablePolicyinfo(_AllTablePolicyInfo, ex);
 
}

void PolicyManager::ClearPreviousPolicyResult()
{
    //clear _allmaskmap
    ClearMaskConditionMap(_allmaskmap);
    ClearUpdateCtrolMap(_all_dup_map);
  
    //clear condition
    ClearPredicateItem();
    ClearMatchedPredicateItem();

    //reset enforcement result
    ResetPCResult();
}


bool PolicyManager::QueryPC_Enforcer(const NxlDBCUserInfo* usr,const NxlDBCResInfo* res, const std::vector<EMTableInfo*> tbs,const std::string& act, S4HException & exc){
    //clear
    ClearPreviousPolicyResult();

    unsigned int uFeature = g_conf->GetFeatureLevel();
    if (DAE_FEATURE_QPC&uFeature) {

        auto st = system_clock::now();
        // Check query result not function return value
        bool bQueryPC = QueryPC( usr,  res,   tbs, act, exc);
        auto ed = system_clock::now();
        auto duration = duration_cast<microseconds>(ed-st);
        double  timed = double (duration.count())*microseconds::period::num / microseconds::period::den;
        if(timed>=1){
            theLog->WriteLog(log_fatal, "QueryPC with long time:%lf second",timed);
        }

        //not apply obligation
        if(g_conf->GetFeatureLevel()&DAE_FEATURE_NOT_APPLY_OBLIGATION){
            ClearPreviousPolicyResult();
        }

        return bQueryPC;
    }else{
        return false;
    }
}


bool PolicyManager::QueryPC_Enforcer_TableList()
{
    const std::string action = "CONFIGURE";
    //
    const std::string user_name = "DAE_CONFIGURE";
    EMResDictionary user;
    //ResourceAttrValue user_attr;
    //user.push_back( std::pair<std::string,ResourceAttrValue>(user_name, user_attr) );
    //
    const std::string resID = "DAE_CONFIGURE";
    EMResDictionary resource;
    ResourceAttrValue resource_attr;
    resource_attr.attrType=CEAttributeType::XACML_string;

    //get SAP SID
    const char *pSapSystemID = getenv("SAPSYSTEMNAME");
    if (pSapSystemID) {
        resource_attr.strValue=pSapSystemID;
        resource.push_back( std::pair<std::string,ResourceAttrValue>(RES_SAP_SID, resource_attr) );
    }
    
    //
    IPolicyRequest * preq = create_request( action,user, resID,resource, user_name);
    //
    IPolicyResult *result = NULL;
    theLog->WriteLog(log_info, "%s single_query_pc", __func__);
    QueryStatus status = single_query_pc(preq, &result);
    free_pc_request(const_cast<IPolicyRequest *>(preq));
    //
    if (QS_S_OK != status || result == NULL)  {
        theLog->WriteLog(log_error, "%s, query pc failed. status:%d, result:%p, msg:%s", __func__, status, result,  g_QueryStatusStr[status].c_str() );
        return  false;
    }
    int icount = result->ObligationCount();
    //
    TablePolicyInfoMap allTablePolicys;
    bool bHaveEnforceTBListOb = false;
    std::string tableString;
    for (int i = 0; i < icount; ++i)
    {
        const IObligation *pobg = result->GetObligationByIndex(i);
        const char *pname = pobg->GetObligationName();
        std::string obname(pobg->GetObligationName());
        //printf("oblgation_name:%s\n", obname.c_str());
        const IAttributes *pattr = pobg->GetAttributes();

        if (CommonFun::StrCaseCmp(pname, "enforce_table_list") == 0)
        {
            bHaveEnforceTBListOb=true;
            int ic = pattr->Count();
            // assert(ic==8);
            for (int j = 0; j < ic; ++j)
            {
                const char *sname = nullptr;
                const char *svalue = nullptr;

                CEAttributeType tp = XACML_string;
                pattr->GetAttrByIndex(j, &sname, &svalue, &tp);

                std::string strname(sname);
                std::string strvalue(svalue);
                MaskConditionMap map;

                if (strvalue.empty())
                    continue;
                if (strname.compare("tables") == 0)
                { //Mask Tables	mask_tables	Single Row
                    std::set<std::string> tokens;
                    parse_fields(strvalue, tokens);
                    for(auto & it:tokens){
                        //
                        const std::string& str = it;
                        if(str.empty()) {
                            continue;
                        }
                      
                        TablePolicyInfo tbPolicyInfo;
                        //to compatible download policy from CC, init the following variables 
                        tbPolicyInfo._actions=0xff;
                        tbPolicyInfo._bdup=tbPolicyInfo._bfilter=tbPolicyInfo._bmask=true;
                        allTablePolicys[str]=tbPolicyInfo;

                        tableString += str + ",";
                    }
                }
            }
        }
    }
    free_pc_result(result);

    if(bHaveEnforceTBListOb){//even if alltables is empty, we need copy it.
        _AllTablePolicyInfo.swap(allTablePolicys);

        //printf log
        theLog->WriteLog(log_info, "AllTablePolicyInfo from obligation:%s", tableString.c_str() );
        return true;
    }
    return false;
}



void CollectUserAttributeForQueryPC(const NxlDBCUserInfo& userInfo, EMResDictionary &map)
{
	ResourceAttrValue val;
    val.attrType = XACML_string;

    val.strValue = userInfo._strName;
	map.push_back( std::pair<std::string, ResourceAttrValue>(USER_ATTR_NAME_USERNAME, val) );

	val.strValue = userInfo._strIP;
	map.push_back( std::pair<std::string, ResourceAttrValue>(USER_ATTR_NAME_HOSTIP, val) );

	val.strValue = userInfo._strMANDT;
	map.push_back( std::pair<std::string, ResourceAttrValue>(USER_ATTR_NAME_MANDT, val) );

	val.strValue = userInfo._tcode;
	map.push_back( std::pair<std::string, ResourceAttrValue>(USER_ATTR_NAME_TCODE, val) );

	for (auto itAttr=userInfo._attribute.begin(); itAttr!=userInfo._attribute.end(); itAttr++) {
		for (auto itValue=itAttr->second.begin(); itValue!=itAttr->second.end(); itValue++) {
			val.strValue = *itValue;
	        map.push_back( std::pair<std::string, ResourceAttrValue>(itAttr->first, val) );
		}
	}
}

bool PolicyManager::QueryPC(const NxlDBCUserInfo* usr,const NxlDBCResInfo* res, const std::vector<EMTableInfo*> tbs,const std::string & act, S4HException & exc){
    size_t s = tbs.size();
    if(s == 0) return false;
    if(0 == QueryCAZCache::Match(_cache_jpc_result, tbs, usr, act, _allmaskmap,_all_dup_map, _lstPredicateItems, _pc_result)){
        return true;
    }
    const IPolicyRequest ** reqs = (const IPolicyRequest**)calloc(s,sizeof(IPolicyRequest*));
    std::vector<ConditionInfo> conds;
    for (size_t i = 0; i < s; ++i)
    {
        EMTableInfo *tbi = tbs[i];
        EMResDictionary dic_res;
        ResourceAttrValue val;
        val.attrType = XACML_string;
         
        std::string strResourceID;

        //set server name
        val.strValue = res->_server;
        dic_res.push_back( std::pair<std::string, ResourceAttrValue>( std::string(RES_SERVER_NAME), val) );

        //set database name
        if(!tbi->_db.empty()){
            val.strValue=tbi->_db;
        } else {
            val.strValue=res->_database;
        }
        dic_res.push_back( std::pair<std::string, ResourceAttrValue>( std::string(RES_DATABASE_NAME), val) );
        strResourceID += val.strValue + ".";

        //set schema name
        if(!tbi->_schema.empty()){
            val.strValue = tbi->_schema;
        } else {
            val.strValue = res->_schema;
        }
        dic_res.push_back( std::pair<std::string, ResourceAttrValue>( std::string(RES_SCHEMA_NAME), val) );
        strResourceID += val.strValue + ".";

        //set table name
        val.strValue = tbi->_tb;
        dic_res.push_back( std::pair<std::string, ResourceAttrValue>( std::string(RES_TABLE_NAME), val) );
        strResourceID += val.strValue;
   
        //set sap server name
        val.strValue = res->_appServer;
        dic_res.push_back( std::pair<std::string, ResourceAttrValue>( std::string(RES_APPSERVER_NAME), val) );
        //dic_res[RES_APPSERVER_NAME] = val;

         //set sap sid
        if(!res->_sapsid.empty()){
            val.strValue = res->_sapsid;
            dic_res.push_back( std::pair<std::string, ResourceAttrValue>( std::string(RES_SAP_SID), val) );
        }

        EMResDictionary dic_user;
        CollectUserAttributeForQueryPC(*usr, dic_user);

        reqs[i] = create_request(act, dic_user, strResourceID, dic_res, usr->_strName);

        std::string qualify_ = tbi->_alias;
        if (qualify_.empty() && tbi->_node)
        {
            qualify_ = tbi->_node->serialize();
        }
         else if(tbi->_viewTableInfo!=NULL){
            EMTableInfo* view = tbi->_viewTableInfo;
            if(view && view->_node->getParent()){
                Node *nd_alias = view->_node->getParent();
                if (nd_alias->GetType() == E_ALIAS && nd_alias->getChild(1)) {
                    qualify_ = nd_alias->getChild(1)->serialize();
                }
            }
        }
        else if(tbi->_node && tbi->_node->getParent() ) {
            Node *nd_alias = tbi->_node->getParent();
            if (nd_alias->GetType() == E_ALIAS && nd_alias->getChild(1)) {
                qualify_ = nd_alias->getChild(1)->serialize();
            }
        }

        conds.push_back(ConditionInfo(tbi->_tb, qualify_, act, usr, res, tbi));
    }

    if(s == 1){
        IPolicyResult* result = NULL;
        theLog->WriteLog(log_info, "query_single_before");
        QueryStatus  status = single_query_pc(reqs[0], &result);
        theLog->WriteLog(log_info, "query_single_end");
        free_pc_request(const_cast<IPolicyRequest*>(reqs[0]));
        free(reqs);

        if (QS_S_OK != status || result == NULL) {
            theLog->WriteLog(log_fatal, "single_query_pc failed, status=%d, strStatus:%s", status,  g_QueryStatusStr[status].c_str() );
            if (CommonFun::StrCaseCmp(g_conf->GetErrorHandler().c_str(), CONFVAL_ERROR_HANDLER_DENY)==0){
                PredicateItem predicItem;
                predicItem._condition = DENY_CONDITION;
                _lstPredicateItems.push_back(predicItem);
                theLog->WriteLog(log_fatal, "added false condition 1=0 because error handler is set to deny", status);
            }   
            return false;
        }

        _pc_result = result->GetEnforcement();

        //QueryPcResult * pc = parser_result_array(result, conds[0], exc);
        parser_result_2_mask_condition(_all_dup_map, _allmaskmap, _lstPredicateItems, result, conds[0], exc);
        if(exc._code != S4H_SUCCESS ){
            theLog->WriteLog(log_warning, "single_query_pc parser_result_array failed: %s", exc._detail.c_str());
            free_pc_result(result);
            return false;
        }

        free_pc_result(result);
    } else {
        //multi query
        IPolicyResult ** result = (IPolicyResult**)calloc(s,sizeof(IPolicyResult*));

        theLog->WriteLog(log_info, "query_multi_before, total:%d", s);
        QueryStatus status= multi_query_pc(&reqs[0], (int)s, result);
        theLog->WriteLog(log_info, "query_multi_end");
        for(size_t i = 0 ;i < s ;++i){
            free_pc_request(const_cast<IPolicyRequest*>(reqs[i]));
        }
        free(reqs);

        //check result
        bool bEmptyResult=false;
        for (size_t iResult=0; iResult<s; ++iResult) {
            if(result[iResult]==nullptr){
                theLog->WriteLog(log_fatal, "multi_query_pc found empty result. total:%d, empty index:%d", s,  iResult);
                bEmptyResult=true;
                break;
            }
        }

        if (QS_S_OK!=status || bEmptyResult) {
            theLog->WriteLog(log_fatal, "multi_query_pc failed, status=%d, strStatus=%s, bEmptyResult=%d ", status,  g_QueryStatusStr[status].c_str(), bEmptyResult);
            if (CommonFun::StrCaseCmp(g_conf->GetErrorHandler().c_str(), CONFVAL_ERROR_HANDLER_DENY)==0){
                PredicateItem predicItem;
                predicItem._condition = DENY_CONDITION;
                _lstPredicateItems.push_back(predicItem);
                theLog->WriteLog(log_fatal, "added false condition 1=0 because error handler is set to deny", status);
            }
            free(result);
            return false;
        }


        for (size_t i = 0; i < s; ++i)
        {
            if(result[i]->GetEnforcement() < _pc_result){
                _pc_result = result[i]->GetEnforcement();
            }

            //QueryPcResult * pc = parser_result_array(result[i], conds[i], e);
            parser_result_2_mask_condition(_all_dup_map,_allmaskmap, _lstPredicateItems, result[i], conds[i], exc);
            if(exc._code != S4H_SUCCESS ){
                theLog->WriteLog(log_warning, "multi_query_pc parser_result_array failed: %s", exc._detail.c_str());
            }
        }

        for (size_t i = 0; i < s; ++i){
            free_pc_result(result[i]);
            result[i]=NULL;
        }

        free(result);

    }

    if(_pc_result == Deny && _all_dup_map.size() == 0){
        PredicateItem predicItem;
        predicItem._condition = DENY_CONDITION;
        _lstPredicateItems.push_back(predicItem);
    }

    QueryCAZCache::Collect(_cache_jpc_result, tbs, usr, act, _allmaskmap,_all_dup_map, _lstPredicateItems, _pc_result);

    return true;
}

//const MaskConditionMap * PolicyManager::GetMaskInfo(const NxlDBCUserInfo* usr,const NxlDBCResInfo* res, const std::vector<EMTableInfo*> tbs, S4HException & exc){
//    //clear _allmaskmap
//    for(auto it:_allmaskmap){
//        for(auto its:*it.second){
//            delete(its.second);
//        }
//        it.second->clear();
//        delete(it.second);
//    }
//    _allmaskmap.clear();
//    bool b = QueryPC_MaskCondition( usr, res, tbs,  exc);
//    return &_allmaskmap;
//}


//std::string PolicyManager::PrintEnforceTable()
//{
//    std::string strEnforcerTable = "Enforce table from policy. table:";
//    for(auto it:_enforce_tbs){
//        strEnforcerTable += it + ",";
//    }
//    return strEnforcerTable;
//}

void PolicyManager::ClearMaskStrategyVec()
{
//    for(auto itMask : _AllMaskStrategyVec){
//        delete itMask;
//    }
//    _AllMaskStrategyVec.clear();
}

IMaskStrategyVec* PolicyManager::GetAllMaskStrategys()
{
#ifdef QUERY_POLICY_LOCAL_CC
    S4HException emexc;
    MaskInfoMap map_mask;
    bool bret = GetMatchMaskInfo(map_mask, false, emexc);

    if(!bret) return NULL;

    ClearMaskStrategyVec();

    for(auto it : map_mask) {
        MaskInfo * pmask = it.second;
        if(!pmask) continue;

        MaskStrategyImpl * pmaskstrategy = create_maskstrategy(pmask->_field, pmask->_symbols, pmask->_conditions);
        pmaskstrategy->SetMaskedTable(pmask->_table );
        pmaskstrategy->SetMaskedTables(pmask->_tables);
        _AllMaskStrategyVec.push_back(pmaskstrategy);
    }
    return &_AllMaskStrategyVec;
#else
    return NULL;
#endif
}

const ITableMetaData * PolicyManager::GetTableMetaData(const std::string& table, EMTableType emTbType, void *connection, S4HException & exc) {
    auto it = _table2meta_map.find(table);
    if(it != _table2meta_map.end()) {
        return it->second;
    } else {
        ITableMetaData * pret =  create_table_metadata(connection, table, emTbType, exc);
        if (NULL != pret){
            if (exc._code == 0)
            {
                _table2meta_map.insert(std::make_pair(table, pret));
                theLog->WriteLog(log_info, std::bind(&ITableMetaData::Print, pret));
            }
            else
            {
                delete(pret);
				pret = NULL;
				theLog->WriteLog(log_error, "GetTableMetaData failed for table:%s, erro:%d, detail:%s", table.c_str(), exc._code, exc._detail.c_str()  );
            }
        }
        else{
            //get metadata failed.
            theLog->WriteLog(log_error, "GetTableMetaData failed for table:%s, erro:%d, detail:%s", table.c_str(), exc._code, exc._detail.c_str()  );

        }
        return pret;
    }
}

void PolicyManager::Clear(){
    //metadata
    for(auto it:_table2meta_map) {
        delete(it.second);
    }
    _table2meta_map.clear();

}

void PolicyManager:: CycleClear(){
    
    if(!_use_table_list){
        UpdateAllTablePolicyInfoMap();
    }

    //cycle clear table metadata
    time_t timeNow = time(0);
    if(difftime(timeNow, _time_last) > _update_cycle ) {
        _time_last = timeNow;
        Clear();

        if(_use_table_list){
            QueryPC_Enforcer_TableList();
        }
    }
}

bool PolicyManager::IsExistMaskOpr(const std::string & user) {
    auto it = _user2col_map.find(user);
    if(it != _user2col_map.end() && !(it->second).empty())
        return true;
    return false;
}

void PolicyManager::InsertMaskInfo(const NxlDBCUserInfo* pUserInfo,const std::string & col,  const MaskSymbols & masksymbol) {
    auto it = _user2col_map.find(pUserInfo->_strName);
    if(it != _user2col_map.end()) {
        Col2MaskSymbols & col2mask = it->second;
        col2mask.insert(std::make_pair(col, masksymbol));
    }else {
        Col2MaskSymbols  col2cond ;
        col2cond.insert(std::make_pair(col, masksymbol));
        _user2col_map.insert(std::make_pair(pUserInfo->_strName, col2cond));
    }
}




std::string PolicyManager::PrintMaskConditionMap(const MaskConditionMap& maskCondMap) const
{
    std::string strMask;
    for(auto itMask : maskCondMap){
        strMask += "Mask table:" + itMask.first + " ";
        if (itMask.second!=NULL) {
            for(auto itMaskItem : *(itMask.second)){
                strMask += itMaskItem.first + ",";
                strMask += itMaskItem.second->_symbols + ",";
                strMask += itMaskItem.second->_condition + ";";
            }
        }  
        strMask += "\n";
    }
    return strMask;
}

void PolicyManager::ClearMaskConditionMap(MaskConditionMap& maskCondMap)
{
    for(auto it:maskCondMap){
        for(auto its:*it.second){
            delete(its.second);
        }
        it.second->clear();
        delete(it.second);
    }
    maskCondMap.clear();
}
void PolicyManager::ClearUpdateCtrolMap(DenyUpdateTableMap& dup_map){
    for(auto it:dup_map){
        for(auto its:*it.second){
            delete(its.second);
        }
        it.second->clear();
        delete(it.second);
    }
    dup_map.clear();
}


bool PolicyManager::IsNeedEnforceByMask(const std::vector<EMTableInfo*>& vecTbs,  void* pDBConnection)
{
    bool bNeedEnforce = false;

    //check if need enforcer by table
    bNeedEnforce = IsNeedEnforceByMaskByTalbe(vecTbs );
  
  #if 0  //didn't support it now
    //check if need enforcer by column
    if (!bNeedEnforce){
        bNeedEnforce = IsNeedEnforcerByMaskByColumn(vecTbs, pMaskMap, pDBConnection);
    }
  #endif 

    //check if need enforcer if it is view and there are mask policy set for its correspond table.
    //if it is a view and the view itself have no policy for it. we check if the underline table have policy for it
    if (!bNeedEnforce){
        bNeedEnforce = IsNeedEnforcerForView(vecTbs, pDBConnection, true);
    }
       
    return bNeedEnforce;
}
bool PolicyManager::IsNeedEnforceByMaskByTalbe(const std::vector<EMTableInfo*>& vecTables)
{
    TablePolicyInfoMap& allPolicyInfo = _AllTablePolicyInfo;
    bool bNeedEnforce = false;
    S4HException  exc;
    for(auto it : vecTables) {
        auto itPolicyInfo = allPolicyInfo.find(it->_tb);
        if((itPolicyInfo!= allPolicyInfo.end()) && (itPolicyInfo->second._actions&ACT_SELECT) && (itPolicyInfo->second._bmask) ) {   
            theLog->WriteLog(log_info, "IsNeedEnforceByMaskByTalbe return true for mask policy table table:%s", it->_tb.c_str() );
            bNeedEnforce = true;
            break;
        }
       
        //check table alias table
        const TableAliasItem* pTableAliasItem = TableAliasMgr::Instance().GetAliasItemByAliasTable(it->_tb);
        if (NULL!=pTableAliasItem) {
            auto itPolicyInfoAlias = allPolicyInfo.find(pTableAliasItem->OriginalTable());
            if((itPolicyInfoAlias!= allPolicyInfo.end()) && (itPolicyInfoAlias->second._actions&ACT_SELECT) && (itPolicyInfoAlias->second._bmask) ) {   
                theLog->WriteLog(log_info, "IsNeedEnforceByMaskByTalbe return true for alias table:%s, original table:%s", it->_tb.c_str(), pTableAliasItem->OriginalTable().c_str() );
                bNeedEnforce = true;
                break;
            }
        }
    }

    return bNeedEnforce;
}

bool PolicyManager::IsNeedEnforcerForView(const std::vector<EMTableInfo*>& vecTables, void* pDBConnection, bool bMask/*check for mask or filter*/)
{
    bool bNeedEnforce = false;
    ViewManager *pViewMgr = ViewManager::GetInstance();
    for (auto itTable= vecTables.begin(); itTable!=vecTables.end(); itTable++)  {
        EMTableInfo* pTableInfo = *itTable;
        ViewInfo *pViewInfo = pViewMgr->GetViewInfoFromDDL27S(pTableInfo->_tb, pDBConnection);
        if (pViewInfo)  {
            auto pSourceInfo = pViewInfo->SourceInfo();
            if (pSourceInfo)  {
                const std::set<std::string> *pViewSourceTables = pSourceInfo->GetSourceTables();
               // theLog->WriteLog(log_info, "IsNeedEnforcerByMaskForView view:%s have source info, table count:%d", pTableInfo->_tb.c_str(), pViewSourceTables->size());
                for (auto itSourceTable = pViewSourceTables->begin();  itSourceTable!=pViewSourceTables->end(); itSourceTable++) {
                    //theLog->WriteLog(log_info, "IsNeedEnforcerByMaskForView source table:%s", itSourceTable->c_str() );
                    size_t ipos = itSourceTable->find(".");
                    std::string stable = *itSourceTable;
                    if (ipos != std::string::npos){ 
                        stable = itSourceTable->substr(ipos + 1);
                    }

                    auto itPolicyInfo = _AllTablePolicyInfo.find(stable) ;
                    if ((itPolicyInfo!=_AllTablePolicyInfo.end()) && (itPolicyInfo->second._actions&ACT_SELECT) ) {
                        if( (bMask&&itPolicyInfo->second._bmask)|| ((!bMask)&&itPolicyInfo->second._bfilter) ){
                            pTableInfo->_viewsource = pSourceInfo;
                            bNeedEnforce = true;
                            theLog->WriteLog(log_info, "IsNeedEnforcerByMaskForView return true for view match. view:%s, mask policy table:%s",
                                             pTableInfo->_tb.c_str(), itSourceTable->c_str());

                            //print view source info
                            if (!pViewInfo->IsSourceInfoPrinted())  {
                                theLog->WriteLog(log_info, std::bind(&ViewSourceInfo::Print, pSourceInfo.get(), pTableInfo->_tb));
                                pViewInfo->SetSourceInfoPrinted(true);
                            }

                            break;
                        }
                       
                    }
                }
            }
            else
            {
                theLog->WriteLog(log_info, "IsNeedEnforcerByMaskForView get source failed for view:%s", pViewInfo->ViewName().c_str() );
            }
        }
    }
    return bNeedEnforce;
}



bool PolicyManager::IsNeedEnforcerByMaskByColumn(const std::vector<EMTableInfo*>& vecTables,
                                           const MaskConditionMap * pAllMaskInfo, void* pDBConnection)
{
    SQLDBC::SQLDBC_Connection* connection= (SQLDBC::SQLDBC_Connection*)pDBConnection;
    S4HException exc;

    bool bNeedEnforce = false;
    MaskConditionMap::const_iterator itMaskInfo= pAllMaskInfo->begin();
    while (itMaskInfo != pAllMaskInfo->end()) {
        if (itMaskInfo->first.compare(ALL_TABLE_SYMBOLS)==0)  {
            MaskItemMap* pMaskItem = itMaskInfo->second;
            if (NULL==pMaskItem){
                ++itMaskInfo;
                continue;
            }
            
            for(auto it : vecTables)  {
               
                const std::string& u8Table = it->_tb;
                const ITableMetaData* pMetaData = PolicyManager::Instance()->GetTableMetaData(u8Table, it->_type, connection, exc); 
                if (NULL==pMetaData){
                    continue;
                }

                //check if the mask filed is exist in the table
                for(auto itMaskField : *pMaskItem) {             
                    if(pMetaData->HasColumn(itMaskField.first)){
                        #if SUPPORT_EXCLUDE_MASKTABLE
                        if (itMaskField.second->_excludeTable.find(it->_tb)!=itMaskField.second->_excludeTable.end()) { //check if the table is in the exluce table list
                            theLog->WriteLog(log_info, "check need enforce by column, exclude, find :%s, table:%s", itMaskField.first.c_str(), it->_tb.c_str() );
                            continue;
                        }
                        else
                        #endif 
                        {
                            bNeedEnforce = true;
                            theLog->WriteLog(log_info, "IsNeedEnforcerByMaskByColumn return true by column, true, find :%s, table:%s", itMaskField.first.c_str(), it->_tb.c_str() );
                            //break;  this should be break, comment for debug reason, it will output all matched column
                        }
                    }
                } 

                if (bNeedEnforce) {
                    break;
                }         
            }
        }  

        if (bNeedEnforce) {
            break;
        }
                
        itMaskInfo++;
    } 

    return bNeedEnforce;   
}

bool PolicyManager::IsNeedEnforceByUpdateCtrol(const std::string & table, void* /*conn*/) {
    bool ret = false;
    S4HException exc;
    auto it  = _AllTablePolicyInfo.find(table);
    if((it!=_AllTablePolicyInfo.end()) && (it->second._actions&ACT_UPDATE) && (it->second._bdup) ){
        return  ret;
    }

#if 0
    it  = _AllDenyUpdateTables.find(ALL_TABLE_SYMBOLS);
    if(it != _AllDenyUpdateTables.end()) {
        DenyUpdateItemMap* p_dupitem = it->second;
        if (NULL==p_dupitem){
            return ret;
        }

        do {
            const std::string& u8Table = table;
            const ITableMetaData* meta = PolicyManager::Instance()->GetTableMetaData(u8Table, EMTABLE_NATRUAL ,  conn, exc);
            if (NULL==meta){
                break;
            }
            //check if the mask filed is exist in the table
            for(auto it_item : *p_dupitem) {

                if(meta->HasColumn(it_item.first)){

                    if (it_item.second->_excludeTable.find(table)!=it_item.second->_excludeTable.end()) { //check if the table is in the exluce table list
                        theLog->WriteLog(log_info, "check need enforce by column, exclude, find :%s, table:%s", it_item.first.c_str(), table.c_str() );
                        continue;
                    }
                    else{
                        ret = true;
                        theLog->WriteLog(log_info, "IsNeedEnforceByUpdateCtrol, true, find :%s, table:%s", it_item.first.c_str(), table.c_str() );
                        break;
                        //break;  this should be break, comment for debug reason, it will output all matched column
                    }
                }
            }

        }while(false);
    }
#endif

    return ret;
}

bool PolicyManager::IsNeedEnforceByFilter(EnumAction emAction, const std::vector<EMTableInfo*>& vec_tbs, void* p_conn){
    bool bNeedEnforce = false;

    TablePolicyInfoMap& allPolicyInfo = _AllTablePolicyInfo;
    S4HException  exc;
    for(auto it : vec_tbs) {
        auto itPolicy = allPolicyInfo.find(it->_tb);
        if( (itPolicy!= allPolicyInfo.end()) && (itPolicy->second._actions&emAction) ){
            bNeedEnforce = true;
            theLog->WriteLog(log_info, "IsNeedEnforceByFilter, true,action:%d, table:%s",emAction, it->_tb.c_str() );
            break;
        }
    }


     if ((!bNeedEnforce) && (emAction==ACT_SELECT)){
        bNeedEnforce = IsNeedEnforcerForView(vec_tbs, p_conn, false);
    }


    return bNeedEnforce;
}

bool PolicyManager::IsNeedEnforce(EMStmtType type, const std::vector<EMTableInfo*>& vec_tbs,void* p_conn){
    bool bret = false;
    switch(type) {
        case E_STMT_TYPE_SELECT: {
            // check select mask
            bret = IsNeedEnforceByMask(vec_tbs, p_conn);
            if (!bret){
                //check filter
                bret = IsNeedEnforceByFilter(ACT_SELECT, vec_tbs, p_conn);
            }
             
        } break;
        case E_STMT_TYPE_UPDATE: {
            // check select mask
            bret = IsNeedEnforceByMask(vec_tbs, p_conn);
            if (!bret){
                //check filter
                bret = IsNeedEnforceByFilter(ACT_UPDATE, vec_tbs,  p_conn);
                if(!bret) {
                    bret = IsNeedEnforceByUpdateCtrol(vec_tbs[0]->_tb, p_conn);
                }
            }
        } break;
        case E_STMT_TYPE_DELETE:
        {
            // check select mask
            bret = IsNeedEnforceByMask(vec_tbs, p_conn);
            if (!bret){
                bret = IsNeedEnforceByFilter(ACT_DELETE,vec_tbs, p_conn);
            }
        }break;
        case E_STMT_TYPE_INSERT:
        {
            // check select mask
            bret = IsNeedEnforceByMask(vec_tbs, p_conn);
            if (!bret){
              bret = IsNeedEnforceByFilter(ACT_INSERT, vec_tbs, p_conn);
            }
        }
        break;
        case E_STMT_TYPE_UPSERT:{
            //check filter table
            bret = IsNeedEnforceByMask(vec_tbs, p_conn);
        } break;
        default:
            break;
    }
    return  bret;
}

std::string PolicyManager::GetFilterCondition(void* pDBConn)
{
    std::string strFilterCond;
    auto itPredicItem = _lstPredicateItems.begin();
    while (itPredicItem != _lstPredicateItems.end()){ 
        bool bPredicValid = true;

        const EMTableInfo* pTableInfo = itPredicItem->_tableInfo;
        if(pTableInfo && pTableInfo->_viewTableInfo){
            pTableInfo = pTableInfo->_viewTableInfo;
        }

        if (pTableInfo){
            //get metadata
            const std::string& ss = pTableInfo->_tb;
            S4HException exc;
            const ITableMetaData *pTBMetaData = GetTableMetaData(ss, pTableInfo->_type, pDBConn, exc);
            if (NULL != pTBMetaData)  {
                //check each field
                auto itField = itPredicItem->_cond_fields.begin();
                while (itField != itPredicItem->_cond_fields.end()) {
                    if (!pTBMetaData->HasColumn(*itField))  {
                        bPredicValid = false;
                         theLog->WriteLog(log_warning, "predicate condition not valid:%s reason:%s is not valid", itPredicItem->_condition.c_str(), itField->c_str() );
                        break;
                    }
                    itField++;
                }
            }
        }
        
        if(bPredicValid){
            if (!itPredicItem->_condition.empty()){
                if (!strFilterCond.empty()){
                    strFilterCond += " AND ";
                }
                strFilterCond += "(" + itPredicItem->_condition + ")";

                AddedMatchedPredicateItem(*itPredicItem);
            }
        }        

        itPredicItem++;
    }
    
   return strFilterCond;
}

bool PolicyManager::UpdatePolicySyncInterval(unsigned int iseconds, unsigned int nQueryPCTimeout){
    UpdateSyncInterval(iseconds);
    _update_cycle = iseconds;

    UpdateQueryPCTimeout(nQueryPCTimeout);

    return  true;
}
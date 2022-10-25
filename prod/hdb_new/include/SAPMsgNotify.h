#ifndef SAP_MSG_NOTIFY
#define SAP_MSG_NOTIFY

#include "EnforcerCtxImpl.h"
#include <string>
#include <map>
#include <vector>
#include "EMMaskDef.h"


class NotifyItemData
{

public:
   int GetCounter();

public:
   char _MANDT[3+1];
   char _USERID[12+1];
   char _SESSION_KEY[32+1];
   char _COUNTER[5+1];  //number
   char _PROCESS_ID[8+1];
   char _APP_NAME[50+1];
   char _UI_TYPE[10+1];
   char _DAE_RELEVANT[1+1];
   char _TABNAME[30+1];
   char _FIELDNAME[30+1];
   char _TYPE[10+1];

   char _PROCESSED[1+1];
};

class SAPMsgNotify
{
public:
    static SAPMsgNotify& Instance(){
        static SAPMsgNotify notify;
        return notify;
    }

public:
    void NotifyMask(void* pDBConn, EnforcerCtxImpl* pCtx,const NxlDBCUserInfo* pUserInfo,  const std::vector<MaskItem>& vecMaskItem, const std::string &strMaskCountSql);
    void NotifyFilter(void* pDBConn, EnforcerCtxImpl* pCtx,const NxlDBCUserInfo* pUserInfo,const std::list<PredicateItem>& lstPredicItems, const std::string& strNewSql, const std::string& strOldSql);
private:
    SQLDBC_Retcode GetTopRecord(void* pDBConn, const std::string& strMandt, const std::string& strUserID, 
                const std::string& strSessionKey, NotifyItemData& notifyData);
    void InsertNotifyDataForMask(void* pDBConn, const NxlDBCUserInfo* pUserInfo, int nCounter, const MaskItem* pMaskItem);

    void InsertNotifyDataForFilter(void* pDBConn, const NxlDBCUserInfo* pUserInfo, int nCounter, const PredicateItem* pMaskItem);

    void InsertNotifyData(void* pDBConn, const std::string& strMANDT, const std::string& strSessionKey, const std::string& strUser, int nCounter, 
                                    const std::string& strTcode, const std::string& strUIType, const std::string& strTable,
                                    const std::string& strField, const std::string& maskSymbol,
                                    const std::string& condition, const char* szType/*MASKING or FILTER*/);

    void PrintNotifyData(const NotifyItemData& notifyData);

    bool GetRowCount(void* pDBConn, EnforcerCtxImpl *pctx, const std::string& strSql, int& outCount);
    bool CheckDoNotifyFilter(void* pDBConn, EnforcerCtxImpl *pctx, const std::string& strNewSql, const std::string& strOldSql);
    void CheckDoNotifyMask(void *pDBConn, EnforcerCtxImpl *pctx, const std::vector<MaskItem> &vecMaskItem, const std::string &strMaskCountSql, std::vector<MaskItem> &vecEffectedMaskItem);
private:
    NotifyItemData _notifyData;

};



#endif


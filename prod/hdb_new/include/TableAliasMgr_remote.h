
#ifndef TABLE_ALIAS_MGR_REMOTE
#define TABLE_ALIAS_MGR_REMOTE

#include <string>
#include <vector>
#include "TableAliasItem.h"

class TableAliasMgr{

public:
    static TableAliasMgr& Instance(){
        static TableAliasMgr tableAliasMgr;
        return tableAliasMgr;
    }
    bool IsInit() { return _bInited; }
    bool Init(void* pDBConn, const double& updateCycle);
    void CycleUpdate(void* pDBConn);
    void UpdateClearCycleInterval(const double& updateCycle);

    const TableAliasItem* GetAliasItemByAliasTable(const std::string& strAliasTable);
    std::string Print();

private:
    TableAliasMgr();
    TableAliasMgr(const TableAliasMgr&){}

private:
    bool ReadFromRemote(void* pDBConn);
    bool ReadAliasHeader(void* pDBConn);
    bool ReadAliasItem(void* pDBConn);
    void ClearTableAlias();


private:
   bool _bInited;
    double _update_cycle;//second
	time_t _time_last_update;//
 
    std::map<std::string, TableAliasItem*> _alias;
};




#endif //TABLE_ALIAS_MGR
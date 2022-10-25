
#ifndef TABLE_ALIAS_MGR
#define TABLE_ALIAS_MGR

#ifdef DAE_LOCAL_CONFIG

#include <string>
#include <vector>
#include "TableAliasItem.h"

class TableAliasMgr{

public:
    static TableAliasMgr& Instance(){
        static TableAliasMgr tableAliasMgr;
        return tableAliasMgr;
    }

    bool Init(const char* szAliasFile);
    void CycleUpdate();

    const TableAliasItem* GetAliasItemByAliasTable(const std::string& strAliasTable);
    std::string Print();

    bool IsInit() { return _bInit;}

private:
    TableAliasMgr();
    TableAliasMgr(const TableAliasMgr&){}

private:
    bool ReadFromFile(const std::string& aliasFile);
    void ClearTableAlias();


private:
    std::string _table_alias_file;

    double _update_cycle;//second
	time_t _time_last_update;//
 
    std::map<std::string, TableAliasItem*> _alias;

    bool _bInit;
};

#else
#include "TableAliasMgr_remote.h"
#endif 




#endif //TABLE_ALIAS_MGR
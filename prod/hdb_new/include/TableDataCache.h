#ifndef TABLE_DATA_CACHE_H_20220111
#define TABLE_DATA_CACHE_H_20220111
#include <map>
#include <list>
#include <string>
#include <memory>
#include <vector>
#include "log.h"

typedef struct tagROW_DATA{
    std::string _table;
    std::string _key; //table+primarykey;
    std::map<std::string, std::string>  _data;
    std::vector<std::string> _maskedFields;
    time_t _time; 
    ~tagROW_DATA();
 }ROW_DATA;


class TableDataCache
{
public:
    static std::string ConstructRowKey(const std::string& table, const std::vector<std::string>& keyData);


public:
    const std::shared_ptr<ROW_DATA> GetRowData(const std::string& key, bool bremove);
    void InsertRowData(const std::string& key, std::shared_ptr<ROW_DATA> rowData);
    void RemoveExpiredData();


private:
    std::map<std::string, std::shared_ptr<ROW_DATA> > _cache; //key to row data;
    static const int _nExpire=60; //expire time,second
};


#endif 
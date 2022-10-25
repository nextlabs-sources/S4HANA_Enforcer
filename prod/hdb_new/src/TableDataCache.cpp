#include "TableDataCache.h"


tagROW_DATA::~tagROW_DATA()
{
    //theLog->WriteLog(log_info, "de-construct cached delete data, key:%s", _key.c_str() );
}

void TableDataCache::InsertRowData(const std::string& key, std::shared_ptr<ROW_DATA> rowData)
{
    _cache[key] = rowData;
}
std::string TableDataCache::ConstructRowKey(const std::string& table, const std::vector<std::string>& keyData)
{
    std::string strKey = table+".";
    for (auto itKey=keyData.begin(); itKey!=keyData.end(); itKey++)
    {
        strKey += *itKey + ".";
    }
    return strKey;
}

const std::shared_ptr<ROW_DATA> TableDataCache::GetRowData(const std::string& key, bool bRemove)
{
    std::shared_ptr<ROW_DATA> rowData;
    auto itRowData = _cache.find(key);
    if(itRowData!=_cache.end()){
        rowData = itRowData->second;

        if(bRemove){
            _cache.erase(itRowData);
        } 
    }
    return rowData;
}

void TableDataCache::RemoveExpiredData()
{
    time_t timeNow=time(0);

    auto itRowData = _cache.begin();
    while (itRowData!=_cache.end()){
         auto rowData = itRowData->second;
        if(difftime(timeNow, rowData->_time)>=_nExpire){
            itRowData = _cache.erase(itRowData);
            theLog->WriteLog(log_info, "remove cached delete data, key:%s", rowData->_key.c_str() );
        }else{
            itRowData++;
        }  
    }
}
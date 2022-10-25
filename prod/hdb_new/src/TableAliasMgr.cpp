#ifdef DAE_LOCAL_CONFIG

#include "TableAliasMgr.h"
#include "tool.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

TableAliasMgr::TableAliasMgr()
{
    _update_cycle = 3600;//second
    _bInit = false;
}

bool TableAliasMgr::Init(const char* szAliasFile)
{
    _table_alias_file = szAliasFile;
    bool bRead = ReadFromFile(_table_alias_file);
    _time_last_update = time(NULL);

    _bInit = true;
    return bRead;
}

void TableAliasMgr::CycleUpdate()
{
    time_t timeNow = time(NULL);
    if (difftime(timeNow, _time_last_update)>_update_cycle){
        ClearTableAlias();

        ReadFromFile(_table_alias_file);
        _time_last_update = time(NULL);
    }
}

void TableAliasMgr::ClearTableAlias()
{ 
    for (auto itTableAliasItem=_alias.begin(); itTableAliasItem!=_alias.end(); itTableAliasItem++)  {   
       delete itTableAliasItem->second;      
    }
    _alias.clear();
}


bool TableAliasMgr::ReadFromFile(const std::string& aliasFile)
{
    try
    {
        boost::property_tree::ptree root;
        boost::property_tree::read_json<boost::property_tree::ptree>(aliasFile, root);
         
        //get update cycle
        float clearcycle = root.get<float>("clearcycle");
        int nSecondClearCycle = clearcycle * 3600;
        _update_cycle = nSecondClearCycle;

        //get alias
        boost::property_tree::ptree alias = root.get_child("alias");
        for(boost::property_tree::ptree::iterator itAlias=alias.begin(); itAlias!=alias.end(); ++itAlias){
    
            boost::property_tree::ptree ptAliasItem = itAlias->second;
            std::string strOriginalTable = ptAliasItem.get<std::string>("original_table");
            std::string strAliasTable = ptAliasItem.get<std::string>("alias_table");
            std::string strUpdateRelation = ptAliasItem.get<std::string>("update_relation");

            
            TableAliasItem* pTableAliasItem = new TableAliasItem(strOriginalTable, strAliasTable, strUpdateRelation);
              

            boost::property_tree::ptree ptAliasFields = ptAliasItem.get_child("alias_fields"); 
            for(boost::property_tree::ptree::iterator itAliasFields=ptAliasFields.begin(); 
                itAliasFields!=ptAliasFields.end(); ++itAliasFields){

                boost::property_tree::ptree ptAliasField = itAliasFields->second;
                
                std::string strOriginalField = ptAliasField.get<std::string>("original_field"); 
               
                boost::property_tree::ptree ptAliasFieldName = ptAliasField.get_child("alias_field"); 
                for(boost::property_tree::ptree::iterator itAliasF=ptAliasFieldName.begin();
                    itAliasF!=ptAliasFieldName.end(); ++itAliasF){

                    pTableAliasItem->AddedFieldAlias(strOriginalField, itAliasF->second.data());
                }
            } 

            _alias[strAliasTable]= pTableAliasItem;
        }      

    }
    catch (const std::exception &e)
    {
        printf("exception on TableAliasMgr::ReadFromFile:%s\n", e.what() );
        return false;
    }

    return true;
}



const TableAliasItem* TableAliasMgr::GetAliasItemByAliasTable(const std::string& strAliasTable)
{
    TableAliasItem* resultItem = NULL;
    auto itAliasItem = _alias.find(strAliasTable);
    if (itAliasItem!=_alias.end()){
         resultItem = itAliasItem->second;
    }

    return resultItem;
}

std::string TableAliasMgr::Print()
{
    std::string strInfo = "TableAliasMgr::";
    return strInfo;
}

#endif 

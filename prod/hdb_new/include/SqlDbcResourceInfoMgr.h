//
// Created by jeff on 2020/3/26.
//

#ifndef HOOK_SQLDBRESOURCEINFOMGR_H
#define HOOK_SQLDBRESOURCEINFOMGR_H

#include <string>
#include <map>
#include "s4hexception.h"

#define RES_SERVER_NAME "server"
#define RES_DATABASE_NAME "database"
#define RES_SCHEMA_NAME "schema"
#define RES_TABLE_NAME "table"
#define RES_APPSERVER_NAME "appserver"
#define RES_SAP_SID  "sapsid"

struct NxlDBCResInfo {
    std::string _server;
    std::string _database;
    std::string _schema;
    std::string _table;
    std::string _appServer;
    std::string _sapsid;
    std::map<std::string, std::string> _others;

    std::string Print() const {
        std::string strInfo = "NxlDbcResInfo: server=";
        strInfo += _server + ",database=";
        strInfo += _database + ",schema=";
        strInfo += _schema + ",table=";
        strInfo += _table + ",appserver=";
        strInfo += _appServer + ",";
        for(auto it: _others) {
            strInfo += it.first + '=';
            strInfo += it.second + ",";
        }
        return strInfo;
    }
    bool Compare(const NxlDBCResInfo* src) const{
        if(src->_server != _server) return false;
        if(src->_database != _database) return false;
        if(src->_schema != _schema) return false;
        if(src->_table != _table) return false;
        if(src->_appServer != _appServer) return false;
        for(auto it: _others) {
            auto it2 = src->_others.find(it.first);
            if(it2 == src->_others.end()){
                return false;
            } else {
                if(it2->second != it.second){
                    return false;
                }
            }
        }
        return true;
    }

};

class SqlDbcResourceInfoMgr {
public:

    ~SqlDbcResourceInfoMgr();

public:
    static SqlDbcResourceInfoMgr* Instance()
    {
        static SqlDbcResourceInfoMgr mgr;
        return &mgr;
    }
    const NxlDBCResInfo * GetResourceInfo(void* connection, S4HException& e);
    const std::string& GetAppServerName();
private:
    SqlDbcResourceInfoMgr(){};
    SqlDbcResourceInfoMgr(const SqlDbcResourceInfoMgr&){}


    NxlDBCResInfo _resource_info;
};


#endif //HOOK_SQLDBRESOURCEINFOMGR_H

//
// Created by jeff on 2019/12/24.
//

#include "NXS4HConfig.h"
#include <iostream>
#include <fstream>

#pragma warning(push)
#pragma warning(disable: 4702)
#include <boost/algorithm/string.hpp>
#pragma warning(push)

#include "log.h"
#include <AESEncrypt.h>
#include <algorithm>
#ifndef _WIN32
#include <experimental/algorithm>
#endif 
#include <tool.h>
std::string parse_value(std::string & read,unsigned int & pos,bool iskey);
std::string parse_title(std::string & read,unsigned int & pos);
bool read_from_file(std::string & ret, const std::string& file_path);

#define CONFVAL_TRUE "true"

NXS4HConfig * g_conf = NULL;

NXS4HConfig::NXS4HConfig()
{
    _update_cycle = 1 * 60;
    _bDeactiveDAE = false;
}

NXS4HConfig::~NXS4HConfig(){
}

bool NXS4HConfig::IsWhiteListUser(const std::string& strUser)
{
    bool bWhite = false;
    std::vector<std::string>::iterator itUser = _whitelistUsers.begin();
    while (itUser != _whitelistUsers.end() ){
        if (CommonFun::StrCaseCmp(strUser, *itUser)==0){
            bWhite = true;
            break;
        }
        itUser++;
    }
    return bWhite;
}

void NXS4HConfig::InitWhiteList()
{
    std::string strWhiteListUser = _value_map_dynamic[WHITELIST_USER];

    strWhiteListUser.erase(0, strWhiteListUser.find_first_not_of(" "));
    strWhiteListUser.erase(strWhiteListUser.find_last_not_of(" ") + 1);

    if (strWhiteListUser.empty())
    {
        return;
    }

    if(',' != strWhiteListUser[strWhiteListUser.length()-1])
    {
        strWhiteListUser += ", ";
    }

    _whitelistUsers.reserve(32);
    std::string delimiters = ", ";
    CommonFun::split(strWhiteListUser, _whitelistUsers, delimiters);
}

void NXS4HConfig::InitConfigValue()
{
    //white list
    InitWhiteList();

    //column alias
    InitColumnAlias();

    //deactive dae
    _bDeactiveDAE = (CommonFun::StrCaseCmp(CONF_TRUE, _value_map_dynamic[DEACTIVE_DAE])==0);
    _view_clear_cycle = _value_map_dynamic[VIEW_CLEAR_CYClE];
}

bool NXS4HConfig::InitConfig(const std::string & path) {
    _configFile = path;
    std::string read;
    if(!read_from_file(read, path)) {
        return false;
    }

    if (!read.empty()) {
        ParseConfig(read);
        _value_map = _value_map_dynamic;

       InitConfigValue();
    }

    _time_last_update = time(nullptr);

    return !read.empty(); 
}

void NXS4HConfig::ParseConfig(const std::string& read)
{        
    unsigned int pos = 0;
    size_t len = read.length();

    std::string key;
    std::string value;
    std::string title = "t";
    bool iskey = true;
    while(pos < len) {
        switch (read[pos]) {
            case ' ':
            case '\r':
            case '\t':     {
                pos++;
            } break;
            case ';':     {
                while(pos<len && read[pos] != '\n') {
                    pos++;
                }
            } break;
            case '[':{
                title = parse_title( read, pos);
            }
            case '\n': {
                if(!key.empty() && !value.empty()) {
                    std::string mapkey;
                    mapkey+=title;
                    mapkey+='.';
                    mapkey+=key;

                    #if 0
                    //theLog->WriteLog(log_debug, "InitConfig|%s=%s", key.c_str(), value.c_str());
                    if (boost::equals("ccpwd", key) || boost::equals("jpcpwd", key)) {
                        //input text : 123blue!
                        //ciphertext : F2C8183110F0EA71BA227CB9D2CC36DA
                        //plain text : 123blue!
                        if (std::string::npos == value.find_first_not_of("0123456789ABCDEF")) {
                            CommonFun::AesEncryptor theAes;
                            value = theAes.DecryptString(value);
                            //theLog->WriteLog(log_debug, "After decrypting, %s=%s", key.c_str(), value.c_str());
                        } else {
                            theLog->WriteLog(log_info, "An unencrypted password string ('%s') was detected!", key.c_str() );
                        }
                    }
                    #endif

                    _value_map_dynamic.insert(make_pair(mapkey, value));
                }
                key.clear();
                value.clear();
                iskey = true;
                pos++;
            } break;
            case '=': {
                iskey = false;
                pos++;
            } break;
            default:
                std::string ss = parse_value(read, pos, iskey);
                if(iskey){
                    key = ss;
                } else {
                    value = ss;
                }
        }

    }

    return;
}

const std::string & NXS4HConfig::operator[](const std::string & key) {
    return _value_map[key];
}

const std::string NXS4HConfig::Print() {
    std::string ret;
    ret+="(CONFIG: ";
    for(auto it:_value_map){
        ret+="( ";
        ret+=it.first;
        ret+=": ";
        ret+=it.second;
        ret+="), ";
    }
    ret+=")\nWhite list users:";
 
    for (auto itUser : _whitelistUsers){
        ret += itUser + ",";
    }
    
    return ret;
}

const std::vector<std::set<std::string>>&  NXS4HConfig::GetColumnAlias()
{
    return _ColumnAliasSet;
}

void NXS4HConfig::InitColumnAlias()
{
    const static std::string strAliasConfigKeyBegin = "COLUMNALIAS.";
    ConfigMap::iterator itConfValue = _value_map_dynamic.begin();
    while (itConfValue != _value_map_dynamic.end()){
        const std::string& strConfKey = itConfValue->first;
        if(strConfKey.find(strAliasConfigKeyBegin)==0){
            std::set<std::string> aliasSet;
            //get alias name
            std::string strAliasName = strConfKey.substr(strAliasConfigKeyBegin.size());
            aliasSet.insert(strAliasName);

            //get alias value
            std::vector<std::string> aliasValues;
            const static std::string delimiters = ", ";
            CommonFun::split(itConfValue->second, aliasValues, delimiters);
            for (auto itAliasValue : aliasValues) {
                aliasSet.insert(itAliasValue);
            }

            //put to vector
            if(aliasSet.size()>1){
                _ColumnAliasSet.push_back(aliasSet);
            }
        }

        itConfValue++;
    }
}

std::string NXS4HConfig::PrintColumnAlias()
{
   std::string strColumnAlias = "Column alias:";

    std::vector<std::set<std::string>>::iterator itColumnAlias = _ColumnAliasSet.begin();
    while (itColumnAlias!=_ColumnAliasSet.end()){
        strColumnAlias += "\n";
        std::set<std::string>::iterator itColumn = itColumnAlias->begin();
        while (itColumn != itColumnAlias->end()){
            strColumnAlias += *itColumn + ",";
            itColumn++;
        }
        
        itColumnAlias++;
    }
    
    return strColumnAlias;
}

std::string NXS4HConfig::PrintWhiteListUser()
{
    std::string strWhiteListUser="User white list:";
    auto itUserWhite = _whitelistUsers.begin();
    while (itUserWhite != _whitelistUsers.end()){
        strWhiteListUser += *itUserWhite + ",";
        itUserWhite++;
    }
    return strWhiteListUser;    
}


void NXS4HConfig::CycleUpdate()
{
    if(difftime(time(nullptr), _time_last_update) > _update_cycle){

        _time_last_update = time(nullptr);

        _whitelistUsers.clear();
        _ColumnAliasSet.clear();
        _value_map_dynamic.clear();

        //read config again
        std::string read;
        if(!read_from_file(read, _configFile)) {
            return ;
        }

        if (!read.empty()) {
            ParseConfig(read);

            InitConfigValue();
        }
    }
}

std::string NXS4HConfig::GetSchema()
{
    std::string strSchema = CONF_PRIDATA_SCHEMA;
    if(strSchema.empty())
    {
        strSchema = "SAPSR3";
    }

    return strSchema;
}

bool read_from_file(std::string & ret, const std::string& file_path)
{
    std::ifstream t(file_path);
    ret = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return true;
}

std::string NXS4HConfig::parse_value(const std::string & read,unsigned int & pos,bool iskey) {
    std::string ret;
    while(pos < read.length()) {
        if(read[pos]=='\0' || read[pos] == '\n' ||  read[pos] == '\r' )
            break;
        if(iskey ){
            if(read[pos] == '\0' || 
                read[pos] == ' ' || 
                read[pos] == '\n' || 
                read[pos] == '\r'|| 
                read[pos] == '\t' ||
                read[pos] == '=') 
                break;
        }
        ret+=read[pos];
        pos++;
    }
    return ret;
}

std::string NXS4HConfig::parse_title(const std::string & read,unsigned int & pos) {
    pos++;
    std::string ret;
    while(pos < read.length()) {
        if(read[pos]=='\0' || read[pos]==' ' || read[pos] == '\n' || read[pos] == '\r'|| read[pos] == '\t')
            break;
        if(read[pos] == ']') {
            pos++;
            break;
        }
        ret+=read[pos];
        pos++;
    }
    return ret;
}

int NXS4HConfig::GetJPCVersion(){
    const std::string& ret = _value_map["POLICY.jpc_version"];
    if(ret.empty()) return 0;
    return int(atoi(ret.c_str()));
}

bool NXS4HConfig::UseSecurityTableList(){
    const std::string& ret = _value_map["POLICY.use_security_table_policy"];
    return CommonFun::StrCaseCmp(ret, CONFVAL_TRUE)==0;
}

const std::string NXS4HConfig::GetJPCTokenPath(){
    std::string ret = _value_map["POLICY.token_path"];
    return ret;
}

const std::string NXS4HConfig::GetJPCPDPPath(){
    return _value_map["POLICY.pdp_path"];
}


const unsigned int NXS4HConfig::GetQueryPCTimeout(){

    std::string ret = _value_map["policy_timeout"]; //read from table first.
    if(ret.empty()){
        ret=_value_map["POLICY.timeout"]; //read frm DAE.conf
    }
    if (ret.empty()){
        return 5;
    }else{
        return atoi(ret.c_str());
    }
}
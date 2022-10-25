#pragma once
#include <string>
#include <vector>
#include "SQLDBC_Types.h"
#include <memory>
#include "json\json.h"

#define LOG(format, ...) fprintf(stdout, format, __VA_ARGS__)



class CONFIG
{
public:
	std::string _strDBHost;
	std::string _strUser;
	std::string _strPwd;
	std::string _strDB;
	std::string _libDBPath;
	int  _repeatTime;

public:
	Json::Value _cfgJsonRoot;
};

class SQLParam {
public:
	std::string _name;
	std::string _value;
};

class SQLItem {
public:
	std::string _sql;
	std::vector<SQLParam> _Params;
	SQLDBC_Retcode _nResult;
	bool _bIgnore;
};

typedef std::vector<std::shared_ptr<SQLItem>> LIST_SQLITEMS;

class CSqlProv
{
public:
	CSqlProv();
	~CSqlProv();

public:
unsigned long ReadSQLFromFile(const std::string &file);
unsigned long ReadSQLWithParamFromFile(CONFIG& cfg);

public:
	LIST_SQLITEMS _vecSQL;
	int _nIgnoreCount;

};


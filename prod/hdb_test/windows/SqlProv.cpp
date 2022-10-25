#include "stdafx.h"
#include "SqlProv.h"
#include <istream>
#include <fstream>
#include <memory.h>
#include "json\json.h"

CSqlProv::CSqlProv()
{
	_nIgnoreCount = 0;
}


CSqlProv::~CSqlProv()
{
}

unsigned long CSqlProv::ReadSQLWithParamFromFile(CONFIG& cfg)
{
	
	Json::Value& root = cfg._cfgJsonRoot;
	Json::Value &jsonSQLs = root["SQLS"];
	if (!jsonSQLs.isArray()){
		LOG("sql file format is incorrect\n");
		return 0;
	}

	for (auto itSQL = jsonSQLs.begin(); itSQL != jsonSQLs.end(); ++itSQL) {

		std::shared_ptr<SQLItem> pSQLItem(new SQLItem);

		//sql
		pSQLItem->_sql = itSQL->operator[]("text").asString();

		//params
		Json::Value& jsonParams = itSQL->operator[]("params");
		if ((!jsonParams.isNull()) && jsonParams.isArray() ) {
			for (auto itParam = jsonParams.begin(); itParam != jsonParams.end(); itParam++) {
				SQLParam param;
				param._name = itParam->operator[]("name").asString();
				param._value = itParam->operator[]("value").asString();
				pSQLItem->_Params.push_back(param);
			}
		}

		
		//bool
		pSQLItem->_bIgnore = itSQL->operator[]("ignore").asBool();
		if (pSQLItem->_bIgnore) {
			_nIgnoreCount++;
		}

		_vecSQL.push_back(pSQLItem);
	}

	return _vecSQL.size();
}


unsigned long CSqlProv::ReadSQLFromFile(const std::string &file)
{
	std::ifstream t(file);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	int i = 0;
	int size = str.length();
	std::string sql;
	while (i < size) {
		if (str[i] == '\n' ) {
			SQLItem sqlItem;
			sqlItem._sql = sql;
			if (!sql.empty()) {
				//_vecSQL.push_back(sqlItem);
			   }
				
				sql.clear();
		}
		else {
			sql += str[i];
		}
		i++;
	}

	return _vecSQL.size();
}

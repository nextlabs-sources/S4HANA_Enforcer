#pragma once
#include "HookManager.h"
#include <string>
#include <vector>

struct EMParamItem {
	unsigned int _index;
	std::string _data;
	unsigned int _len;
};

typedef std::vector<EMParamItem*> EMParamItemVec;
typedef std::vector<EMParamItemVec*> EMParamItemVecVec;

struct EMSqlItem {
	unsigned int _id;
	std::string _sql;
	EMParamItemVecVec _params;
};


typedef bool (WINAPI *fn_read_test_data_from_file)(std::vector<EMSqlItem *> &sqls, const std::string &file);


class MyTest
{
public:
	MyTest();
	~MyTest();

public:
	static MyTest* Instance() {
		return &_instance;
	}

	bool Init();

private:
	DWORD GetFuncOffset(const char* szModName, const char* szFuncName);

private:
	static bool My_read_test_data_from_file(std::vector<EMSqlItem *> &sqls, const std::string &file);

private:
	static MyTest _instance;
	HookManager* _hookMgr;

private:
	static fn_read_test_data_from_file _read_test_data_from_file_old;
};


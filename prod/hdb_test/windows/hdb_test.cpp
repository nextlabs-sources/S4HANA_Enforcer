// hdb_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyHDBWrapper.h"
#include <memory>
#include <string>
#include <vector>
#include <istream>
#include <fstream>
#include <memory.h>
#include "json\json.h"
#include "SqlProv.h"
#include <process.h>
#include <stdlib.h>

CONFIG theConfig;
bool GetConfigureData(const char* pCfgData, DWORD dwSizeL, CONFIG& cfg)
{
	Json::CharReaderBuilder builder;
	std::unique_ptr<Json::CharReader> pJsonRead(builder.newCharReader());
	Json::String jsonErr;
	if (!pJsonRead->parse(pCfgData, pCfgData + dwSizeL, &cfg._cfgJsonRoot, &jsonErr)) {
		LOG("json string is incorrect, error:%s\n", jsonErr.c_str());
		return false;
	}

	//get server info
	Json::Value& root = cfg._cfgJsonRoot;
	Json::Value& jsonServerInfo = root["SERVER"];
	cfg._strDBHost = jsonServerInfo["host"].asString();
	cfg._strDB = jsonServerInfo["db"].asString();
	cfg._strUser = jsonServerInfo["user"].asString();
	cfg._strPwd = jsonServerInfo["pwd"].asString();

	//database library
	cfg._libDBPath = root["DBLIB"].asString();
	cfg._repeatTime = atoi(root["REPEAT"].asString().c_str());

	
	return true;

}

void PrintConfigureInfo(const CONFIG& theConfig)
{
	LOG("Server:%s\n", theConfig._strDBHost.c_str());
	LOG("Database:%s\n", theConfig._strDB.c_str());
	LOG("User:%s\n", theConfig._strUser.c_str());
	LOG("Pwd:%s\n", theConfig._strPwd.c_str());
	LOG("LibFile:%s\n", theConfig._libDBPath.c_str());
	LOG("Repeat:%d\n", theConfig._repeatTime);
}

int main(int argc, char** argv)
{
	if (argc<2){
		LOG("usage disp+work.exe configfile\n");
		return 1;
	}

	int nProcID = ::_getpid();
    LOG("Porcess id:%d\n", nProcID);

    //set env SAPSYSTEMNAME=hdb_test
    const char* szEnvName = "SAPSYSTEMNAME";
    const char* szSID = "hdb_test";
    if (::_putenv_s(szEnvName, szSID))     {
        LOG("set env failed, %s=%s\n", szEnvName, szSID);
        return 1;
    }
	LOG("Set environment variable, %s=%s\n", szEnvName, szSID);

	const std::string strCfgFile = argv[1];

	//read file
	HANDLE hFile = CreateFileA(strCfgFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}
	DWORD dwSizeH = 0;
	DWORD dwSizeL = GetFileSize(hFile, &dwSizeH);
	char* pCfgData = NULL;
	HANDLE hMap = ::CreateFileMappingA(hFile, NULL, PAGE_READONLY, dwSizeH, dwSizeL, NULL);
	if (hMap)
	{
		pCfgData = (char*)::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
		if (NULL == pCfgData)
		{
			LOG("MapViewOfFile failed. last error=%d\n", GetLastError());
		}
	}


	//get configure data
	CONFIG theConfig;
	bool bCfg = GetConfigureData(pCfgData, dwSizeL, theConfig);
	PrintConfigureInfo(theConfig);
	
	//get sql statement
	CSqlProv theSqlProv;
	unsigned long uSQLCount = theSqlProv.ReadSQLWithParamFromFile(theConfig);
	LOG("SQL count:%d, ignore:%d\n", uSQLCount, theSqlProv._nIgnoreCount);
	
	//release memory
	theConfig._cfgJsonRoot.clear();
	UnmapViewOfFile(pCfgData);
	pCfgData = NULL;
	CloseHandle(hMap);
	hMap = NULL;
	CloseHandle(hFile);
	hFile = NULL;

	//load database library
	MyHDBWrapper theHDBWrapper;
	theHDBWrapper.LoadLibrary(theConfig._libDBPath.c_str());

	//connect to database
	//BOOL bConn = theHDBWrapper.ConnectDB(NULL, NULL, NULL, NULL);
	BOOL bConn = theHDBWrapper.ConnectDB(theConfig._strDBHost.c_str(), theConfig._strUser.c_str(), theConfig._strPwd.c_str(), theConfig._strDB.c_str());
	if (!bConn){
		LOG("Connect to server failed.\n");
		return 1;
	}

	//call first prepare. during this prepare, DAE will do initialize
	theHDBWrapper.BeforeExecute();

	//execute sql
	LOG("Press Enter to start...\n");
	getchar();
	const int nExeTims = theConfig._repeatTime;
	int nExeI = 0;
	while (nExeI<nExeTims || nExeTims==-1)
	{
		LOG("Execute times:%d\n", ++nExeI);
		theHDBWrapper.Execute(theSqlProv._vecSQL);
		//Sleep(10);
	}
	
	LOG("Press anykey to exit\n");
	getchar();

    return 0;
}

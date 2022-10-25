#include "stdafx.h"
#include "MyHDBWrapper.h"
#include <string>
#include <locale>
#include <codecvt>
#include <memory>
#include <functional>

typedef struct ConnectArgsT {
	char * username;
	char * password;
	char * dbname;
	char * host;
} ConnectArgsT;

ConnectArgsT connectArgs;

MyHDBWrapper::MyHDBWrapper()
{
	_pConn = NULL;
}


MyHDBWrapper::~MyHDBWrapper()
{
	
}

BOOL MyHDBWrapper::LoadLibrary(const char* szHdbLib)
{
	m_modHandle = NULL;
	m_pGetClientRuntime = NULL;
	m_pSQLDBC_Environment_constructor = NULL;
	m_pSQLDBC_Environment_createConnection = NULL;
	m_pSQLDBC_Connection_connect = NULL;
	m_modHandle = ::LoadLibraryA(szHdbLib);

	if (!m_modHandle)
	{
		LOG("Load library failed. last error=%d\n", GetLastError());
		return FALSE;
	}
	m_pGetClientRuntime = (FN_GetClientRuntime)GetProcAddress(m_modHandle, "?GetClientRuntime@SQLDBC@@YAPEAVRuntime@1@PEADH@Z");
	m_pSQLDBC_Environment_constructor = (FN_SQLDBC_Environment_constructor)GetProcAddress(m_modHandle, "??0SQLDBC_Environment@SQLDBC@@QEAA@PEAVRuntime@1@@Z");
	m_pSQLDBC_Environment_createConnection = (FN_SQLDBC_Environment_createConnection)GetProcAddress(m_modHandle, "?createConnection@SQLDBC_Environment@SQLDBC@@QEBAPEAVSQLDBC_Connection@2@XZ");
	m_pSQLDBC_Connection_connect = (FN_SQLDBC_Connection_connect)GetProcAddress(m_modHandle, "?connect@SQLDBC_Connection@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD000@Z");
	m_pSQLDBC_Connection_createStatement = (FN_SQLDBC_Connection_createStatement)GetProcAddress(m_modHandle, "?createStatement@SQLDBC_Connection@SQLDBC@@QEAAPEAVSQLDBC_Statement@2@XZ");
	m_pSQLDBC_Connection_createPreparedStatement= (FN_SQLDBC_Connection_createPreparedStatement)GetProcAddress(m_modHandle, "?createPreparedStatement@SQLDBC_Connection@SQLDBC@@QEAAPEAVSQLDBC_PreparedStatement@2@XZ");
	m_pSQLDBC_Connection_releaseStatement = (FN_SQLDBC_Connection_releaseStatement)GetProcAddress(m_modHandle, "?releaseStatement@SQLDBC_Connection@SQLDBC@@QEAAXPEAVSQLDBC_Statement@2@@Z");
	m_pSQLDBC_Connection_releasePreparedStatement = (FN_SQLDBC_Connection_releasePreparedStatement)GetProcAddress(m_modHandle, "?releaseStatement@SQLDBC_Connection@SQLDBC@@QEAAXPEAVSQLDBC_PreparedStatement@2@@Z");
	m_pSQLDBC_Statement_execute = (FN_SQLDBC_Statement_execute)GetProcAddress(m_modHandle, "?execute@SQLDBC_Statement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD@Z");
	m_pSQLDBC_Statement_getResultSet = (FN_SQLDBC_Statement_getResultSet)GetProcAddress(m_modHandle, "?getResultSet@SQLDBC_Statement@SQLDBC@@QEAAPEAVSQLDBC_ResultSet@2@XZ");
	m_pSQLDBC_ResultSet_next = (FN_SQLDBC_ResultSet_next)GetProcAddress(m_modHandle, "?next@SQLDBC_ResultSet@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");
	m_pSQLDBC_ResultSet_getObject = (FN_SQLDBC_ResultSet_getObject)GetProcAddress(m_modHandle, "?getObject@SQLDBC_ResultSet@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@HW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
	m_pSQLDBC_ResultSet_getResultSetMetaData = (FN_SQLDBC_ResultSet_getResultSetMetaData)GetProcAddress(m_modHandle, "?getResultSetMetaData@SQLDBC_ResultSet@SQLDBC@@QEAAPEAVSQLDBC_ResultSetMetaData@2@XZ");
	m_pSQLDBC_ResultSetMetaData_getColumnCount = (FN_SQLDBC_ResultSetMetaData_getColumnCount)GetProcAddress(m_modHandle, "?getColumnCount@SQLDBC_ResultSetMetaData@SQLDBC@@QEAAHXZ");
	m_SQLDBC_PreparedStatement_prepare= (FN_SQLDBC_PreparedStatement_prepare)GetProcAddress(m_modHandle, "?prepare@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD_JW4Encoding@SQLDBC_StringEncodingType@@@Z");
	m_SQLDBC_SQLDBC_PreparedStatement_execute = (FN_SQLDBC_SQLDBC_PreparedStatement_execute)GetProcAddress(m_modHandle, "?execute@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");
	m_SQLDBC_ConnectionItem_error = (FN_SQLDBC_ConnectionItem_error)GetProcAddress(m_modHandle, "?error@SQLDBC_ConnectionItem@SQLDBC@@QEAAAEAVSQLDBC_ErrorHndl@2@XZ");
	m_SQLDBC_ErrorHndl_getErrorText = (FN_SQLDBC_ErrorHndl_getErrorText)GetProcAddress(m_modHandle, "?getErrorText@SQLDBC_ErrorHndl@SQLDBC@@QEBAPEBDXZ");
	m_SQLDBC_Statement_getConnection = (FN_SQLDBC_Statement_getConnection)GetProcAddress(m_modHandle, "?getConnection@SQLDBC_Statement@SQLDBC@@QEAAPEAVSQLDBC_Connection@2@XZ");
	m_SQLDBC_PreparedStatement_bindParameter = (FN_SQLDBC_PreparedStatement_bindParameter)GetProcAddress(m_modHandle, "?bindParameter@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@IW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
	m_SQLDBC_Statement_getRowsAffected = (FN_SQLDBC_Statement_getRowsAffected)GetProcAddress(m_modHandle, "?getRowsAffected@SQLDBC_Statement@SQLDBC@@QEBA_JXZ");
	m_SQLDBC_SQLDBC_Statement_isQuery = (FN_SQLDBC_SQLDBC_Statement_isQuery)GetProcAddress(m_modHandle,"?isQuery@SQLDBC_Statement@SQLDBC@@QEBA_NXZ");
	m_SQLDBC_SQLDBC_ResultSet_getResultCount = (FN_SQLDBC_SQLDBC_ResultSet_getResultCount)GetProcAddress(m_modHandle, "?getResultCount@SQLDBC_ResultSet@SQLDBC@@QEBA_JXZ");
	m_SQLDBC_PreparedStatement_bindParameterAddr = (FN_SQLDBC_PreparedStatement_bindParameterAddr)GetProcAddress(m_modHandle, "?bindParameterAddr@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@IW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
	m_SQLDBC_PreparedStatement_setBatchSize=(FN_SQLDBC_PreparedStatement_setBatchSize)GetProcAddress(m_modHandle, "?setBatchSize@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@I@Z");
	m_SQLDBC_PreparedStat_SetBindingType = (FN_SQLDBC_PreparedStat_SetBindingType)GetProcAddress(m_modHandle, "?setBindingType@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@_K@Z");
	m_SQLDBC_Connection_Commit = (FN_SQLDBC_Connection_Commit)GetProcAddress(m_modHandle, "?commit@SQLDBC_Connection@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");
	return TRUE;
}

std::u16string FromUTF8(const std::string& src) 
{
	std::u16string dst;
	try
	{
#if (defined _WIN32) && (_MSC_VER >= 1900)
		auto p = std::wstring_convert< std::codecvt_utf8<int16_t>, int16_t >{}.from_bytes(src);
		auto t = reinterpret_cast<const char16_t *>(p.data());
		dst = std::u16string(t, t + p.size());
#else
		dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(src);

		//        for (size_t i = 0; i < dst.length(); i++)
		//        {
		//            dst[i] = htobe16(dst[i]);
		//        }

#endif
	}
	catch (const std::exception& e)
	{
		dst.clear();
	}

	return dst;
}

const char* MyHDBWrapper::GetErrorText(void* pConnectItem)
{
	SQLDBC::SQLDBC_ErrorHndl& errorHdl = m_SQLDBC_ConnectionItem_error(pConnectItem);
	return m_SQLDBC_ErrorHndl_getErrorText(&errorHdl);
}

void MyHDBWrapper::BeforeExecute()
{
	std::u16string u16SQL = u"SELECT * FROM DRAW";
	void* pStatement = m_pSQLDBC_Connection_createPreparedStatement(_pConn);
	if (pStatement == nullptr) {
		LOG("Create Statement failed, error=%s\n", GetErrorText(_pConn));
		return;
	}


	//prepare
	SQLDBC_Retcode rc = m_SQLDBC_PreparedStatement_prepare(pStatement, (char*)u16SQL.c_str(),
		u16SQL.size() * 2, SQLDBC_StringEncodingType::UCS2Swapped);
	if (SQLDBC_OK != rc) {
		LOG("Prepare failed, error=%s\n", GetErrorText(pStatement));
		return;
	}
}

//function used in std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement> to delete statement automatically
void MyHDBWrapper::DeletePraparedStatement(void* pPreparedStamt) 
{
	auto hdb_connection = m_SQLDBC_Statement_getConnection(pPreparedStamt);
	m_pSQLDBC_Connection_releasePreparedStatement(hdb_connection, pPreparedStamt);
}


void MyHDBWrapper::Execute(LIST_SQLITEMS& vecSQLItem)
{
	int nSQL = 0;
	for (auto itSQL = vecSQLItem.begin(); itSQL != vecSQLItem.end(); itSQL++, nSQL++) {
		auto pSQLItem = *itSQL;
		if (pSQLItem->_bIgnore){
			continue;
		}

		LOG("Begin execute,%d, sql:%s\n", nSQL, pSQLItem->_sql.c_str());
		void* pStatement = NULL;
		do 
		{
			//create statement
			pStatement = m_pSQLDBC_Connection_createPreparedStatement(_pConn);
			if (pStatement == nullptr) {
				LOG("Create Statement failed, error=%s\n", GetErrorText(_pConn));
				pSQLItem->_nResult = SQLDBC_OK;
				continue;
			}

			//prepare
			std::u16string u16SQL = FromUTF8(pSQLItem->_sql);
			SQLDBC_Retcode rc = m_SQLDBC_PreparedStatement_prepare(pStatement, (char*)u16SQL.c_str(),
				u16SQL.size() * 2, SQLDBC_StringEncodingType::UCS2Swapped);
			if (SQLDBC_OK != rc) {
				LOG("Prepare failed, error=%s\n", GetErrorText(pStatement));
				break;
			}

			//bind param
			int iParam = 1;
			SQLDBC_Length nLen = 0;
			SQLDBC_Length nLenInd = SQLDBC_NTS;
			for (auto itParam = pSQLItem->_Params.begin(); itParam!= pSQLItem->_Params.end(); itParam++, iParam++)
			{
				nLen = itParam->_value.size();
				rc = m_SQLDBC_PreparedStatement_bindParameter(pStatement, iParam, SQLDBC_HOSTTYPE_ASCII, 
											(void*)itParam->_value.c_str(), &nLenInd, nLen, TRUE );

				if (SQLDBC_OK != rc) {
					LOG("bind failed, error=%s\n", GetErrorText(pStatement));
					break;
				}
			}
			

			//execute
			rc = m_SQLDBC_SQLDBC_PreparedStatement_execute(pStatement);
			if (SQLDBC_OK != rc && rc!=SQLDBC_NO_DATA_FOUND) {
				LOG("Execute failed, rc=%d, error=%s\n", rc, GetErrorText(pStatement));
			}
			else {
				SQLDBC_Bool bIsQuery = m_SQLDBC_SQLDBC_Statement_isQuery(pStatement);
				if (bIsQuery) {
					void* pResultSet = m_pSQLDBC_Statement_getResultSet(pStatement);
					if (pResultSet){
						//SQLDBC_Int4 nRowCount = m_SQLDBC_SQLDBC_ResultSet_getResultCount(pResultSet);
						SQLDBC_Int4 nRowCount = 0;
						while (m_pSQLDBC_ResultSet_next(pResultSet) == SQLDBC_OK) {
							nRowCount++;
						}

						LOG("Execute success. result rows:%d\n", nRowCount);
					}
					else {
						LOG("Execute success. can't get result set\n");
					}
				}
				else {
					int n = m_SQLDBC_Statement_getRowsAffected(pStatement);
					LOG("Execute success. rows affected:%d\n", n);
				}

				
			}
			pSQLItem->_nResult = rc;

			

		} while (false);

		if (pStatement!=nullptr)
		{
			m_pSQLDBC_Connection_releasePreparedStatement(_pConn, pStatement);
			pStatement = NULL;
		}
		


   }

}


BOOL MyHDBWrapper::ConnectDB(const char* szServer, const char* szUser, const char* szPwd,
	                        const char* szDB)
{
	char errorText[200] = { 0 };

	void *pRuntime = m_pGetClientRuntime(errorText, sizeof(errorText));
	if (pRuntime == nullptr) {
		LOG("Get ClientRuntime failed\n");
		return false;
	}

	void *pEnv = malloc(sizeof(SQLDBC::SQLDBC_Environment));
	m_pSQLDBC_Environment_constructor(pEnv, pRuntime);
	if (pEnv == nullptr) {
		LOG("Create SQLDBC_Environment failed\n");
		return false;
	}

	/*
	* Create a new connection object and open a session to the database.
	*/
	_pConn = m_pSQLDBC_Environment_createConnection(pEnv);
	if (_pConn == nullptr) {
		LOG("Create Connection failed\n");
		return false;
	}
	
#if 0
	//192.168.187.160 -d D39 -u SAPHANADB -p Next1234
	std::string strServer = "192.168.187.160:30215";
	std::string strUser = "SAPHANADB";
	std::string strPwd = "Next1234";
	std::string strDB = "D39";
#elif 0
	//10.23.56.70:39015; Uid = SYSTEM; PWD = 123Blue123Blue
	std::string strServer = "10.23.56.70:39015";
	std::string strUser = "SYSTEM";
	std::string strPwd = "123Blue123Blue";
	std::string strDB = "HXE";
#else
	std::string strServer = szServer;
	std::string strUser = szUser;
	std::string strPwd = szPwd;
	std::string strDB = szDB;
#endif 

	
	SQLDBC_Retcode rc = m_pSQLDBC_Connection_connect(_pConn, strServer.c_str(), strDB.c_str(), strUser.c_str(), strPwd.c_str() );
	if (SQLDBC_OK != rc) {
		LOG("Connecting to the database failed\n" );
		return FALSE;
	}
	LOG("The database connected sucessfully\n");
	return TRUE;
}

#if 0
void MyHDBWrapper::Test1()
{
	void *pConn = ConnectDB();
	if (pConn == nullptr) {
		OutputDebugStringA("Connection null\n");
		return;
	}
	
	//create sql
	//int logonHdl = 1;
	//const char sqlFmt[] = "SELECT BNAME,TERMID,TERMINAL FROM USR41 WHERE TERMID=%d LIMIT 1;";
	//char szSql[sizeof(sqlFmt) + 100];
	//sprintf_s(szSql, sqlFmt, logonHdl);
	//const char szSql[] = "SELECT BNAME,TERMID,TERMINAL FROM USR41 ORDER BY TERMID DESC LIMIT 3;";
	const char szSql[] = "SELECT * FROM BUS0T LIMIT 3;";

	void* pStatement = m_pSQLDBC_Connection_createStatement(pConn);
	if (pStatement == nullptr) {
		OutputDebugStringA("Create Statement failed\n");
		return;
	}
	SQLDBC_Retcode rc = m_pSQLDBC_Statement_execute(pStatement, szSql);
	if (SQLDBC_OK != rc) {
		OutputDebugStringA("Execute failed\n");
		return;
	}
	void* pResultSet = m_pSQLDBC_Statement_getResultSet(pStatement);
	if (pResultSet == nullptr) {
		OutputDebugStringA("Get ResultSet failed\n");
		return;
	}
	rc = m_pSQLDBC_ResultSet_next(pResultSet);
	if (SQLDBC_OK != rc) {
		OutputDebugStringA("ResultSet next failed\n");
		return;
	}
	do
	{
		char szUserName[255] = { 0 };
		char szTermID[255] = { 0 };
		char szTerminal[255] = { 0 };
		SQLDBC_Length lenUserName = 0;
		SQLDBC_Length lenTermID = 0;
		SQLDBC_Length lenTerminal = 0;
		rc = m_pSQLDBC_ResultSet_getObject(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szUserName, &lenUserName, sizeof(szUserName), SQLDBC_TRUE);
		if (SQLDBC_OK != rc) {
			OutputDebugStringA("Error getObject1\n");
			return;
		}
		OutputDebugStringA(szUserName); OutputDebugStringA("\n");
		rc = m_pSQLDBC_ResultSet_getObject(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, szTermID, &lenTermID, sizeof(szTerminal), SQLDBC_TRUE);
		if (SQLDBC_OK != rc) {
			OutputDebugStringA("Error getObject2\n");
			return;
		}
		OutputDebugStringA(szTermID); OutputDebugStringA("\n");
		rc = m_pSQLDBC_ResultSet_getObject(pResultSet, 3, SQLDBC_HOSTTYPE_ASCII, szTerminal, &lenTerminal, sizeof(szTerminal), SQLDBC_TRUE);
		if (SQLDBC_OK != rc) {
			OutputDebugStringA("Error getObject3\n");
			return;
		}
		OutputDebugStringA(szTerminal); OutputDebugStringA("\n");

		rc = m_pSQLDBC_ResultSet_next(pResultSet);
	} while (rc == SQLDBC_OK);

	m_pSQLDBC_Connection_releaseStatement(pConn, pStatement);
}

void MyHDBWrapper::Test2()
{
	void *pConn = ConnectDB();
	if (pConn == nullptr) {
		OutputDebugStringA("Connection null\n");
		return;
	}

	const char szSql[] = "DELETE BUS0T WHERE BUS0T_B='BBG'";

	void* pStatement = m_pSQLDBC_Connection_createStatement(pConn);
	if (pStatement == nullptr) {
		OutputDebugStringA("Create Statement failed\n");
		return;
	}
	SQLDBC_Retcode rc = m_pSQLDBC_Statement_execute(pStatement, szSql);
	if (SQLDBC_OK != rc) {
		OutputDebugStringA("Execute failed\n");
		return;
	}

	m_pSQLDBC_Connection_releaseStatement(pConn, pStatement);
}
#endif 
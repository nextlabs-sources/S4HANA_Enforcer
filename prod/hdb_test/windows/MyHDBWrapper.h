#pragma once
#include "SQLDBC_Types.h"
#include "SQLDBC.h"
#include "SqlProv.h"

typedef void *(WINAPI *FN_GetClientRuntime)(char *, int);
typedef void(WINAPI *FN_SQLDBC_Environment_constructor)(void *pEnv, void *pRunTime);
typedef void *(WINAPI *FN_SQLDBC_Environment_createConnection)(void *pEnv);
typedef SQLDBC_Retcode(WINAPI *FN_SQLDBC_Connection_connect)(void *pConn, const char *servernode, const char *serverdb, const char *username, const char *password);
typedef void *(WINAPI *FN_SQLDBC_Connection_createStatement)(void *pConn);
typedef void *(WINAPI *FN_SQLDBC_Connection_createPreparedStatement)(void *pConn);
typedef void(WINAPI *FN_SQLDBC_Connection_releaseStatement)(void *pConn, void *pStatement);
typedef void(WINAPI *FN_SQLDBC_Connection_releasePreparedStatement)(void *pConn, void *pStatement);
typedef SQLDBC_Retcode(WINAPI *FN_SQLDBC_Statement_execute)(void *pThis, char const *szSql);
typedef void *(WINAPI *FN_SQLDBC_Statement_getResultSet)(void *pStatement);
typedef SQLDBC_Retcode(WINAPI *FN_SQLDBC_ResultSet_next)(void *pResultSet);
typedef SQLDBC_Retcode(WINAPI *FN_SQLDBC_ResultSet_getObject)(void *pResultSet, int Index,
	SQLDBC_HostType Type, void *paramAddr, __int64 *LengthIndicator, __int64 Size, bool Terminate);
typedef void *(WINAPI *FN_SQLDBC_ResultSet_getResultSetMetaData)(void *pResultSet);
typedef int(WINAPI *FN_SQLDBC_ResultSetMetaData_getColumnCount)(void *pMetaData);

typedef SQLDBC_Retcode(WINAPI  *FN_SQLDBC_PreparedStatement_prepare)(void* self, const char * 	sql,
	const SQLDBC_Length 	sqlLength,
	const SQLDBC_StringEncoding 	encoding
	);

typedef SQLDBC_Retcode(WINAPI * FN_SQLDBC_SQLDBC_PreparedStatement_execute)(void* self);

typedef SQLDBC::SQLDBC_ErrorHndl&(WINAPI* FN_SQLDBC_ConnectionItem_error)(void* self);

typedef  const char*(*FN_SQLDBC_ErrorHndl_getErrorText)(void* self);

typedef SQLDBC::SQLDBC_Connection*(*FN_SQLDBC_Statement_getConnection)(void* self);

typedef SQLDBC_Retcode(*FN_SQLDBC_PreparedStatement_bindParameter) (void *self,
	const SQLDBC_UInt2     Index,
	const SQLDBC_HostType  Type,
	void                  *paramAddr,
	SQLDBC_Length         *LengthIndicator,
	const SQLDBC_Length    Size,
	const SQLDBC_Bool      Terminate);

typedef const SQLDBC_Int4(*FN_SQLDBC_Statement_getRowsAffected)(void* self);

typedef SQLDBC_Bool(*FN_SQLDBC_SQLDBC_Statement_isQuery)(void* self);
typedef SQLDBC_Int4(*FN_SQLDBC_SQLDBC_ResultSet_getResultCount)(void* self);


typedef SQLDBC_Retcode(*FN_SQLDBC_PreparedStatement_bindParameterAddr)(void* self, const SQLDBC_UInt2 Index,
	const SQLDBC_HostType 	Type,
	void * 	paramAddr,
	SQLDBC_Length * 	LengthIndicator,
	const SQLDBC_Length 	Size,
	const SQLDBC_Bool 	Terminate
	);

typedef SQLDBC_Retcode(*FN_SQLDBC_PreparedStatement_setBatchSize)(void* self, SQLDBC_UInt4 rowarraysize);

typedef SQLDBC_Retcode(*FN_SQLDBC_PreparedStat_SetBindingType)(void *self, SQLDBC_size_t size);

typedef SQLDBC_Retcode(*FN_SQLDBC_Connection_Commit)(void* self);


class MyHDBWrapper
{
public:
	MyHDBWrapper();
	~MyHDBWrapper();

public:
	BOOL LoadLibrary(const char* szHdbLib);
	BOOL ConnectDB(const char* szServer, const char* szUser, const char* szPwd, const char*DB);
	void BeforeExecute();
	void Execute(LIST_SQLITEMS& vecSQLItem);

public:
	

private:
	HINSTANCE m_modHandle;

private:
	FN_GetClientRuntime m_pGetClientRuntime;
	FN_SQLDBC_Environment_constructor m_pSQLDBC_Environment_constructor;
	FN_SQLDBC_Environment_createConnection m_pSQLDBC_Environment_createConnection;
	FN_SQLDBC_Connection_connect m_pSQLDBC_Connection_connect;
	FN_SQLDBC_Connection_createStatement m_pSQLDBC_Connection_createStatement;
	FN_SQLDBC_Connection_releaseStatement m_pSQLDBC_Connection_releaseStatement;
	FN_SQLDBC_Connection_createPreparedStatement m_pSQLDBC_Connection_createPreparedStatement;
	FN_SQLDBC_Connection_releasePreparedStatement m_pSQLDBC_Connection_releasePreparedStatement;
	FN_SQLDBC_Statement_execute m_pSQLDBC_Statement_execute;
	FN_SQLDBC_Statement_getResultSet m_pSQLDBC_Statement_getResultSet;
	FN_SQLDBC_ResultSet_next m_pSQLDBC_ResultSet_next;
	FN_SQLDBC_ResultSet_getObject m_pSQLDBC_ResultSet_getObject;
	FN_SQLDBC_ResultSet_getResultSetMetaData m_pSQLDBC_ResultSet_getResultSetMetaData;
	FN_SQLDBC_ResultSetMetaData_getColumnCount m_pSQLDBC_ResultSetMetaData_getColumnCount;
	FN_SQLDBC_PreparedStatement_prepare m_SQLDBC_PreparedStatement_prepare;
	FN_SQLDBC_SQLDBC_PreparedStatement_execute m_SQLDBC_SQLDBC_PreparedStatement_execute;
	FN_SQLDBC_ConnectionItem_error m_SQLDBC_ConnectionItem_error;
	FN_SQLDBC_ErrorHndl_getErrorText m_SQLDBC_ErrorHndl_getErrorText;
	FN_SQLDBC_Statement_getConnection m_SQLDBC_Statement_getConnection;
	FN_SQLDBC_PreparedStatement_bindParameter m_SQLDBC_PreparedStatement_bindParameter;
	FN_SQLDBC_Statement_getRowsAffected m_SQLDBC_Statement_getRowsAffected;
	FN_SQLDBC_SQLDBC_Statement_isQuery m_SQLDBC_SQLDBC_Statement_isQuery;
	FN_SQLDBC_SQLDBC_ResultSet_getResultCount m_SQLDBC_SQLDBC_ResultSet_getResultCount;
	FN_SQLDBC_PreparedStatement_bindParameterAddr m_SQLDBC_PreparedStatement_bindParameterAddr;
	FN_SQLDBC_PreparedStatement_setBatchSize m_SQLDBC_PreparedStatement_setBatchSize;
	FN_SQLDBC_PreparedStat_SetBindingType m_SQLDBC_PreparedStat_SetBindingType;
	FN_SQLDBC_Connection_Commit m_SQLDBC_Connection_Commit;

private:
	const char* GetErrorText(void* pConnectItem);
	void DeletePraparedStatement(void* pPreparedStamt);

private:
	void* _pConn;
};


#ifndef DAE_LOCAL_CONFIG

#include "TableAliasMgr_remote.h"
#include "SQLDBC.h"
#include "talk_with_hdb.h"
#include "log.h"


TableAliasMgr::TableAliasMgr()
{
    _update_cycle = 3600;//second
    _bInited = false;
}

bool TableAliasMgr::Init(void* pDBConn, const double& updateCycle)
{
    //get update cycle
    _update_cycle = updateCycle;

    //read data from retmote 
    bool bRead = ReadFromRemote(pDBConn);
    _time_last_update = time(NULL);

     _bInited = true;
    
    return bRead;
}

void TableAliasMgr::UpdateClearCycleInterval(const double& updateCycle)
{
    _update_cycle = updateCycle;
}

void TableAliasMgr::CycleUpdate(void* pDBConn)
{
    time_t timeNow = time(NULL);
    if (difftime(timeNow, _time_last_update)>_update_cycle){
        ClearTableAlias();

        ReadFromRemote(pDBConn);
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

const TableAliasItem* TableAliasMgr::GetAliasItemByAliasTable(const std::string& strAliasTable)
{
    TableAliasItem* resultItem = NULL;
    auto itAliasItem = _alias.find(strAliasTable);
    if (itAliasItem!=_alias.end()){
         resultItem = itAliasItem->second;
    }

    return resultItem;
}

bool TableAliasMgr::ReadFromRemote(void* pDBConn)
{
    bool bRead = ReadAliasHeader(pDBConn);
    if (bRead){
        bRead = ReadAliasItem(pDBConn);
    }
    return true;
}

bool TableAliasMgr::ReadAliasItem(void* pDBConn)
{
    const char* szSql= "SELECT ORIGINAL_FIELD, ALIAS_FIELD FROM \"/NXLDAE/ALIAS_IT\" WHERE ORIGINAL_TABLE=? AND ALIAS_TABLE=?";

    //create statement
	SQLDBC::SQLDBC_Connection* hdb_connection = (SQLDBC::SQLDBC_Connection*)pDBConn;
    SQLDBC::SQLDBC_PreparedStatement* pStatement = NULL;
	do
	{
		pStatement = SQLDBC_Connection_createPreparedStatement_Old(hdb_connection);
	    if(pStatement==nullptr){
			theLog->WriteLog(log_error, "ReadAliasItem, createStatement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
			break;
		}

		SQLDBC_Retcode rc = prepare_3_old(pStatement, (char*)szSql, strlen(szSql), SQLDBC_StringEncodingType::Ascii);
        if (SQLDBC_OK != rc) {
			theLog->WriteLog(log_error, "ReadAliasItem, prepare_3_old failed, rc=%d", rc);
        	break;
   	 	}

        for (auto itAlias=_alias.begin(); itAlias!=_alias.end(); itAlias++){
            TableAliasItem* pAliasItem = itAlias->second;
            const std::string& originalTable = pAliasItem->OriginalTable();
            const std::string& aliasTable = pAliasItem->AliasTable();

            rc = bind_parameter_old(pStatement, 1, SQLDBC_HOSTTYPE_ASCII, (void *)originalTable.c_str(), nullptr, 0, SQLDBC_TRUE);
            if (SQLDBC_OK != rc) {
                theLog->WriteLog(log_error, "ReadAliasItem, bind_parameter_old 1 failed, rc=%d", rc);
                break;
            }

            rc = bind_parameter_old(pStatement, 2, SQLDBC_HOSTTYPE_ASCII, (void *)aliasTable.c_str(), nullptr, 0, SQLDBC_TRUE);
            if (SQLDBC_OK != rc) {
                theLog->WriteLog(log_error, "ReadAliasItem, bind_parameter_old 2 failed, rc=%d", rc);
                break;
            }

            rc = execute_old(pStatement);
			if(rc!=SQLDBC_OK){
				theLog->WriteLog(log_error, "ReadAliasItem, execute failed. o_table:%s, a_table:%S rc=%d, error=%s", originalTable.c_str(), aliasTable.c_str(),  rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
				continue;
			}

            auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				theLog->WriteLog(log_error, "ReadAliasItem, get_result_set==null, o_table:%s, a_talbe:%s", originalTable.c_str(), aliasTable.c_str() );
				continue;
			}

            while (true){

          		rc =  SQLDBC_ResultSet_next_Old(pResultSet);
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					if (rc!=SQLDBC_NO_DATA_FOUND){
						theLog->WriteLog(log_error, "ReadAliasItem, call next failed,o_talbe:%s, a_table:%s, rc=%d, error=%s", originalTable.c_str(), aliasTable.c_str(), rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					}
					break;
				}

				char originalField[31]={0}; //from sap table DD27S, field length is 30
				SQLDBC_Length nLenOField = 0;
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, originalField, &nLenOField, sizeof(originalField), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_error, "ReadAliasItem, get colomn 1 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				originalField[nLenOField] = 0;
                
				char aliasField[31]={0};
				SQLDBC_Length nLenAField = 0;
				 rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, aliasField, &nLenAField, sizeof(aliasField), SQLDBC_TRUE );
				if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
					theLog->WriteLog(log_error, "ReadAliasItem, get colomn 2 attribute value failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					continue;
				}
				aliasField[nLenAField] = 0;
				if (nLenOField>0 && nLenAField>0){
					pAliasItem->AddedFieldAlias(originalField, aliasField);
                    theLog->WriteLog(log_trace, "ReadAliasItem, added item:o_table:%s, a_talbe:%s, o_field:%s, a_field:%s", originalTable.c_str(), aliasTable.c_str(), originalField, aliasField);
				}

			}

        }        		
	} while (false);

    //release
    if (NULL != pStatement) {
        SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pStatement);
        pStatement = NULL;
    }

    return true;

}

bool TableAliasMgr::ReadAliasHeader(void* pDBConn)
{
    //create sql
	const char* szSql = "select ORIGINAL_TABLE, ALIAS_TABLE, RELATION, DESCRPTION from \"/NXLDAE/ALIAS_HD\"";
	
    //create statement
	SQLDBC::SQLDBC_Connection* hdb_connection = (SQLDBC::SQLDBC_Connection*)pDBConn;
    SQLDBC::SQLDBC_Statement* pStatement = NULL;
	do{
		pStatement = SQLDBC_Connection_createStatement_Old(hdb_connection);
	    if(pStatement==nullptr){
			theLog->WriteLog(log_error, "ReadAliasHeader, createStatement failed. error=%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(hdb_connection)) );
			break;
		}

		SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStatement, szSql);
		if(rc!=SQLDBC_OK){
			theLog->WriteLog(log_error, "ReadAliasHeader, execute failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
			break;
		}
	
		auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
		if (pResultSet==nullptr){
			theLog->WriteLog(log_error, "ReadAliasHeader, get_result_set==null");
			break;
		}

		while (true){
          	rc =  SQLDBC_ResultSet_next_Old(pResultSet);
			if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
			    if (rc!=SQLDBC_NO_DATA_FOUND){
					theLog->WriteLog(log_error, "ReadAliasHeader, call next failed, rc=%d, error=%s", rc,SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
				}	
				break;
			}
              
			    
            char originalTable[31]={0};
            SQLDBC_Length nLenOriginalTable = 0;
            rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, originalTable, &nLenOriginalTable, sizeof(originalTable)-1, SQLDBC_TRUE );
            if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                theLog->WriteLog(log_error, "ReadAliasHeader, get colomn 1 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                continue;
            }
            originalTable[nLenOriginalTable] = 0;


            char aliasTable[31]={0};
            SQLDBC_Length nLenAliasTable = 0;
            rc =SQLDBC_ResultSet_getObject_Old(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, aliasTable, &nLenAliasTable, sizeof(aliasTable)-1, SQLDBC_TRUE );
            if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                theLog->WriteLog(log_error, "ReadAliasHeader, get colomn 2 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                continue;
            }
            aliasTable[nLenAliasTable] = 0;

            char relation[1024]={0};
            SQLDBC_Length nLenRelation = 0;
            rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 3, SQLDBC_HOSTTYPE_ASCII, relation, &nLenRelation, sizeof(relation)-1, SQLDBC_TRUE );
            if((rc!=SQLDBC_OK) && (rc!=SQLDBC_SUCCESS_WITH_INFO)){
                theLog->WriteLog(log_error, "ReadAliasHeader, get colomn 3 attribute name failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
                continue;
            }
            relation[nLenRelation] = 0;

            if (nLenOriginalTable>0 && nLenAliasTable>0 && nLenRelation>0){
                 TableAliasItem* pTableAliasItem = new TableAliasItem(originalTable, aliasTable, relation);
                 _alias[aliasTable] = pTableAliasItem;
                theLog->WriteLog(log_trace, "ReadAliasHeader,  added alias header: original:%s,alias:%s,relation:%s", originalTable, aliasTable, relation);
            }
        
		}

    } while(false);

    //release
    if (NULL != pStatement) {
        SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
        pStatement = NULL;
    }

    return true;
}

std::string TableAliasMgr::Print()
{
    std::string strInfo = "TableAliasMgr::";
    return strInfo;
}

#endif // DAE_LOCAL_CONFIG
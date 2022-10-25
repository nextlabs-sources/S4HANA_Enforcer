
#include "../include/ViewManager.h"
#include "log.h"
#include "SQLDBC.h"
#include "../include/talk_with_hdb.h"
#include <memory>
#include "tool.h"
#include "../parser_view/view_interface.h"
#include "../parser_view/EMViewMgr.h"
#include "s4hexception.h"
#include "NXS4HConfig.h"

ViewManager* ViewManager::_pViewMgr = NULL;

  //when the view is created based on another view. use this funciton to replace it with the next one.
void ViewSourceInfo::ReplaceSourceInfo(const std::string& strTable, const ViewSourceInfo& dstSourceInfo)
{
	std::map<std::string, ViewColumnSourceInfo>::iterator itColumnSource = _SourceColumns.begin();
	while (itColumnSource!=_SourceColumns.end()){
	    ViewColumnSourceInfo& colSrcInfo = itColumnSource->second;
		if (CommonFun::StrCaseCmp(colSrcInfo.SourceTableName().c_str(), strTable.c_str())==0){

			//here is column actuall comes from another view, we need to replace its source
			const ViewColumnSourceInfo* pSubColSrcInfo = dstSourceInfo.GetSourceColumn(colSrcInfo.SourceColumnName());
			if (pSubColSrcInfo){
				colSrcInfo.SetSourceTableName(pSubColSrcInfo->SourceTableName().c_str());
			    colSrcInfo.SetSourceColumnName(pSubColSrcInfo->SourceColumnName().c_str());
				_sourceTables.insert(pSubColSrcInfo->SourceTableName() );
			}		
		}
		itColumnSource++;
	}

	//now replease the source table
	_sourceTables.erase(strTable);
}

void ViewSourceInfo::ClearSourceInfo()
{
	_sourceTables.clear();
	_SourceColumns.clear();
}

const ViewColumnSourceInfo* ViewSourceInfo::GetSourceColumn(const std::string& column) const
{
    std::map<std::string, ViewColumnSourceInfo>::const_iterator itColSrc = _SourceColumns.begin();
	while (itColSrc != _SourceColumns.end()){
		 if (CommonFun::StrCaseCmp(column.c_str(), itColSrc->first.c_str())==0) {
			 return &(itColSrc->second);
		 }
		 itColSrc++;
	}

	return NULL;
}

std::vector<const ViewColumnSourceInfo*> ViewSourceInfo::MatchSourceInfo(const std::string & col, const std::string & table, const std::string & tbcol) const{
    std::vector<const ViewColumnSourceInfo*> vecViewSourceInfo;
    std::map<std::string, ViewColumnSourceInfo>::const_iterator itColSrc = _SourceColumns.begin();
    while (itColSrc != _SourceColumns.end()){
        if (col.empty() || CommonFun::StrCaseCmp(col.c_str(), itColSrc->first.c_str())==0  ) {
            if( table.empty() || CommonFun::StrCaseCmp(table.c_str(), itColSrc->second.SourceTableName().c_str())==0){
                if( tbcol.empty() || CommonFun::StrCaseCmp(tbcol.c_str(), itColSrc->second.SourceColumnName().c_str())==0){
                    vecViewSourceInfo.push_back(&(itColSrc->second));
                }
            }
        }
        itColSrc++;
    }
    return vecViewSourceInfo;
}
//const ViewColumnSourceInfo* ViewSourceInfo::GetSourceColumn(const std::string& column) const{
//    auto it = _SourceColumns.find(column);
//    if(it == _SourceColumns.end()){
//        return NULL;
//    } else {
//        return &(it->second);
//    }
//}

void ParseView(const std::string& strViewDef, ViewSourceInfo& viewSourceInfo,void * pconn)
{
    ParseMgr mgr;
    S4HException exc;
    IViewStmt * view = parser_view(strViewDef, &mgr, exc);
    if(view){
        viewSourceInfo.AddedSources(view->GetViewSources(pconn, exc));
    }
#ifdef TEST_JEFF
    printf("%s\n", strViewDef.c_str());
    printf("%s\n", viewSourceInfo.Print().c_str());
#endif
}

ViewInfo* ViewManager::GetViewInfoFromDDL27S(const std::string& viewName, void * pconn)
{
	//find 
     MAP_VIEWINFO::iterator itViewInfo = _allViewInfo.find(viewName);
     if (itViewInfo != _allViewInfo.end() ){
		 ViewInfo* pViewInfo = itViewInfo->second;
		 bool bViewSourceExpired = false;
		 if (pViewInfo->SourceInfo()==NULL || (true == (bViewSourceExpired=IsViewSourceExpired(pViewInfo))) )
		 {
 
            ViewSourceInfo* pSourceInfo = NULL;
            if (bViewSourceExpired) {//when the source information is expired, just clear its content.
				pSourceInfo = pViewInfo->SourceInfo().get();
				pSourceInfo->ClearSourceInfo();
				pViewInfo->SetSourceInfoPrinted(false);
			}
			else{
				 pSourceInfo = new ViewSourceInfo();
				 pViewInfo->SetSourceInfo(pSourceInfo); 
			}
			
			if(!QueryViewSourceInfo(viewName, pconn, *pSourceInfo)){
				theLog->WriteLog(log_info, "GetViewInfoFromDDL27S failed QueryViewSourceInfo 1, viewName:%s", viewName.c_str() );
				return NULL;
			}
			pViewInfo->SetSourceUpdateTime(time(0));
			
			//recursion to check if the view if created based on another view
			const std::set<std::string> *pSourceTable = pSourceInfo->GetSourceTables();
			std::set<std::string>::const_iterator itSourceTable;
			do {
				itSourceTable = pSourceTable->begin();
				while (itSourceTable != pSourceTable->end()) {

					MAP_VIEWINFO::iterator itSubViewInfo = _allViewInfo.find(*itSourceTable);
					if (itSubViewInfo != _allViewInfo.end()) {
						//theLog->WriteLog(log_info, "GetViewInfoFromDDL27S find Recursion view, view name:%s, source view:%s", viewName.c_str(), itSourceTable->c_str());
                        bool bReplaceSource = false;
						ViewInfo *pSubViewInfo = itSubViewInfo->second;
						if (pSubViewInfo->SourceInfo() == NULL) {
							ViewSourceInfo subViewSourceInfo;
							if(QueryViewSourceInfo(*itSourceTable, pconn, subViewSourceInfo)){
								pSourceInfo->ReplaceSourceInfo(*itSourceTable, subViewSourceInfo);
								bReplaceSource = true;
							}
							else {
								theLog->WriteLog(log_info, "GetViewInfoFromDDL27S failed QueryViewSourceInfo 2, viewName:%s, subViewName:%s", viewName.c_str(), itSourceTable->c_str() );
							}							
						}
						else {
							pSourceInfo->ReplaceSourceInfo(*itSourceTable, *(pSubViewInfo->SourceInfo()));
							bReplaceSource = true;
						}

                        if(bReplaceSource){
							break; //1. didn't move to the next source table, because ReplaceSourceInfo will modify the container of SourceTable. so need begin with .begin()
						}
					}
					else {
						//theLog->WriteLog(log_info, "GetViewInfoFromDDL27S find Recursion view, view name:%s, source table:%s", viewName.c_str(), itSourceTable->c_str());
					}

					itSourceTable++;
				}
			} while (itSourceTable != pSourceTable->end());
		 }
		 return itViewInfo->second;
	 }

	 return NULL;
}

double ViewManager::GetViewClearCycle()
{
	return g_conf->GetViewClearCycle();
}

void ViewManager::CycleUpdate(void* pDBConn)
{
	//
	if (NULL==pDBConn){
		return;
	}
	SQLDBC::SQLDBC_Connection* pConn = static_cast<SQLDBC::SQLDBC_Connection*>(pDBConn);
	
	//get update cycle, get cycle time from config to make the cycle time can be changed at the running time.
    double update_cycle = GetViewClearCycle();

	//cycle update
    if(difftime(time(0), _time_last_update) > update_cycle ) {
		theLog->WriteLog(log_info, "ViewManager, the view cache is being updated");
        GetAllViewInfo(pConn);
    }
	
}

bool ViewManager::IsViewSourceExpired(const ViewInfo* pViewInfo)
{
	double update_cycle = GetViewClearCycle();
    return (difftime(time(0), pViewInfo->GetSourceUpdateTime() )>update_cycle);
} 


ViewInfo* ViewManager::GetViewInfoFromDefinition(const char* schemaName, const std::string& viewName, void * pconn, bool bRecursion/*=true*/)
 {
     //find 
     const std::string strViewKey = schemaName + viewName;
     MAP_VIEWINFO::iterator itViewInfo = _allViewInfo.find(strViewKey);
     if (itViewInfo != _allViewInfo.end() ){
		 ViewInfo* pViewInfo = itViewInfo->second;
		 if (pViewInfo->SourceInfo()==NULL){

            ViewSourceInfo* pSourceInfo = new ViewSourceInfo();
			ParseView(pViewInfo->ViewDefinition(), *pSourceInfo, pconn);
			pViewInfo->SetSourceInfo(pSourceInfo);
              
			//recursion to check if the view if created based on another view
			if (bRecursion){
				const std::set<std::string>* pSourceTable = pSourceInfo->GetSourceTables();
			  	std::set<std::string>::const_iterator itSourceTable;
			  	do{
				   itSourceTable = pSourceTable->begin();
				   while (itSourceTable != pSourceTable->end()){

					   const std::string strSubViewKey = schemaName + *itSourceTable;
     				   MAP_VIEWINFO::iterator itSubViewInfo = _allViewInfo.find(strSubViewKey);
     				   if (itSubViewInfo != _allViewInfo.end() ){
                             theLog->WriteLog(log_info, "GetViewInfo find Recursion view, view name:%s, source view:%s", viewName.c_str(), itSourceTable->c_str() );

							 ViewInfo* pSubViewInfo= itSubViewInfo->second;
							 if (pSubViewInfo->SourceInfo() == NULL) {
								 ViewSourceInfo subViewSourceInfo;
								 ParseView(pSubViewInfo->ViewDefinition(), subViewSourceInfo, pconn);
								 pSourceInfo->ReplaceSourceInfo(*itSourceTable, subViewSourceInfo);
							 }
							 else{
								 pSourceInfo->ReplaceSourceInfo(*itSourceTable, *(pSubViewInfo->SourceInfo()));
							 }

							 break; //1. didn't move to the next source table, because ReplaceSourceInfo will modify the container of SourceTable. so need begin with .begin() 
						}
						else{
						   theLog->WriteLog(log_info, "GetViewInfo find Recursion view, view name:%s, source table:%s", viewName.c_str(), itSourceTable->c_str() );
					   }

					   itSourceTable++;
				   }	  
			  	}while(itSourceTable!=pSourceTable->end());	  
			}
		 }	 
         return  itViewInfo->second;
     }
     
     return NULL;
 }


 void ViewManager::InitViewInfo(SQLDBC::SQLDBC_Connection* pConn)
 {
     if (!IsViewInfoInited()){
         GetAllViewInfo(pConn);
     }
 }


 void ViewManager::GetAllViewInfo(SQLDBC::SQLDBC_Connection* pdbConn)
 {
     auto hdb_connection = pdbConn;
		do
		{
			auto pStatement = SQLDBC_Connection_createStatement_Old(hdb_connection);
	    	if(pStatement==nullptr){
				theLog->WriteLog(log_error, "GetAllViewInfo, createStatement failed");
				break;
			}
            
            const std::string strSQL = "SELECT VIEW_NAME from sys.views  where "
            "IS_VALId='TRUE' and SCHEMA_NAME!='SYS' and SCHEMA_NAME!='_SYS_BIC' and SCHEMA_NAME!='_SYS_REPO' and "
            "SCHEMA_NAME!='_SYS_BI' and SCHEMA_NAME!='_SYS_STATISTICS' and SCHEMA_NAME!='_SYS_XS';";

			SQLDBC_Retcode rc = SQLDBC_Statement_execute_Old(pStatement, strSQL.c_str());
			if(rc!=SQLDBC_OK){
				theLog->WriteLog(log_error, "GetAllViewInfo, execute failed, rc=%d", rc);
				SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
				break;
			}
	
			auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
			if (pResultSet==nullptr){
				theLog->WriteLog(log_error, "GetAllViewInfo, get_result_set==null");
				SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
				break;
			}
            
            //get result
            const int nMaxSchemaLen = 256;//from the definition of the SYS.VIEW. schemaname,viewname max length is 256
 
            char szSchemaName[nMaxSchemaLen+1]; 
            char szViewName[nMaxSchemaLen+1];
			SQLDBC_Length nLen = 0;
            
			time_t update_time = time(0);
            do{
                rc =  SQLDBC_ResultSet_next_Old(pResultSet); 
			    if(rc!=SQLDBC_OK){
					if (rc!=SQLDBC_NO_DATA_FOUND) {
						theLog->WriteLog(log_error, "GetAllViewInfo, call next failed, rc=%d", rc);
					}    
				    break;
			    }

                //get view name
			    rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szViewName, &nLen, sizeof(szViewName), SQLDBC_TRUE );
			    if(rc!=SQLDBC_OK){
				    theLog->WriteLog(log_error, "GetAllViewInfo get szViewName failed.iCol=%d,rc=%d",1, rc);
				    continue;
			    }
				if(nLen==0){
					theLog->WriteLog(log_info, "GetAllViewInfo get szViewName empty, icol=%d", 1 );
				    continue;
				}
                szViewName[nLen] = 0;

               
                //store view info
				auto itViewInfo = _allViewInfo.find(szViewName);
				if (itViewInfo==_allViewInfo.end()){
					ViewInfo* pViewInfo = new ViewInfo("", szViewName, "" );
					pViewInfo->SetUpdateTime(update_time);
                    _allViewInfo[pViewInfo->ViewName()] = pViewInfo;

					if (_time_last_update!=0){
						theLog->WriteLog(log_info, "ViewManager::GetAllViewInfo add view:%s", szViewName);
					}
					
				}
				else {
					itViewInfo->second->SetUpdateTime(update_time);
				}
				
            }while (true);
           
            SQLDBC_Connection_releaseStatement_Old(hdb_connection, pStatement);
			pStatement = NULL;

			//remove not exit views
			auto itViewInfo = _allViewInfo.begin();
			while (itViewInfo != _allViewInfo.end()){
                ViewInfo* pViewInfo = itViewInfo->second;
				if ((NULL!=pViewInfo) && (pViewInfo->GetUpdateTime()!=update_time)){
					theLog->WriteLog(log_info, "ViewManager::GetAllViewInfo remove view:%s", itViewInfo->first.c_str() );
					delete pViewInfo;
					itViewInfo->second=NULL;
					itViewInfo = _allViewInfo.erase(itViewInfo);
					continue; //now itViewInfo point to the next one. no need ++;
				}
				itViewInfo++;
			}

			//set last update
			_time_last_update = update_time;
			
		} while (false);

          
    theLog->WriteLog(log_info, "ViewManager::GetAllViewInfo count:%d", _allViewInfo.size() );
 }

 bool ViewManager::QueryViewSourceInfo(const std::string& viewName, void * pconn, ViewSourceInfo& sourceInfo)
 {
	 bool bRet = false;
	 SQLDBC::SQLDBC_Connection* pdbConn= (SQLDBC::SQLDBC_Connection*)(pconn);

     SQLDBC::SQLDBC_PreparedStatement* pStatement=NULL; 
	 do
	 {
		 pStatement = SQLDBC_Connection_createPreparedStatement_Old(pdbConn);
		 if (pStatement == nullptr) {
			 theLog->WriteLog(log_error, "QueryViewSourceInfo, createStatement failed:%s", SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pdbConn)));
			 break;
		 }

		const static std::string strSQL = "SELECT VIEWFIELD, TABNAME, FIELDNAME from DD27S where VIEWNAME=? ORDER BY OBJPOS";
        SQLDBC_Retcode rc = prepare_3_old(pStatement, strSQL.c_str(), strSQL.length(), SQLDBC_StringEncoding::Ascii);
		if (rc!=SQLDBC_OK) {
			 theLog->WriteLog(log_error, "QueryViewSourceInfo, prepare_3_old failed:%s",SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)));
			 break;
		}

		rc = bind_parameter_old(pStatement, 1, SQLDBC_HostType::SQLDBC_HOSTTYPE_ASCII, (void*)viewName.c_str(), NULL, viewName.length(), SQLDBC_TRUE );
		if (rc!=SQLDBC_OK){
			 theLog->WriteLog(log_error, "QueryViewSourceInfo, bind_parameter_old failed:%s",SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)));
			 break;
		}

		rc = execute_old(pStatement);
		if(rc!=SQLDBC_OK){
			theLog->WriteLog(log_error, "QueryViewSourceInfo, execute_old failed, rc=%d, msg:%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStatement)) );
			break;
		}
	
		auto pResultSet = (SQLDBC::SQLDBC_ResultSet*)get_result_set_old(pStatement);
		if (pResultSet==nullptr){
			theLog->WriteLog(log_error, "QueryViewSourceInfo, get_result_set==null");
			break;
		}
            
            //get result
            const SQLDBC_Length nMaxBufLen = 32;//from the definition of the DD27S. the length is 30
 
            char szViewField[nMaxBufLen+1]; 
            char szTabName[nMaxBufLen+1];
			char szFieldName[nMaxBufLen+1];
			SQLDBC_Length nLen = 0;

			do
			{
				rc = SQLDBC_ResultSet_next_Old(pResultSet);
				if (rc != SQLDBC_OK) {
					if (rc != SQLDBC_NO_DATA_FOUND) {
						theLog->WriteLog(log_error, "QueryViewSourceInfo, call next failed, rc=%d, msg:%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pResultSet)) );
					}
					break;
				}

				//view field
			    rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 1, SQLDBC_HOSTTYPE_ASCII, szViewField, &nLen, nMaxBufLen, SQLDBC_TRUE );
			    if(rc!=SQLDBC_OK){
				    theLog->WriteLog(log_error, "QueryViewSourceInfo get szViewField failed.iCol=%d,rc=%d",1, rc);
				    continue;
			    }
				if(nLen==0){
					theLog->WriteLog(log_info, "QueryViewSourceInfo get szViewField empty. iCol=%d,rc=%d",1, rc);
				    continue;
				}
                szViewField[nLen] = 0;

				//table name
				rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 2, SQLDBC_HOSTTYPE_ASCII, szTabName, &nLen, nMaxBufLen, SQLDBC_TRUE );
			    if((rc!=SQLDBC_OK) || (nLen==0)){
				    theLog->WriteLog(log_error, "QueryViewSourceInfo get szTabName failed.iCol=%d,rc=%d",2, rc);
				    continue;
			    }
                szTabName[nLen] = 0;

				//field name
                rc = SQLDBC_ResultSet_getObject_Old(pResultSet, 3, SQLDBC_HOSTTYPE_ASCII, szFieldName, &nLen, nMaxBufLen, SQLDBC_TRUE );
			    if((rc!=SQLDBC_OK) || (nLen==0)){
				    theLog->WriteLog(log_error, "QueryViewSourceInfo get szFieldName failed.iCol=%d,rc=%d",2, rc);
				    continue;
			    }
                szFieldName[nLen] = 0;

				if (strlen(szViewField)>0 && strlen(szTabName)>0 && strlen(szFieldName)>0)
				{
					ViewColumnSourceInfo viewColumnInfo(szViewField, szTabName, szFieldName);
					sourceInfo.AddedSourceColumn(szViewField, viewColumnInfo);
					sourceInfo.AddedSourceTable(szTabName);
					bRet = true;
				}
				
			} while (true);

		 /* code */
	 } while (false);

	 if(pStatement){
		  SQLDBC_Connection_ReleaseStatement_Prepared_Old(pdbConn, pStatement);
		  pStatement = NULL;
	 }

	return bRet;
 }

void ViewSourceInfo::AddedSourceTable(const char* szTable){
    _sourceTables.insert(szTable);
}
void ViewSourceInfo::AddedSourceColumn(const char* szColumn, const ViewColumnSourceInfo& columnSrcInfo){
    _SourceColumns.insert(std::make_pair(szColumn,columnSrcInfo));
}
void ViewSourceInfo::AddedSources(const ViewSourceInfo & info){
    _sourceTables.insert(info._sourceTables.begin(),info._sourceTables.end());
    _SourceColumns.insert(info._SourceColumns.begin(),info._SourceColumns.end());
}
size_t ViewSourceInfo::GetTablesSize(){
    return _sourceTables.size();
}

const std::string ViewSourceInfo::Print(const std::string& viewName) const {
    std::string ret = "ViewSourceInfo:" + viewName + "--tables(";
    for(auto it: _sourceTables){
        ret += it;
        ret +=", ";
    }
    ret+=")--columns(";
    for(auto it:_SourceColumns){
        ret+="[";
        ret+=it.second.ColumnName();
        ret+=", ";
        ret+=it.second.SourceTableName();
        ret+=", ";
        ret+=it.second.SourceColumnName();
        ret+="],";
    }
    ret+=")";
    return ret;
}

std::string ViewInfo::Print(const char *szPrefix) const{
	std::string ret(szPrefix == NULL ? "" : szPrefix);

	ret += " view name:" + _viewName;

    ret += " source table:";
	auto sourceTables = _pSourceInfo->GetSourceTables();
	for (auto itSrcTable = sourceTables->begin(); itSrcTable != sourceTables->end(); itSrcTable++){
		ret += *itSrcTable + ",";
	}
	return ret;
}


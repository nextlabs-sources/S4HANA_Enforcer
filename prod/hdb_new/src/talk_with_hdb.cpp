#include "talk_with_hdb.h"
#include "interface.h"
#include "stdio.h"
#include "tool.h"
#include <string>
#include "log.h"
#include "EnforcerCtxImpl.h"
#include "MyDisp+Work.h"
#include "../parser_yacc/EMS4HUpdateAst.h"
#include "../parser_yacc/EMS4HSelectAst.h"
#include "../parser_yacc/EMS4HInsertAst.h"
#include "../parser_yacc/EMS4HDeleteAst.h"
#include "ViewManager.h"
#include "PolicyManager.h"
#include "NXS4HConfig.h"
#include "NxlDbDataMgr.h"
#include "KeyMaskOpr.h"
#include "hdbEnforce.h"
#include "TableAliasMgr.h"
#include "SAPMsgNotify.h"

extern "C"{
  void Delay_init();
}

void LogEnforcerCtxInfo();


extern std::string g_strDAEInstallPath;

SQLDBC_Retcode SQLDBC_Connection_Commit_New(SQLDBC::SQLDBC_Connection* self)
{
    SQLDBC_Retcode  rc = SQLDBC_Connection_Commit_Old(self);

 #if 0  // this function not hooked
    //prepare back for update statement
    auto itEnforceCtx = BINDINGS.begin();
    while (itEnforceCtx != BINDINGS.end() )
    {
        EnforcerCtxImpl* pCtx = itEnforceCtx->second;
        if (pCtx && pCtx->IsOriginalStmtReprepared() )
        {
         std::u16string u16SQl = pCtx->GetOriginalSql();

         //to do: check GetOriginalStmt is valid.
         // if failed, no resolve way, just log it
         /*SQLDBC_Retcode rcRePrepare =*/ prepare_3_old((SQLDBC::SQLDBC_PreparedStatement*)pCtx->GetOriginalStmt(),
                                                  (char*)u16SQl.c_str(), 
                                                  u16SQl.size() * 2, 
                                                  SQLDBC_StringEncodingType::UCS2Swapped );

         theLog->WriteLog(log_info, "execute new prepare back:%d", rc);
         pCtx->SetOriginalStmtReprepared(false);
        }
        
        itEnforceCtx++;
    }
#endif 

    return rc;
}

SQLDBC_Retcode bind_parameter_addr_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                        const SQLDBC_UInt2 Index, 
                                        const SQLDBC_HostType Type, 
                                        void *paramAddr, 
                                        SQLDBC_Length *LengthIndicator, 
                                        const SQLDBC_Length Size, 
                                        const SQLDBC_Bool Terminate)
{
    EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
    if(ctx){
       ctx->SetCAlledAPI(API_BIND_PARAMETER_ADDR);
    }

    return bind_parameter_addr_old (self, Index, Type, paramAddr, LengthIndicator, Size, Terminate);

}

//current not hooked
SQLDBC_Retcode SetBindingType_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                SQLDBC_size_t size)
{
    theLog->WriteLog(log_info, "SetBindingType_new self:%p, size:%d", self, size);

    return SetBindingType_old(self, size);
}

SQLDBC_Retcode prepare_3_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql /* this is char16_t */,
                           const SQLDBC_Length sqlLength,
                           const SQLDBC_StringEncoding encoding)
{
    try {
     
        std::u16string sql_str = (char16_t*)sql;
        S4HException  exc;
        SQLDBC::SQLDBC_Connection* pConn = SQLDBC_Statement_getConnection_Old(self);

        //init config and other things
        if (!g_conf->IsInited()){
            std::string strCfgFile = g_strDAEInstallPath + "/config/DAE.conf";
            g_conf->InitConfig(strCfgFile.c_str(), pConn);
            //delay init other things
            Delay_init();
        }
        
		std::string u8SQL = CommonFun::ToUTF8(sql_str);
       	theLog->WriteLog(log_trace, "prepare_3_new,stmt:%p, sql:%s", self,  u8SQL.c_str() );
                
        //parse SQL
        std::shared_ptr<EMS4HAst> pAst(create_ast(u8SQL,  exc));
        if(NULL!=pAst) {
            register_enforcer_ctx(self, pAst);
        }

        //init view information
        ViewManager* pViewMgr = ViewManager::GetInstance();
        if (!pViewMgr->IsViewInfoInited()){       
            pViewMgr->InitViewInfo(pConn);
        }

        //init table alias
        TableAliasMgr& aliasMgr = TableAliasMgr::Instance();
        if (!aliasMgr.IsInit()){
            #ifdef DAE_LOCAL_CONFIG
                std::string strAliasFile = g_strDAEInstallPath + "\\config\\table_alias.conf";
                aliasMgr.Init(strAliasFile.c_str()); 
            #else
                aliasMgr.Init(pConn, g_conf->GetTBAliasClearCycle());
            #endif 
        }
        
        //NxlData init
        #if 0
        NxlDBDataMgr* pNxlDataMgr = NxlDBDataMgr::Instance();
        if (!pNxlDataMgr->IsInit()){
            pNxlDataMgr->Init(pConn);
        }
        #endif 
        
        
    #if RECORD_SQL_PARSE_ERROR
        if(!exc._detail.empty()){
            pNxlDataMgr->RecordSQLParseError(pConn, exc._detail);
        }
    #endif 

        /* code */
    }
    catch(const std::exception& e) {
         theLog->WriteLog(log_trace, "prepare_3_new, exception:%s", e.what() );
    }
    catch(...){
        theLog->WriteLog(log_trace, "prepare_3_new, exception ..." );
    }

    return prepare_3_old(self, sql, sqlLength, encoding);
}

#if 0
std::string GetClientIDValueFromOriginalSQL(EnforcerCtxImpl *ctx, EMS4HAst *ast)
{

    std::string strClientID;
    const static std::string strClientIDName = "MANDT";
    uint nClientIDPos=0; 
    bool bGetClientID = ast->GetWhereClauseConditionRightParamId(strClientIDName, nClientIDPos );
    if (bGetClientID) {
         bool bGetByPos = ctx->GetParamValueByPosition(nClientIDPos, strClientID);
         if (!bGetByPos){
             strClientID = "100"; 
            theLog->WriteLog(log_info, "GetClientIDValueFromOriginalSQL GetParamValueByPosition, failed, pos:%d", nClientIDPos);
         } else{
             theLog->WriteLog(log_info, "GetClientIDValueFromOriginalSQL  success, pos:%d, value:%s", nClientIDPos, strClientID.c_str() );
                   
         }
    }
    else{
         theLog->WriteLog(log_error, "GetClientIDValueFromOriginalSQL get client id param position failed., sql=%s", ctx->GetOriginalSqlU8().c_str() );
         strClientID = "100";
    }

    return strClientID;   
}
#endif 

SQLDBC_Retcode nxl_execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
	void *p,
	bool b,bool bItab)
{
	SQLDBC_Retcode rc;
//////////////////////////////////////////////////////////////////////////
	try {
		//
		auto connection = SQLDBC_Statement_getConnection_Old(self);

		//cycle update information, 
		PolicyManager* pPolicyMgr = PolicyManager::Instance();
		pPolicyMgr->CycleClear();
#ifdef DAE_LOCAL_CONFIG
		g_conf->CycleUpdate(connection);
		TableAliasMgr::Instance().CycleUpdate();
#else
		g_conf->CycleUpdate(connection);
		TableAliasMgr::Instance().CycleUpdate(connection);
#endif 
		ViewManager::GetInstance()->CycleUpdate(connection);

		LogEnforcerCtxInfo();
		EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
		do {
			if (ctx == nullptr) break;
			ctx->SetEnforced(false);
            bool bNotFreeNxlStmt = g_conf->GetFeatureLevel()&DAE_FEATURE_NOT_FREE_NXLSTMT;
            if(ctx->GetEnforcedStmt()!=nullptr && (!bNotFreeNxlStmt)){
                SQLDBC::SQLDBC_PreparedStatement* pEnforcedStmt = reinterpret_cast<SQLDBC::SQLDBC_PreparedStatement*>(ctx->GetEnforcedStmt());
                SQLDBC_Connection_ReleaseStatement_Prepared_Old(connection, pEnforcedStmt);
				ctx->SetEnforcedStmt(nullptr);
            }

			if (!pPolicyMgr->IsPCInited()) {
				break;
			}

			S4HException exc;
			std::shared_ptr<EMS4HAst> ast = ctx->GetAst();
			if (ast == NULL) {
				break;
			}

			if (!ast->CheckSupport()) {
				//theLog->WriteLog(log_error, "execute_itab_new return for ast->CheckSupport. sql=%s", ctx->GetOriginalSqlU8().c_str() );
				break;
			}

			//check if there is a KEY_MASK data in parameter, if yes, replace it with the original value
			HdbEnforce::ReplaceKeyMastValueWithOriginalValue(self, ctx);

			//check if need enforce
			std::vector<EMTableInfo*>& vecTables = ast->GetAllTables();
			bool bNeedEnforce = pPolicyMgr->IsNeedEnforce(ast->GetStatementType(), vecTables, connection);
			if (!bNeedEnforce) {
				break;
			}

			//get current user info
			const NxlDBCUserInfo* pUserInfo = CDbcUserInfoMgr::Instance()->GetCurrentUserInfo(connection, ctx, ast.get());
			if (NULL == pUserInfo) {
				theLog->WriteLog(log_info, "execute_itab_new get user info failed.");
				break;
			}
			theLog->WriteLog(log_info, std::bind(&NxlDBCUserInfo::Print, pUserInfo));

			//if user is within the white list, ignore it
			if (g_conf->IsWhiteListUser(pUserInfo->_strName)) {
				theLog->WriteLog(log_info, "execute_itab_new  ignore white list user:%s", pUserInfo->_strName.c_str());
				break;
			}

            //check if this sql statement is supported, put here to reduce the log
            if(ctx->CanNotSupported()){
                std::string strSql = ast->Output();
                theLog->WriteLog(log_fatal, "execute_itab_new ctx not supported, CalledAPI:%lu, sql=%s", ctx->GetCalledAPI(), strSql.c_str() );
				break;
            }

			//get current resource info
			const NxlDBCResInfo *  presinfo = SqlDbcResourceInfoMgr::Instance()->GetResourceInfo(connection, exc);
            
			std::string nativeQueryExpr, newQueryExpr; 
			bool brewrite = false;
			switch (ast->GetStatementType()) {
			case E_STMT_TYPE_UPDATE:
			{
				bool bret = ast->RewriteSQL(connection, pUserInfo, presinfo, ctx);//filter
				if (bret) {
					brewrite = true;
				}
				EMS4HUpdateAst* pUpdateAst = dynamic_cast<EMS4HUpdateAst*>(ast.get());
				bret = pUpdateAst->RewriteSQLForDenyUpdate(connection, pUserInfo, presinfo, ctx); //deny mask data + deny_update policy
				if (bret) {
					brewrite = true;
				}
				if (!brewrite) {
					theLog->WriteLog(log_info, "execute_new after E_STMT_TYPE_UPDATE RewriteSQL return = false.");
				}
			} break;
			case E_STMT_TYPE_UPSERT:
			{
				bool bret = ast->RewriteSQL(connection, pUserInfo, presinfo, ctx); //filter
				if (bret) {
					brewrite = true;
				}
				EMS4HUpsertAst* pUpsertAst = dynamic_cast<EMS4HUpsertAst*>(ast.get());
				bret = pUpsertAst->RewriteSQLForDenyUpsert(connection, pUserInfo, presinfo, ctx); ////deny mask data 
				if (bret) {
					brewrite = true;
				}
				if (!brewrite) {
					theLog->WriteLog(log_info, "execute_new after E_STMT_TYPE_UPSERT RewriteSQL return = false.");
				}

			} break;
			case E_STMT_TYPE_SELECT:{ // filter + mask
				EMS4HSelectAst* pSelectAst = dynamic_cast<EMS4HSelectAst*>(ast.get());

				nativeQueryExpr = pSelectAst->ConstructQueryExprSQL();

                brewrite = ast->RewriteSQL(connection, pUserInfo, presinfo, ctx);
                if (!brewrite) {
                    theLog->WriteLog(log_info, "execute_itab_new after RewriteSQL return = false.");
                }

				newQueryExpr = pSelectAst->ConstructQueryExprSQL();
            }
            break;
			case E_STMT_TYPE_DELETE: {   // filter
                //cache delete data
                std::shared_ptr<EMS4HDeleteAst> pDeleteAst =  std::dynamic_pointer_cast<EMS4HDeleteAst>(ast);
                if(pDeleteAst){
                    pDeleteAst->CacheDeleteData(connection, pUserInfo, presinfo, ctx);
                }
                
                //Rewrite sql
				brewrite = ast->RewriteSQL(connection, pUserInfo, presinfo, ctx);
				if (!brewrite) {
					theLog->WriteLog(log_info, "execute_itab_new after RewriteSQL return = false.");
				}
			} break;
			case E_STMT_TYPE_INSERT: {
                //restore data cache in previous DELETE
                std::shared_ptr<EMS4HInsertAst> pInsertAst =  std::dynamic_pointer_cast<EMS4HInsertAst>(ast);
                if(pInsertAst){
                    pInsertAst->RestoreFromDeleteData(ctx, connection, pUserInfo, presinfo);
                }

                //do filter
				bool b_inetrcept = ast->InterceptSQL(connection, pUserInfo, presinfo);
				if (b_inetrcept) { // deny insert
					return SQLDBC_NOT_OK;
				}
			} break;
			}
			if (!brewrite) {
				break;
			}

            std::string new_sql = ast->Output();
			theLog->WriteLog(log_info, "execute_itab_new, stmt:%p, new sql:%s", self, new_sql.c_str());

			//write notify log
            bool bDisableNotify = g_conf->GetFeatureLevel()&DAE_FEATURE_DISABLE_MSG_NOTIF;
			if ((ast->GetStatementType() == E_STMT_TYPE_SELECT) && (!bDisableNotify) ) {
				EMS4HSelectAst* pSelect = dynamic_cast<EMS4HSelectAst*>(ast.get());
				const std::vector<MaskItem>& mapMaskItem = pSelect->GetMatchedMaskItem();
                if(!mapMaskItem.empty()){
                    std::string strMaskCountSql = pSelect->ConstructMaskCountSql();
				    SAPMsgNotify::Instance().NotifyMask(connection, ctx, pUserInfo, mapMaskItem, strMaskCountSql);
                }

				//write notify log for filter
				const std::list<PredicateItem>& lstPredicItem = pPolicyMgr->GetMatchedPredicateItem();
				if (!lstPredicItem.empty()) {
					SAPMsgNotify::Instance().NotifyFilter(connection, ctx, pUserInfo, lstPredicItem, newQueryExpr, nativeQueryExpr);
				}
			}

			ctx->SetEnforced(true);  //mark this statement is enforced 
									 //get previous enforced statement. if the new sql is not the same, will release previous statement
			SQLDBC::SQLDBC_PreparedStatement*  enforced_stmt = (SQLDBC::SQLDBC_PreparedStatement*)ctx->GetEnforcedStmt();
			if ((enforced_stmt != NULL) && (CommonFun::StrCaseCmp(new_sql, ctx->GetPreviousNewSql()) != 0)) {
				SQLDBC_Connection_ReleaseStatement_Prepared_Old(connection, enforced_stmt);
				ctx->SetEnforcedStmt(nullptr);
				enforced_stmt = NULL;

				theLog->WriteLog(log_info, "execute_itab_new, release previous enforce statement");
			}

			//create new statement
			if (enforced_stmt == NULL) {
				enforced_stmt = SQLDBC_Connection_createPreparedStatement_Old(connection);
				ctx->SetEnforcedStmt(enforced_stmt);
				ctx->SetPreviousNewSql(new_sql.c_str());

				std::u16string u16sql = CommonFun::FromUTF8(new_sql);
				rc = prepare_3_old(enforced_stmt, (char*)u16sql.c_str(), u16sql.size() * 2, SQLDBC_StringEncodingType::UCS2Swapped);
				if (rc != SQLDBC_OK) {
					theLog->WriteLog(log_info, "execute_itab_new with enforcer prepare_3_old failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(enforced_stmt)));
				}
				theLog->WriteLog(log_info, "execute_itab_new new, create previous enforce statement");
			}

			S4HException e;
			ctx->EnforcedStmtBindParameters(e);
			if (bItab) {
				rc = execute_itab_old(enforced_stmt, p, b);
			}
			else {
				rc = execute_old(enforced_stmt);
			}

			if (rc != SQLDBC_OK && rc != SQLDBC_SUCCESS_WITH_INFO && rc != SQLDBC_NO_DATA_FOUND) {
				theLog->WriteLog(log_info, "execute_itab_new failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old(&SQLDBC_ConnectionItem_error_Old(enforced_stmt)));
			}

			ast->ResetSQL();
			return rc;

		} while (false);
	}
	catch (const std::exception& ex) {
		theLog->WriteLog(log_info, "execute_itab_new exception:%s", ex.what());
	}
	catch (...) {
		theLog->WriteLog(log_info, "execute_itab_new exception...");
	}

//////////////////////////////////////////////////////////////////////////
	if (bItab) {
		rc = execute_itab_old(self, p, b);
	}
	else {
		rc = execute_old(self);
	}
	return rc;
}

SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p,
                                bool b)
{
	return nxl_execute_itab_new(self, p, b, true);
}

void* get_itab_reader_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
    if (ctx != nullptr && ctx->IsEnforced() ) {
        return get_itab_reader_old((SQLDBC::SQLDBC_PreparedStatement*)ctx->GetEnforcedStmt());
    }
    return get_itab_reader_old(self);
}

SQLDBC_Retcode bind_parameter_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                  const SQLDBC_UInt4     Index,
                                  const SQLDBC_HostType  Type,
                                  void                  *paramAddr,
                                  SQLDBC_Length         *LengthIndicator,
                                  const SQLDBC_Length    Size,
                                  const SQLDBC_Bool      Terminate)
{//note!!! in the function, the paramAddr may not contains the value. the value may be set after this function and before execute function
 //so we can't analyze parameter value here.
    
    EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
    if (ctx != nullptr) {
        ctx->RecordParameter( Index, Type, paramAddr, LengthIndicator, Size, Terminate );
    }
    
    return  bind_parameter_old(self, Index, Type, paramAddr, LengthIndicator, Size, Terminate); ;
}

SQLDBC_Retcode execute_new(SQLDBC::SQLDBC_PreparedStatement *self) {
	return nxl_execute_itab_new(self, nullptr, true, false);
#if 0
    try{
        auto connection = SQLDBC_Statement_getConnection_Old(self); 

        //cycle update information, 
        PolicyManager* pPolicyMgr = PolicyManager::Instance();
        pPolicyMgr->CycleClear();
        #ifdef DAE_LOCAL_CONFIG
        g_conf->CycleUpdate(connection);
        TableAliasMgr::Instance().CycleUpdate();
        #else
        g_conf->CycleUpdate(connection);
        TableAliasMgr::Instance().CycleUpdate(connection);
        #endif 
        ViewManager::GetInstance()->CycleUpdate(connection);

        LogEnforcerCtxInfo();

        EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
        do {
			if (ctx == nullptr) break;
            ctx->SetEnforced(false);

            if (!pPolicyMgr->IsPCInited()){
                break;
            }

            S4HException exc;
            std::shared_ptr<EMS4HAst> ast = ctx->GetAst();
            if(ast==NULL){
                break;
            }

            if (!ast->CheckSupport()) {  
                //theLog->WriteLog(log_error, "execute_new return for ast->CheckSupport. sql=%s", ctx->GetOriginalSqlU8().c_str() );
                break;
            }

            //check if there is a KEY_MASK data in parameter, if yes, replace it with the original value
            HdbEnforce::ReplaceKeyMastValueWithOriginalValue(self, ctx);

            bool brewrite = false;
            do{ //-------filter and mask and denuupdate-------
            
            std::vector<EMTableInfo*>& vecTables = ast->GetAllTables();
            bool bNeedEnforce = pPolicyMgr->IsNeedEnforce(ast->GetStatementType(), vecTables, connection);
            if (!bNeedEnforce){
                break;
            }
        
            //get current user info
            const NxlDBCUserInfo* pUserInfo = CDbcUserInfoMgr::Instance()->GetCurrentUserInfo(connection, ctx, ast.get());
            if(NULL==pUserInfo){
                break;        
            }
            theLog->WriteLog(log_info, std::bind(&NxlDBCUserInfo::Print, pUserInfo) );

            //if user is within the white list, ignore it
            if (g_conf->IsWhiteListUser(pUserInfo->_strName)){
                theLog->WriteLog(log_info, "execute_new ignore white list user:%s", pUserInfo->_strName.c_str() );
                break;
            }
        

            //get current resource info
            const NxlDBCResInfo *  presinfo = SqlDbcResourceInfoMgr::Instance()->GetResourceInfo(connection, exc);
            theLog->WriteLog(log_info, std::bind(&NxlDBCResInfo::Print, presinfo) );

                switch(ast->GetStatementType()){
                    case E_STMT_TYPE_UPDATE:  
                    {
                        bool bret = ast->RewriteSQL(connection, pUserInfo, presinfo,ctx); //filter
                        if(bret) {
                            brewrite = true;
                        }
                        EMS4HUpdateAst* pupdate = dynamic_cast<EMS4HUpdateAst*>(ast.get());
                        bret = pupdate->RewriteSQLForDenyUpdate(connection, pUserInfo, presinfo, ctx); //deny mask data + deny_update policy
                        if(bret) {
                            brewrite = true;
                        }
                        if (!brewrite){
                            theLog->WriteLog(log_error, "execute_new after E_STMT_TYPE_UPDATE RewriteSQL return = false.");
                        }
                    } break;
                    case E_STMT_TYPE_UPSERT:    
                    {
                        bool bret = ast->RewriteSQL(connection, pUserInfo, presinfo,ctx); //filter
                        if(bret) {
                            brewrite = true;
                        }
                        EMS4HUpsertAst* pUpsertAst = dynamic_cast<EMS4HUpsertAst*>(ast.get());
                        bret = pUpsertAst->RewriteSQLForDenyUpsert(connection, pUserInfo, presinfo, ctx); ////deny mask data 
                        if(bret) {
                            brewrite = true;
                        }
                        if (!brewrite){
                            theLog->WriteLog(log_error, "execute_new after E_STMT_TYPE_UPSERT RewriteSQL return = false.");
                        }

                    } break;
                    case E_STMT_TYPE_SELECT:    // filter + mask
                    case E_STMT_TYPE_DELETE:{   // filter
                        //Rewrite sql
                        brewrite = ast->RewriteSQL(connection, pUserInfo, presinfo,ctx);
                        if (!brewrite){
                            theLog->WriteLog(log_error, "execute_new after RewriteSQL return = false.");
                        }
                    } break;
                    case E_STMT_TYPE_INSERT:{
                        bool b_inetrcept = ast->InterceptSQL(connection, pUserInfo, presinfo);
                        if(b_inetrcept) { // deny insert
                            return SQLDBC_NOT_OK;
                        }
                    } break;

                }
                if (!brewrite){
                    break;
                }

    #if 0
                //write notify log for mask
                if (ast->GetStatementType() == E_STMT_TYPE_SELECT) {
                    EMS4HSelectAst *pSelect = dynamic_cast<EMS4HSelectAst *>(ast.get());
                    const std::vector<MaskItem> &mapMaskItem = pSelect->GetMatchedMaskItem();
                    SAPMsgNotify::Instance().NotifyMask(connection, ctx, pUserInfo, mapMaskItem);
                }

                //write notify log for filter
                const std::list<PredicateItem>& lstPredicItem = pPolicyMgr->GetMatchedPredicateItem();
                if (!lstPredicItem.empty()) {
                    SAPMsgNotify::Instance().NotifyFilter(connection, ctx, pUserInfo, lstPredicItem);
                }
    #endif 
                

            }while(false);

            if (!brewrite){
                break;
            }
            

            
            std::string new_sql = ast->Output();
            theLog->WriteLog(log_info, "execute_new stmt:%p, new sql:%s\n", self,  new_sql.c_str() );

    #ifdef TEST_JEFF
            printf("new:%s\n", new_sql.c_str());
    #endif
            ctx->SetEnforced(true);  //mark this statement is enforced 
            //get previous enforced statement. if the new sql is not the same, will release previous statement
            SQLDBC::SQLDBC_PreparedStatement*  enforced_stmt = (SQLDBC::SQLDBC_PreparedStatement*)ctx->GetEnforcedStmt();
             if ((enforced_stmt!=NULL) && (CommonFun::StrCaseCmp(new_sql, ctx->GetPreviousNewSql())!=0)) {
                  SQLDBC_Connection_ReleaseStatement_Prepared_Old(connection, enforced_stmt );
                 ctx->SetEnforcedStmt(nullptr);
                 enforced_stmt = NULL;

                 theLog->WriteLog(log_info, "execute_new, release previous enforce statement" );
            }
        
            //create new statement
           SQLDBC_Retcode rc;
            if (enforced_stmt==NULL){
                enforced_stmt = SQLDBC_Connection_createPreparedStatement_Old(connection);
                ctx->SetEnforcedStmt(enforced_stmt);
                ctx->SetPreviousNewSql(new_sql.c_str());

                 std::u16string u16sql = CommonFun::FromUTF8(new_sql);
                rc =prepare_3_old(enforced_stmt, (char*)u16sql.c_str(), u16sql.size()*2, SQLDBC_StringEncodingType::UCS2Swapped);
                if (rc!=SQLDBC_OK){
                    theLog->WriteLog(log_info, "execute_new with enforcer prepare_3_old failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(enforced_stmt)) );
               } 

               theLog->WriteLog(log_info, "execute_new, create previous enforce statement" );   
            }
            
            S4HException e;
            ctx->EnforcedStmtBindParameters(e);
            rc = execute_old(enforced_stmt);
            if (rc!=SQLDBC_OK && rc!=SQLDBC_SUCCESS_WITH_INFO  && rc!=SQLDBC_NO_DATA_FOUND){
                theLog->WriteLog(log_info, "execute_new with enforcer failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(enforced_stmt)) );
            }

            ast->ResetSQL();

            return rc;

        } while (false);

    }
    catch(const std::exception& ex){
        theLog->WriteLog(log_info, "execute_new exception:%s", ex.what() );
    }
    catch(...){
         theLog->WriteLog(log_info, "execute_new exception..." );
    } 

    SQLDBC_Retcode rc =  execute_old(self);

    if (rc!=SQLDBC_OK && rc!=SQLDBC_SUCCESS_WITH_INFO && rc!=SQLDBC_NO_DATA_FOUND){
        theLog->WriteLog(log_info, "execute_new without enforcer failed, rc=%d, error=%s", rc, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(self)) );
    }


    return rc;
#endif 
}

SQLDBC::SQLDBC_ResultSet *get_result_set_new(SQLDBC::SQLDBC_Statement *self) {
    EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
    if (ctx != nullptr && ctx->IsEnforced() ) {
        return get_result_set_old((SQLDBC::SQLDBC_PreparedStatement*)ctx->GetEnforcedStmt());
    }
    return get_result_set_old(self);
}

SQLDBC_Int4 get_rows_affected_new(SQLDBC::SQLDBC_Statement *self){
    EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
    if (ctx != nullptr && ctx->IsEnforced() ) {
        return get_rows_affected_old((SQLDBC::SQLDBC_Statement*)ctx->GetEnforcedStmt());
    }
    return get_rows_affected_old(self);
}

void SQLDBC_Connection_ReleaseStatement_Prepared_New(SQLDBC::SQLDBC_Connection* self, SQLDBC::SQLDBC_PreparedStatement *stat)
{
    //remove enforcer context
    remove_enforcer_ctx(stat);

    //call old 
    SQLDBC_Connection_ReleaseStatement_Prepared_Old(self, stat);
}

SQLDBC_Retcode SQLDBC_PreparedStatement_setBatchSize_New(SQLDBC::SQLDBC_PreparedStatement* self, SQLDBC_UInt4 rowarraysize)
{
    EnforcerCtxImpl *ctx = get_enforcer_ctx(self);
    if(ctx){
        ctx->SetBatchSize(rowarraysize);
        //theLog->WriteLog(log_info, "setBatchSize_New self:%p, size:%u", self, rowarraysize);
    }

    return SQLDBC_PreparedStatement_setBatchSize_Old(self, rowarraysize);
}

//function used in std::unique_ptr<SQLDBC::SQLDBC_PreparedStatement> to delete statement automatically
void DeletePraparedStatement(SQLDBC::SQLDBC_PreparedStatement* pPreparedStamt) {
    auto hdb_connection = SQLDBC_Statement_getConnection_Old(pPreparedStamt);
    SQLDBC_Connection_ReleaseStatement_Prepared_Old(hdb_connection, pPreparedStamt);
}
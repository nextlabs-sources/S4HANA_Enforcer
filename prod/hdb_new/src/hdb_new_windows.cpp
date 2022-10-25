
#include <windows.h>
#include <Shlobj.h>
#include <shlwapi.h>
#include <string.h>
#include "interface.h"
#include "talk_with_hdb.h"
#include "PolicyManager.h"
#include <string.h>
#include <thread>
#include "log.h"
#include "NXS4HConfig.h"
#include "MyDisp+Work.h"
#include "TableAliasMgr.h"
#include "PreStartLogProxy.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "detours.h"


#define FUNC_EXPORT  __declspec(dllexport) __cdecl


Prepare_3_Fun       prepare_3_old       = nullptr;
ExecuteItabFun      execute_itab_old    = nullptr;
GetItabReaderFun    get_itab_reader_old = nullptr;
BindParameterFun    bind_parameter_old  = nullptr;
ExecuteFun          execute_old         = nullptr;
GetResultSetFun     get_result_set_old  = nullptr;
SQLDBC_Connection_ReleaseStatement_Prepared_Fun SQLDBC_Connection_ReleaseStatement_Prepared_Old=nullptr;

PreparedStat_SetBindingType_Fun SetBindingType_old = nullptr;
BindParameterAddrFun bind_parameter_addr_old = nullptr;
SQLDBC_Connection_Commit_Fun SQLDBC_Connection_Commit_Old = nullptr;
GetRowsAffectedFun get_rows_affected_old = nullptr;

SQLDBC_Statement_getConnection_Fun SQLDBC_Statement_getConnection_Old=NULL;
SQLDBC_Connection_createPreparedStatement_Fun SQLDBC_Connection_createPreparedStatement_Old=NULL;
SQLDBC_Connection_createStatement_Fun SQLDBC_Connection_createStatement_Old=NULL;
SQLDBC_ConnectionItem_error_Fun SQLDBC_ConnectionItem_error_Old=NULL;
SQLDBC_ErrorHndl_getErrorText_Fun SQLDBC_ErrorHndl_getErrorText_Old=NULL;
SQLDBC_ResultSet_next_Fun SQLDBC_ResultSet_next_Old = NULL;
SQLDBC_Statement_execute_Fun SQLDBC_Statement_execute_Old=NULL;
SQLDBC_ResultSet_getObject_Fun SQLDBC_ResultSet_getObject_Old=NULL;
SQLDBC_Connection_releaseStatement_Fun SQLDBC_Connection_releaseStatement_Old=NULL;
SQLDBC_ResultSet_getResultCount_Fun SQLDBC_ResultSet_getResultCount_Old=NULL;
SQLDBC_PreparedStatement_clearParameters_Fun SQLDBC_PreparedStatement_clearParameters_Old = NULL;
SQLDBC_ResultSetMetaData_getColumnCount_Fun SQLDBC_ResultSetMetaData_getColumnCount_Old=NULL;
SQLDBC_ResultSet_getResultSetMetaData_Fun SQLDBC_ResultSet_getResultSetMetaData_Old = NULL;
SQLDBC_PreparedStatement_setBatchSize_Fun SQLDBC_PreparedStatement_setBatchSize_Old = NULL;
SQLDBC_ResultSetMetaData_getColumnLabel_Fun SQLDBC_ResultSetMetaData_getColumnLabel_Old = NULL;


const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);

std::string g_strDAEInstallPath;

extern "C" {
    
void FUNC_EXPORT DAE_init(const TyFuncWriteLog pFuncWriteLog, const char* szInstallPath);

BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD  nReason, LPVOID    /*Reserved*/ )
 {
    UNREFERENCED_PARAMETER(hDllHandle);
  BOOLEAN bSuccess = TRUE;

  switch ( nReason )
   {
    case DLL_PROCESS_ATTACH:
        // DAE_init();
    break;

    case DLL_PROCESS_DETACH:
      break;
   }

   return bSuccess;
 }

 #define DETOURS_HOOK(fun_old, fun_new)  if(fun_old){ \
                                            long lResult = DetourAttach((PVOID*)&fun_old, (void*)fun_new); \
                                            if(0!=lResult){ \
                                                g_preLog->WriteLog("DetourAttach %s failed.", #fun_old); \
                                                return; \
                                             }  \
                                          }else{ \
                                                    g_preLog->WriteLog("GetProcAddr %s failed.", #fun_old); \
                                                    return; \
                                          } 


void __cdecl DAE_init(const TyFuncWriteLog pFuncWriteLog, const char* szInstallPath) {

   //init pre_start log. log information is read from config table. before we read the config table. we need a place to write DAE log
    g_preLog = new CPreStartLog();
    g_preLog->Init(pFuncWriteLog);
    
    g_preLog->WriteLog("DAE_init enter");
    g_strDAEInstallPath = szInstallPath;
    
    if(NULL == g_conf) {
        g_conf = new NXS4HConfig();
    }

    HMODULE hSQLHDBLib= LoadLibrary("libSQLDBCHDB.dll");
    if(NULL==hSQLHDBLib){
         g_preLog->WriteLog("load hdb library failed.");
         return;
    }
    g_preLog->WriteLog("load libSQLDBCHDB.dll success.");
  
    //prepare Detours
    DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

    //hook commit
    SQLDBC_Connection_Commit_Old= (SQLDBC_Connection_Commit_Fun)GetProcAddress(hSQLHDBLib, "?commit@SQLDBC_Connection@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");
     if(NULL==SQLDBC_Connection_Commit_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_Connection_Commit_Old");
         return;
    }
    //DETOURS_HOOK(SQLDBC_Connection_Commit_Old, SQLDBC_Connection_Commit_New);
   
    //hook bind addr   
    bind_parameter_addr_old = (BindParameterAddrFun)GetProcAddress(hSQLHDBLib, "?bindParameterAddr@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@IW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
     DETOURS_HOOK(bind_parameter_addr_old, bind_parameter_addr_new);
    

     #if 0
     //hook setbindingtype
     SetBindingType_old = (PreparedStat_SetBindingType_Fun)GetProcAddress(hSQLHDBLib, "?setBindingType@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@_K@Z");
     DETOURS_HOOK(SetBindingType_old, SetBindingType_new);
     #endif 
  
     //hook setBatchSize
    SQLDBC_PreparedStatement_setBatchSize_Old = (SQLDBC_PreparedStatement_setBatchSize_Fun)GetProcAddress(hSQLHDBLib,"?setBatchSize@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@I@Z");
    DETOURS_HOOK(SQLDBC_PreparedStatement_setBatchSize_Old, SQLDBC_PreparedStatement_setBatchSize_New );

    //hook prepare_3
    prepare_3_old = (Prepare_3_Fun)GetProcAddress(hSQLHDBLib, "?prepare@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD_JW4Encoding@SQLDBC_StringEncodingType@@@Z");
    DETOURS_HOOK(prepare_3_old, prepare_3_new);


    execute_itab_old = (ExecuteItabFun)GetProcAddress(hSQLHDBLib, "?executeItab@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEAX_N@Z");
    DETOURS_HOOK(execute_itab_old, execute_itab_new);
   
    get_itab_reader_old = (GetItabReaderFun)GetProcAddress(hSQLHDBLib, "?getItabReader@SQLDBC_PreparedStatement@SQLDBC@@QEAAPEAVSQLDBC_ItabReader@2@XZ");
    DETOURS_HOOK(get_itab_reader_old,(void*)get_itab_reader_new);

    bind_parameter_old = (BindParameterFun)GetProcAddress(hSQLHDBLib, "?bindParameter@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@IW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
    DETOURS_HOOK(bind_parameter_old, (void*)bind_parameter_new);
   

    execute_old = (ExecuteFun)GetProcAddress(hSQLHDBLib, "?execute@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");
    DETOURS_HOOK(execute_old, execute_new);
  
    get_result_set_old = (GetResultSetFun)GetProcAddress(hSQLHDBLib,"?getResultSet@SQLDBC_Statement@SQLDBC@@QEAAPEAVSQLDBC_ResultSet@2@XZ");
    DETOURS_HOOK(get_result_set_old, get_result_set_new);
  

    get_rows_affected_old = (GetRowsAffectedFun) GetProcAddress(hSQLHDBLib, "?getRowsAffected@SQLDBC_Statement@SQLDBC@@QEBA_JXZ");
    DETOURS_HOOK(get_rows_affected_old, get_rows_affected_new);
   
    SQLDBC_Connection_ReleaseStatement_Prepared_Old =   (SQLDBC_Connection_ReleaseStatement_Prepared_Fun)GetProcAddress(hSQLHDBLib,"?releaseStatement@SQLDBC_Connection@SQLDBC@@QEAAXPEAVSQLDBC_PreparedStatement@2@@Z");
    DETOURS_HOOK(SQLDBC_Connection_ReleaseStatement_Prepared_Old, SQLDBC_Connection_ReleaseStatement_Prepared_New );   
    
    //commit hook
    long lResult = DetourTransactionCommit();
	 g_preLog->WriteLog("DAE_init DetourTransactionCommit result:%ld, %s", lResult,  lResult==0 ? "Success" : "Failed");
    if(0!=lResult){
        return;
    }

    //get other fun address
    SQLDBC_Statement_getConnection_Old=(SQLDBC_Statement_getConnection_Fun)GetProcAddress(hSQLHDBLib, "?getConnection@SQLDBC_Statement@SQLDBC@@QEAAPEAVSQLDBC_Connection@2@XZ");
    if(NULL==SQLDBC_Statement_getConnection_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_Statement_getConnection_Old");
         return;
    }

    SQLDBC_Connection_createPreparedStatement_Old=(SQLDBC_Connection_createPreparedStatement_Fun)GetProcAddress(hSQLHDBLib, "?createPreparedStatement@SQLDBC_Connection@SQLDBC@@QEAAPEAVSQLDBC_PreparedStatement@2@XZ");
    if(NULL==SQLDBC_Connection_createPreparedStatement_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_Connection_createPreparedStatement_Old");
         return;
    }

    SQLDBC_Connection_createStatement_Old=(SQLDBC_Connection_createStatement_Fun)GetProcAddress(hSQLHDBLib, "?createStatement@SQLDBC_Connection@SQLDBC@@QEAAPEAVSQLDBC_Statement@2@XZ");
    if(NULL==SQLDBC_Connection_createStatement_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_Connection_createStatement_Old");
         return;
    }

    SQLDBC_ConnectionItem_error_Old= (SQLDBC_ConnectionItem_error_Fun)GetProcAddress(hSQLHDBLib, "?error@SQLDBC_ConnectionItem@SQLDBC@@QEAAAEAVSQLDBC_ErrorHndl@2@XZ") ;
    if(NULL==SQLDBC_ConnectionItem_error_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ConnectionItem_error_Old");
         return;
    }

    SQLDBC_ErrorHndl_getErrorText_Old = (SQLDBC_ErrorHndl_getErrorText_Fun)GetProcAddress(hSQLHDBLib, "?getErrorText@SQLDBC_ErrorHndl@SQLDBC@@QEBAPEBDXZ");
     if(NULL==SQLDBC_ErrorHndl_getErrorText_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ErrorHndl_getErrorText_Old");
         return;
    }

    SQLDBC_ResultSet_next_Old=(SQLDBC_ResultSet_next_Fun)GetProcAddress(hSQLHDBLib, "?next@SQLDBC_ResultSet@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ") ;
      if(NULL==SQLDBC_ResultSet_next_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSet_next_Old");
         return;
    }
  
    SQLDBC_Statement_execute_Old=(SQLDBC_Statement_execute_Fun)GetProcAddress(hSQLHDBLib, "?execute@SQLDBC_Statement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD@Z");
    if(NULL==SQLDBC_Statement_execute_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_Statement_execute_Old");
         return;
    }

    SQLDBC_ResultSet_getObject_Old =(SQLDBC_ResultSet_getObject_Fun)GetProcAddress(hSQLHDBLib, "?getObject@SQLDBC_ResultSet@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@HW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
    if(NULL==SQLDBC_ResultSet_getObject_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSet_getObject_Old");
         return;
    }

    SQLDBC_Connection_releaseStatement_Old =(SQLDBC_Connection_releaseStatement_Fun)GetProcAddress(hSQLHDBLib, "?releaseStatement@SQLDBC_Connection@SQLDBC@@QEAAXPEAVSQLDBC_Statement@2@@Z") ;
      if(NULL==SQLDBC_Connection_releaseStatement_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_Connection_releaseStatement_Old");
         return;
    }

    SQLDBC_ResultSet_getResultCount_Old =(SQLDBC_ResultSet_getResultCount_Fun)GetProcAddress(hSQLHDBLib, "?getResultCount@SQLDBC_ResultSet@SQLDBC@@QEBA_JXZ") ;
    if(NULL==SQLDBC_ResultSet_getResultCount_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSet_getResultCount_Old");
         return;
    }

    SQLDBC_PreparedStatement_clearParameters_Old =(SQLDBC_PreparedStatement_clearParameters_Fun)GetProcAddress(hSQLHDBLib, "?clearParameters@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");
    if(NULL==SQLDBC_PreparedStatement_clearParameters_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_PreparedStatement_clearParameters_Old");
         return;
    }

    SQLDBC_ResultSetMetaData_getColumnCount_Old =(SQLDBC_ResultSetMetaData_getColumnCount_Fun)GetProcAddress(hSQLHDBLib, "?getColumnCount@SQLDBC_ResultSetMetaData@SQLDBC@@QEAAHXZ");
    if(NULL==SQLDBC_ResultSetMetaData_getColumnCount_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSetMetaData_getColumnCount_Old");
         return;
    }
    
    SQLDBC_ResultSet_getResultSetMetaData_Old = (SQLDBC_ResultSet_getResultSetMetaData_Fun) GetProcAddress(hSQLHDBLib, "?getResultSetMetaData@SQLDBC_ResultSet@SQLDBC@@QEAAPEAVSQLDBC_ResultSetMetaData@2@XZ");
     if(NULL==SQLDBC_ResultSet_getResultSetMetaData_Old){
         g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSet_getResultSetMetaData_Old");
         return;
    }

     SQLDBC_ResultSetMetaData_getColumnLabel_Old=(SQLDBC_ResultSetMetaData_getColumnLabel_Fun) GetProcAddress(hSQLHDBLib, "?getColumnLabel@SQLDBC_ResultSetMetaData@SQLDBC@@QEBA?AW4SQLDBC_Retcode@@HPEADW4Encoding@SQLDBC_StringEncodingType@@_JPEA_J@Z");
     if (NULL==SQLDBC_ResultSetMetaData_getColumnLabel_Old){
          g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSetMetaData_getColumnLabel_Old");
          return;
     }

    //init Disp+Work
    g_theMyDW = MyDisp_Work::Instance();
    g_theMyDW->Init();

    /* todo log */
}

void Delay_init()
{
    //https://www.boost.org/doc/libs/1_69_0/libs/log/doc/html/log/detailed/sink_backends.html#log.detailed.sink_backends.text_file.managing_rotated_files
    uintmax_t maxHistoryFiles = strtoull(g_conf->GetLogMaxfiles().c_str(), NULL, 0);
    if (maxHistoryFiles==0){
         maxHistoryFiles = LOG_MAX_FILE_NUM;
         g_preLog->WriteLog("log configure failed,maxHistoryFiles set default:%d", LOG_MAX_FILE_NUM);
    }
    
    uintmax_t rotationSize = strtoull(g_conf->GetLogFilesize().c_str(), NULL, 0);
    if (rotationSize==0)  {
         rotationSize = LOG_ROTATION_SIZE;
         g_preLog->WriteLog("log configure failed,rotationSize set default:%d", LOG_ROTATION_SIZE);
    }
    rotationSize *= 1024 * 1024; //the unit in configure file is M

    uintmax_t minFreeSpace = strtoull(g_conf->GetLogMinfreespace().c_str(), NULL, 0);
    if (minFreeSpace==0)  {
         minFreeSpace = LOG_MIN_FREE_SPACE;
         g_preLog->WriteLog("log configure failed,minFreeSpace set default:%d", LOG_MIN_FREE_SPACE);
    }
    minFreeSpace *=1024*1024*1024; //the unit in configure file is G

    //expand path
    char szExpandLogLocation[MAX_PATH]={0};
    ExpandEnvironmentStringsA(g_conf->GetLogLocation().c_str(), szExpandLogLocation, sizeof(szExpandLogLocation) );
    g_preLog->WriteLog("log location:%s", szExpandLogLocation);

    
    if (strlen(szExpandLogLocation)) {
        try {
            boost::filesystem::path logdir(szExpandLogLocation);
            logdir /=  "hdbnew";
            logdir +=  std::to_string( GetCurrentProcessId() );
            sink_ptr logFSSink = theLog->InitLog(logdir.string(), rotationSize, maxHistoryFiles, minFreeSpace);
        } catch (const boost::filesystem::filesystem_error &err)  {
             g_preLog->WriteLog("Delay_init init log failed. %s", err.what() );
        }  catch (...)  {
             g_preLog->WriteLog("Delay_init init log failed. ..." );
        }
    }
  
    //update log level
    theLog->UpdateLogLevel(g_conf->GetLogLevel() );

    theLog->WriteLog(log_info, std::bind(&NXS4HConfig::Print, g_conf)  );

    //init policy
    bool bInitPC=PolicyManager::Instance()->init_query_pc(g_conf->GetPCClearCycle());
    theLog->WriteLog(log_info, "init_query_pc result:%s", bInitPC ? "success" : "failed" );
}

}

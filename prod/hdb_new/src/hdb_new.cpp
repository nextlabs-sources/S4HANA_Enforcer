#include <dlfcn.h>
#include <unistd.h>
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
#ifdef _AIX
 #include <sys/procfs.h>
#endif
#include <sys/stat.h>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#define FUNC_EXPORT  __attribute__((visibility("default")))

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

#define SQLDBC_SQLDBC_PreparedStatement_prepare_2   "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcN25SQLDBC_StringEncodingType8EncodingE"
#define SQLDBC_SQLDBC_PreparedStatement_prepare_3   "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcxN25SQLDBC_StringEncodingType8EncodingE"
#define SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool                                   "_ZN6SQLDBC24SQLDBC_PreparedStatement11executeItabEPvb"
#define SQLDBC_SQLDBC_PreparedStatement_getItabReader                                           "_ZN6SQLDBC24SQLDBC_PreparedStatement13getItabReaderEv"
#define SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool "_ZN6SQLDBC24SQLDBC_PreparedStatement13bindParameterEj15SQLDBC_HostTypePvPxxb"
#define SQLDBC_SQLDBC_PreparedStatement_execute                                                 "_ZN6SQLDBC24SQLDBC_PreparedStatement7executeEv"
#define SQLDBC_SQLDBC_Statement_getResultSet                                                    "_ZN6SQLDBC16SQLDBC_Statement12getResultSetEv"
#define SQLDBC_SQLDBC_Statement_getRowsAffected                                                 "_ZNK6SQLDBC16SQLDBC_Statement15getRowsAffectedEv"
#define SQLDBC_Connection_releaseStatement_Prepared                                             "_ZN6SQLDBC17SQLDBC_Connection16releaseStatementEPNS_24SQLDBC_PreparedStatementE"
#define SQLDBC_SQLDBC_PreparedStatement_setBindingType    "_ZN6SQLDBC24SQLDBC_PreparedStatement14setBindingTypeEm"
#define SQLDBC_SQLDBC_PreparedStatement_bindParameterAddr "_ZN6SQLDBC24SQLDBC_PreparedStatement17bindParameterAddrEj15SQLDBC_HostTypePvPxxb"
#define SQLDBC_Connection_Commit "_ZN6SQLDBC17SQLDBC_Connection6commitEv"

const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);

std::string g_strDAEInstallPath;

extern "C" {

std::string GetProcNameByPID(int npid)
{
    std::string strProgName;

    #ifdef _AIX
    FILE *pPSInfoFile = NULL;
    do  {
        struct psinfo info;
        char filename[64];
        sprintf(filename, "/proc/%d/psinfo", npid);
        if ((pPSInfoFile = fopen(filename, "r")) == NULL) {
            g_preLog->WriteLog( "GetProcNameByPID failed to open file:%s", filename);
            break;
        }
        if (fread(&info, sizeof(info), 1, pPSInfoFile) <= 0) {
            g_preLog->WriteLog( "GetProcNameByPID failed to fread. error=%d", errno);
            break;
        }

        strProgName = info.pr_fname;
    } while (false);

    if (NULL != pPSInfoFile) {
        fclose(pPSInfoFile);
        pPSInfoFile = NULL;
    }
     
    #else
    do
    {
        const char *szlink = "/proc/self/exe";
        char linkname[1024];
        ssize_t r = readlink(szlink, linkname, sizeof(linkname) - 1);
        if (r < 0) {
            break;
        }

        linkname[r] = '\0';
        
        char* pName = strrchr(linkname, '/');
        if (pName) {
            strProgName = pName+1;
        }
        else{
            strProgName = linkname;
        }   
    } while (false);

#endif

      g_preLog->WriteLog( "GetProcNameByPID program name=%s", strProgName.c_str());

    return strProgName;
}


bool IsLoadBySAP()
{
    int nPID = ::getpid();
    std::string strProcName = GetProcNameByPID(nPID);

    if (CommonFun::StrCaseCmp(strProcName, "disp+work")==0){
        return true;
    }
    return false;
}


FUNC_EXPORT void DAE_init(const TyFuncWriteLog pFuncWriteLog, const char* szInstallPath)
{
   //init pre_start log. log information is read from config table. before we read the config table. we need a place to write DAE log
    g_preLog = new CPreStartLog();
    g_preLog->Init(pFuncWriteLog);
    
    g_preLog->WriteLog("DAE_init enter");
    g_strDAEInstallPath = szInstallPath;

    if(NULL == g_conf) {
        g_conf = new NXS4HConfig();
    }

    // IHook *hook_by_sub = create_hook(IHook::HOOK_BY_SUBHOOK);
    IHook *hook_by_func = create_hook(IHook::HOOK_BY_FUNCHOOK);

    void* pDBLib = dlopen("libSQLDBCHDB.so", RTLD_NOW);
    if (pDBLib==NULL){
        g_preLog->WriteLog("load libSQLDBCHDB.so failed.");
        return;
    }
    
    //hook commit
    SQLDBC_Connection_Commit_Old =(SQLDBC_Connection_Commit_Fun) dlsym(pDBLib, SQLDBC_Connection_Commit);
    int nHook=hook_by_func->Prepare((void**)&SQLDBC_Connection_Commit_Old,  (void*)SQLDBC_Connection_Commit_New);
    if(nHook!=0){
         g_preLog->WriteLog( "hook commit failed");
         return;
    }

    //hook bind addr
    #if 1
    bind_parameter_addr_old = (BindParameterAddrFun)dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_bindParameterAddr);
    nHook = hook_by_func->Prepare((void**)&bind_parameter_addr_old, (void*)bind_parameter_addr_new);
    if(nHook!=0){
        g_preLog->WriteLog( "hook bind_parameter_addr_old failed");
        return;
    }

    //hook setbindingtype
    SetBindingType_old = (PreparedStat_SetBindingType_Fun)dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_setBindingType);
    nHook = hook_by_func->Prepare((void**)&SetBindingType_old, (void*)SetBindingType_new);
    if(nHook!=0){
         g_preLog->WriteLog("hook SetBindingType failed");
         return;
    }

    //hook setbatchsize
    SQLDBC_PreparedStatement_setBatchSize_Old = (SQLDBC_PreparedStatement_setBatchSize_Fun)dlsym(pDBLib, "_ZN6SQLDBC24SQLDBC_PreparedStatement12setBatchSizeEj");
    nHook=hook_by_func->Prepare((void**)&SQLDBC_PreparedStatement_setBatchSize_Old,  (void*)SQLDBC_PreparedStatement_setBatchSize_New);
    if(nHook!=0){
         g_preLog->WriteLog("hook setBatchSize failed");
         return;
    }

    #endif 

    //hook prepare_3
    prepare_3_old = (Prepare_3_Fun)dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_prepare_3);
    nHook = hook_by_func->Prepare((void**)&prepare_3_old, (void*)prepare_3_new);
    if (nHook!=0) {
        g_preLog->WriteLog("hook prepare_3 failed");
        return;
    }

    execute_itab_old =(ExecuteItabFun) dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool);
    nHook = hook_by_func->Prepare((void**)&execute_itab_old, (void*)execute_itab_new);
    if (nHook!=0) {
        g_preLog->WriteLog( "hook executeItab failed");
        return;
    }

    get_itab_reader_old = (GetItabReaderFun)dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_getItabReader);
    nHook = hook_by_func->Prepare((void**)&get_itab_reader_old,  (void*)get_itab_reader_new);
    if (get_itab_reader_old == nullptr) {
        g_preLog->WriteLog("hook getItabReader failed");
        return;
    }

    bind_parameter_old = (BindParameterFun)dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool);
    nHook = hook_by_func->Prepare((void**)&bind_parameter_old, (void*)bind_parameter_new);
    if (nHook!=0) {
        g_preLog->WriteLog("hook bindParameter failed");
        return;
    }

    execute_old = (ExecuteFun)dlsym(pDBLib, SQLDBC_SQLDBC_PreparedStatement_execute);
    nHook = hook_by_func->Prepare((void**)&execute_old,   (void*)execute_new);
    if (nHook!=0) {
        g_preLog->WriteLog("hook Execute failed");
        return;
    }

    get_result_set_old =(GetResultSetFun) dlsym(pDBLib, SQLDBC_SQLDBC_Statement_getResultSet);
    nHook = hook_by_func->Prepare((void**)&get_result_set_old, (void*)get_result_set_new);
    if (nHook!=0) {
         g_preLog->WriteLog("hook getResultSet failed");
         return;
    }

    get_rows_affected_old =(GetRowsAffectedFun) dlsym(pDBLib, SQLDBC_SQLDBC_Statement_getRowsAffected);
     nHook = hook_by_func->Prepare((void**)&get_rows_affected_old, (void*)get_rows_affected_new);
    if (nHook!=0) {
         g_preLog->WriteLog("hook GetRowsAffected failed");
         return;
    }

    //hook void SQLDBC::SQLDBC_Connection::releaseStatement	( 	SQLDBC_PreparedStatement * 	stmt)
    SQLDBC_Connection_ReleaseStatement_Prepared_Old = (SQLDBC_Connection_ReleaseStatement_Prepared_Fun)dlsym(pDBLib, SQLDBC_Connection_releaseStatement_Prepared);
    nHook = hook_by_func->Prepare((void**)&SQLDBC_Connection_ReleaseStatement_Prepared_Old,
                                                     (void*)SQLDBC_Connection_ReleaseStatement_Prepared_New );   
    if(nHook!=0){
         g_preLog->WriteLog("hook SQLDBC_Connection_ReleaseStatement_Prepared_Old failed");
         return;
    }	

    //hoook ItabReader::read_2
#if 0
    SQLDBC_ItabReader_read_2_Old = (SQLDBC_ItabReader_read_2_Fun)dlsym(pDBLib, "_ZN6SQLDBC17SQLDBC_ItabReader4readER20SQLDBC_ShmDescriptorj");
    nHook = hook_by_func->Prepare((void**)&SQLDBC_ItabReader_read_2_Old,
                                                     (void*)SQLDBC_ItabReader_read_2_New );   
    if(nHook!=0){
         g_preLog->WriteLog("hook SQLDBC_ItabReader_read_2_Old failed");
         return;
    }
#endif 

    //install hook
    nHook= hook_by_func->Install();
    if (nHook!=0){
        g_preLog->WriteLog("hdb_new call hook_install failed.");
        return;
    }

    //get function address
    SQLDBC_Statement_getConnection_Old = (SQLDBC_Statement_getConnection_Fun)dlsym(pDBLib, "_ZN6SQLDBC16SQLDBC_Statement13getConnectionEv");
    if(NULL==SQLDBC_Statement_getConnection_Old){
         g_preLog->WriteLog("get function SQLDBC_Statement_getConnection_Old failed.");
        return;
    }

    SQLDBC_Connection_createPreparedStatement_Old=(SQLDBC_Connection_createPreparedStatement_Fun)dlsym(pDBLib, "_ZN6SQLDBC17SQLDBC_Connection23createPreparedStatementEv");
    if (NULL==SQLDBC_Connection_createPreparedStatement_Old) {
        g_preLog->WriteLog("get function SQLDBC_Connection_createPreparedStatement_Old failed.");
        return;
    }

    SQLDBC_Connection_createStatement_Old=(SQLDBC_Connection_createStatement_Fun)dlsym(pDBLib, "_ZN6SQLDBC17SQLDBC_Connection15createStatementEv");
    if (NULL==SQLDBC_Connection_createStatement_Old) {
        g_preLog->WriteLog("get function SQLDBC_Connection_createPreparedStatement_Old failed.");
        return;
    }

    SQLDBC_ConnectionItem_error_Old=(SQLDBC_ConnectionItem_error_Fun)dlsym(pDBLib, "_ZN6SQLDBC21SQLDBC_ConnectionItem5errorEv");
    if (NULL==SQLDBC_ConnectionItem_error_Old)  {
        g_preLog->WriteLog("get function SQLDBC_ConnectionItem_error_Old failed.");
        return;
    }

    SQLDBC_ErrorHndl_getErrorText_Old=(SQLDBC_ErrorHndl_getErrorText_Fun)dlsym(pDBLib, "_ZNK6SQLDBC16SQLDBC_ErrorHndl12getErrorTextEv");
    if (NULL==SQLDBC_ErrorHndl_getErrorText_Old){
        g_preLog->WriteLog("get function SQLDBC_ErrorHndl_getErrorText_Old failed.");
        return;
    }

    SQLDBC_ResultSet_next_Old=(SQLDBC_ResultSet_next_Fun)dlsym(pDBLib, "_ZN6SQLDBC16SQLDBC_ResultSet4nextEv");
    if (NULL==SQLDBC_ResultSet_next_Old)  {
        g_preLog->WriteLog("get function SQLDBC_ResultSet_next_Old failed.");
        return;
    }

    SQLDBC_Statement_execute_Old=(SQLDBC_Statement_execute_Fun)dlsym(pDBLib, "_ZN6SQLDBC16SQLDBC_Statement7executeEPKc");
    if (NULL==SQLDBC_Statement_execute_Old){
        g_preLog->WriteLog("get function SQLDBC_Statement_execute_Old failed.");
        return;
    }

    SQLDBC_ResultSet_getObject_Old=(SQLDBC_ResultSet_getObject_Fun)dlsym(pDBLib, "_ZN6SQLDBC16SQLDBC_ResultSet9getObjectEi15SQLDBC_HostTypePvPxxb");
    if (NULL==SQLDBC_ResultSet_getObject_Old) {
        g_preLog->WriteLog("get function SQLDBC_ResultSet_getObject_Old failed.");
        return;
    }


    SQLDBC_Connection_releaseStatement_Old=(SQLDBC_Connection_releaseStatement_Fun)dlsym(pDBLib, "_ZN6SQLDBC17SQLDBC_Connection16releaseStatementEPNS_16SQLDBC_StatementE");
    if (NULL==SQLDBC_Connection_releaseStatement_Old)  {
         g_preLog->WriteLog("get function SQLDBC_Connection_releaseStatement_Old failed.");
        return;
    }

    SQLDBC_ResultSet_getResultCount_Old=(SQLDBC_ResultSet_getResultCount_Fun)dlsym(pDBLib, "_ZNK6SQLDBC16SQLDBC_ResultSet14getResultCountEv");
    if (NULL==SQLDBC_ResultSet_getResultCount_Old)  {
        g_preLog->WriteLog("get function SQLDBC_ResultSet_getResultCount_Old failed.");
        return;
    }
    
    SQLDBC_PreparedStatement_clearParameters_Old=(SQLDBC_PreparedStatement_clearParameters_Fun)dlsym(pDBLib, "_ZN6SQLDBC24SQLDBC_PreparedStatement15clearParametersEv");
    if (NULL==SQLDBC_PreparedStatement_clearParameters_Old){
        g_preLog->WriteLog("get function SQLDBC_PreparedStatement_clearParameters_Old failed.");
        return;
    }

    SQLDBC_ResultSetMetaData_getColumnCount_Old=(SQLDBC_ResultSetMetaData_getColumnCount_Fun)dlsym(pDBLib, "_ZN6SQLDBC24SQLDBC_ResultSetMetaData14getColumnCountEv");
    if (NULL==SQLDBC_ResultSetMetaData_getColumnCount_Old){
        g_preLog->WriteLog("get function SQLDBC_ResultSetMetaData_getColumnCount_Old failed.");
        return;
    }
	
	SQLDBC_ResultSetMetaData_getColumnLabel_Old=(SQLDBC_ResultSetMetaData_getColumnLabel_Fun)dlsym(pDBLib, "_ZNK6SQLDBC24SQLDBC_ResultSetMetaData14getColumnLabelEiPcN25SQLDBC_StringEncodingType8EncodingExPx");
	if (NULL==SQLDBC_ResultSetMetaData_getColumnLabel_Old){
		g_preLog->WriteLog("DAE_init get fun:%s addr failed", "SQLDBC_ResultSetMetaData_getColumnLabel_Old");
		return;
	}

    SQLDBC_ResultSet_getResultSetMetaData_Old=(SQLDBC_ResultSet_getResultSetMetaData_Fun)dlsym(pDBLib, "_ZN6SQLDBC16SQLDBC_ResultSet20getResultSetMetaDataEv");
    if(NULL==SQLDBC_ResultSet_getResultSetMetaData_Old){
        g_preLog->WriteLog("get function SQLDBC_ResultSet_getResultSetMetaData_Old failed.");
        return;
    }
    
    //init Disp+Work
    g_theMyDW = MyDisp_Work::Instance();
    g_theMyDW->Init();
}

void Delay_init()
{
    //https://www.boost.org/doc/libs/1_69_0/libs/log/doc/html/log/detailed/sink_backends.html#log.detailed.sink_backends.text_file.managing_rotated_files
    uintmax_t rotationSize, maxHistoryFiles, minFreeSpace;
    std::string log_max_files = g_conf->GetLogMaxfiles();
    if (log_max_files.empty()) {
        maxHistoryFiles = LOG_MAX_FILE_NUM;
    } else {
        maxHistoryFiles = strtoul(log_max_files.c_str(), NULL, 0);
    }

    std::string log_file_size = g_conf->GetLogFilesize();
    if (log_file_size.empty()) {
        rotationSize = LOG_ROTATION_SIZE;
    } else {
        rotationSize = strtoul(log_file_size.c_str(), NULL, 0);
    }
    rotationSize *= 1024 * 1024; //the unit in configure file is M

    std::string log_min_free_space = g_conf->GetLogMinfreespace();
    if (log_min_free_space.empty()) {
        minFreeSpace = LOG_MIN_FREE_SPACE;
    } else {
        minFreeSpace = strtoul(log_min_free_space.c_str(), NULL, 0);
    }
    minFreeSpace *=1024*1024*1024; //the unit in configure file is G

    bool fallback = false;
    sink_ptr logFSSink;
    std::string log_location = g_conf->GetLogLocation();

     //check log location
    bool bLogLocationGood = true;
    if(log_location.empty()){
         g_preLog->WriteLog("log location empty.") ;
        bLogLocationGood = false;
    }
    else if (access(log_location.c_str(), F_OK)!=0){
        g_preLog->WriteLog("log location doesn't exist:%s", log_location.c_str() ) ;
        bLogLocationGood = false;
    }else if(access(log_location.c_str(), R_OK | W_OK | X_OK)!=0){
        g_preLog->WriteLog("log location permission is not correct%s, SAP Admin user must have RWX permisson", log_location.c_str() ) ;
        bLogLocationGood = false;
    }

    if (bLogLocationGood) {
        try {
            log_location += "/";
           //get thread name
            char szCommName[32]={0};
            int nName=pthread_getname_np(pthread_self(), szCommName, sizeof(szCommName));
            if(nName==0){
                log_location += szCommName;
            }else{
                 g_preLog->WriteLog("InitLog get thread name failed. rc=%d, last error=%d", nName, errno ) ;
		        int dwProcID = ::getpid();
                log_location += "hdbnew" + std::to_string(dwProcID);
            }
            logFSSink = theLog->InitLog(log_location, rotationSize, maxHistoryFiles, minFreeSpace);

             if (!logFSSink) {
                g_preLog->WriteLog("InitLog failed with, log path:%s", log_location.c_str() ) ;
            }

        } catch (const boost::filesystem::filesystem_error &err)  {
             g_preLog->WriteLog("InitLog failed with, log path:%s, error:%s", log_location.c_str(), err.what() ) ;
            fallback = true;
        }
        catch (...)  {
             g_preLog->WriteLog("InitLog failed with, log path:%s, error:...", log_location.c_str() ) ;
            fallback = true;
        }
    }

    //update log level
    theLog->UpdateLogLevel(g_conf->GetLogLevel());

    theLog->WriteLog(log_info, std::bind(&NXS4HConfig::Print, g_conf)  );

    //init policy
    bool bInitPC=PolicyManager::Instance()->init_query_pc(g_conf->GetPCClearCycle());
    theLog->WriteLog(log_info, "init_query_pc result:%s", bInitPC ? "success" : "failed" );
}

}

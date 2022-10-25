//
// Created by jeff on 2020/3/13.
//

#ifndef EMPOLICY_EXPORT_H
#define EMPOLICY_EXPORT_H

#include "StructDefine.h"

#ifndef WIN32
    #undef EMP_API
    #define EMP_API __attribute__((visibility("default")))
#else
    #ifdef HDB_POLICYMGR_IMP
        #define EMP_API __declspec(dllexport)
    #else
        #define EMP_API __declspec(dllimport)
    #endif 
#endif

#ifdef __cplusplus
extern "C" {                         /* Assume C declarations for C++ */
#endif

//Init Policy sync
bool EMP_API PolicyInit(const std::string &cchost, const std::string &ccport, const std::string &ccuser, const std::string &ccpwd, const std::string &model,
           unsigned int sync_interval_seconds, S4HException &exc, WriteLog_Fun log_fun=NULL);

bool EMP_API GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc);

bool EMP_API Exit();

void EMP_API UpdateSyncInterval(unsigned int iseconds);

#ifdef __cplusplus
}
#endif






#endif //EMPOLICYMGR_INTERFACE_H

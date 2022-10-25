
#ifndef DAE_MAIN_UTILI_H
#define DAE_MAIN_UTILI_H
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef _WIN32
    #define __cdecl
#endif 

extern const char* g_kpchFuncName_DAE_init;
typedef size_t(__cdecl *TyFuncWriteLog)(const char* kpchLogInfo, size_t stLength);
typedef size_t(__cdecl *TyFuncDAE_init)(const TyFuncWriteLog pFuncWriteLog, const char* InstallPath);



#endif 
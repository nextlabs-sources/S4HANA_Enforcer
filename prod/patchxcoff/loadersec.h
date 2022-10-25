#ifndef LOADER_SEC_H
#define LOADER_SEC_H
#include "patchxcoff.h"
int parse_loader_section(const unsigned char* pLoader, unsigned long long nLen);

int added_import_fileid(const unsigned char* pLoader,const long long nLoaderLen, const char* szNeeded,
                        unsigned char** pNewLoader, long long* nNewLoaderLen);
#endif

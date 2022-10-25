#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "loadersec.h"

int parse_loader_section(const unsigned char* pLoader, unsigned long long nLen)
{
    LDHDR* pLoadHDR = (LDHDR*)pLoader;
    const unsigned long long nFileIDOffset = be64toh(pLoadHDR->l_impoff);
    const unsigned int nFileIDLen = be32toh(pLoadHDR->l_istlen);
    const int nFileIDNum = be32toh(pLoadHDR->l_nimpid);
    printf("find loader. pLoader:%p, nLen=%d, version:%d file id offset:%d, file id length:%d, file id count:%ld\n", 
            pLoader, nLen, be32toh(pLoadHDR->l_version), nFileIDOffset, nFileIDLen, nFileIDNum);


    const char* pImpFileIDDesc = pLoader + nFileIDOffset;
    for (size_t iFileID = 0; iFileID < nFileIDNum; iFileID++) {
        
        const char* pFilePath = pImpFileIDDesc;
        const char* pBaseName = pFilePath + strlen(pFilePath)+1;
        const char* pMember = pBaseName + strlen(pBaseName)+1;

        printf("path:%s, base:%s, member:%s\n", pFilePath, pBaseName, pMember);

        pImpFileIDDesc = pMember + strlen(pMember) + 1;
    }
    
    
    return 1;
}

int added_import_fileid(const unsigned char* pLoader,const long long nLoaderLen, const char* szNeeded,
                        unsigned char** pNewLoader, long long* nNewLoaderLen)
{
    LDHDR* pLoadHDR = (LDHDR*)pLoader;
    const unsigned long long nFileIDOffset = be64toh(pLoadHDR->l_impoff);
    const unsigned int nFileIDLen = be32toh(pLoadHDR->l_istlen);
    const int nFileIDNum = be32toh(pLoadHDR->l_nimpid);
   // printf("find loader. pLoader:%p, version:%d file id offset:%d, file id length:%d, file id count:%ld\n", 
     //       pLoader,  be32toh(pLoadHDR->l_version), nFileIDOffset, nFileIDLen, nFileIDNum);

    //get path and name from szNeeded
    char* pNeededLocal = malloc(strlen(szNeeded)+1);
    memcpy(pNeededLocal, szNeeded, strlen(szNeeded)+1);
  
   const char* pNeededFilePath = "";
   const char* pNeededFileName = szNeeded;
   char* pLastSlash= strrchr(pNeededLocal, '/');  
    if (pLastSlash){
        *pLastSlash = 0;
        pNeededFilePath = pNeededLocal;
        pNeededFileName = pLastSlash + 1;
    }
    printf("begin added need, path:%s, name:%s\n", pNeededFilePath, pNeededFileName);
    
    
    //construct new import file id strings 
    char* pNewImportFileIDBuf = 0;
    unsigned int nNewFileIDLen = 0;
    {
        char szNewMember[] = "";
        pNewImportFileIDBuf = (char*)malloc(nFileIDLen + strlen(pNeededFilePath) + 1 + strlen(pNeededFileName) + 1 + sizeof(szNewMember) );

        //copy old data
        nNewFileIDLen = nFileIDLen;
        memcpy(pNewImportFileIDBuf, pLoader+nFileIDOffset, nFileIDLen);

        //copy new data
        memcpy(pNewImportFileIDBuf+nNewFileIDLen, pNeededFilePath, strlen(pNeededFilePath)+1 );
        nNewFileIDLen += strlen(pNeededFilePath)+1 ;

        memcpy(pNewImportFileIDBuf+nNewFileIDLen, pNeededFileName, strlen(pNeededFileName)+1);
        nNewFileIDLen += strlen(pNeededFileName)+1;

        memcpy(pNewImportFileIDBuf+nNewFileIDLen, szNewMember, sizeof(szNewMember));
        nNewFileIDLen += sizeof(szNewMember);
    }
    printf("New file id len=%d\n", nNewFileIDLen);

    //construct new load section
    *pNewLoader = (unsigned char*)malloc(nLoaderLen+nNewFileIDLen);
    memcpy(*pNewLoader, pLoader, nLoaderLen);
    memcpy(*pNewLoader+nLoaderLen, pNewImportFileIDBuf, nNewFileIDLen);

    *nNewLoaderLen = nLoaderLen+nNewFileIDLen;

    //modify loader section header
    LDHDR* pLoadHDRNew = (LDHDR*)(*pNewLoader);
    pLoadHDRNew->l_impoff = htobe64(nLoaderLen);
    pLoadHDRNew->l_istlen = htobe32(nNewFileIDLen);
    pLoadHDRNew->l_nimpid = htobe32(nFileIDNum+1);
   
    return 1;
}
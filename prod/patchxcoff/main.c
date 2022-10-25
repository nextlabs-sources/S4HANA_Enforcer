#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#ifndef _AIX
#include <endian.h>
#endif 
#include <string.h>
#include <errno.h>
#include "patchxcoff.h"
#include "loadersec.h"

void print_usage()
{
    printf("patchxcoff --add-needed libfoo.so my-program\n");
}

int write_new_loader_section(const char* szLibrary,unsigned char* pNewLoadSec, const long long nNewLoadSecLen)
{
    int fdNew = open(szLibrary, O_RDWR);
    if (fdNew == -1)  {
        printf("open file failed with read/write, error=%d\n", errno);
        return 1;
    }
  
    struct stat st;
    int r = fstat(fdNew, &st);
    if (r == -1)  {
        printf("get file size failed. errno:%d\n", errno);
        return 1;
    }

    const off_t nNewlen = st.st_size + nNewLoadSecLen;

    int nRt = ftruncate(fdNew, nNewlen);
    if (nRt!=0){
        printf("ftruncate failed. len=%d errno:%d\n",nNewlen, errno);
    }
    
    unsigned char * pFileNew = mmap(NULL, nNewlen, PROT_READ | PROT_WRITE, MAP_SHARED, fdNew, 0);
    if (pFileNew == NULL || pFileNew == (void *)-1)
    {
        printf("map new file failed. errno:%d\n", errno);
        close(fdNew);
        return 1;
    }

    //write new section
    printf("begin write new loader section...\n");
    memcpy(pFileNew + st.st_size, pNewLoadSec, nNewLoadSecLen);
    printf("write new loader section success.\n");

    //update loader section header
    unsigned char *pDataNew = pFileNew;
    FILHDR *pXCoffHdrNew = (FILHDR *)pDataNew;
    pDataNew += sizeof(FILHDR);
    pDataNew += be16toh(pXCoffHdrNew->f_opthdr);
    for (int iSection = 0; iSection < be16toh(pXCoffHdrNew->f_nscns); iSection++, pDataNew += sizeof(SCNHDR))
    {
        SCNHDR *pSecHDRNew = (SCNHDR *)pDataNew;

        if (strcasecmp(pSecHDRNew->s_name, _LOADER) == 0)
        {
            pSecHDRNew->s_size = nNewLoadSecLen;
            pSecHDRNew->s_scnptr = htobe64(st.st_size);
        }
    }

    nRt = msync(pFileNew, nNewlen, MS_SYNC);
    if (nRt!=0){
        printf("msync failed. errno:%d\n", errno);
    }
    
    munmap(pFileNew, nNewlen);
    pFileNew = NULL;

    return 1;
}

int add_needed(const char* szNeeded, const char* szLibrary)
{
   
   int fdLibrary = open(szLibrary, O_RDONLY);
   if (fdLibrary==-1){
       printf("open file failed. errno=%d\n", errno);
       return 1 ;
   }

    do
    {
        struct stat st;
        int r = fstat(fdLibrary, &st);
        if (r == -1)  {
            printf("get file size failed. errno:%d\n", errno);
            break;
        }

        const int nLiblen = st.st_size;
        unsigned char * pFile = mmap(NULL, nLiblen, PROT_READ, MAP_SHARED, fdLibrary, 0);
        if (pFile == NULL || pFile == (void *)-1) {
            printf("map failed, errno:%d\n", errno);
            break;
        }
   
        //close file, use mmap only
        close(fdLibrary);
        fdLibrary=-1;

        
        const unsigned char *pData = pFile;
        FILHDR *pXCoffHdr = (FILHDR *)pData;
       // printf("f_magic:%04x, f_nscns:%x, f_opthdr:%x\n", be16toh(pXCoffHdr->f_magic), be16toh(pXCoffHdr->f_nscns), be16toh(pXCoffHdr->f_opthdr));

        //move to section headers
        pData += sizeof(FILHDR);
        pData += be16toh(pXCoffHdr->f_opthdr);

        //look through section header
        unsigned char *pNewLoadSec = 0;
        long long nNewLoadSecLen = 0;
        for (int iSection = 0; iSection < be16toh(pXCoffHdr->f_nscns); iSection++, pData += sizeof(SCNHDR))  {

            SCNHDR *pSecHDR = (SCNHDR *)pData;
            unsigned long long secLen = be64toh(pSecHDR->s_size);
            long long secOffset = be64toh(pSecHDR->s_scnptr);
           
            if (strcasecmp(pSecHDR->s_name, _LOADER) == 0)  {
                printf("find loader section: sec name:%s, data offiset:%d length:%d,  s_flags:%x\n",  pSecHDR->s_name, secOffset, secLen, pSecHDR->s_flags);

                printf("\nParse old load section:\n");
                const unsigned char* pLoaderSecAddr = pFile+secOffset;
                parse_loader_section(pLoaderSecAddr, secLen);
 
                added_import_fileid(pLoaderSecAddr, secLen, szNeeded, &pNewLoadSec, &nNewLoadSecLen);

                printf("\nParse new load section:\n");
                parse_loader_section(pNewLoadSec, nNewLoadSecLen);

                break;   
            }
        }
  
        munmap(pFile, nLiblen);
        pFile = NULL;
        
         //re-open the file and write the new data
         write_new_loader_section(szLibrary, pNewLoadSec, nNewLoadSecLen);

         //free memory
         free(pNewLoadSec);
         pNewLoadSec = NULL;
         nNewLoadSecLen = 0;

    } while (0);
    
   
    
    return 0;
}


/* useage: patchxcoff --add-needed libfoo.so my-program */
int main(int argc, char** argv)
{
   if (argc<2){
       print_usage();
       exit(1);
   }
   else{
       char* pCmd = argv[1];
       if (strcasecmp(pCmd, "--add-needed")==0 && argc==4) {
           add_needed(argv[2], argv[3]);          
       }
       else{
           print_usage();
           exit(1);
       }       
   }

   return 1;
}
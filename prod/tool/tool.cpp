#include "tool.h"
#include <assert.h>
#include <locale>
#include <codecvt>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <Shlobj.h>
#include <shlwapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#ifdef _AIX
    #include <fcntl.h>
#else
    #include <sys/fcntl.h>
#endif
#endif
#include "md5.h"

#define BOOST_PP_STRINGIZE(text) BOOST_PP_STRINGIZE_I(text)
#define BOOST_PP_STRINGIZE_I(text) #text

const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);


#ifdef _WIN32
#ifndef LIBTOOL_STATIC
HINSTANCE g_hDllHandleTool=NULL;
extern "C" BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    UNREFERENCED_PARAMETER(Reserved);

    BOOLEAN bSuccess = TRUE;

    switch (nReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hDllHandleTool = hDllHandle;
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return bSuccess;
}
#endif
#endif

namespace CommonFun {

    std::string ToUTF8(const std::string &src, std::string &dst) {
        dst = src;
        return dst;
    }

    std::string ToUTF8(const std::u16string &src, std::string &dst) {
        try
        {
           #if (defined _WIN32) && (_MSC_VER >= 1900)
             auto p = reinterpret_cast<const int16_t *>(src.data());
             dst = std::wstring_convert< std::codecvt_utf8_utf16<int16_t>, int16_t >{}.to_bytes(p, p + src.size());
           #else
             dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(src);
           #endif
        }
        catch(const std::exception& e)
        {
            dst.clear();
        }

        return dst;
    }

    std::string ToUTF8(const std::u16string& src) {
        try
        {
            std::string dst ;
            ToUTF8(src, dst);     
            return dst;
        }
        catch(const std::exception& e)
        {
            return "";
        }
    }

    std::string ToUTF8(const std::u32string &src, std::string &dst) {
        try
        {
            #if (defined _WIN32) && (_MSC_VER >= 1900)
            auto p = reinterpret_cast<const __int32 *>(src.data());
            dst = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.to_bytes(p, p + src.size());
            #else
            dst = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(src);
            #endif
        }
        catch(const std::exception& e)
        {
            dst.clear();
        }
        

        return dst;
    }

    std::string ToUTF8(const std::wstring &src, std::string &dst) {
        try
        {
            dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(src);
        }
        catch(const std::exception& e)
        {
            dst.clear();
        }
        
        return dst; 
    }


    std::string FromUTF8(const std::string &src, std::string &dst) {
        dst = src;
        return dst;
    }

    std::u16string FromUTF8(const std::string &src, std::u16string &dst) {
        dst = FromUTF8(src);
        return dst;
    }

    std::u16string FromUTF8(const std::string& src){
        std::u16string dst;
        try
        {
            #if (defined _WIN32) && (_MSC_VER >= 1900)
            auto p = std::wstring_convert< std::codecvt_utf8<int16_t>, int16_t >{}.from_bytes(src);
            auto t = reinterpret_cast<const char16_t *>(p.data());
            dst = std::u16string(t, t + p.size());
            #else
            dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(src);

                //        for (size_t i = 0; i < dst.length(); i++)
                //        {
                //            dst[i] = htobe16(dst[i]);
                //        }
        
            #endif
        }
        catch(const std::exception& e)
        {
            dst.clear();
        }
        
        return dst;
    }

    std::u32string FromUTF8(const std::string &src, std::u32string &dst) {
        try
        {
            #if (defined _WIN32) && (_MSC_VER >= 1900)
            auto p = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.from_bytes(src);
            auto t = reinterpret_cast<const char32_t *>(p.data());
            dst = std::u32string(t, t + p.size());
            #else
            dst = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(src);
            #endif
        }
        catch(const std::exception& e)
        {
            dst.clear();
        }
        
        return dst;
    }

    std::wstring FromUTF8(const std::string &src, std::wstring &dst) {
        try
        {
             dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(src);
        }
        catch(const std::exception& e)
        {
            dst.clear();
        }

        return dst;
    }

    std::string UrlEncode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char) c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    int StrCaseCmp(const std::string& s1, const std::string& s2){
#ifndef _WIN32
        return strcasecmp(s1.c_str(), s2.c_str());
#else
        return stricmp(s1.c_str(), s2.c_str());
#endif
    }

    int StrCaseCmp(const std::u16string& s1, const std::u16string& s2) {
        std::string s11, s22;
        ToUTF8(s1, s11); ToUTF8(s2, s22);
#ifndef _WIN32
        return strcasecmp(s11.c_str(), s22.c_str());
#else
		return stricmp(s11.c_str(), s22.c_str());
#endif
    }

    int StrCaseCmp(const std::u16string& s1, const std::string& s2) {
        std::string s11;
        ToUTF8(s1, s11);
#ifndef _WIN32
        return strcasecmp(s11.c_str(), s2.c_str());
#else
		return stricmp(s11.c_str(), s2.c_str());
#endif
    }

    int StrCaseCmp(const char *s1, const char *s2) {
#ifndef _WIN32
        return strcasecmp(s1, s2);
#else
        return stricmp(s1, s2);
#endif
    }

#ifndef _WIN32
    unsigned long get_module_base(pid_t pid, const char* module_name)
    {
        FILE *fp = NULL;
        unsigned long addr = 0;
        char *pAddrRange = NULL;
        char filename[32] = {0};
        char line[1024] = {0};

        if (pid < 0)
        {
            snprintf(filename, sizeof(filename), "/proc/self/maps");
        }
        else
        {
            snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
        }
        fp = fopen(filename, "r");
        if (fp != NULL)
        {
            while (fgets(line, sizeof(line), fp))
            {
                if (strstr(line, module_name))
                {
                    pAddrRange = strtok(line, "-");
                    addr = strtoul(pAddrRange, NULL, 16);
#if defined(__x86_64__)
                    if (addr == 0x400000)
                    {
                        addr = 0;
                    }
#elif defined(__i386__)
                    if (addr == 0x08048000)
				{
					addr = 0;
				}
#endif
                    break;
                }
            }
            fclose(fp);
        }
        return addr;
    }
#endif

void split(const std::string& src, std::vector<std::string>& tokens, const std::string& delimiters)
{
    std::string::size_type lastPos = src.find_first_not_of(delimiters, 0);
    std::string::size_type pos = src.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos!=lastPos){
        tokens.push_back(src.substr(lastPos, pos-lastPos));
        lastPos = src.find_first_not_of(delimiters, pos);
        pos = src.find_first_of(delimiters, lastPos);
    }
}

uint64_t ConvertBCDNumber(unsigned char* pData, int nLen)
{
    unsigned char* pCurrent = pData + nLen - 1;
    int nRate = 1;
    uint64_t nValue = 0;
    do
    {
        uint64_t nValue1 = ((*pCurrent)&0x0F) * nRate;
        nRate *= 10;

        uint64_t nValue2 = (((*pCurrent)>>4)&0x0F) * nRate;
        nRate *= 10;

        nValue += nValue1 + nValue2;
        pCurrent--;

    } while (pCurrent>=pData);

    return nValue;
}

#ifndef LIBTOOL_STATIC
std::string  GetInstallPath()
{
#ifdef _WIN32
    char szPath[2*MAX_PATH]={0};
    DWORD dwLen = GetModuleFileNameA(g_hDllHandleTool, szPath, sizeof(szPath) );
    if(dwLen){
        char* pName = strrchr(szPath, '\\');
        if(pName){
            *pName='\0';  //remove the file name

            char* pBin = strrchr(szPath, '\\');
            if(pBin){
                *pBin= '\0'; //remove bin directory
            }
        }
    }
    return szPath;
#else
    return "/usr/NextLabs/DAE";
#endif 
}

std::string TOOL_EXPORT GetProgramDataFolder()
{
       //get programdata folder
    std::string strDataFolder;
#ifdef _WIN32
	strDataFolder =  GetInstallPath() + "\\data\\"; // for enterprise environment, can't write data to c:\ driver
    if (!PathFileExistsA(strDataFolder.c_str())) {
        int nRes = SHCreateDirectoryExA(NULL, strDataFolder.c_str(), NULL);
        if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_ALREADY_EXISTS) && (nRes != ERROR_FILE_EXISTS))  {
            strDataFolder = "";
        }
    }
#else
    strDataFolder =  GetInstallPath() + "/data/"; 
    if (access(strDataFolder.c_str(), 0) != 0)  {
        if (mkdir(strDataFolder.c_str(), S_IRUSR | S_IWUSR | S_IXUSR))  {
            strDataFolder.clear();
        }
    }
   
#endif 

    return strDataFolder;
}
#endif

//////////////////////////////

std::string TOOL_EXPORT md5(const std::string& str)
{
    MD5 md5 = MD5(str);
	return md5.hexdigest();
}

std::string TOOL_EXPORT md5(const unsigned char* pData, unsigned int len)
{
    MD5 md5Obj;
	md5Obj.update(pData, len);
	md5Obj.finalize();
	return md5Obj.hexdigest();
}

int TOOL_EXPORT md5_file(const char* filename, std::string& strmd5)
{
    int nRet = 0;
	
#ifdef _WIN32

    HANDLE fh = NULL, mh = NULL;
    const unsigned char* szFile = NULL;
    do {
        fh = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if(fh == INVALID_HANDLE_VALUE)
        {
            nRet = 1;
            break;
        }

        DWORD nFile = GetFileSize(fh, NULL);

        mh = CreateFileMappingA(fh, NULL, PAGE_READONLY, 0, 0, NULL); 
        if(mh == NULL)
        {
            nRet = 2;
            break;
        }

        szFile = (unsigned char*)MapViewOfFile(mh, FILE_MAP_READ, 0, 0, 0); 
        if(szFile == NULL)
        {
            nRet = 3;
            break;
        }

        strmd5= md5(szFile, (unsigned int)nFile);
    } while (false);

    //close
    if(szFile){
        UnmapViewOfFile(szFile);
        szFile = NULL;
    }
    if(mh){
        CloseHandle(mh);
        mh = NULL;
    }
    if(fh){
        CloseHandle(fh);
        fh = NULL;
    }
#else

    //open the file
    int fd = -1;
    do {
        fd = open(filename, O_RDONLY);
        if (-1 == fd) {
            nRet = 1;
            break;
        }

        //get the filesize
        struct stat statbuf = {0};
        if (stat(filename, &statbuf) == -1) {
            nRet=2;
            break;
        }
        const intmax_t _dwDataLen = (intmax_t)statbuf.st_size;

        //call mmap
        unsigned char* pdata = (unsigned char *)mmap(0, _dwDataLen, PROT_READ, MAP_SHARED, fd, 0);
        if (pdata == (void *)-1) {
            nRet=3;
            break;
        }

        strmd5= md5(pdata, _dwDataLen);

        //unmap
         munmap(pdata, _dwDataLen);
        pdata = NULL;

    } while (false);

    //release the file descriptor
    if(fd>0){
        close(fd);
        fd=0;
    }
#endif

    return nRet;
}

}// name space
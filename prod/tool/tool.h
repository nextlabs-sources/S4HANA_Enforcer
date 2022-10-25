#ifndef TOOL_H
#define TOOL_H

#ifdef _WIN32
#pragma warning(disable: 4996)
#define strdup _strdup
#else
#define sprintf_s snprintf
#endif
#include <cctype>
#include <iomanip>
#include <sstream>
#include <vector>
#include "tool_export.h"

namespace  CommonFun {
    std::string TOOL_EXPORT ToUTF8(const std::string& src, std::string& dst);
    std::string TOOL_EXPORT ToUTF8(const std::u16string& src, std::string& dst);
    std::string TOOL_EXPORT ToUTF8(const std::u16string& src);
    std::string TOOL_EXPORT ToUTF8(const std::u32string& src, std::string& dst);
    std::string TOOL_EXPORT ToUTF8(const std::wstring& src, std::string& dst);


    std::string TOOL_EXPORT FromUTF8(const std::string& src, std::string& dst);
    std::u16string TOOL_EXPORT FromUTF8(const std::string& src, std::u16string& dst);
    std::u16string TOOL_EXPORT FromUTF8(const std::string& src);
    std::u32string TOOL_EXPORT FromUTF8(const std::string& src, std::u32string& dst);
    std::wstring TOOL_EXPORT FromUTF8(const std::string& src, std::wstring& dst);

    std::string TOOL_EXPORT UrlEncode(const std::string &value);

    int TOOL_EXPORT StrCaseCmp(const char *s1, const char *s2);
    int TOOL_EXPORT StrCaseCmp(const std::u16string& s1, const std::u16string& s2);
    int TOOL_EXPORT StrCaseCmp(const std::u16string& s1, const std::string& s2);
    int TOOL_EXPORT StrCaseCmp(const std::string& s1, const std::string& s2);

    struct TOOL_EXPORT IgnoreCaseCmp : public std::binary_function<std::string, std::string, bool> {
        bool operator()(const std::string& left, const std::string& right) const {
            return StrCaseCmp(left.c_str(), right.c_str()) < 0;
        }
    };
#ifndef _WIN32
    unsigned long TOOL_EXPORT get_module_base(pid_t pid, const char* module_name);
#endif
   
     void TOOL_EXPORT split(const std::string& src, std::vector<std::string>& tokens, const std::string& delimiters);
     uint64_t TOOL_EXPORT ConvertBCDNumber(unsigned char* pData, int nLen);

#ifndef LIBTOOL_STATIC
     std::string  TOOL_EXPORT GetInstallPath();
     std::string TOOL_EXPORT GetProgramDataFolder();
#endif

    std::string TOOL_EXPORT md5(const std::string& str);
    std::string TOOL_EXPORT md5(const unsigned char* pData, unsigned int len);
    int TOOL_EXPORT md5_file(const char* filename, std::string& strmd5);
}

#endif
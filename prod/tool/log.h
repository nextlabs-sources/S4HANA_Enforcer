#ifndef LOG_H_2019_01_28
#define LOG_H_2019_01_28
#include <stdio.h>
#include <string>
#include "tool_export.h"

#pragma warning(push)
#pragma warning(disable : 4714)
#include <boost/shared_ptr.hpp>
#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp> 
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/console.hpp> // boost::log::add_console_log
#include <boost/log/utility/setup/file.hpp> // boost::log::add_file_log
#include <boost/filesystem.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#pragma warning(pop)

enum emLogLevel
{
    log_trace = boost::log::trivial::trace,
    log_debug = boost::log::trivial::debug,
    log_info = boost::log::trivial::info,
    log_warning = boost::log::trivial::warning,
    log_error = boost::log::trivial::error,
    log_fatal = boost::log::trivial::fatal
};

typedef std::function<std::string(void)> LOG_PRINT_FUN;

namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
typedef sinks::synchronous_sink < sinks::text_file_backend > sink_t;
typedef boost::shared_ptr< sink_t > sink_ptr;
typedef boost::shared_ptr< sinks::file::collector > log_collector_ptr;

//The path name MUST end with a directory separator
#ifdef _WIN32
#define EM_PRODUCT_DIR "C:\\ProgramData\\Nextlabs\\HANADB_Enforcer\\"
#define LOG_DEFAULT_DIR EM_PRODUCT_DIR "log\\"
#else
#define EM_PRODUCT_DIR "/usr/nextlabs/ems4h/"
#define LOG_DEFAULT_DIR EM_PRODUCT_DIR "log/"
#endif


//format: hdbnew{pid}_%Y-%m-%d_%H-%M-%S.%N.log
#define LOG_NAME_REGEX "hdbnew\\d+_\\d{4}-\\d{2}-\\d{2}_\\d{2}-\\d{2}-\\d{2}\\.\\d+\\.log" 

#define LOG_ROTATION_SIZE (10)  //uinit is M
#define LOG_MIN_FREE_SPACE (10)  //unit is G
#define LOG_MAX_FILE_NUM (2)

class TOOL_EXPORT CLog
{
 public:
 static CLog* Instance()
 {
     static CLog* theLog = new CLog();
     return theLog;
 }

 sink_ptr InitLog(const std::string& strModuleName,
    uintmax_t nMaxFileSize = LOG_ROTATION_SIZE,
    uintmax_t nMaxFileNum = LOG_MAX_FILE_NUM,
    uintmax_t minFreeSpace = LOG_MIN_FREE_SPACE);
 int  WriteLog(int lvl, const wchar_t* fmt, ...);
 int  WriteLog(int lvl, const char* fmt, ...);
 void  WriteLog(int lvl, LOG_PRINT_FUN logFun);

 void UpdateLogLevel(const std::string& strlevel);

 void TidyUpLog();
 //void TidyUpLog(const boost::filesystem::path& logdir, log_collector_ptr collector);

 protected:
 CLog();
 CLog(const CLog&){}

 private:
void WriteLogInternal(int lvl, const char* msg);
std::string ToUTF8(const std::u16string& src, std::string& dst);
std::string ToUTF8(const std::wstring& src, std::string& dst);

 private:
 int m_logLevel;
 boost::filesystem::path m_logdir;
 //log_collector_ptr m_collector;
  bool _logInited;
};

#define theLog CLog::Instance()

#endif 
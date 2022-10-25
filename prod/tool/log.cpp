#include "log.h"
//#include "commfun.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <Shlobj.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#endif 
#include <locale>
#include <codecvt>
#include <regex> //since C++11

#define MAX_MESSAGE_SIZE_CHARS 1024*10

// Declare attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

CLog::CLog()
{
  m_logLevel =   boost::log::trivial::trace;
   _logInited = false;
}

std::string CLog::ToUTF8(const std::u16string& src, std::string& dst) 
{
#if (defined _WIN32) && (_MSC_VER >= 1900)
	auto p = reinterpret_cast<const int16_t *>(src.data());
	dst = std::wstring_convert< std::codecvt_utf8_utf16<int16_t>, int16_t >{}.to_bytes(p, p + src.size());
#else
	dst = std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(src);
#endif
	return dst;
}

std::string CLog::ToUTF8(const std::wstring& src, std::string& dst) {
	dst = std::wstring_convert< std::codecvt_utf8<wchar_t >, wchar_t >{}.to_bytes(src);
	return dst;
}


//@param log_file_name_prefix e.g. hdb, C:\\TMP\\hdb, /usr/nextlabs/ems4h/log
//Note if log_file_name_prefix doesn't contain a parent, it uses the default log directory path.
sink_ptr CLog::InitLog(const std::string& strModuleName, uintmax_t nMaxFileSize, uintmax_t nMaxFileNum, uintmax_t minFreeSpace)
{
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();

	core->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
	boost::log::add_common_attributes(); // Add TimeStamp, ThreadID so that we can use those attributes in Format.

	/* log formatter: [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message */
	auto fmtTimeStamp = boost::log::expressions::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S.%f");
	//auto fmtThreadId = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
	auto fmtSeverity = boost::log::expressions::attr<boost::log::trivial::severity_level>(severity.get_name());

	boost::log::formatter logFmt = boost::log::expressions::format("%1%|%2%|%3%")
		% fmtTimeStamp %  fmtSeverity % boost::log::expressions::smessage;
	

#if 1
	{
	// build_log_path
	boost::filesystem::path fspath(strModuleName);
	//https://www.boost.org/doc/libs/1_67_0/libs/filesystem/doc/reference.html#path-parent_path
	//bool has_parent_path() const     { return !parent_path().empty(); }
	boost::filesystem::path parent = fspath.parent_path();
	m_logdir = fspath;
	fspath.concat("_%N.log");
	//Now, fspath looks like /usr/nextlabs/ems4h/log/app_%Y-%m-%d_%H-%M-%S.%N.log

		typedef sinks::text_file_backend backend_t;
		boost::shared_ptr< sinks::text_file_backend > backend =
			boost::make_shared< sinks::text_file_backend >(
				boost::log::keywords::file_name = fspath,
				boost::log::keywords::enable_final_rotation = true,
				boost::log::keywords::rotation_size = nMaxFileSize,
				boost::log::keywords::open_mode = std::ios_base::app);
		sink_ptr fsSink(new sink_t(backend));
		fsSink->set_formatter(logFmt);
		fsSink->locked_backend()->auto_flush(true);

		// `sinks::file::make_collector` will call `filesystem::create_directories(m_StorageDir);` and it may throw
		// `boost::system::errc::permission_denied`. So, finally, add the `fsSink` or throw such `filesystem_error` when log messages
        	auto collector = sinks::file::make_collector(
			boost::log::keywords::target = parent,
			boost::log::keywords::min_free_space = minFreeSpace,
			boost::log::keywords::max_files = nMaxFileNum);

		//1. Set up where the rotated files will be stored
		fsSink->locked_backend()->set_file_collector(collector);

		//2. Upon restart, scan the directory for files matching the file_name pattern. Only save file list.
		//   Here, all files are affected and old files will be deleted at next rotation if condition met.
		//This will cause the file collector scan the target directory for log files that could have been left from the previous runs.
		//If the file name pattern includes a file counter, the call can also detect the next counter value, which is what you want.
		uintmax_t nfiles = fsSink->locked_backend()->scan_for_files();

		core->add_sink(fsSink);
		_logInited = true;
		return fsSink;
	}
#endif

#if 0
	/* Windows debugger output backend OutputDebugString */
	typedef sinks::asynchronous_sink< sinks::debug_output_backend > debug_sink_t;
	// Create the sink. The backend requires synchronization in the frontend.
	boost::shared_ptr< debug_sink_t > dbgSink = boost::make_shared<debug_sink_t>();
	// Set the special filter to the frontend in order to skip the sink when no debugger is available
//	dbgSink->set_filter(boost::log::expressions::is_debugger_present());
	dbgSink->set_formatter(logFmt);
	core->add_sink(dbgSink);
#endif 
}


void CLog::WriteLogInternal(int nlevel, const char* szLog)
{
	try{

		if(_logInited && nlevel>=m_logLevel) {
			switch(nlevel) {
				case boost::log::trivial::trace:
					BOOST_LOG_TRIVIAL(trace)<<szLog;
					break;
				case boost::log::trivial::debug:
					BOOST_LOG_TRIVIAL(debug)<<szLog;
					break;
				case boost::log::trivial::info:
					BOOST_LOG_TRIVIAL(info)<<szLog;
					break;
				case boost::log::trivial::warning:
					BOOST_LOG_TRIVIAL(warning)<<szLog;
					break;
				case boost::log::trivial::error:
					BOOST_LOG_TRIVIAL(error)<<szLog;
					break;
				case boost::log::trivial::fatal:
					BOOST_LOG_TRIVIAL(fatal)<<szLog;
					break;
				default:
				break;
			}
		}
	}
	catch(...){
	}
	
}

int CLog::WriteLog(int nlevel, const char* szFmt, ... )
{
	if(nlevel >= m_logLevel)
	{
		char szLog[MAX_MESSAGE_SIZE_CHARS] = {0};

	//format log content
	va_list args;
	va_start(args, szFmt);
	int nLog = vsnprintf(szLog, MAX_MESSAGE_SIZE_CHARS-1, szFmt, args);
	va_end(args);

    WriteLogInternal(nlevel, szLog);

	 return nLog;
	}
	
	return 0;
}

int CLog::WriteLog(int nlevel, const wchar_t* wszFmt, ... )
{
	if(nlevel >= m_logLevel)
	{
        wchar_t wszLog[MAX_MESSAGE_SIZE_CHARS] = {0};

        //format log content
        va_list args;
        va_start(args, wszFmt);
        int nLog = vswprintf(wszLog, MAX_MESSAGE_SIZE_CHARS-1, wszFmt, args);
        va_end(args);

        //convert from wchar to char
        std::string u8_cvt_str;
        ToUTF8(wszLog, u8_cvt_str);
        WriteLogInternal(nlevel, u8_cvt_str.c_str() );
        return nLog;
	}

	return 0;
}

void CLog::WriteLog(int nLevel, LOG_PRINT_FUN logFun)
{
	if((nLevel>=m_logLevel) && (logFun!=nullptr) )
	{
		std::string strLog = logFun();
		WriteLogInternal(nLevel, strLog.c_str() );
	}
}


void CLog::UpdateLogLevel(const std::string& strlevel)
{
	int nLevel = atoi(strlevel.c_str());
	if(nLevel>=boost::log::trivial::trace && nLevel<=boost::log::trivial::fatal)
	{
       boost::log::core::get()->set_filter(boost::log::trivial::severity >= nLevel);
	   m_logLevel = nLevel;
	}
}

void CLog::TidyUpLog()
{
	//TidyUpLog(m_logdir, m_collector);
}

#if 0
void CLog::TidyUpLog(const boost::filesystem::path& logdir, log_collector_ptr collector)
{
	UNREFERENCED_PARAMETER(logdir);
	UNREFERENCED_PARAMETER(collector);

	//boost::filesystem::exists(logdir)
	if (boost::filesystem::is_directory(logdir))
	{
		//https://docs.microsoft.com/en-us/cpp/standard-library/regular-expressions-cpp?view=vs-2019
		const std::regex re{LOG_NAME_REGEX};

		for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(logdir), {}))
		{
			const auto &path = entry.path();
			WriteLog(log_debug, "tidy_up_log|path: %s", path.c_str());
			if (!boost::filesystem::is_regular_file(path) || !std::regex_match(path.filename().c_str(), re))
			{
				WriteLog(log_debug, "tidy_up_log|not regular_file and name not matched %s", path.c_str());
				continue;
			}
			try
			{
				collector->store_file(path);
			}
			catch (const boost::filesystem::filesystem_error &err)
			{
				// err.code() == boost::system::errc::permission_denied
				WriteLog(log_debug, "store_file(%s) failed with %d, %s", path.c_str(), err.code(), err.code().message());
			}
		}
	}
}
#endif
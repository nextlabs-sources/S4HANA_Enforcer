#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include <vector>

#define MAX_MESSAGE_SIZE_CHARS 1024*10
char g_szLog[MAX_MESSAGE_SIZE_CHARS];

std::vector<STEPS::StepStatus> g_vecBootResults;

void STEPS::PrintSummaryResults()
{
	if(!g_vecBootResults.empty())
	{
		std::string summary;

		for(auto ss: g_vecBootResults)
		{
			if(!ss.step.empty())
				summary.append(ss.step).append(ss.status ? ": [Success]\n" : ": [Failed]\n");
		}

		if(!summary.empty())
		{
			printf("\nBootstrap Result\n");
			printf("%s\n", summary.c_str());
		}
	}
}

void STEPS::SetStepStatus(std::string strStep, bool bStatus)
{
    bool bExisted = false;
    for(auto &e: g_vecBootResults){
        if(!e.step.compare(strStep)){
            bExisted = true;
            e.status = bStatus;
            break;
        }
    }

    if(!bExisted){
        g_vecBootResults.emplace_back(strStep, bStatus);
    }
}

int LOG::WriteLog( const char* szFmt, ... )
{
	//format log content
	va_list args;
	va_start(args, szFmt);
	#ifdef _WIN32
	/*int nLog =*/ vsnprintf_s(g_szLog, MAX_MESSAGE_SIZE_CHARS-1, szFmt, args);
	#else
		vsnprintf(g_szLog, MAX_MESSAGE_SIZE_CHARS-1, szFmt, args);
	#endif 
	va_end(args);

    printf("%s\n", g_szLog);
	
	return 0;
}
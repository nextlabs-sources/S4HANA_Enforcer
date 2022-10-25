#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "log.h"

// Cross platform, C/C++ help tools
bool GetSapSystemIDFromPath(const char* szDispWork, std::string& refSapSystemID, std::string& refSapInstanceID);
std::string ParserSAPSID(const char* szDispWork);
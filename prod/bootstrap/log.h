#ifndef DIAGTOOL_LOG_H
#define DIAGTOOL_LOG_H

#include <string>
#include <vector>

#define STEP_DETACH_DAE "Detach DAE"
#define STEP_ATTACH_DAE "Attach DAE"
#define STEP_GET_INTERNAL_SYMBOLS "Get Internal Symbols"
#define STEP_CONNECT_TO_JPC "Connect to JPC"
#define STEP_VALID_JPC "Valid JPC"
#define STEP_CONNECT_TO_CC "Connect to CC"
#define STEP_VALID_CC_USER "Valid CC User"
#define STEP_VALID_JPC_USER "Valid JPC User"
#define STEP_VALID_JPC_SERVER "Valid JPC Server"
#define STEP_READ_CONFIG "Read Config"

namespace STEPS
{
  struct StepStatus{
    std::string step;
    bool status;
    StepStatus() : step(""),status(false) {}
    StepStatus(std::string s, bool st = false) : step(s),status(st) {}
  };

  void PrintSummaryResults();
  void SetStepStatus(std::string, bool);
};

namespace LOG
{
  int WriteLog(const char* szFmt, ... );
};

#endif 

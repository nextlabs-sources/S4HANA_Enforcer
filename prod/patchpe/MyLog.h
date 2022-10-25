#pragma once
#include <stdio.h>

extern bool g_bOutputDebugLog;

// Log only two level, one is for user, another is for develop
// #define HZPrintLogW(strFormat, ...) (wprintf_s((strFormat), ##__VA_ARGS__))
#define HZPrintDebugLogA(strFormat, ...) ((g_bOutputDebugLog) ? (printf_s((strFormat), ##__VA_ARGS__)) : (NULL))
#define HZPrintUserLogA(strFormat, ...) (printf_s((strFormat), ##__VA_ARGS__))
#define HZPrintLogA(bDebug, strFormat, ...) ((bDebug) ? (HZPrintDebugLogA((strFormat), ##__VA_ARGS__)) : (HZPrintUserLogA((strFormat), ##__VA_ARGS__)))

// Boolean helper
#define HZBNum(bFlag) ((bFlag) ? 1 : 0)
// #define HZBStrW(bFlag) ((bFlag) ? L"true" : "false")
// #define HZBSuccessW(bFlag) ((bFlag) ? L"success" : L"failed")
#define HZBStrA(bFlag) ((bFlag) ? "true" : "false")
#define HZBSuccessA(bFlag) ((bFlag) ? "success" : "failed")
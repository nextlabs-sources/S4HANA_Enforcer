#pragma once
#include <windows.h>

extern "C" {

	typedef struct {
		char16_t client[3];
		char16_t user[12];

#if defined(_AIX) || defined(_WIN32)
		char16_t terminal[20];
#else
		char16_t terminal[64];
#endif 

		char16_t tcod[20];
#ifdef _AIX
		char16_t programInfo[24];
#else
		char16_t programInfo[40];
#endif

		time_t diatime;
	}DP_LOGON_INFO;
}
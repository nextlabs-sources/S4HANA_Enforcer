
#include "stdafx.h"
#include <Windows.h>
#include "MyDisp+Work_fun.h"

extern "C" {

__declspec(dllexport) int DpSesGetLogonInfo(int logon, DP_LOGON_INFO* logonInfo)
  {
	char16_t pClient[3] = { u'8', u'1', u'1' };
    memcpy(logonInfo->client, pClient, sizeof(pClient) );

	char16_t pTcode[20] = u"testApp";
	memcpy(logonInfo->tcod, pTcode, sizeof(pTcode) );

	char16_t pUser[12] = u"testUser";
	memcpy(logonInfo->user, pUser, sizeof(pUser));

     return 1;
  }

}
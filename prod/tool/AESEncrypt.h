#ifndef SRC_UTILS_AES_ENCRYPTOR_H
#define SRC_UTILS_AES_ENCRYPTOR_H

#include <string>
#include "tool_export.h"

#define AES_DEFAULT_KEY "343949349~!@##$$$+__)(**&^%$%NM<<>>>>>JHGFDZXCBNM<>???PPO(*&^%$$$$&POPPOOIII"

namespace CommonFun
{

class AES;

class TOOL_EXPORT AesEncryptor
{
public:
	AesEncryptor(unsigned char *key = (unsigned char *)AES_DEFAULT_KEY);
	~AesEncryptor(void);

	std::string EncryptString(std::string strInfor);
	std::string DecryptString(std::string strMessage);

	void EncryptTxtFile(const char *inputFileName, const char *outputFileName);
	void DecryptTxtFile(const char *inputFileName, const char *outputFileName);

private:
	void Byte2Hex(const unsigned char *src, int len, char *dest);
	void Hex2Byte(const char *src, int len, unsigned char *dest);
	int Char2Int(char c);

private:
	AES *m_pEncryptor;
};

} // namespace CommonFun
#endif // SRC_UTILS_AES_ENCRYPTOR_H#pragma once

// cfg.cpp : Defines the entry point for the console application.
//

#include <string>
#include <AESEncrypt.h>
#include <string.h>
#define BOOST_PP_STRINGIZE(text) BOOST_PP_STRINGIZE_I(text)
#define BOOST_PP_STRINGIZE_I(text) #text
const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);

int main(int argc, char **argv)
{
	if (2 <= argc)
	{
		// TODO change the code to make the szKey can't be crack easier directly.
		CommonFun::AesEncryptor theAes;
		if (2 == argc)
		{
			puts(theAes.EncryptString(argv[1]).c_str());
			return 0;
		}
		else if ('d' == argv[1][0])
		{
			puts(theAes.DecryptString(argv[2]).c_str());
			return 0;
		}
	}
	printf("usage: cfg <plain text>\n       cfg d <cipher text>\n");
	return 1;
}

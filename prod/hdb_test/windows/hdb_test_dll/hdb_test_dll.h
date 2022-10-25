// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HDB_TEST_DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HDB_TEST_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HDB_TEST_DLL_EXPORTS
#define HDB_TEST_DLL_API __declspec(dllexport)
#else
#define HDB_TEST_DLL_API __declspec(dllimport)
#endif

// This class is exported from the hdb_test_dll.dll
class HDB_TEST_DLL_API Chdb_test_dll {
public:
	Chdb_test_dll(void);
	// TODO: add your methods here.
};

extern HDB_TEST_DLL_API int nhdb_test_dll;

HDB_TEST_DLL_API int fnhdb_test_dll(void);

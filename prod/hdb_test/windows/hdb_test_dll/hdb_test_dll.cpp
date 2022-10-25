// hdb_test_dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "hdb_test_dll.h"


// This is an example of an exported variable
HDB_TEST_DLL_API int nhdb_test_dll=0;

// This is an example of an exported function.
HDB_TEST_DLL_API int fnhdb_test_dll(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see hdb_test_dll.h for the class definition
Chdb_test_dll::Chdb_test_dll()
{
    return;
}

include_directories(../hdb/include)
#include_directories(../../subhook)
include_directories(../tool)

set(CMAKE_CXX_FLAGS "-L/home/user001/tool/instantclient_12_2 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../tool ${CMAKE_CXX_FLAGS}")

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH ${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH} ${NXL_RPATH})
else()
set(CMAKE_CXX_FLAGS "-Wl,-rpath,${NXL_RPATH} ${CMAKE_CXX_FLAGS}")
endif()


add_executable(hdb_test ./main.cpp)

target_link_libraries(hdb_test libSQLDBCHDB.so)

target_link_libraries(hdb_test dl)
#target_link_libraries(hdb_test libtool.so)
#target_link_libraries(hdb_test libboost_system.so)

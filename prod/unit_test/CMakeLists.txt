include_directories(../microtest)
include_directories(../hdb_new/include)
include_directories(../hdb)
include_directories(../QueryCloudAZSDKCpp/include)

add_executable(unit_test ./test.cpp)

set(CMAKE_CXX_FLAGS "-L../hdb ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../tool ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../subhook ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../funchook ${CMAKE_CXX_FLAGS}")

target_link_libraries(unit_test SQLDBCHDB)
target_link_libraries(unit_test hdb_new)

target_link_libraries(unit_test tool)
target_link_libraries(unit_test QueryCloudAZSDKCpp)
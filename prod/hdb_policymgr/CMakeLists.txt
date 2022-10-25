include_directories(./include)
include_directories(../tool)
include_directories(../jsoncpp/include)
include_directories(${OPENSSL_ROOT}/include/ )
include_directories(${BOOST_ROOT}/include)

set(CMAKE_CXX_FLAGS "-L../jsoncpp ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L${OPENSSL_ROOT}/lib/ ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../tool  ${CMAKE_CXX_FLAGS}")

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH ${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH} ${NXL_RPATH})
else()
set(CMAKE_CXX_FLAGS "-Wl,-rpath,${NXL_RPATH} ${CMAKE_CXX_FLAGS}")
endif()

aux_source_directory(./src SRCS)

#set(CMAKE_CXX_FLAGS "-Wl,--version-script=${PROJECT_SOURCE_DIR}/hdb_policymgr/export.map ${CMAKE_CXX_FLAGS}")

add_library(hdb_policymgr SHARED 
            src/Lex.cpp 
            src/log.cpp 
            src/NXLHttpClient.cpp 
            src/parse_expression.cpp 
            src/patch.cpp 
            src/policy_expression.cpp 
            src/policy_regex.cpp 
            src/Policy.cpp
            src/PolicyEngine.cpp 
            src/PolicyExport.cpp 
            src/PolicyModelList.cpp 
            src/TalkWithCC_Request.cpp 
            src/TalkWithCC_Search.cpp 
            src/TalkWithCC.cpp)

set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(hdb_policymgr -static-libgcc -static-libstdc++)

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")

set(CMAKE_CXX_FLAGS " -Wl,-bstatic  -lssl -lcrypto -Wl,-bdynamic  -ltool ${CMAKE_CXX_FLAGS}")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system log thread filesystem)
target_link_libraries(hdb_policymgr ${Boost_LIBRARIES})


else()

target_link_libraries(hdb_policymgr tool)
target_link_libraries(hdb_policymgr libssl.a)
target_link_libraries(hdb_policymgr libcrypto.a)

endif()

target_link_libraries(hdb_policymgr jsoncpp)
target_link_libraries(hdb_policymgr dl)


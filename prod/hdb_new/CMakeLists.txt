
include_directories(../hdb/include)
include_directories(./include)
include_directories(../subhook)
include_directories(../funchook/include)
include_directories(../tool)
include_directories(../QueryCloudAZSDKCpp/include)
include_directories(../hdb_policymgr/include)
include_directories(${BOOST_ROOT}/include)

set(CMAKE_CXX_FLAGS "-L../subhook ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../funchook ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../tool ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../QueryCloudAZSDKCpp/ ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../hdb_policymgr ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../jsoncpp ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L${OPENSSL_ROOT}/lib/ ${CMAKE_CXX_FLAGS}")

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_CXX_FLAGS "-L../oracle_sdk/12.2/lib/AIX/ ${CMAKE_CXX_FLAGS}")
#use big TOC to avoid TOC overflow when compile
set(CMAKE_CXX_FLAGS "-Wl,-bbigtoc ${CMAKE_CXX_FLAGS}")
set(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH ${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH} ${NXL_RPATH})
else()
set(CMAKE_CXX_FLAGS "-Wl,-rpath,${NXL_RPATH} ${CMAKE_CXX_FLAGS}")
endif()

#-Wl,--no-undefined -Wl,--unresolved-symbols=ignore-in-shared-libs

aux_source_directory(./src SRCS)


add_library(hdb_new SHARED
        ./src/hdb_new.cpp
        ./src/talk_with_hdb.cpp
        ./src/TableMetaDataImpl.cpp
        ./src/HookImpl.cpp
        ./src/EnforcerCtxImpl.cpp
        ./src/MaskStrategyImpl.cpp
        ./src/MyDisp+Work.cpp
        ./src/ViewManager.cpp
        ./src/NxlDbDataMgr.cpp
        ./src/PolicyManager.cpp
        ./src/NXS4HConfig.cpp
        ./src/SqlDbcUserInfoMgr.cpp
        ./src/SqlDbcResourceInfoMgr.cpp
        ./src/QueryCAZPolicyOpr.cpp
        ./src/KeyMaskOpr.cpp
        ./src/NxlDbDataMgr.cpp
        ./src/hdbEnforce.cpp
        ./src/TableAliasItem.cpp
        ./src/TableAliasMgr_remote.cpp
        ./src/SAPMsgNotify.cpp
        ./src/PreStartLogProxy.cpp
        ./src/QueryCAZPolicyCache.cpp
        ./src/TableDataCache.cpp

        ./parser_yacc/node.cpp
        ./parser_yacc/sqlparser_hana_bison.cpp
        ./parser_yacc/sqlparser_hana_flex.cpp
        ./parser_yacc/parse_tool.cpp
        ./parser_yacc/EMS4HAst.cpp
        ./parser_yacc/EMS4HSelectAst.cpp
        ./parser_yacc/EMS4HUpdateAst.cpp
        ./parser_yacc/EMS4HInsertAst.cpp
        ./parser_yacc/EMS4HDeleteAst.cpp
        ./parser_yacc/EMS4HUpsertAst.cpp
        ./parser_yacc/EMS4HMutliSelectAst.cpp
        ./parser_yacc/EMMaskOpr.cpp

        ./parser_view/EMViewStmt.cpp
        ./parser_view/EMColumnExpr.cpp
        ./parser_view/EMTableExpr.cpp
        ./parser_view/EMParser.cpp
        ./parser_view/EMViewMgr.cpp
)

set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(hdb_new -static-libgcc -static-libstdc++)

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")

set(CMAKE_CXX_FLAGS " -lfunchook -ltool  -lQueryCloudAZSDKCpp -lhdb_policymgr -lclntsh ${CMAKE_CXX_FLAGS}")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system log thread filesystem)
target_link_libraries(hdb_new ${Boost_LIBRARIES})

else()

target_link_libraries(hdb_new dl)
target_link_libraries(hdb_new funchook)
target_link_libraries(hdb_new tool)
target_link_libraries(hdb_new QueryCloudAZSDKCpp)
target_link_libraries(hdb_new hdb_policymgr)

endif()


include_directories(../tool)
include_directories(${BOOST_ROOT}/include)
include_directories(${OPENSSL_ROOT}/include)


set(CMAKE_CXX_FLAGS "-L../tool  ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-L../tool -L${OPENSSL_ROOT}/lib ${CMAKE_CXX_FLAGS}")

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH ${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH} ${NXL_RPATH})
else()
set(CMAKE_CXX_FLAGS "-Wl,-rpath,${NXL_RPATH} ${CMAKE_CXX_FLAGS}")
endif()

set(DIAGTOOL_MOD_NAME "bootstrap")

add_executable(${DIAGTOOL_MOD_NAME} ./main.cpp
                                    ./NXS4HConfig.cpp
                                    ./log.cpp
                                    ./NXLHttpClientAsync.cpp
                                    ./VerifyConfig.cpp
                                    ./GetInternalFunction.cpp
                                    ./AddDependence.cpp
                                    ./CommonTools.cpp)


target_link_libraries(${DIAGTOOL_MOD_NAME} -static-libgcc -static-libstdc++)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system )
target_link_libraries(${DIAGTOOL_MOD_NAME} ${Boost_LIBRARIES})


if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")

set(CMAKE_CXX_FLAGS " -ltool ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS " -Wl,-bstatic  -lssl -lcrypto -Wl,-bdynamic ${CMAKE_CXX_FLAGS}")

else()

target_link_libraries(${DIAGTOOL_MOD_NAME} tool)
target_link_libraries(${DIAGTOOL_MOD_NAME} libssl.a)
target_link_libraries(${DIAGTOOL_MOD_NAME} libcrypto.a)

endif()

target_link_libraries(${DIAGTOOL_MOD_NAME} dl)




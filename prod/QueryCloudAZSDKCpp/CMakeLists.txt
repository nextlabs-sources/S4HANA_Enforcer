cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
SET( CMAKE_VERBOSE_MAKEFILE on )
# We cannot tolerate most warnings.
set(CMAKE_CXX_FLAGS "-fPIC -Wall -pedantic ${CMAKE_CXX_FLAGS}")

set(BOOST_INCLUDEDIR  ${BOOST_ROOT}/include/)
include_directories( ${BOOST_INCLUDEDIR} )

include_directories(./include)
include_directories( ${OPENSSL_ROOT}/include/)
aux_source_directory(./src SRCS)

link_directories(${OPENSSL_ROOT}/lib/)
add_library(QueryCloudAZSDKCpp SHARED ${SRCS})

#static link to libc libc++
set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(QueryCloudAZSDKCpp -static-libgcc  -static-libstdc++)

#static link to boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system thread)
target_link_libraries(QueryCloudAZSDKCpp ${Boost_LIBRARIES})

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_CXX_FLAGS " -Wl,-bstatic  -lssl -lcrypto -Wl,-bdynamic ${CMAKE_CXX_FLAGS}")
else()
target_link_libraries(QueryCloudAZSDKCpp libssl.a)
target_link_libraries(QueryCloudAZSDKCpp libcrypto.a)
endif()

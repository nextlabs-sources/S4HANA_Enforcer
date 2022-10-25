#set(CMAKE_CXX_FLAGS "-DBOOST_ALL_DYN_LINK ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")

 


add_library(tool SHARED ./tool.cpp ./log.cpp aes.cpp AESEncrypt.cpp md5.cpp)
add_library(tool_static STATIC ./tool.cpp ./log.cpp aes.cpp AESEncrypt.cpp md5.cpp)

 
include_directories(${BOOST_ROOT}/include)

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH ${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH} ${NXL_RPATH})
else()
set(CMAKE_CXX_FLAGS "-Wl,-rpath,${NXL_RPATH} ${CMAKE_CXX_FLAGS}")
endif()

 

#set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(tool -static-libgcc -static-libstdc++)

 

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system log thread filesystem)
target_link_libraries(tool ${Boost_LIBRARIES})
target_link_libraries(tool pthread)

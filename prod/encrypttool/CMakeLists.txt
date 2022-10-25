# https://serverfault.com/questions/279068/cant-find-so-in-the-same-directory-as-the-executable
# LDFLAGS="-Wl,-rpath '-Wl,\$\$ORIGIN'" objdump -p cfg/encrytool | grep -e RPATH -e RUNPATH

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH ${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH} ${NXL_RPATH})
else()
set(CMAKE_CXX_FLAGS "-Wl,-rpath,${NXL_RPATH} ${CMAKE_CXX_FLAGS}")
endif()

#set(CMAKE_CXX_FLAGS "-L../tool ${CMAKE_CXX_FLAGS}")


set(CMAKE_CXX_FLAGS "-L../tool ${CMAKE_CXX_FLAGS}")

include_directories(../tool)

add_executable(encrypttool ./encrypttool.cpp)
#add_library(encrypttool SHARED ./encrypttool.cpp)

#set(CMAKE_EXE_LINKER_FLAGS " -static")

target_link_libraries(encrypttool -static-libgcc -static-libstdc++)

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_CXX_FLAGS " -ltool ${CMAKE_CXX_FLAGS}")
else()
target_link_libraries(encrypttool tool)
endif()

#set_property(TARGET tool PROPERTY POSITION_INDEPENDENT_CODE ON)

#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost REQUIRED)
#target_link_libraries(encrypttool ${Boost_LIBRARIES})
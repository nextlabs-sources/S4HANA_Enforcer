set(DAEMAIN_MOD_NAME "dae_main")

add_library(${DAEMAIN_MOD_NAME} SHARED
            ./dae_main_linux.cpp
            ./PreStartLog.cpp)


target_link_libraries(${DAEMAIN_MOD_NAME} -static-libgcc -static-libstdc++)

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system log)
target_link_libraries(${DAEMAIN_MOD_NAME} ${Boost_LIBRARIES})
else()
endif()

target_link_libraries(${DAEMAIN_MOD_NAME} dl)
target_link_libraries(${DAEMAIN_MOD_NAME} tool_static)

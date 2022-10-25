cmake_minimum_required(VERSION 2.8)
SET( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 14)

#hide the symbol by default
set(CMAKE_CXX_FLAGS " -fvisibility=hidden ${CMAKE_CXX_FLAGS}")

#set -pthread instead of -lpthread. reference: http://blog.chinaunix.net/uid-69906223-id-5817313.html
set(CMAKE_CXX_FLAGS " -pthread ${CMAKE_CXX_FLAGS}")

set(ARCH_x64  x64)
set(ARCH_ppc64le  ppc64le)
set(OS_aix  aix)

#flags special for AIX
if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
set(CMAKE_CXX_FLAGS " -maix64 ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-maix64 ${CMAKE_CFLAGS}")

#load .so in -l
set(CMAKE_CXX_FLAGS " -Wl,-brtl ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS " -Wl,-brtl ${CMAKE_C_FLAGS}")

set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> -X32_64 qc <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -X32_64 qc  <TARGET> <LINK_FLAGS> <OBJECTS>")
endif()

set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

set(NXL_RPATH /usr/NextLabs/DAE/bin)


# https://stackoverflow.com/questions/14375519/how-to-write-a-nice-function-that-passes-variables-by-reference
# https://stackoverflow.com/questions/45091722/cmake-add-definitions-does-not-seem-to-work
function(propagate_env_vars_as_macro name)
	#message(STATUS "[Env2Macro]arg=${name}")
	IF(DEFINED ENV{${name}})
		MESSAGE(STATUS "[Env2Macro]${name} env seen: $ENV{${name}}")
		ADD_DEFINITIONS(-D${name}=$ENV{${name}})
	ELSE()
		MESSAGE(STATUS "[Env2Macro]${name} env not defined")
	ENDIF()
endfunction()

# bash: export VERSION_MAJOR=1; echo $VERSION_MAJOR
propagate_env_vars_as_macro(VERSION_PRODUCT)
propagate_env_vars_as_macro(VERSION_MAJOR)
propagate_env_vars_as_macro(VERSION_MINOR)
propagate_env_vars_as_macro(VERSION_MAINTENANCE)
propagate_env_vars_as_macro(VERSION_PATCH)
propagate_env_vars_as_macro(BUILD_NUMBER)


add_subdirectory(QueryCloudAZSDKCpp)
add_subdirectory(funchook)
add_subdirectory(tool)
add_subdirectory(jsoncpp)
add_subdirectory(hdb_policymgr)
add_subdirectory(hdb_new)
#add_subdirectory(hdb_test)
add_subdirectory(encrypttool)
#add_subdirectory(unit_test)
add_subdirectory(bootstrap)
add_subdirectory(DAE_main)

if("${OPERATE_SYSTEM}" STREQUAL  "${OS_aix}")
add_subdirectory(patchxcoff)
add_dependencies(hdb_new hdb_policymgr)
endif()
#target machine 10.23.56.93 admin 123blue!

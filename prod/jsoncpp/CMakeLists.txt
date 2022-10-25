

include_directories(./include)

set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")

aux_source_directory(./src/lib_json SRCS)


add_library(jsoncpp STATIC
         ./src/lib_json/json_reader.cpp
         ./src/lib_json/json_value.cpp
         ./src/lib_json/json_valueiterator.inl
         ./src/lib_json/json_writer.cpp
)


#set(CMAKE_EXE_LINKER_FLAGS " -static")
#target_link_libraries(hdb_new -static-libgcc -static-libstdc++)


#target_link_libraries(hdb_new dl)
#target_link_libraries(hdb_new libsubhook.so)
#target_link_libraries(hdb_new libfunchook.so)
#target_link_libraries(hdb_new tool)
#target_link_libraries(hdb_new libQueryCloudAZSDKCpp.so)
#target_link_libraries(hdb_new empolicymgr)






cmake_minimum_required(VERSION 3.17)

add_executable(MyProject main.cpp ${MY_INCLUDE_DIR}/json.h json.cpp ${MY_INCLUDE_DIR}/json_reader.h json_reader.cpp ${MY_INCLUDE_DIR}/json_input.h json_input.cpp ${MY_INCLUDE_DIR}/json_builder.h json_builder.cpp ${MY_INCLUDE_DIR}/search_server.h search_server.cpp)
target_include_drectories(MyProject PUBLIC ${MY_INCLUDE_DIR})

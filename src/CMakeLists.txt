cmake_minimum_required(VERSION 3.17)

project(search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(search_engine

  src/main.cpp

  src/json.cpp src/json_builder.cpp src/json_input.cpp

  src/json_reader.cpp src/search_server.cpp

)

target_include_directories(search_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)
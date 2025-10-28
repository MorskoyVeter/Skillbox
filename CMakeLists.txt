cmake_minimum_required(VERSION 3.17)
project("MyProject" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(search_engine PRIVATE gtest_main)

 include(GoogleTest)
 gtest_discover_tests(search_engine)
add_subdirectory(src)
include(FetchContent)
 FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)



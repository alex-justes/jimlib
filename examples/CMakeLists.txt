cmake_minimum_required(VERSION 3.3)
project(jimlib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -ffast-math  -funroll-loops -fexpensive-optimizations -Wall")
include_directories(
        ../include
        )
set(SOURCE_FILES main.cpp PngImage.cpp)
add_executable(jimlib ${SOURCE_FILES})
target_link_libraries(jimlib png)
target_compile_definitions(jimlib PUBLIC -DNDEBUG)

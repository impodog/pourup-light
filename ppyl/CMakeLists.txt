cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

project(ppyl_lib)

add_library(ppyl_lib SHARED
        ppyl.h
        format.cpp
        macros.cpp
        work.cpp
        json_work.cpp)

project(ppyl)

link_directories("cmake-build-debug")

add_executable(ppyl main.cpp)

target_link_libraries(ppyl ppyl_lib)


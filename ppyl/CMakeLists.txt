cmake_minimum_required(VERSION 3.26)
project(ppyl)

set(CMAKE_CXX_STANDARD 17)

add_executable(ppyl main.cpp
        ppyl.h
        format.cpp
        work.cpp
)

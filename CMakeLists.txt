cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

project(pourup_light)
add_library(pourup_light SHARED
        src/c-vector/vec.c
        src/c-hashmap/map.c
        src/syntax.c
        src/syntax.h
        src/const.h
        src/item.c
        src/item.h
        src/environ.c
        src/environ.h
        src/reader.c
        src/reader.h
        src/operation.c
        src/operation.h
        src/sys.c
        src/sys.h
)

project(pourup_light_test)
link_directories("./cmake-build-debug")
add_executable(pourup_light_test test.c pupl.h)
target_link_libraries(pourup_light_test pourup_light)
cmake_minimum_required(VERSION 3.18.1)

project("test")

include_directories(include)

add_library( # Sets the name of the library.
        test

        # Sets the library as a shared library.
        SHARED
        src/test.c)
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries(
        test
        ${log-lib}
)
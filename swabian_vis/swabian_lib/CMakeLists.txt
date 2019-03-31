cmake_minimum_required(VERSION 3.13)
project(swabian_lib C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(swabian_lib_exe
        unit_test.c)

add_library(swabian_lib SHARED swabian_lib.c)


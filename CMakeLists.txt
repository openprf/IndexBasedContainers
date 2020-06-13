cmake_minimum_required(VERSION 3.16)
project(Containers C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Containers main.c include/ib_buffer.h include/ib_buffer.c)

add_library(ibconainers SHARED include/ib_buffer.h include/ib_buffer.c)
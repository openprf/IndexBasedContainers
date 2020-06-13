cmake_minimum_required(VERSION 3.16)
project(Containers C)

set(CMAKE_C_STANDARD 11)

set(SOURCES_DIR include)
set(SOURCES
        ${SOURCES_DIR}/ib_buffer.h
        ${SOURCES_DIR}/ib_buffer.c
        ${SOURCES_DIR}/ib_map.h
        ${SOURCES_DIR}/ib_map.c)

include_directories(${SOURCES_DIR})

add_library(ibconainers SHARED ${SOURCES})

add_executable(Tests tests/test.c ${SOURCES} )

add_executable(Examples examples/main.c ${SOURCES})
cmake_minimum_required(VERSION 3.22)
project(Hanoi2)

set(CMAKE_CXX_STANDARD 17)

file(GLOB Assets "./Assets")
file(COPY ${Assets} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/freetype.dll DESTINATION ${CMAKE_BINARY_DIR})

link_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/lib)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/include)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Game)
add_subdirectory(Utils)
add_subdirectory(Render)

add_executable(Hanoi2
    main.cpp
    glad.c
    General.h)

target_link_libraries(Hanoi2 game render util freetype.lib glfw3.lib)

cmake_minimum_required(VERSION 3.19)

project(GOOSE C ASM)
set(CMAKE_BUILD_TYPE DEBUG)

add_subdirectory(libc)
add_subdirectory(kernel)
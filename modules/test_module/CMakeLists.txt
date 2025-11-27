cmake_minimum_required(VERSION 3.19)

project(GOOSE_test_mod C ASM)

set(toolchain ${PROJECT_SOURCE_DIR}/../../toolchain/i686-goose)
set(CMAKE_C_COMPILER ${toolchain}/bin/i686-goose-gcc)

add_library(test_module test_module.c)
target_link_libraries(test_module gcc c)
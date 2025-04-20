cmake_minimum_required(VERSION 3.19)

project(GOOSE_LIBC C ASM)

enable_language(ASM)

set(toolchain ${PROJECT_SOURCE_DIR}/../toolchain/i686-goose)
set(CMAKE_C_COMPILER ${toolchain}/bin/i686-goose-gcc)
set(CMAKE_ASM_COMPILER ${toolchain}/bin/i686-goose-as)
set(CMAKE_AR ${toolchain}/bin/i686-goose-ar)

set(CMAKE_C_FLAGS "-ffreestanding -nostdlib -O2 -Wall -Werror -std=c17")
set(CMAKE_ASM_FLAGS "")

file(GLOB_RECURSE SRCS *.s *.c)
list(FILTER SRCS EXCLUDE REGEX "CMakeFiles")

add_library(c STATIC ${SRCS})
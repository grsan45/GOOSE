cmake_minimum_required(VERSION 3.19)

project(GOOSE C ASM)

enable_language(ASM)
set(toolchain ${PROJECT_SOURCE_DIR}/toolchain/i686-goose)
set(CMAKE_C_COMPILER ${toolchain}/bin/i686-goose-gcc)
set(CMAKE_ASM_COMPILER ${toolchain}/bin/i686-goose-as)

set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Werror -std=c17")
set(CMAKE_ASM_FLAGS "")

file(GLOB SRCS kernel/**/*.c kernel/**/*.s kernel/*.s kernel/*.c)

add_library(font OBJECT IMPORTED GLOBAL)
set_property(TARGET font PROPERTY IMPORTED_OBJECTS ${PROJECT_SOURCE_DIR}/kernel/display/font.o)

add_executable(goose ${PROJECT_SOURCE_DIR}/kernel/display/font.o ${SRCS})
target_link_libraries(goose gcc)
target_link_options(goose PUBLIC -nostdlib -static-libgcc -T ${PROJECT_SOURCE_DIR}/kernel/linker.ld -ffreestanding -O2)
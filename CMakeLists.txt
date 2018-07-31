cmake_minimum_required(VERSION 2.8.12)

project("CHIP-8 Emulator")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
#Test compilation

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

add_subdirectory(test)

add_executable(chip8c chip8c.c)

add_executable(chip8 chip8.c c8Decode.c c8Env.c c8Instructions.c c8IO.c)
target_link_libraries(chip8 SDL2)

enable_testing()
add_test(NAME c8Instructions COMMAND c8Instruction_tests)

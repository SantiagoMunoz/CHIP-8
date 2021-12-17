cmake_minimum_required(VERSION 3.7)

project("CHIP-8 Emulator")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")


add_subdirectory(test)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(chip8c chip8c.c)

add_executable(chip8 chip8.c c8Decode.c c8Env.c c8Instructions.c c8IO.c)
target_link_libraries(chip8 ${SDL2_LIBRARIES})

enable_testing()
add_test(NAME c8Instructions COMMAND c8Instruction_tests)

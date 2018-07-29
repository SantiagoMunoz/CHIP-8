cmake_minimum_required(VERSION 2.8.12)

project("CHIP-8 Emulator")

set(CMAKE_BUILD_TYPE Debug)
add_executable(compiler_test compiler_test.c aux.c)
add_executable(chip8c chip8c.c aux.c)
add_executable(chip8 chip8.c c8Decode.c c8Env.c c8Instructions.c c8IO.c)
target_link_libraries(chip8 SDL2)

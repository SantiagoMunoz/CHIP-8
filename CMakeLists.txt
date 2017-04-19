cmake_minimum_required(VERSION 2.8.12)

project("CHIP-8 Emulator")

set(SMAKE_BUILD_TYPE Debug)

add_executable(chip8 chip8.c)
add_executable(chip8c chip8c.c)


find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

add_executable(c8Instruction_tests c8Instructions_test.c ${CMAKE_SOURCE_DIR}/c8Instructions.c ${CMAKE_SOURCE_DIR}/c8Env.c)
target_link_libraries(c8Instruction_tests ${CHECK_LIBRARIES})
add_executable(compiler_tests compiler_tests.c)

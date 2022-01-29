cmake_minimum_required(VERSION 3.21)
project(80x86 C)

set(CMAKE_C_STANDARD 17)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

include_directories(include/)

add_library(80x86 STATIC src/cpu.c)

add_executable(hexdump src/hexdump.c)

enable_testing()
add_executable(test_cpu tests/test_cpu.c)
target_link_libraries(test_cpu 80x86)
add_test("CPU Registers" test_cpu)
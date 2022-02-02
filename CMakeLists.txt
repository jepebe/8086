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

add_executable(test_add tests/test_add.c)
target_link_libraries(test_add 80x86)
add_test("Testcase ADD" test_add)

add_executable(test_bitwise tests/test_bitwise.c)
target_link_libraries(test_bitwise 80x86)
add_test("Testcase BITWISE" test_bitwise)

add_executable(test_shifts tests/test_shifts.c)
target_link_libraries(test_shifts 80x86)
add_test("Testcase SHIFTS" test_shifts)

add_executable(test_control tests/test_control.c)
target_link_libraries(test_control 80x86)
add_test("Testcase CONTROL" test_control)

add_executable(test_rotate tests/test_rotate.c)
target_link_libraries(test_rotate 80x86)
add_test("Testcase ROTATE" test_rotate)

add_executable(test_sub tests/test_sub.c)
target_link_libraries(test_sub 80x86)
add_test("Testcase SUB" test_sub)

add_executable(test_cmpneg tests/test_cmpneg.c)
target_link_libraries(test_cmpneg 80x86)
add_test("Testcase CMPNEG" test_cmpneg)
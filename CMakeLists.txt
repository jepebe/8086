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

set(SOURCES
        src/addressing.c
        src/cpu.c
        src/disassembly.c
        src/debug.c
        src/machine.c
        src/memory.c
        src/opcodes.c
        src/instructions/support.c
        src/instructions/arithmetic.c
        src/instructions/bcdcnv.c
        src/instructions/bitwise.c
        src/instructions/control.c
        src/instructions/io.c
        src/instructions/jumps.c
        src/instructions/movs.c
        src/instructions/rotates.c
        src/instructions/shifts.c
        src/instructions/stack.c
        src/instructions/strings.c
        )


include_directories(include/)

add_library(80x86 STATIC ${SOURCES})

add_executable(hexdump src/hexdump.c)

enable_testing()

add_executable(test_cpu tests/test_cpu.c)
target_link_libraries(test_cpu 80x86)
add_test("CPU Registers" test_cpu)

add_executable(test_hello_world_bootloader tests/test_hello_world_bootloader.c)
target_link_libraries(test_hello_world_bootloader 80x86)
add_test("CPU Hello World Bootloader" test_hello_world_bootloader)

add_executable(test_pcxtbios tests/test_pcxtbios.c)
target_link_libraries(test_pcxtbios 80x86)
add_test("CPU PC XT BIOS" test_pcxtbios)

add_executable(test_add tests/artlav/test_add.c)
target_link_libraries(test_add 80x86)
add_test("Testcase ADD" test_add)

add_executable(test_bitwise tests/artlav/test_bitwise.c)
target_link_libraries(test_bitwise 80x86)
add_test("Testcase BITWISE" test_bitwise)

add_executable(test_cmpneg tests/artlav/test_cmpneg.c)
target_link_libraries(test_cmpneg 80x86)
add_test("Testcase CMPNEG" test_cmpneg)

add_executable(test_control tests/artlav/test_control.c)
target_link_libraries(test_control 80x86)
add_test("Testcase CONTROL" test_control)

add_executable(test_div tests/artlav/test_div.c)
target_link_libraries(test_div 80x86)
add_test("Testcase DIV" test_div)

add_executable(test_interrupt tests/artlav/test_interrupt.c)
target_link_libraries(test_interrupt 80x86)
add_test("Testcase INTERRUPT" test_interrupt)

add_executable(test_mul tests/artlav/test_mul.c)
target_link_libraries(test_mul 80x86)
add_test("Testcase MUL" test_mul)

add_executable(test_rotate tests/artlav/test_rotate.c)
target_link_libraries(test_rotate 80x86)
add_test("Testcase ROTATE" test_rotate)

add_executable(test_shifts tests/artlav/test_shifts.c)
target_link_libraries(test_shifts 80x86)
add_test("Testcase SHIFTS" test_shifts)

add_executable(test_strings tests/artlav/test_strings.c)
target_link_libraries(test_strings 80x86)
add_test("Testcase STRINGS" test_strings)

add_executable(test_sub tests/artlav/test_sub.c)
target_link_libraries(test_sub 80x86)
add_test("Testcase SUB" test_sub)

add_executable(test_rep tests/artlav/test_rep.c)
target_link_libraries(test_rep 80x86)
add_test("Testcase REP" test_rep)

add_executable(test_segpr tests/artlav/test_segpr.c)
target_link_libraries(test_segpr 80x86)
add_test("Testcase SEGPR" test_segpr)

add_executable(test_jump1 tests/artlav/test_jump1.c)
target_link_libraries(test_jump1 80x86)
add_test("Testcase JUMP1" test_jump1)

add_executable(test_jump2 tests/artlav/test_jump2.c)
target_link_libraries(test_jump2 80x86)
add_test("Testcase JUMP2" test_jump2)

add_executable(test_jmpmov tests/artlav/test_jmpmov.c)
target_link_libraries(test_jmpmov 80x86)
add_test("Testcase JMPMOV" test_jmpmov)

add_executable(test_datatrnf tests/artlav/test_datatrnf.c)
target_link_libraries(test_datatrnf 80x86)
add_test("Testcase DATATRNF" test_datatrnf)

add_executable(test_bcdcnv tests/artlav/test_bcdcnv.c)
target_link_libraries(test_bcdcnv 80x86)
add_test("Testcase BCDCNV" test_bcdcnv)

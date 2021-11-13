cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_memory test_memory.cpp)
target_compile_options(test_memory PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_memory PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_addressing test_addressing.cpp)
target_compile_options(test_addressing PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_addressing PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_move test_in_move.cpp)
target_compile_options(test_in_move PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_move PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_moveq test_in_moveq.cpp)
target_compile_options(test_in_moveq PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_moveq PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_add test_in_add.cpp)
target_compile_options(test_in_add PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_add PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_adda test_in_adda.cpp)
target_compile_options(test_in_adda PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_adda PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_addx test_in_addx.cpp)
target_compile_options(test_in_addx PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_addx PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_addi test_in_addi.cpp)
target_compile_options(test_in_addi PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_addi PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_addq test_in_addq.cpp)
target_compile_options(test_in_addq PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_addq PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_sub test_in_sub.cpp)
target_compile_options(test_in_sub PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_sub PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_suba test_in_suba.cpp)
target_compile_options(test_in_suba PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_suba PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_subx test_in_subx.cpp)
target_compile_options(test_in_subx PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_subx PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_subi test_in_subi.cpp)
target_compile_options(test_in_subi PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_subi PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_subq test_in_subq.cpp)
target_compile_options(test_in_subq PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_subq PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(test_in_nop test_in_nop.cpp)
target_compile_options(test_in_nop PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_in_nop PRIVATE m68k-emu -static-libgcc -static-libstdc++)

include(CTest)
add_test(memory test_memory)
add_test(addressing test_addressing)
add_test(in_move test_in_move)
add_test(in_moveq test_in_moveq)
add_test(in_add test_in_add)
add_test(in_adda test_in_adda)
add_test(in_addx test_in_addx)
add_test(in_addi test_in_addi)
add_test(in_addq test_in_addq)
add_test(in_sub test_in_sub)
add_test(in_suba test_in_suba)
add_test(in_subx test_in_subx)
add_test(in_subi test_in_subi)
add_test(in_subq test_in_subq)
add_test(in_nop test_in_nop)
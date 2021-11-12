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

include(CTest)
add_test(memory test_memory)
add_test(addressing test_addressing)
add_test(in_move test_in_move)
add_test(in_add test_in_add)
add_test(in_adda test_in_adda)
add_test(in_addx test_in_addx)
add_test(in_addi test_in_addi)
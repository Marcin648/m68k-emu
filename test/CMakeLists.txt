cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_memory test_memory.cpp)
target_compile_options(test_memory PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(test_memory PRIVATE m68k-emu -static-libgcc -static-libstdc++)

add_executable(in_move test_in_move.cpp)
target_compile_options(in_move PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(in_move PRIVATE m68k-emu -static-libgcc -static-libstdc++)

include(CTest)
add_test(memory test_memory)
add_test(in_move in_move)
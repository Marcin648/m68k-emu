cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_memory ${SRC} test_memory.cpp)

target_link_libraries(test_memory)

include(CTest)
add_test(memory test_memory)
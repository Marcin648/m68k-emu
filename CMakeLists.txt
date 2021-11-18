cmake_minimum_required(VERSION 3.14)
project(m68k-emu LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/ELFIO EXCLUDE_FROM_ALL)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/registers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu_state.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_functions.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/move.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/moveq.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/lea.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/add.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/adda.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/addx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/addi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/addq.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/sub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/suba.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/subx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/subi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/subq.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/illegal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/nop.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/jmp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/jsr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/rts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/link.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions/unlk.cpp
)

add_library(
    m68k-emu
    ${SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

include_directories(include)

target_compile_options(m68k-emu PRIVATE -Wall -Wextra -Wpedantic -ggdb)
target_link_libraries(m68k-emu PRIVATE elfio -static-libgcc -static-libstdc++)

enable_testing()
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


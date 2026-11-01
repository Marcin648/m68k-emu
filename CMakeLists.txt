cmake_minimum_required(VERSION 3.21)
project(m68k-emu VERSION 0.1.0 LANGUAGES CXX)

option(M68K_BUILD_TESTS "Build m68k-emu tests" ${PROJECT_IS_TOP_LEVEL})
option(M68K_ENABLE_LTO "Enable link-time optimization when supported" ON)

include(FetchContent)
include(CheckIPOSupported)

if(M68K_ENABLE_LTO)
    check_ipo_supported(RESULT M68K_LTO_SUPPORTED OUTPUT M68K_LTO_ERROR LANGUAGES CXX)
    if(NOT M68K_LTO_SUPPORTED)
        message(STATUS "m68k-emu: LTO not supported: ${M68K_LTO_ERROR}")
    endif()
endif()

FetchContent_Declare(
    elfio
    GIT_REPOSITORY https://github.com/serge1/ELFIO.git
    GIT_TAG        Release_3.12
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(elfio)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(m68k-emu ${SOURCES})
add_library(m68k::emu ALIAS m68k-emu)

target_include_directories(m68k-emu PUBLIC include)
target_compile_features(m68k-emu PUBLIC cxx_std_20)
target_compile_options(m68k-emu PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(m68k-emu PRIVATE elfio::elfio -static-libgcc -static-libstdc++)

if(M68K_LTO_SUPPORTED)
    set_property(TARGET m68k-emu PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(M68K_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

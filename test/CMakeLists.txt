FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.16.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(m68k_tests ${TEST_SOURCES})
target_compile_options(m68k_tests PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(m68k_tests PRIVATE M68K_TEST_BINARY_DIR="${CMAKE_CURRENT_SOURCE_DIR}/binary")
target_link_libraries(m68k_tests PRIVATE m68k-emu Catch2::Catch2)

if(M68K_LTO_SUPPORTED)
    set_property(TARGET m68k_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(m68k_tests)

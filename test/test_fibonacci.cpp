#include <catch2/catch_test_macros.hpp>
#include "m68k.hpp"

using namespace M68K;

TEST_CASE("Program: fibonacci", "[program]") {
    CPU cpu = CPU();
    cpu.loadELF(M68K_TEST_BINARY_DIR "/fibonacci.elf");

    while (cpu.state.registers.get(REG_PC, SIZE_LONG) != 0x10058) {
        cpu.step();
    }

    uint32_t data_ptr = 0x4000;
    uint32_t data = cpu.state.memory.get(data_ptr, SIZE_LONG);

    CHECK(data == 6765);
}

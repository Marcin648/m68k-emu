#include <catch2/catch_test_macros.hpp>
#include "m68k.hpp"

using namespace M68K;

TEST_CASE("Program: sha256", "[program]") {
    CPU cpu = CPU();
    cpu.load_elf(M68K_TEST_BINARY_DIR "/sha256.elf");

    uint32_t data_ptr = 0x4000;
    uint32_t done_ptr = data_ptr + 32;

    while (cpu.state.memory.get(done_ptr, SIZE_LONG) == 0) {
        cpu.step();
    }

    const uint8_t expected[32] = {
        0xe5, 0xd6, 0x8b, 0x03, 0x99, 0x61, 0x47, 0xc7,
        0xc3, 0x57, 0x9f, 0xc2, 0x78, 0x98, 0xa0, 0x67,
        0xcd, 0x3f, 0x46, 0x96, 0xea, 0x0a, 0x07, 0x0b,
        0x61, 0xe8, 0x74, 0x0f, 0x9b, 0xc5, 0x02, 0x31,
    };

    for (size_t i = 0; i < 32; i++) {
        CHECK(cpu.state.memory.get(data_ptr + i, SIZE_BYTE) == expected[i]);
    }
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("LINK/UNLK: link A1, -8 then unlk A1", "[instruction][link][unlk]") {
    auto instruction_link = INSTRUCTION::Link::create(0x4E51); // link A1, -8
    CPUState state = CPUState();
    uint32_t return_data;
    
    state.registers.set(REG_USP, SIZE_LONG, MEMORY_SIZE);
    state.registers.set(REG_A1, SIZE_WORD, 0x2000);
    state.memory.set(0x0000, SIZE_WORD, 0x4E51); // opcode
    state.memory.set(0x0002, SIZE_WORD, 0xFFF8); // -8

    SECTION("link A1, -8") {
        instruction_link.get()->execute(state);
        
        return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
        CHECK(return_data == 0x00FFFFFC);

        return_data = state.registers.get(REG_USP, DataSize::SIZE_LONG);
        CHECK(return_data == 0x00FFFFF4);

        SECTION("unlk A1") {
            auto instruction_unlk = INSTRUCTION::Unlk::create(0x4E59); // unlk A1
            instruction_unlk.get()->execute(state);

            return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
            CHECK(return_data == 0x2000);

            return_data = state.registers.get(REG_USP, DataSize::SIZE_LONG);
            CHECK(return_data == MEMORY_SIZE);
        }
    }
}

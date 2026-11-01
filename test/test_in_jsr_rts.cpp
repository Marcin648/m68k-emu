#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("JSR/RTS: jsr (A0) then rts", "[instruction][jsr][rts]") {
    uint32_t return_data;
    CPUState state = CPUState();

    SECTION("jsr (A0)") {
        auto instruction_jsr = INSTRUCTION::Jsr::create(0x4E90); // jsr (A0)

        state.registers.set(REG_USP, SIZE_LONG, MEMORY_SIZE);
        state.registers.set(REG_PC, SIZE_LONG, 0xF000);
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x2000);

        instruction_jsr.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        CHECK(return_data == 0x2000);

        SECTION("rts") {
            auto instruction_rts = INSTRUCTION::Rts::create(0x4E75); // rts

            instruction_rts.get()->execute(state);
            return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
            CHECK(return_data == 0xF002);
        }
    }
}

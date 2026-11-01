#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("OR: or.b D0, D1", "[instruction][or]") {
    auto instruction = INSTRUCTION::Or::create(0x8200);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000080);
    state.registers.set(REG_D1, SIZE_LONG, 0xAAAAAA01);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0xAAAAAA81);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("OR: or.w D1, (A0)", "[instruction][or]") {
    auto instruction = INSTRUCTION::Or::create(0x8350);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_D1, SIZE_LONG, 0x000000F0);
    state.memory.set(0x1000, SIZE_WORD, 0x0F00);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_WORD) == 0x0FF0);
    check_flags(state, false, false, false, false, false);
}

TEST_CASE("ORI: ori.l #$80000000, D0", "[instruction][ori]") {
    auto instruction = INSTRUCTION::Ori::create(0x0080);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0080);
    state.memory.set(2, SIZE_LONG, 0x80000000);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x80000001);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 6);
    check_flags(state, false, true, false, false, false);
}

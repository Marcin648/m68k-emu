#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("AND: and.w D0, D1", "[instruction][and]") {
    auto instruction = INSTRUCTION::And::create(0xC240);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xFF00F0F0);
    state.registers.set(REG_D1, SIZE_LONG, 0x1234FFFF);
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x1234F0F0);
    check_flags(state, true, true, false, false, false);
}

TEST_CASE("AND: and.l D1, (A0) (zero)", "[instruction][and]") {
    auto instruction = INSTRUCTION::And::create(0xC390);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_D1, SIZE_LONG, 0xF0F0F0F0);
    state.memory.set(0x1000, SIZE_LONG, 0x0F0F0F0F);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_LONG) == 0x00000000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("ANDI: andi.b #$0f, D0", "[instruction][andi]") {
    auto instruction = INSTRUCTION::Andi::create(0x0200);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0200);
    state.memory.set(2, SIZE_WORD, 0x000F);
    state.registers.set(REG_D0, SIZE_LONG, 0x123456F0);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x12345600);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    check_flags(state, false, false, true, false, false);
}

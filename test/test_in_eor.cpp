#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("EOR: eor.w D1, D0", "[instruction][eor]") {
    auto instruction = INSTRUCTION::Eor::create(0xB340);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAA0F0F);
    state.registers.set(REG_D1, SIZE_LONG, 0x0000FFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xAAAAF0F0);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("EOR: eor.l D1, D0 (zero)", "[instruction][eor]") {
    auto instruction = INSTRUCTION::Eor::create(0xB380);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x12345678);
    state.registers.set(REG_D1, SIZE_LONG, 0x12345678);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("EORI: eori.b #$ff, D0", "[instruction][eori]") {
    auto instruction = INSTRUCTION::Eori::create(0x0A00);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0A00);
    state.memory.set(2, SIZE_WORD, 0x00FF);
    state.registers.set(REG_D0, SIZE_LONG, 0x1234560F);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x123456F0);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    check_flags(state, false, true, false, false, false);
}

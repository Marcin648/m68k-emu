#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("CMP: cmp.l D0, D1 (borrow)", "[instruction][cmp]") {
    auto instruction = INSTRUCTION::Cmp::create(0xB280);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000000);
    check_flags(state, false, true, false, false, true);
}

TEST_CASE("CMP: cmp.w D0, D1 (overflow)", "[instruction][cmp]") {
    auto instruction = INSTRUCTION::Cmp::create(0xB240);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    state.registers.set(REG_D1, SIZE_LONG, 0x00008000);

    instruction.get()->execute(state);
    check_flags(state, false, false, false, true, false);
}

TEST_CASE("CMPI: cmpi.l #1, D0 (borrow)", "[instruction][cmpi]") {
    auto instruction = INSTRUCTION::Cmpi::create(0x0C80);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0C80);
    state.memory.set(2, SIZE_LONG, 0x00000001);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 6);
    check_flags(state, false, true, false, false, true);
}

TEST_CASE("CMPA: cmpa.w D0, A1 (compares 32 bits)", "[instruction][cmpa]") {
    auto instruction = INSTRUCTION::Cmpa::create(0xB2C0);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(REG_A1, SIZE_LONG, 0x00010000);

    instruction.get()->execute(state);
    check_flags(state, false, false, false, false, false);
}

TEST_CASE("CMPA: cmpa.w D0, A1 (sign-extended source)", "[instruction][cmpa]") {
    auto instruction = INSTRUCTION::Cmpa::create(0xB2C0);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x0000FFFF);
    state.registers.set(REG_A1, SIZE_LONG, 0xFFFFFFFF);

    instruction.get()->execute(state);
    check_flags(state, false, false, true, false, false);
}

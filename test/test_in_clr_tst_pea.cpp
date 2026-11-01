#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("CLR: clr.w D0", "[instruction][clr]") {
    auto instruction = INSTRUCTION::Clr::create(0x4240);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);
    state.registers.set(SR_FLAG_NEGATIVE, true);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xFFFF0000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("CLR: clr.l (A0)", "[instruction][clr]") {
    auto instruction = INSTRUCTION::Clr::create(0x4290);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_LONG, 0x12345678);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_LONG) == 0x00000000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("TST: tst.b D0 (negative)", "[instruction][tst]") {
    auto instruction = INSTRUCTION::Tst::create(0x4A00);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000080);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000080);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("TST: tst.l D0 (zero)", "[instruction][tst]") {
    auto instruction = INSTRUCTION::Tst::create(0x4A80);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("PEA: pea (A0)", "[instruction][pea]") {
    auto instruction = INSTRUCTION::Pea::create(0x4850);
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_A0, SIZE_LONG, 0x1234);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x0FFC);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x1234);
}

TEST_CASE("PEA: pea ($10,A0)", "[instruction][pea]") {
    auto instruction = INSTRUCTION::Pea::create(0x4868);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x4868);
    state.memory.set(2, SIZE_WORD, 0x0010);
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_A0, SIZE_LONG, 0x1234);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x1244);
}

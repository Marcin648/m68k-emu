#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("NOT: not.b D0", "[instruction][not]") {
    auto instruction = INSTRUCTION::Not::create(0x4600);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x123456F0);
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x1234560F);
    check_flags(state, true, false, false, false, false);
}

TEST_CASE("NOT: not.l D0 (zero)", "[instruction][not]") {
    auto instruction = INSTRUCTION::Not::create(0x4680);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("NOT: not.w (A0) (negative)", "[instruction][not]") {
    auto instruction = INSTRUCTION::Not::create(0x4650);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x00FF);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_WORD) == 0xFF00);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("NOT: validity and disassembly", "[instruction][not]") {
    CHECK(INSTRUCTION::Not::create(0x46C0).get()->is_valid == false); // move to sr
    CHECK(INSTRUCTION::Not::create(0x4648).get()->is_valid == false); // not.w A0

    CPUState state = CPUState();
    CHECK(INSTRUCTION::Not::create(0x4650).get()->disassembly(state) == "not.w (a0)");
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("SWAP: swap D1", "[instruction][swap]") {
    auto instruction = INSTRUCTION::Swap::create(0x4841);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0x12345678);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x56781234);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 2);
    check_flags(state, false, false, false, false, false);
}

TEST_CASE("SWAP: swap D1 (negative)", "[instruction][swap]") {
    auto instruction = INSTRUCTION::Swap::create(0x4841);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0x00008000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x80000000);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("SWAP: swap D1 (zero)", "[instruction][swap]") {
    auto instruction = INSTRUCTION::Swap::create(0x4841);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000000);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("SWAP: disassembly", "[instruction][swap]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Swap::create(0x4841).get()->disassembly(state) == "swap d1");
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;
TEST_CASE("SBCD: sbcd D0, D1", "[instruction][sbcd]") {
    auto instruction = INSTRUCTION::Sbcd::create(0x8300);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000038);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000045);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000007);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == false);
    CHECK(state.registers.get(SR_FLAG_CARRY) == false);
}

TEST_CASE("SBCD: sbcd D0, D1 (borrow)", "[instruction][sbcd]") {
    auto instruction = INSTRUCTION::Sbcd::create(0x8300);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000099);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == true);
    CHECK(state.registers.get(SR_FLAG_CARRY) == true);
}

TEST_CASE("SBCD: sbcd -(A0), -(A1) (extend)", "[instruction][sbcd]") {
    auto instruction = INSTRUCTION::Sbcd::create(0x8308);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1001);
    state.registers.set(REG_A1, SIZE_LONG, 0x2001);
    state.memory.set(0x1000, SIZE_BYTE, 0x00);
    state.memory.set(0x2000, SIZE_BYTE, 0x10);
    state.registers.set(SR_FLAG_EXTEND, true);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x2000, SIZE_BYTE) == 0x09);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == false);
}

TEST_CASE("SBCD: disassembly", "[instruction][sbcd]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Sbcd::create(0x8300).get()->disassembly(state) == "sbcd d0, d1");
}

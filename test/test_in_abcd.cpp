#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;
TEST_CASE("ABCD: abcd D0, D1", "[instruction][abcd]") {
    auto instruction = INSTRUCTION::Abcd::create(0xC300);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000038);
    state.registers.set(REG_D1, SIZE_LONG, 0xAAAAAA45);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0xAAAAAA83);
    CHECK(state.registers.get(SR_FLAG_ZERO) == false);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == false);
    CHECK(state.registers.get(SR_FLAG_CARRY) == false);
}

TEST_CASE("ABCD: abcd D0, D1 (carry, zero keeps Z)", "[instruction][abcd]") {
    auto instruction = INSTRUCTION::Abcd::create(0xC300);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000099);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000000);
    CHECK(state.registers.get(SR_FLAG_ZERO) == true);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == true);
    CHECK(state.registers.get(SR_FLAG_CARRY) == true);
}

TEST_CASE("ABCD: abcd D0, D1 (extend)", "[instruction][abcd]") {
    auto instruction = INSTRUCTION::Abcd::create(0xC300);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000009);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000010);
    state.registers.set(SR_FLAG_EXTEND, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000020);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == false);
}

TEST_CASE("ABCD: abcd -(A0), -(A1)", "[instruction][abcd]") {
    auto instruction = INSTRUCTION::Abcd::create(0xC308);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1001);
    state.registers.set(REG_A1, SIZE_LONG, 0x2001);
    state.memory.set(0x1000, SIZE_BYTE, 0x25);
    state.memory.set(0x2000, SIZE_BYTE, 0x17);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x2000, SIZE_BYTE) == 0x42);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x1000);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x2000);
}

TEST_CASE("ABCD: disassembly", "[instruction][abcd]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Abcd::create(0xC308).get()->disassembly(state) == "abcd -(a0), -(a1)");
}

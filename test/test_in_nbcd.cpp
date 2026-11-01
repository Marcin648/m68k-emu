#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;
TEST_CASE("NBCD: nbcd D0", "[instruction][nbcd]") {
    auto instruction = INSTRUCTION::Nbcd::create(0x4800);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000099);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == true);
    CHECK(state.registers.get(SR_FLAG_CARRY) == true);
}

TEST_CASE("NBCD: nbcd D0 (zero keeps Z)", "[instruction][nbcd]") {
    auto instruction = INSTRUCTION::Nbcd::create(0x4800);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(SR_FLAG_ZERO, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    CHECK(state.registers.get(SR_FLAG_ZERO) == true);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == false);
    CHECK(state.registers.get(SR_FLAG_CARRY) == false);
}

TEST_CASE("NBCD: nbcd (A0) (extend)", "[instruction][nbcd]") {
    auto instruction = INSTRUCTION::Nbcd::create(0x4810);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_BYTE, 0x10);
    state.registers.set(SR_FLAG_EXTEND, true);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_BYTE) == 0x89);
    CHECK(state.registers.get(SR_FLAG_EXTEND) == true);
}

TEST_CASE("NBCD: validity and disassembly", "[instruction][nbcd]") {
    CHECK(INSTRUCTION::Nbcd::create(0x4808).get()->is_valid == false); // nbcd A0
    CHECK(INSTRUCTION::Nbcd::create(0x483A).get()->is_valid == false); // nbcd d16(PC)

    CPUState state = CPUState();
    CHECK(INSTRUCTION::Nbcd::create(0x4800).get()->disassembly(state) == "nbcd d0");
}

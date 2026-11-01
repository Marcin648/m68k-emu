#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("CMPM: cmpm.b (A0)+, (A1)+ (equal)", "[instruction][cmpm]") {
    auto instruction = INSTRUCTION::Cmpm::create(0xB308);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_A1, SIZE_LONG, 0x2000);
    state.memory.set(0x1000, SIZE_BYTE, 0x05);
    state.memory.set(0x2000, SIZE_BYTE, 0x05);
    state.registers.set(SR_FLAG_EXTEND, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x1001);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x2001);
    check_flags(state, true, false, true, false, false);
}

TEST_CASE("CMPM: cmpm.w (A0)+, (A1)+ (borrow)", "[instruction][cmpm]") {
    auto instruction = INSTRUCTION::Cmpm::create(0xB348);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_A1, SIZE_LONG, 0x2000);
    state.memory.set(0x1000, SIZE_WORD, 0x0001);
    state.memory.set(0x2000, SIZE_WORD, 0x0000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x1002);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x2002);
    check_flags(state, false, true, false, false, true);
}

TEST_CASE("CMPM: cmpm.l (A0)+, (A1)+ (overflow)", "[instruction][cmpm]") {
    auto instruction = INSTRUCTION::Cmpm::create(0xB388);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_A1, SIZE_LONG, 0x2000);
    state.memory.set(0x1000, SIZE_LONG, 0x00000001);
    state.memory.set(0x2000, SIZE_LONG, 0x80000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x1004);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x2004);
    CHECK(state.memory.get(0x2000, SIZE_LONG) == 0x80000000);
    check_flags(state, false, false, false, true, false);
}

TEST_CASE("CMPM: validity and disassembly", "[instruction][cmpm]") {
    CHECK(INSTRUCTION::Cmpm::create(0xB3C8).get()->is_valid == false); // cmpa.l A0, A1

    CPUState state = CPUState();
    CHECK(INSTRUCTION::Cmpm::create(0xB308).get()->disassembly(state) == "cmpm.b (a0)+, (a1)+");
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("BitShift: lsl.b #1, D0", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE308); // lsl.b #1, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 0x12345681);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x12345602);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 2);
    checkFlags(state, true, false, false, false, true);
}

TEST_CASE("BitShift: lsr.w #8, D1 (count 0 means 8)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE049); // lsr.w #8, D1
    CPUState state = CPUState();

    state.registers.set(REG_D1, SIZE_LONG, 0x1111ABCD);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x111100AB);
    checkFlags(state, true, false, false, false, true);
}

TEST_CASE("BitShift: asr.b #2, D0", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE400); // asr.b #2, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 0x00000082);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x000000E0);
    checkFlags(state, true, true, false, false, true);
}

TEST_CASE("BitShift: asl.b #1, D0 (overflow)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE300); // asl.b #1, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 0x00000040);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000080);
    checkFlags(state, false, true, false, true, false);
}

TEST_CASE("BitShift: asl.l D1, D0", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE3A0); // asl.l D1, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 0x10000001);
    state.registers.set(REG_D1, SIZE_LONG, 4);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000010);
    checkFlags(state, true, false, false, true, true);
}

TEST_CASE("BitShift: rol.w #1, D0 (X not affected)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE358); // rol.w #1, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 0x00008001);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000003);
    checkFlags(state, false, false, false, false, true);
}

TEST_CASE("BitShift: ror.l #4, D2", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE89A); // ror.l #4, D2
    CPUState state = CPUState();

    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(REG_D2, SIZE_LONG, 0x12345678);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D2, SIZE_LONG) == 0x81234567);
    checkFlags(state, true, true, false, false, true);
}

TEST_CASE("BitShift: roxl.b #1, D0 (X=1)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE310); // roxl.b #1, D0
    CPUState state = CPUState();

    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000080);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000001);
    checkFlags(state, true, false, false, false, true);
}

TEST_CASE("BitShift: roxr.w #1, D0 (uses X, not C)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE250); // roxr.w #1, D0
    CPUState state = CPUState();

    state.registers.set(SR_FLAG_EXTEND, false);
    state.registers.set(SR_FLAG_CARRY, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    checkFlags(state, true, false, true, false, true);
}

TEST_CASE("BitShift: roxl.l D1, D0 (count 0, C = X)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE3B0); // roxl.l D1, D0
    CPUState state = CPUState();

    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000005);
    state.registers.set(REG_D1, SIZE_LONG, 0);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000005);
    checkFlags(state, true, false, false, false, true);
}

TEST_CASE("BitShift: lsl.l D1, D0 (count 0)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE3A8); // lsl.l D1, D0
    CPUState state = CPUState();

    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(SR_FLAG_CARRY, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x80000000);
    state.registers.set(REG_D1, SIZE_LONG, 0);
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x80000000);
    checkFlags(state, true, true, false, false, false);
}

TEST_CASE("BitShift: lsr.l D1, D0 (count modulo 64)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE2A8); // lsr.l D1, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);
    state.registers.set(REG_D1, SIZE_LONG, 96); // 96 % 64 = 32
    instruction.get()->execute(state);

    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    checkFlags(state, true, false, true, false, true);
}

TEST_CASE("BitShift: lsr.w (A0)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE2D0); // lsr.w (A0)
    CPUState state = CPUState();

    state.registers.set(REG_A0, SIZE_LONG, 0x2000);
    state.memory.set(0x2000, SIZE_WORD, 0x8001);
    instruction.get()->execute(state);

    CHECK(state.memory.get(0x2000, SIZE_WORD) == 0x4000);
    checkFlags(state, true, false, false, false, true);
}

TEST_CASE("BitShift: asl.w 4(A0)", "[instruction][bit_shift]") {
    auto instruction = INSTRUCTION::BitShift::create(0xE1E8); // asl.w 4(A0)
    CPUState state = CPUState();

    state.memory.set(0, SIZE_WORD, 0xE1E8); // opcode
    state.memory.set(2, SIZE_WORD, 0x0004); // displacement
    state.registers.set(REG_A0, SIZE_LONG, 0x2000);
    state.memory.set(0x2004, SIZE_WORD, 0x4000);
    instruction.get()->execute(state);

    CHECK(state.memory.get(0x2004, SIZE_WORD) == 0x8000);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    checkFlags(state, false, true, false, true, false);
}

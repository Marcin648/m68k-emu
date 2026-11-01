#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("MOVEP: movep.l D0, ($2,A0)", "[instruction][movep]") {
    auto instruction = INSTRUCTION::Movep::create(0x01C8);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x01C8);
    state.memory.set(2, SIZE_WORD, 0x0002);
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, SIZE_LONG, 0x11223344);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1002, SIZE_LONG) == 0x11002200);
    CHECK(state.memory.get(0x1006, SIZE_LONG) == 0x33004400);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
}

TEST_CASE("MOVEP: movep.w D0, ($0,A0)", "[instruction][movep]") {
    auto instruction = INSTRUCTION::Movep::create(0x0188);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0188);
    state.memory.set(2, SIZE_WORD, 0x0000);
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, SIZE_LONG, 0x0000AABB);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_LONG) == 0xAA00BB00);
}

TEST_CASE("MOVEP: movep.l ($0,A0), D1", "[instruction][movep]") {
    auto instruction = INSTRUCTION::Movep::create(0x0348);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0348);
    state.memory.set(2, SIZE_WORD, 0x0000);
    state.memory.set(0x1000, SIZE_LONG, 0x12FF34FF);
    state.memory.set(0x1004, SIZE_LONG, 0x56FF78FF);
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x12345678);
}

TEST_CASE("MOVEP: movep.w ($0,A0), D1", "[instruction][movep]") {
    auto instruction = INSTRUCTION::Movep::create(0x0308);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0308);
    state.memory.set(2, SIZE_WORD, 0x0000);
    state.memory.set(0x1000, SIZE_LONG, 0x12FF34FF);
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);
    state.registers.set(REG_D1, SIZE_LONG, 0xFFFF0000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0xFFFF1234);
}

TEST_CASE("MOVEP: disassembly", "[instruction][movep]") {
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0x0002);
    CHECK(INSTRUCTION::Movep::create(0x01C8).get()->disassembly(state) == "movep.l d0, ($2,a0)");

    state.registers.set(REG_PC, SIZE_LONG, 0);
    CHECK(INSTRUCTION::Movep::create(0x0348).get()->disassembly(state) == "movep.l ($2,a0), d1");
}

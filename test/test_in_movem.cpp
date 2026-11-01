#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("MOVEM: movem.l d0/d1/a0, -(a7) then movem.l (a7)+, d0/d1/a0", "[instruction][movem]") {
    auto instruction_push = INSTRUCTION::Movem::create(0x48E7);
    auto instruction_pop = INSTRUCTION::Movem::create(0x4CDF);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x48E7);
    state.memory.set(2, SIZE_WORD, 0xC080);
    state.memory.set(4, SIZE_WORD, 0x4CDF);
    state.memory.set(6, SIZE_WORD, 0x0103);
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, SIZE_LONG, 0x11111111);
    state.registers.set(REG_D1, SIZE_LONG, 0x22222222);
    state.registers.set(REG_A0, SIZE_LONG, 0x33333333);

    instruction_push.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x0FF4);
    CHECK(state.memory.get(0x0FF4, SIZE_LONG) == 0x11111111);
    CHECK(state.memory.get(0x0FF8, SIZE_LONG) == 0x22222222);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x33333333);

    state.registers.set(REG_D0, SIZE_LONG, 0);
    state.registers.set(REG_D1, SIZE_LONG, 0);
    state.registers.set(REG_A0, SIZE_LONG, 0);

    instruction_pop.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 8);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x11111111);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x22222222);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x33333333);
}

TEST_CASE("MOVEM: movem.w (a0), d0/a1 (sign extend)", "[instruction][movem]") {
    auto instruction = INSTRUCTION::Movem::create(0x4C90);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x4C90);
    state.memory.set(2, SIZE_WORD, 0x0201);
    state.memory.set(0x2000, SIZE_WORD, 0x8000);
    state.memory.set(0x2002, SIZE_WORD, 0x1234);
    state.registers.set(REG_A0, SIZE_LONG, 0x2000);
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAAAAAA);
    state.registers.set(REG_A1, SIZE_LONG, 0xAAAAAAAA);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xFFFF8000);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x00001234);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x2000);
}

TEST_CASE("MOVEM: movem.w d0/d1, (a0)", "[instruction][movem]") {
    auto instruction = INSTRUCTION::Movem::create(0x4890);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x4890);
    state.memory.set(2, SIZE_WORD, 0x0003);
    state.registers.set(REG_A0, SIZE_LONG, 0x2000);
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAA1111);
    state.registers.set(REG_D1, SIZE_LONG, 0xBBBB2222);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x2000, SIZE_WORD) == 0x1111);
    CHECK(state.memory.get(0x2002, SIZE_WORD) == 0x2222);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x2000);
}

TEST_CASE("MOVEM: movem.l d0, ($3000).w", "[instruction][movem]") {
    auto instruction = INSTRUCTION::Movem::create(0x48F8);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x48F8);
    state.memory.set(2, SIZE_WORD, 0x0001);
    state.memory.set(4, SIZE_WORD, 0x3000);
    state.registers.set(REG_D0, SIZE_LONG, 0x12345678);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x3000, SIZE_LONG) == 0x12345678);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 6);
}

TEST_CASE("MOVEM: validity and disassembly", "[instruction][movem]") {
    CHECK(INSTRUCTION::Movem::create(0x48D8).get()->is_valid == false); // movem.l <list>, (A0)+
    CHECK(INSTRUCTION::Movem::create(0x4CE0).get()->is_valid == false); // movem.l -(A0), <list>
    CHECK(INSTRUCTION::Movem::create(0x48FA).get()->is_valid == false); // movem.l <list>, d16(PC)
    CHECK(INSTRUCTION::Movem::create(0x4CC0).get()->is_valid == false); // movem.l D0, <list>
    CHECK(INSTRUCTION::Movem::create(0x4CFA).get()->is_valid == true); // movem.l d16(PC), <list>

    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0xC080);
    CHECK(INSTRUCTION::Movem::create(0x48E7).get()->disassembly(state) == "movem.l d0/d1/a0, -(a7)");

    state.registers.set(REG_PC, SIZE_LONG, 0);
    state.memory.set(2, SIZE_WORD, 0x0103);
    CHECK(INSTRUCTION::Movem::create(0x4CDF).get()->disassembly(state) == "movem.l (a7)+, d0/d1/a0");
}

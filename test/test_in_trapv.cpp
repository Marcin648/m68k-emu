#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("TRAPV: overflow set", "[instruction][trapv]") {
    auto instruction = INSTRUCTION::Trapv::create(0x4E76);
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_PC, SIZE_LONG, 0x600);
    state.registers.set(SR_FLAG_OVERFLOW, true);
    state.memory.set(VECTOR_TRAPV * 4, SIZE_LONG, 0x3000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.memory.get(0x0FFA, SIZE_WORD) == 0x2702);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 0x602);
}

TEST_CASE("TRAPV: overflow clear", "[instruction][trapv]") {
    auto instruction = INSTRUCTION::Trapv::create(0x4E76);
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_PC, SIZE_LONG, 0x600);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x602);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
}

TEST_CASE("TRAPV: disassembly", "[instruction][trapv]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Trapv::create(0x4E76).get()->disassembly(state) == "trapv");
}

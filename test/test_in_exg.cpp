#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("EXG: exg D1, D2", "[instruction][exg]") {
    auto instruction = INSTRUCTION::Exg::create(0xC342);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0x11111111);
    state.registers.set(REG_D2, SIZE_LONG, 0x22222222);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x22222222);
    CHECK(state.registers.get(REG_D2, SIZE_LONG) == 0x11111111);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 2);
}

TEST_CASE("EXG: exg A1, A2", "[instruction][exg]") {
    auto instruction = INSTRUCTION::Exg::create(0xC34A);
    CPUState state = CPUState();
    state.registers.set(REG_A1, SIZE_LONG, 0x11111111);
    state.registers.set(REG_A2, SIZE_LONG, 0x22222222);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x22222222);
    CHECK(state.registers.get(REG_A2, SIZE_LONG) == 0x11111111);
}

TEST_CASE("EXG: exg D1, A2", "[instruction][exg]") {
    auto instruction = INSTRUCTION::Exg::create(0xC38A);
    CPUState state = CPUState();
    state.registers.set(REG_D1, SIZE_LONG, 0x11111111);
    state.registers.set(REG_A2, SIZE_LONG, 0x22222222);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x22222222);
    CHECK(state.registers.get(REG_A2, SIZE_LONG) == 0x11111111);
}

TEST_CASE("EXG: validity and disassembly", "[instruction][exg]") {
    CHECK(INSTRUCTION::Exg::create(0xC382).get()->is_valid == false); // opmode 10000
    CHECK(INSTRUCTION::Exg::create(0xC300).get()->is_valid == false); // abcd D0, D1

    CPUState state = CPUState();
    CHECK(INSTRUCTION::Exg::create(0xC38A).get()->disassembly(state) == "exg d1, a2");
}

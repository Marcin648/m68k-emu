#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;
TEST_CASE("MOVE from SR: move.w sr, D0", "[instruction][move_from_sr]") {
    auto instruction = INSTRUCTION::MoveFromSR::create(0x40C0);
    CPUState state = CPUState();
    state.registers.set(REG_SR, SIZE_WORD, 0x2715);
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAAAAAA);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xAAAA2715);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2715);
}

TEST_CASE("MOVE from SR: move.w sr, (A0)", "[instruction][move_from_sr]") {
    auto instruction = INSTRUCTION::MoveFromSR::create(0x40D0);
    CPUState state = CPUState();
    state.registers.set(REG_SR, SIZE_WORD, 0x2715);
    state.registers.set(REG_A0, SIZE_LONG, 0x1000);

    instruction.get()->execute(state);
    CHECK(state.memory.get(0x1000, SIZE_WORD) == 0x2715);
}

TEST_CASE("MOVE from SR: validity and disassembly", "[instruction][move_from_sr]") {
    CHECK(INSTRUCTION::MoveFromSR::create(0x40C8).get()->is_valid == false); // move.w sr, A0
    CHECK(INSTRUCTION::MoveFromSR::create(0x40FA).get()->is_valid == false); // move.w sr, d16(PC)

    CPUState state = CPUState();
    CHECK(INSTRUCTION::MoveFromSR::create(0x40C1).get()->disassembly(state) == "move.w sr, d1");
}

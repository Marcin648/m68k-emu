#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;
TEST_CASE("MOVE to CCR: move.w D0, ccr", "[instruction][move_to_ccr]") {
    auto instruction = INSTRUCTION::MoveToCCR::create(0x44C0);
    CPUState state = CPUState();
    state.registers.set(REG_SR, SIZE_WORD, 0x2700);
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x271F);
    check_flags(state, true, true, true, true, true);
}

TEST_CASE("MOVE to CCR: move.w #$0004, ccr", "[instruction][move_to_ccr]") {
    auto instruction = INSTRUCTION::MoveToCCR::create(0x44FC);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x44FC);
    state.memory.set(2, SIZE_WORD, 0x0004);
    state.registers.set(REG_SR, SIZE_WORD, 0x001F);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("MOVE to CCR: validity and disassembly", "[instruction][move_to_ccr]") {
    CHECK(INSTRUCTION::MoveToCCR::create(0x44C8).get()->is_valid == false); // move.w A0, ccr
    CHECK(INSTRUCTION::MoveToCCR::create(0x44FA).get()->is_valid == true); // move.w d16(PC), ccr

    CPUState state = CPUState();
    CHECK(INSTRUCTION::MoveToCCR::create(0x44C0).get()->disassembly(state) == "move.w d0, ccr");
}

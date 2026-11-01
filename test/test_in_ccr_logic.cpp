#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("ORI to CCR: ori #$1f, ccr", "[instruction][ccr_logic]") {
    auto instruction = INSTRUCTION::CcrLogic::create(0x003C);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x003C);
    state.memory.set(2, SIZE_WORD, 0x00FF);
    state.registers.set(REG_SR, SIZE_WORD, 0x2700);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x271F);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    check_flags(state, true, true, true, true, true);
}

TEST_CASE("ANDI to CCR: andi #$04, ccr", "[instruction][ccr_logic]") {
    auto instruction = INSTRUCTION::CcrLogic::create(0x023C);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x023C);
    state.memory.set(2, SIZE_WORD, 0x0004);
    state.registers.set(REG_SR, SIZE_WORD, 0x271F);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2704);
    check_flags(state, false, false, true, false, false);
}

TEST_CASE("EORI to CCR: eori #$ff, ccr", "[instruction][ccr_logic]") {
    auto instruction = INSTRUCTION::CcrLogic::create(0x0A3C);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0A3C);
    state.memory.set(2, SIZE_WORD, 0x00FF);
    state.registers.set(REG_SR, SIZE_WORD, 0x0005);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x001A);
    check_flags(state, true, true, false, true, false);
}

TEST_CASE("CCR logic: validity and disassembly", "[instruction][ccr_logic]") {
    CHECK(INSTRUCTION::CcrLogic::create(0x043C).get()->is_valid == false);

    auto instruction = INSTRUCTION::CcrLogic::create(0x003C);
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0x001F);
    CHECK(instruction.get()->disassembly(state) == "ori #$1f, ccr");
}

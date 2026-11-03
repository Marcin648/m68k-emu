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

TEST_CASE("ORI to SR: ori #$8000, sr", "[instruction][ccr_logic]") {
    auto instruction = INSTRUCTION::CcrLogic::create(0x007C);
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0x8000);
    state.registers.set(REG_SR, SIZE_WORD, 0x2704);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0xA704);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
}

TEST_CASE("ANDI to SR: andi #$d8ff, sr (switch to user mode)", "[instruction][ccr_logic]") {
    auto instruction = INSTRUCTION::CcrLogic::create(0x027C);
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0xD8FF);
    state.registers.set(REG_SR, SIZE_WORD, 0x2715);
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set_usp(0x3000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x0015);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x3000);
    CHECK(state.registers.get_ssp() == 0x1000);
}

TEST_CASE("EORI to SR: eori #$0701, sr", "[instruction][ccr_logic]") {
    auto instruction = INSTRUCTION::CcrLogic::create(0x0A7C);
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0x0701);
    state.registers.set(REG_SR, SIZE_WORD, 0x2300);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_SR, SIZE_WORD) == 0x2401);
}

TEST_CASE("SR logic: privilege violation", "[instruction][ccr_logic]") {
    check_privilege_violation(0x007C);
    check_privilege_violation(0x027C);
    check_privilege_violation(0x0A7C);
}

TEST_CASE("SR logic: disassembly", "[instruction][ccr_logic]") {
    CPUState state = CPUState();
    state.memory.set(2, SIZE_WORD, 0x2700);
    CHECK(INSTRUCTION::CcrLogic::create(0x027C).get()->disassembly(state) == "andi #$2700, sr");
}

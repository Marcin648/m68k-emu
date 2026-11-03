#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

static CPUState chk_state(uint16_t value) {
    CPUState state = CPUState();
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, SIZE_LONG, 0xAAAA0000 | value);
    state.memory.set(0, SIZE_WORD, 0x41BC);
    state.memory.set(2, SIZE_WORD, 10);
    state.memory.set(VECTOR_CHK * 4, SIZE_LONG, 0x3000);
    return state;
}

TEST_CASE("CHK: chk.w #10, D0 (in bounds)", "[instruction][chk]") {
    auto instruction = INSTRUCTION::Chk::create(0x41BC);
    CPUState state = chk_state(10);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    CHECK(state.registers.get(REG_A7, SIZE_LONG) == 0x1000);
}

TEST_CASE("CHK: chk.w #10, D0 (negative)", "[instruction][chk]") {
    auto instruction = INSTRUCTION::Chk::create(0x41BC);
    CPUState state = chk_state(0xFFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 4);
    CHECK(state.registers.get(SR_FLAG_NEGATIVE) == true);
}

TEST_CASE("CHK: chk.w #10, D0 (above bound)", "[instruction][chk]") {
    auto instruction = INSTRUCTION::Chk::create(0x41BC);
    CPUState state = chk_state(11);
    state.registers.set(SR_FLAG_NEGATIVE, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 4);
    CHECK(state.registers.get(SR_FLAG_NEGATIVE) == false);
}

TEST_CASE("CHK: validity", "[instruction][chk]") {
    CHECK(INSTRUCTION::Chk::create(0x4181).get()->is_valid == true); // chk.w D1, D0
    CHECK(INSTRUCTION::Chk::create(0x4188).get()->is_valid == false); // chk.w A0, D0
}

TEST_CASE("CHK: disassembly", "[instruction][chk]") {
    CPUState state = CPUState();
    CHECK(INSTRUCTION::Chk::create(0x4181).get()->disassembly(state) == "chk.w d1, d0");
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("ADD: add.w #$CCDD, D1", "[instruction][add]") {
    auto instruction = INSTRUCTION::Add::create(0xD27C); // add.w $AABBCCDD, D1
    CPUState state = CPUState();

    state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x11111111);
    state.memory.set(0, DataSize::SIZE_WORD, 0xD27C); // opcode
    state.memory.set(2, DataSize::SIZE_WORD, 0xCCDD); // value to add

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == 0x1111DDEE);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == false);
    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

TEST_CASE("ADD: add.w (A1), D0", "[instruction][add]") {
    auto instruction = INSTRUCTION::Add::create(0xD051); // add.w (A1), D0
    CPUState state = CPUState();

    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x0001);
    state.memory.set(0x1000, DataSize::SIZE_WORD, 0x7FFF);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG);
    CHECK(return_data == 0x00008000);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == false);
    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == true);
    CHECK(flag_carry == false);
}

TEST_CASE("ADD: add.w D0, (A1)", "[instruction][add]") {
    auto instruction = INSTRUCTION::Add::create(0xD151); // add.w D0, (A1)
    CPUState state = CPUState();

    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x0001);
    state.memory.set(0x1000, DataSize::SIZE_WORD, 0x7FFF);

    instruction.get()->execute(state);
    uint32_t return_data = state.memory.get(0x1000, DataSize::SIZE_WORD);
    CHECK(return_data == 0x00008000);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == false);
    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == true);
    CHECK(flag_carry == false);
}

TEST_CASE("ADD: add.l D0, D1 (carry)", "[instruction][add]") {
    auto instruction = INSTRUCTION::Add::create(0xD280);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000001);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000000);
    checkFlags(state, true, false, true, false, true);
}

TEST_CASE("ADD: add.l D0, D1 (overflow)", "[instruction][add]") {
    auto instruction = INSTRUCTION::Add::create(0xD280);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x7FFFFFFF);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000001);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x80000000);
    checkFlags(state, false, true, false, true, false);
}

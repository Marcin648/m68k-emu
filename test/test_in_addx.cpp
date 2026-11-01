#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("ADDX: addx.w D0, D1", "[instruction][addx]") {
    auto instruction = INSTRUCTION::Addx::create(0xD340); // addx.w D0, D1
    CPUState state = CPUState();

    uint32_t return_data;

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xFFFF);
    state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x0001);
    instruction.get()->execute(state);
    return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
    CHECK(return_data == 0x0000);

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xFFFF);
    state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x0001);
    instruction.get()->execute(state);
    return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
    CHECK(return_data == 0x0001);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == true);
    CHECK(flag_negative == false);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == true);
}

TEST_CASE("ADDX: addx.w -(A0), -(A1)", "[instruction][addx]") {
    auto instruction = INSTRUCTION::Addx::create(0xD348); // addx.w -(A0), -(A1)
    CPUState state = CPUState();

    uint32_t return_data;

    state.memory.set(0x1002, SIZE_WORD, 0xFFFF);
    state.memory.set(0x1000, SIZE_WORD, 0xFFFF);
    state.memory.set(0x2002, SIZE_WORD, 0x0001);
    state.memory.set(0x2000, SIZE_WORD, 0x0001);
    state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1004);
    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x2004);

    instruction.get()->execute(state);
    return_data = state.memory.get(0x2002, SIZE_WORD);
    CHECK(return_data == 0x0000);

    state.memory.set(0x2002, SIZE_WORD, 0x0001);
    instruction.get()->execute(state);
    return_data = state.memory.get(0x2000, SIZE_WORD);
    CHECK(return_data == 0x0001);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == true);
    CHECK(flag_negative == false);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == true);
}

TEST_CASE("ADDX: addx.b D0, D1 (overflow with X)", "[instruction][addx]") {
    auto instruction = INSTRUCTION::Addx::create(0xD300);
    CPUState state = CPUState();
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(REG_D1, SIZE_LONG, 0x0000007F);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000080);
    check_flags(state, false, true, false, true, false);
}

TEST_CASE("ADDX: addx.l D0, D1 (carry, Z unchanged)", "[instruction][addx]") {
    auto instruction = INSTRUCTION::Addx::create(0xD380);
    CPUState state = CPUState();
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(SR_FLAG_ZERO, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(REG_D1, SIZE_LONG, 0xFFFFFFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x00000000);
    check_flags(state, true, false, true, false, true);
}

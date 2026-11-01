#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("SUBX: subx.w D0, D1", "[instruction][subx]") {
    auto instruction = INSTRUCTION::Subx::create(0x9340); // subx.w D0, D1
    CPUState state = CPUState();

    uint32_t return_data;

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x0001);
    state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x0000);
    instruction.get()->execute(state);
    return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
    CHECK(return_data == 0xFFFF);

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x0001);
    state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x0000);
    instruction.get()->execute(state);
    return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
    CHECK(return_data == 0xFFFE);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == true);
    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == true);
}

TEST_CASE("SUBX: subx.w -(A0), -(A1)", "[instruction][subx]") {
    auto instruction = INSTRUCTION::Subx::create(0x9348); // subx.w -(A0), -(A1)
    CPUState state = CPUState();

    uint32_t return_data;

    state.memory.set(0x1002, SIZE_WORD, 0x0001);
    state.memory.set(0x1000, SIZE_WORD, 0x0001);
    state.memory.set(0x2002, SIZE_WORD, 0x0000);
    state.memory.set(0x2000, SIZE_WORD, 0x0000);
    state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1004);
    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x2004);

    instruction.get()->execute(state);
    return_data = state.memory.get(0x2002, SIZE_WORD);
    CHECK(return_data == 0xFFFF);

    state.memory.set(0x2002, SIZE_WORD, 0x0001);
    instruction.get()->execute(state);
    return_data = state.memory.get(0x2000, SIZE_WORD);
    CHECK(return_data == 0xFFFE);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == true);
    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == true);
}

TEST_CASE("SUBX: subx.l D0, D1 (borrow)", "[instruction][subx]") {
    auto instruction = INSTRUCTION::Subx::create(0x9380);
    CPUState state = CPUState();
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0xFFFFFFFF);
    checkFlags(state, true, true, false, false, true);
}

TEST_CASE("SUBX: subx.b D0, D1 (overflow with X)", "[instruction][subx]") {
    auto instruction = INSTRUCTION::Subx::create(0x9300);
    CPUState state = CPUState();
    state.registers.set(SR_FLAG_EXTEND, true);
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);
    state.registers.set(REG_D1, SIZE_LONG, 0x00000080);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x0000007F);
    checkFlags(state, false, false, false, true, false);
}

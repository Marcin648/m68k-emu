#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("MOVE: move.w A1, D1", "[instruction][move]") {
    auto instruction = INSTRUCTION::Move::create(0x3209); // move.w A1, D1
    CPUState state = CPUState();

    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0xAABBCCDD);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == 0xCCDD);

    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

TEST_CASE("MOVE: move.l A1, D1", "[instruction][move]") {
    auto instruction = INSTRUCTION::Move::create(0x2209); // move.l A1, D1
    CPUState state = CPUState();

    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0xAABBCCDD);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == 0xAABBCCDD);

    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

TEST_CASE("MOVE: movea.w D0, A1", "[instruction][move]") {
    auto instruction = INSTRUCTION::Move::create(0x3240); // movea.w D0, A1
    CPUState state = CPUState();

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
    CHECK(return_data == 0xFFFFCCDD);

    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_negative == false);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

TEST_CASE("MOVE: movea.l D0, A1", "[instruction][move]") {
    auto instruction = INSTRUCTION::Move::create(0x2240); // movea.w D0, A1
    CPUState state = CPUState();

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
    CHECK(return_data == 0xAABBCCDD);

    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_negative == false);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

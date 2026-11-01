#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("SUBQ: subq #7, D0", "[instruction][subq]") {
    auto instruction = INSTRUCTION::Subq::create(0x5E40); // subq #7, D0
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_WORD, 107);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG);
    CHECK(return_data == 100);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == false);
    CHECK(flag_negative == false);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

TEST_CASE("SUBQ: subq.l #1, D0 (borrow)", "[instruction][subq]") {
    auto instruction = INSTRUCTION::Subq::create(0x5380);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xFFFFFFFF);
    checkFlags(state, true, true, false, false, true);
}

TEST_CASE("SUBQ: subq.w #1, D0 (overflow)", "[instruction][subq]") {
    auto instruction = INSTRUCTION::Subq::create(0x5340);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00008000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00007FFF);
    checkFlags(state, false, false, false, true, false);
}

TEST_CASE("SUBQ: subq.w #1, A0 (full 32-bit, no flags)", "[instruction][subq]") {
    auto instruction = INSTRUCTION::Subq::create(0x5348);
    CPUState state = CPUState();
    state.registers.set(REG_A0, SIZE_LONG, 0x00010000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A0, SIZE_LONG) == 0x0000FFFF);
    checkFlags(state, false, false, false, false, false);
}

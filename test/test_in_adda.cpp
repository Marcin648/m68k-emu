#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("ADDA: adda.w D0, A1", "[instruction][adda]") {
    auto instruction = INSTRUCTION::Adda::create(0xD2C0); // adda.w D0, A1
    CPUState state = CPUState();
    
    state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x2000);
    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x1000);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_WORD);
    CHECK(return_data == 0x3000);

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

TEST_CASE("ADDA: adda.w D0, A1 (sign-extended source)", "[instruction][adda]") {
    auto instruction = INSTRUCTION::Adda::create(0xD2C0);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x0000FFFF);
    state.registers.set(REG_A1, SIZE_LONG, 0x00010000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x0000FFFF);
}

TEST_CASE("ADDA: adda.w D0, A1 (32-bit result)", "[instruction][adda]") {
    auto instruction = INSTRUCTION::Adda::create(0xD2C0);
    CPUState state = CPUState();
    state.registers.set(REG_D0, SIZE_LONG, 0x00000001);
    state.registers.set(REG_A1, SIZE_LONG, 0x0000FFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_A1, SIZE_LONG) == 0x00010000);
}

#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("ADDQ: addq #7, D0", "[instruction][addq]") {
    auto instruction = INSTRUCTION::Addq::create(0x5E40); // addq #7, D0
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_WORD, 100);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG);
    CHECK(return_data == 107);

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

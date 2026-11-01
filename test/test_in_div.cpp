#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("DIV: divu #$0002, D0", "[instruction][div]") {
    auto instruction = INSTRUCTION::Div::create(0x80FC); // divu #$0002, D0
    CPUState state = CPUState();
    uint32_t return_data;

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x000A);
    state.memory.set(0, DataSize::SIZE_WORD, 0x80FC); // opcode
    state.memory.set(2, DataSize::SIZE_WORD, 0x0003); // imm

    instruction.get()->execute(state);
    return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
    CHECK(return_data == 3);

    return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG) >> 16;
    CHECK(return_data == 1);

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

TEST_CASE("DIV: muls #$FFFE, D0", "[instruction][div]") {
    auto instruction = INSTRUCTION::Div::create(0x81FC); // muls #$FFFE, D0
    CPUState state = CPUState();
    int32_t return_data;

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x000A);
    state.memory.set(0, DataSize::SIZE_WORD, 0x81FC); // opcode
    state.memory.set(2, DataSize::SIZE_WORD, 0xFFFD); // imm

    instruction.get()->execute(state);

    return_data = static_cast<int16_t>(state.registers.get(REG_D0, DataSize::SIZE_WORD));
    CHECK(return_data == -3);

    return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG) >> 16;
    CHECK(return_data == 1);

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

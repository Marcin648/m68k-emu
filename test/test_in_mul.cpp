#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("MUL: mulu #$0002, D0", "[instruction][mul]") {
    auto instruction = INSTRUCTION::Mul::create(0xC0FC); // mulu #$0002, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x1000);
    state.memory.set(0, DataSize::SIZE_WORD, 0xC0FC); // opcode
    state.memory.set(2, DataSize::SIZE_WORD, 0x0002); // imm

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
    CHECK(return_data == 0x2000);

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

TEST_CASE("MUL: muls #$FFFE, D0", "[instruction][mul]") {
    auto instruction = INSTRUCTION::Mul::create(0xC1FC); // muls #$FFFE, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x1000);
    state.memory.set(0, DataSize::SIZE_WORD, 0xC1FC); // opcode
    state.memory.set(2, DataSize::SIZE_WORD, 0xFFFE); // imm

    instruction.get()->execute(state);
    int32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG);
    CHECK(return_data == -0x2000);

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

TEST_CASE("MUL: mulu #$FFFF, D0", "[instruction][mul]") {
    auto instruction = INSTRUCTION::Mul::create(0xC0FC);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0xC0FC);
    state.memory.set(2, SIZE_WORD, 0xFFFF);
    state.registers.set(REG_D0, SIZE_LONG, 0x0000FFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xFFFE0001);
    check_flags(state, false, true, false, false, false);
}

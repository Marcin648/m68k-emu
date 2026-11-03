#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

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

TEST_CASE("DIV: divu #2, D0 (32-bit dividend)", "[instruction][div]") {
    auto instruction = INSTRUCTION::Div::create(0x80FC);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x80FC);
    state.memory.set(2, SIZE_WORD, 0x0002);
    state.registers.set(REG_D0, SIZE_LONG, 0x00010000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00008000);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("DIV: divu #2, D0 (overflow)", "[instruction][div]") {
    auto instruction = INSTRUCTION::Div::create(0x80FC);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x80FC);
    state.memory.set(2, SIZE_WORD, 0x0002);
    state.registers.set(REG_D0, SIZE_LONG, 0x00100000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00100000);
    check_flags(state, false, false, false, true, false);
}

TEST_CASE("DIV: divs #4, D0 (negative dividend)", "[instruction][div]") {
    auto instruction = INSTRUCTION::Div::create(0x81FC);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x81FC);
    state.memory.set(2, SIZE_WORD, 0x0004);
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFE795F);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0xFFFF9E58);
    check_flags(state, false, true, false, false, false);
}

TEST_CASE("DIV: divs #1, D0 (overflow)", "[instruction][div]") {
    auto instruction = INSTRUCTION::Div::create(0x81FC);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x81FC);
    state.memory.set(2, SIZE_WORD, 0x0001);
    state.registers.set(REG_D0, SIZE_LONG, 0x00100000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00100000);
    check_flags(state, false, false, false, true, false);
}

TEST_CASE("DIV: divu/divs #0, D0 (zero divide)", "[instruction][div]") {
    uint16_t opcode = GENERATE(0x80FC, 0x81FC);
    auto instruction = INSTRUCTION::Div::create(opcode);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, opcode);
    state.memory.set(2, SIZE_WORD, 0x0000);
    state.memory.set(VECTOR_ZERO_DIVIDE * 4, SIZE_LONG, 0x3000);
    state.registers.set(REG_A7, SIZE_LONG, 0x1000);
    state.registers.set(REG_D0, SIZE_LONG, 0x12345678);
    state.registers.set(SR_FLAG_CARRY, true);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x12345678);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x3000);
    CHECK(state.registers.get(SR_FLAG_CARRY) == false);
    CHECK(state.memory.get(0x0FFC, SIZE_LONG) == 4);
}

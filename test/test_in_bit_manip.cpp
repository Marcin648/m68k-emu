#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

TEST_CASE("Bit manipulation: btst D0, D1", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x0101); // btst D0, D1
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_LONG, 41);
    state.registers.set(REG_D1, SIZE_LONG, (1 << 9));

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == (1 << 9));

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

TEST_CASE("Bit manipulation: btst D0, (A1)", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x0111); // btst D0, (A1)
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_LONG, 41);
    state.registers.set(REG_A0, SIZE_LONG, 0);

    state.memory.set(0, SIZE_BYTE, (1 << 1));

    instruction.get()->execute(state);
    uint32_t return_data = state.memory.get(0, DataSize::SIZE_BYTE);
    CHECK(return_data == (1 << 1));

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

TEST_CASE("Bit manipulation: bchg D0, D1", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x0141); // bchg D0, D1
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_LONG, 41);
    state.registers.set(REG_D1, SIZE_LONG, (1 << 9));

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == 0);

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

TEST_CASE("Bit manipulation: bclr D0, D1", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x0181); // bclr D0, D1
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_LONG, 41);
    state.registers.set(REG_D1, SIZE_LONG, (1 << 9));

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == 0);

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

TEST_CASE("Bit manipulation: bset D0, D1", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x01C1); // bclr D0, D1
    CPUState state = CPUState();
    
    state.registers.set(REG_D0, SIZE_LONG, 41);
    state.registers.set(REG_D1, SIZE_LONG, 0);

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
    CHECK(return_data == (1 << 9));

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == false);
    CHECK(flag_negative == false);
    CHECK(flag_zero == true);
    CHECK(flag_overflow == false);
    CHECK(flag_carry == false);
}

TEST_CASE("Bit manipulation: btst #31, D1 (immediate is a word)", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x0801); // btst #31, D1
    CPUState state = CPUState();

    state.memory.set(0, SIZE_WORD, 0x0801); // opcode
    state.memory.set(2, SIZE_WORD, 0x001F); // bit number
    state.registers.set(REG_D1, SIZE_LONG, 0x80000000);

    instruction.get()->execute(state);
    CHECK(state.registers.get(SR_FLAG_ZERO) == false);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
}

TEST_CASE("Bit manipulation: bset D0, D1 (bit 31)", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x01C1); // bset D0, D1
    CPUState state = CPUState();

    state.registers.set(REG_D0, SIZE_LONG, 31);
    state.registers.set(REG_D1, SIZE_LONG, 0);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D1, SIZE_LONG) == 0x80000000);
    CHECK(state.registers.get(SR_FLAG_ZERO) == true);
}

TEST_CASE("Bit manipulation: btst D0, d16(PC)", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x013A); // btst D0, $10(PC)
    CPUState state = CPUState();
    CHECK(instruction.get()->is_valid == true);

    state.registers.set(REG_PC, SIZE_LONG, 0x1000);
    state.memory.set(0x1000, SIZE_WORD, 0x013A); // opcode
    state.memory.set(0x1002, SIZE_WORD, 0x0010); // displacement
    state.memory.set(0x1012, SIZE_BYTE, 0x04);
    state.registers.set(REG_D0, SIZE_LONG, 2);

    instruction.get()->execute(state);
    CHECK(state.registers.get(SR_FLAG_ZERO) == false);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 0x1004);
}

TEST_CASE("Bit manipulation: btst D0, #imm", "[instruction][bit_manip]") {
    auto instruction = INSTRUCTION::BitManip::create(0x013C); // btst D0, #$05
    CPUState state = CPUState();
    CHECK(instruction.get()->is_valid == true);

    state.memory.set(0, SIZE_WORD, 0x013C); // opcode
    state.memory.set(2, SIZE_WORD, 0x0005); // imm
    state.registers.set(REG_D0, SIZE_LONG, 2);

    instruction.get()->execute(state);
    CHECK(state.registers.get(SR_FLAG_ZERO) == false);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 4);
}

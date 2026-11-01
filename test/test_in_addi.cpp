#include <catch2/catch_test_macros.hpp>

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"

using namespace M68K;

TEST_CASE("ADDI: addi.w #$0001, D0", "[instruction][addi]") {
    auto instruction = INSTRUCTION::Addi::create(0x0640); // addi.w #$0001, D0
    CPUState state = CPUState();

    state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x7FFF);

    state.memory.set(0, DataSize::SIZE_WORD, 0x0640); // opcode
    state.memory.set(2, DataSize::SIZE_WORD, 0x0001); // imm

    instruction.get()->execute(state);
    uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
    CHECK(return_data == 0x8000);

    bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
    bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
    bool flag_zero = state.registers.get(SR_FLAG_ZERO);
    bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
    bool flag_carry = state.registers.get(SR_FLAG_CARRY);

    CHECK(flag_extend == false);
    CHECK(flag_negative == true);
    CHECK(flag_zero == false);
    CHECK(flag_overflow == true);
    CHECK(flag_carry == false);
}

TEST_CASE("ADDI: addi.l #1, D0 (carry)", "[instruction][addi]") {
    auto instruction = INSTRUCTION::Addi::create(0x0680);
    CPUState state = CPUState();
    state.memory.set(0, SIZE_WORD, 0x0680);
    state.memory.set(2, SIZE_LONG, 0x00000001);
    state.registers.set(REG_D0, SIZE_LONG, 0xFFFFFFFF);

    instruction.get()->execute(state);
    CHECK(state.registers.get(REG_D0, SIZE_LONG) == 0x00000000);
    CHECK(state.registers.get(REG_PC, SIZE_LONG) == 6);
    checkFlags(state, true, false, true, false, true);
}

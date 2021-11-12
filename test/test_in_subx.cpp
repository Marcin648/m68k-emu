#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction SUBX");

    {
        TEST_LABEL("subx.w D0, D1");
        auto instruction = INSTRUCTION::Subx::create(0x9340); // subx.w D0, D1
        CPUState state = CPUState();

        uint32_t return_data;

        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x0001);
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x0000);
        instruction.get()->execute(state);
        return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xFFFF);

        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x0001);
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0x0000);
        instruction.get()->execute(state);
        return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0xFFFE);

        bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_extend == true);
        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == true);
        TEST_TRUE(flag_carry == true);
    }

    {
        TEST_LABEL("subx.w -(A0), -(A1)");
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
        TEST_TRUE(return_data == 0xFFFF);

        state.memory.set(0x2002, SIZE_WORD, 0x0001);
        instruction.get()->execute(state);
        return_data = state.memory.get(0x2000, SIZE_WORD);
        TEST_TRUE(return_data == 0xFFFE);

        bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_extend == true);
        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == true);
        TEST_TRUE(flag_carry == true);
    }
}

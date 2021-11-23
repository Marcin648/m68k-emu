#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction Sub");
    
    {
        TEST_LABEL("sub.w #$1111, D1");
        auto instruction = INSTRUCTION::Sub::create(0x927C); // sub.w #$1111, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, DataSize::SIZE_LONG, 0xAABBCCDD);
        state.memory.set(0, DataSize::SIZE_WORD, 0x927C); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x1111); // value to sub

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBBBCC);

        bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_extend == false);
        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }

    {
        TEST_LABEL("sub.w (A1), D0");
        auto instruction = INSTRUCTION::Sub::create(0x9051); // sub.w (A1), D0
        CPUState state = CPUState();
        
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x1000);
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xFFFF);
        state.memory.set(0x1000, DataSize::SIZE_WORD, 0x0001);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xFFFE);

        bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_extend == false);
        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }

    {
        TEST_LABEL("sub.w D0, (A1)");
        auto instruction = INSTRUCTION::Sub::create(0x9151); // sub.w D0, (A1)
        CPUState state = CPUState();
        
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x1000);
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xFFFE);
        state.memory.set(0x1000, DataSize::SIZE_WORD, 0xFFFF);

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0x1000, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0x00000001);

        bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_extend == false);
        TEST_TRUE(flag_negative == false);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }
}

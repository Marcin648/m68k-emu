#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction bit manipulation");

    {
        TEST_LABEL("btst D0, D1");
        auto instruction = INSTRUCTION::BitManip::create(0x0101); // btst D0, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, SIZE_LONG, 41);
        state.registers.set(REG_D1, SIZE_LONG, (1 << 9));

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == (1 << 9));

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

    {
        TEST_LABEL("btst D0, (A1)");
        auto instruction = INSTRUCTION::BitManip::create(0x0111); // btst D0, (A1)
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, SIZE_LONG, 41);
        state.registers.set(REG_A0, SIZE_LONG, 0);

        state.memory.set(0, SIZE_BYTE, (1 << 1));

        instruction.get()->execute(state);
        uint32_t return_data = state.memory.get(0, DataSize::SIZE_BYTE);
        TEST_TRUE(return_data == (1 << 1));

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

    {
        TEST_LABEL("bchg D0, D1");
        auto instruction = INSTRUCTION::BitManip::create(0x0141); // bchg D0, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, SIZE_LONG, 41);
        state.registers.set(REG_D1, SIZE_LONG, (1 << 9));

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0);

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

    {
        TEST_LABEL("bclr D0, D1");
        auto instruction = INSTRUCTION::BitManip::create(0x0181); // bclr D0, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, SIZE_LONG, 41);
        state.registers.set(REG_D1, SIZE_LONG, (1 << 9));

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0);

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

    {
        TEST_LABEL("bset D0, D1");
        auto instruction = INSTRUCTION::BitManip::create(0x01C1); // bclr D0, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, SIZE_LONG, 41);
        state.registers.set(REG_D1, SIZE_LONG, 0);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == (1 << 9));

        bool flag_extend = state.registers.get(SR_FLAG_EXTEND);
        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_extend == false);
        TEST_TRUE(flag_negative == false);
        TEST_TRUE(flag_zero == true);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }
    
}

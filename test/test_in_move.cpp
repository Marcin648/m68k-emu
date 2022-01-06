#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction MOVE / MOVEA");
    
    {
        TEST_LABEL("move.w A1, D1");
        auto instruction = INSTRUCTION::Move::create(0x3209); // move.w A1, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xCCDD);

        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }

    {
        TEST_LABEL("move.l A1, D1");
        auto instruction = INSTRUCTION::Move::create(0x2209); // move.l A1, D1
        CPUState state = CPUState();
        
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);

        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_negative == true);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }

    {
        TEST_LABEL("movea.w D0, A1");
        auto instruction = INSTRUCTION::Move::create(0x3240); // movea.w D0, A1
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xFFFFCCDD);

        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_negative == false);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }

    {
        TEST_LABEL("movea.l D0, A1");
        auto instruction = INSTRUCTION::Move::create(0x2240); // movea.w D0, A1
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0xAABBCCDD);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);

        bool flag_negative = state.registers.get(SR_FLAG_NEGATIVE);
        bool flag_zero = state.registers.get(SR_FLAG_ZERO);
        bool flag_overflow = state.registers.get(SR_FLAG_OVERFLOW);
        bool flag_carry = state.registers.get(SR_FLAG_CARRY);

        TEST_TRUE(flag_negative == false);
        TEST_TRUE(flag_zero == false);
        TEST_TRUE(flag_overflow == false);
        TEST_TRUE(flag_carry == false);
    }
}
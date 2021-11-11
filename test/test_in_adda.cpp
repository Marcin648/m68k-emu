#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction ADDA");

    {
        TEST_LABEL("adda.w D0, A1");
        auto instruction = INSTRUCTION::Adda::create(0xD2C0); // adda.w D0, A1
        CPUState state = CPUState();
        
        state.registers.set(REG_A1, DataSize::SIZE_LONG, 0x2000);
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0x3000);

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

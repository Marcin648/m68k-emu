#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction MUL");

    {
        TEST_LABEL("mulu #$0002, D0");
        auto instruction = INSTRUCTION::Mul::create(0xC0FC); // mulu #$0002, D0
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x1000);
        state.memory.set(0, DataSize::SIZE_WORD, 0xC0FC); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x0002); // imm

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 0x2000);

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
        TEST_LABEL("muls #$FFFE, D0");
        auto instruction = INSTRUCTION::Mul::create(0xC1FC); // muls #$FFFE, D0
        CPUState state = CPUState();
        
        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x1000);
        state.memory.set(0, DataSize::SIZE_WORD, 0xC1FC); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0xFFFE); // imm

        instruction.get()->execute(state);
        int32_t return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == -0x2000);

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
}

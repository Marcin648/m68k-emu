#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction DIV");

    {
        TEST_LABEL("divu #$0002, D0");
        auto instruction = INSTRUCTION::Div::create(0x80FC); // divu #$0002, D0
        CPUState state = CPUState();
        uint32_t return_data;

        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x000A);
        state.memory.set(0, DataSize::SIZE_WORD, 0x80FC); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0x0003); // imm

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_D0, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == 3);

        return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG) >> 16;
        TEST_TRUE(return_data == 1);

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
        auto instruction = INSTRUCTION::Div::create(0x81FC); // muls #$FFFE, D0
        CPUState state = CPUState();
        int32_t return_data;

        state.registers.set(REG_D0, DataSize::SIZE_LONG, 0x000A);
        state.memory.set(0, DataSize::SIZE_WORD, 0x81FC); // opcode
        state.memory.set(2, DataSize::SIZE_WORD, 0xFFFD); // imm

        instruction.get()->execute(state);

        return_data = static_cast<int16_t>(state.registers.get(REG_D0, DataSize::SIZE_WORD));
        TEST_TRUE(return_data == -3);

        return_data = state.registers.get(REG_D0, DataSize::SIZE_LONG) >> 16;
        TEST_TRUE(return_data == 1);

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

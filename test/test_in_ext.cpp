#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction EXT");

    {
        TEST_LABEL("ext.w D1");
        auto instruction = INSTRUCTION::Ext::create(0x4881); // ext.w D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, SIZE_BYTE, static_cast<int8_t>(-10));
        instruction.get()->execute(state);

        int16_t return_data = state.registers.get(REG_D1, DataSize::SIZE_WORD);
        TEST_TRUE(return_data == -10);
    }

    {
        TEST_LABEL("ext.l D1");
        auto instruction = INSTRUCTION::Ext::create(0x48c1); // ext.l D1
        CPUState state = CPUState();
        
        state.registers.set(REG_D1, SIZE_WORD, static_cast<int16_t>(-10));
        instruction.get()->execute(state);

        int32_t return_data = state.registers.get(REG_D1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == -10);
    }
}

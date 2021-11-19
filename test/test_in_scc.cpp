#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction Scc");

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("svs D0");
        auto instruction = INSTRUCTION::Scc::create(0x59C0); // svs D0

        state.registers.set(REG_D0, SIZE_BYTE, 0x00);    
        state.registers.set(SR_FLAG_OVERFLOW, true);

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_D0, SIZE_BYTE);
        TEST_TRUE(return_data == 0xFF);
    }

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("svs D0");
        auto instruction = INSTRUCTION::Scc::create(0x59C0); // svs D0

        state.registers.set(REG_D0, SIZE_BYTE, 0xFF);    
        state.registers.set(SR_FLAG_OVERFLOW, false);

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_D0, SIZE_BYTE);
        TEST_TRUE(return_data == 0x00);
    }
}

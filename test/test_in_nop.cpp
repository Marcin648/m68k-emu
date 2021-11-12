#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction NOP");

    {
        TEST_LABEL("nop");
        auto instruction = INSTRUCTION::Nop::create(0x4E71); // nop
        CPUState state = CPUState();
        

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x00000002);
    }
}

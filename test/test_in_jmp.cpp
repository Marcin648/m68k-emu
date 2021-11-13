#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction JMP");

    {
        TEST_LABEL("jmp (A0)");
        auto instruction = INSTRUCTION::Jmp::create(0x4ED0); // jmp (A0)
        CPUState state = CPUState();
        
        state.memory.set(0x1000, SIZE_LONG, 0x2000);
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x2000);
    }
}

#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction LEA");

    {
        TEST_LABEL("lea 0xAABBCCDD, A1");
        auto instruction = INSTRUCTION::Lea::create(0x43F9); // lea 0xAABBCCDD, A1
        CPUState state = CPUState();
        
        state.memory.set(0x0000, SIZE_WORD, 0x43F9); // opcode
        state.memory.set(0x0002, SIZE_LONG, 0xAABBCCDD); // address

        instruction.get()->execute(state);
        uint32_t return_data = state.registers.get(REG_A1, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xAABBCCDD);
    }
}

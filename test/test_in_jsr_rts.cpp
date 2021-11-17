#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction JSR/RTS");

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("jsr (A0)");
        auto instruction_jsr = INSTRUCTION::Jsr::create(0x4E90); // jsr (A0)
        
        state.registers.set(REG_USP, SIZE_LONG, MEMORY_SIZE);
        state.registers.set(REG_PC, SIZE_LONG, 0xF000);
        state.registers.set(REG_A0, DataSize::SIZE_LONG, 0x1000);
        state.memory.set(0x1000, SIZE_LONG, 0x2000);

        instruction_jsr.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x2000);

        TEST_LABEL("rts");
        auto instruction_rts = INSTRUCTION::Rts::create(0x4E75); // rts

        instruction_rts.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0xF002);
    }
}

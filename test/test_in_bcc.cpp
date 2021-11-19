#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

using namespace M68K;

int main(int, char**){
    TEST_NAME("Instruction Bcc");

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("bvs.s 0x04");
        auto instruction = INSTRUCTION::Bcc::create(0x6902); // bvs.s 0x04
    
        state.registers.set(SR_FLAG_OVERFLOW, true);
        state.registers.set(REG_PC, SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x1004);
    }

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("bvs.s 0x04");
        auto instruction = INSTRUCTION::Bcc::create(0x6902); // bvs.s 0x04
    
        state.registers.set(SR_FLAG_OVERFLOW, false);
        state.registers.set(REG_PC, SIZE_LONG, 0x1000);

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x1002);
    }

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("bvs.w 0x1002");
        auto instruction = INSTRUCTION::Bcc::create(0x6900); // bvs.w 0x1002
    
        state.registers.set(SR_FLAG_OVERFLOW, true);
        state.registers.set(REG_PC, SIZE_LONG, 0x1000);

        state.memory.set(0x1000, SIZE_WORD, 0x6900); // opcode
        state.memory.set(0x1002, SIZE_WORD, 0x1000); // data

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x2002);
    }

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("bvs.l 0x00010002");
        auto instruction = INSTRUCTION::Bcc::create(0x69FF); // bvs.l 0x00010002
    
        state.registers.set(SR_FLAG_OVERFLOW, true);
        state.registers.set(REG_PC, SIZE_LONG, 0x1000);

        state.memory.set(0x1000, SIZE_WORD, 0x69FF); // opcode
        state.memory.set(0x1002, SIZE_LONG, 0x10000); // data

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x11002);
    }

    {
        uint32_t return_data;
        CPUState state = CPUState();

        TEST_LABEL("bvs.l 0x00010002");
        auto instruction = INSTRUCTION::Bcc::create(0x69FF); // bvs.l 0x00010002
    
        state.registers.set(SR_FLAG_OVERFLOW, false);
        state.registers.set(REG_PC, SIZE_LONG, 0x1000);

        state.memory.set(0x1000, SIZE_WORD, 0x69FF); // opcode
        state.memory.set(0x1002, SIZE_LONG, 0x10000); // data

        instruction.get()->execute(state);
        return_data = state.registers.get(REG_PC, DataSize::SIZE_LONG);
        TEST_TRUE(return_data == 0x1006);
    }
}

#include "tests_functions.hpp"

#include "m68k.hpp"
#include "memory.hpp"
#include "helpers.hpp"

int main(int, char**){
    TEST_NAME("Instruction MOVE");

    M68K::InstructionDecoder decoder = M68K::InstructionDecoder();
    M68K::INSTRUCTION::Instruction* instruction = nullptr;
    uint32_t return_data = 0;

    M68K::CPUState state = M68K::CPUState();
    state.registers.set(M68K::REG_A1, M68K::DataSize::SIZE_LONG, 0xAABBCCDD);
    
    // move.w D1, A1 
    instruction = decoder.Decode(0x3209); // move.w D1, A1
    TEST_LABEL("move.w A1, D1");
    instruction->execute(state);
    return_data = state.registers.get(M68K::REG_D1, M68K::DataSize::SIZE_LONG);
    
    TEST_TRUE(return_data == 0xCCDD);
}
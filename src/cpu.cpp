#include "cpu.hpp"

using namespace M68K;

void CPU::step(){
    uint32_t pc = this->state.registers.get(REG_PC);
    uint16_t opcode = this->state.memory.get(pc, SIZE_WORD);

    auto instruction = this->instruction_decoder.Decode(opcode);
    instruction->execute(this->state);
}
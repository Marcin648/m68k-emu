#include "instructions/nop.hpp"
#include "instruction_decoder.hpp"
#include "instruction_functions.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Nop::Nop(uint16_t opcode) : Instruction(opcode){
    ;
}

void Nop::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += 2;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
}

std::shared_ptr<INSTRUCTION::Instruction> Nop::create(uint16_t opcode){
    return std::make_shared<Nop>(opcode);
}
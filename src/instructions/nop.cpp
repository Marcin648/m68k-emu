#include "instructions/nop.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Nop::Nop(uint16_t opcode) : Instruction(opcode){
    ;
}

void Nop::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
}

std::string Nop::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "nop";
}


std::shared_ptr<INSTRUCTION::Instruction> Nop::create(uint16_t opcode){
    return std::make_shared<Nop>(opcode);
}
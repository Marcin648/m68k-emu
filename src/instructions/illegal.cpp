#include "instructions/illegal.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

void Illegal::execute(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    throw std::invalid_argument("Invalid opcode");
}

std::string Illegal::disassembly(CPUState& cpu_state){
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "illegal";
}

std::shared_ptr<INSTRUCTION::Instruction> Illegal::create(uint16_t opcode){
    return std::make_shared<Illegal>(opcode);
}
#include "instructions/illegal.hpp"

using namespace M68K;
using namespace INSTRUCTION;

void Illegal::execute(CPUState& cpu_state) const {
    switch (this->opcode >> 12) {
        case 0xA: cpu_state.exception(VECTOR_LINE_A); break;
        case 0xF: cpu_state.exception(VECTOR_LINE_F); break;
        default: cpu_state.exception(VECTOR_ILLEGAL); break;
    }
}

std::string Illegal::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "illegal";
}

std::unique_ptr<INSTRUCTION::Instruction> Illegal::create(uint16_t opcode) {
    return std::make_unique<Illegal>(opcode);
}

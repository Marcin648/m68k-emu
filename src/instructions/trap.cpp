#include "instructions/trap.hpp"

using namespace M68K;
using namespace INSTRUCTION;

Trap::Trap(uint16_t opcode) : Instruction(opcode) {
    this->vector = opcode & 0xF;
}

void Trap::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    cpu_state.exception(VECTOR_TRAP + this->vector);
}

std::string Trap::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "trap #$" << std::hex << static_cast<uint32_t>(this->vector);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Trap::create(uint16_t opcode) {
    return std::make_unique<Trap>(opcode);
}

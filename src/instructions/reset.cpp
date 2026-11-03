#include "instructions/reset.hpp"

using namespace M68K;
using namespace INSTRUCTION;

void Reset::execute(CPUState& cpu_state) const {
    if (!cpu_state.require_supervisor()) {
        return;
    }

    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
}

std::string Reset::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "reset";
}

std::unique_ptr<INSTRUCTION::Instruction> Reset::create(uint16_t opcode) {
    return std::make_unique<Reset>(opcode);
}

#include "instructions/rte.hpp"

using namespace M68K;
using namespace INSTRUCTION;

void Rte::execute(CPUState& cpu_state) const {
    if (!cpu_state.require_supervisor()) {
        return;
    }

    uint32_t sr = cpu_state.stack_pop(SIZE_WORD);
    uint32_t pc = cpu_state.stack_pop(SIZE_LONG);
    cpu_state.registers.set(REG_SR, SIZE_WORD, sr);
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
}

std::string Rte::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "rte";
}

std::unique_ptr<INSTRUCTION::Instruction> Rte::create(uint16_t opcode) {
    return std::make_unique<Rte>(opcode);
}

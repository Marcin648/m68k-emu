#include "instructions/stop.hpp"

using namespace M68K;
using namespace INSTRUCTION;

void Stop::execute(CPUState& cpu_state) const {
    if (!cpu_state.require_supervisor()) {
        return;
    }

    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t sr = cpu_state.get_data(ADDR_MODE_IMMEDIATE, REG_D0, SIZE_WORD);
    cpu_state.registers.set(REG_SR, SIZE_WORD, sr);
    cpu_state.stopped = true;
}

std::string Stop::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "stop " << DISASSEMBLER::effective_address(ADDR_MODE_IMMEDIATE, REG_D0, SIZE_WORD, cpu_state);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Stop::create(uint16_t opcode) {
    return std::make_unique<Stop>(opcode);
}

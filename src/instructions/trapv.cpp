#include "instructions/trapv.hpp"

using namespace M68K;
using namespace INSTRUCTION;

void Trapv::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    if (cpu_state.registers.get(SR_FLAG_OVERFLOW)) {
        cpu_state.exception(VECTOR_TRAPV);
    }
}

std::string Trapv::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "trapv";
}

std::unique_ptr<INSTRUCTION::Instruction> Trapv::create(uint16_t opcode) {
    return std::make_unique<Trapv>(opcode);
}

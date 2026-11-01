#include "instructions/rtr.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Rtr::Rtr(uint16_t opcode) : Instruction(opcode) {
    ;
}

void Rtr::execute(CPUState& cpu_state) const {
    uint32_t ccr_data = cpu_state.stack_pop(SIZE_WORD);
    cpu_state.registers.set(REG_SR, SIZE_BYTE, MASK_CCR(ccr_data));

    uint32_t return_addr = cpu_state.stack_pop(SIZE_LONG);
    cpu_state.registers.set(REG_PC, SIZE_LONG, return_addr);
}

std::string Rtr::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);
    return "rtr";
}

std::unique_ptr<INSTRUCTION::Instruction> Rtr::create(uint16_t opcode) {
    return std::make_unique<Rtr>(opcode);
}

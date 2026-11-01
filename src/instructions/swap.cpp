#include "instructions/swap.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Swap::Swap(uint16_t opcode) : Instruction(opcode) {
    uint16_t data_reg_part = (opcode >> 0) & 0x7;
    this->data_reg = get_register_type(0, data_reg_part);
}

void Swap::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t data = cpu_state.registers.get(this->data_reg, SIZE_LONG);
    uint32_t result = (data << 16) | (data >> 16);
    cpu_state.registers.set(this->data_reg, SIZE_LONG, result);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, SIZE_LONG));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(result, SIZE_LONG));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::string Swap::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "swap " << DISASSEMBLER::reg(this->data_reg);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Swap::create(uint16_t opcode) {
    return std::make_unique<Swap>(opcode);
}

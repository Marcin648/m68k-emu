#include "instructions/tas.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Tas::Tas(uint16_t opcode) : Instruction(opcode) {
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->dest_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->dest_reg = get_register_type(ea_mode_part, ea_reg_part);

    if (!IS_DATA_ALTERABLE(this->dest_mode)) {
        this->is_valid = false;
    }
}

void Tas::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t dest_data = cpu_state.get_data_silent(this->dest_mode, this->dest_reg, SIZE_BYTE);
    cpu_state.set_data(this->dest_mode, this->dest_reg, SIZE_BYTE, dest_data | 0x80);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(dest_data, SIZE_BYTE));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(dest_data, SIZE_BYTE));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::string Tas::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "tas " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, SIZE_BYTE, cpu_state);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Tas::create(uint16_t opcode) {
    return std::make_unique<Tas>(opcode);
}

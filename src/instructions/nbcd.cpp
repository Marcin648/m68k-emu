#include "instructions/nbcd.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Nbcd::Nbcd(uint16_t opcode) : Instruction(opcode) {
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->dest_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->dest_reg = get_register_type(ea_mode_part, ea_reg_part);

    if (!IS_DATA_ALTERABLE(this->dest_mode)) {
        this->is_valid = false;
    }
}

void Nbcd::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t extend_data = cpu_state.registers.get(SR_FLAG_EXTEND) ? 1 : 0;
    uint32_t dest_data = cpu_state.get_data_silent(this->dest_mode, this->dest_reg, SIZE_BYTE);
    uint32_t result = BCD_SUB(dest_data, 0, extend_data);

    cpu_state.set_data(this->dest_mode, this->dest_reg, SIZE_BYTE, result);

    if (!IS_ZERO(result, SIZE_BYTE)) { // change only if non zero
        cpu_state.registers.set(SR_FLAG_ZERO, false);
    }

    cpu_state.registers.set(SR_FLAG_EXTEND, IS_CARRY(result, SIZE_BYTE));
    cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, SIZE_BYTE));
}

std::string Nbcd::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "nbcd " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, SIZE_BYTE, cpu_state);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Nbcd::create(uint16_t opcode) {
    return std::make_unique<Nbcd>(opcode);
}

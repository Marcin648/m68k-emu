#include "instructions/sbcd.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Sbcd::Sbcd(uint16_t opcode) : Instruction(opcode) {
    uint16_t dest_reg_part = (opcode >> 9) & 0x7;
    uint16_t src_reg_part = (opcode >> 0) & 0x7;
    uint16_t mode_part = (opcode >> 3) & 0x1;

    this->dest_mode = mode_part == 0 ? ADDR_MODE_DIRECT_DATA : ADDR_MODE_INDIRECT_PREDECREMENT;
    this->dest_reg = get_register_type(mode_part, dest_reg_part);

    this->src_mode = this->dest_mode;
    this->src_reg = get_register_type(mode_part, src_reg_part);
}

void Sbcd::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t extend_data = cpu_state.registers.get(SR_FLAG_EXTEND) ? 1 : 0;
    uint32_t src_data = cpu_state.get_data(this->src_mode, this->src_reg, SIZE_BYTE);
    uint32_t dest_data = cpu_state.get_data_silent(this->dest_mode, this->dest_reg, SIZE_BYTE);
    uint32_t result = BCD_SUB(src_data, dest_data, extend_data);

    cpu_state.set_data(this->dest_mode, this->dest_reg, SIZE_BYTE, result);

    if (!IS_ZERO(result, SIZE_BYTE)) { // change only if non zero
        cpu_state.registers.set(SR_FLAG_ZERO, false);
    }

    cpu_state.registers.set(SR_FLAG_EXTEND, IS_CARRY(result, SIZE_BYTE));
    cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, SIZE_BYTE));
}

std::string Sbcd::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "sbcd " << DISASSEMBLER::effective_address(this->src_mode, this->src_reg, SIZE_BYTE, cpu_state)
           << ", " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, SIZE_BYTE, cpu_state);

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Sbcd::create(uint16_t opcode) {
    return std::make_unique<Sbcd>(opcode);
}

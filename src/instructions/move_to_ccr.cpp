#include "instructions/move_to_ccr.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

MoveToCCR::MoveToCCR(uint16_t opcode) : Instruction(opcode) {
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->src_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->src_reg = get_register_type(ea_mode_part, ea_reg_part);
    this->is_sr = (opcode >> 9) & 0x1;

    if ((this->src_mode == ADDR_MODE_UNKNOWN) || (this->src_mode == ADDR_MODE_DIRECT_ADDR)) {
        this->is_valid = false;
    }
}

void MoveToCCR::execute(CPUState& cpu_state) const {
    if (this->is_sr && !cpu_state.require_supervisor()) {
        return;
    }

    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = cpu_state.get_data(this->src_mode, this->src_reg, SIZE_WORD);
    if (this->is_sr) {
        cpu_state.registers.set(REG_SR, SIZE_WORD, src_data);
    } else {
        cpu_state.registers.set(REG_SR, SIZE_BYTE, MASK_CCR(src_data));
    }
}

std::string MoveToCCR::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "move" << DISASSEMBLER::size_suffix(SIZE_WORD)
           << " " << DISASSEMBLER::effective_address(this->src_mode, this->src_reg, SIZE_WORD, cpu_state)
           << (this->is_sr ? ", sr" : ", ccr");

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> MoveToCCR::create(uint16_t opcode) {
    return std::make_unique<MoveToCCR>(opcode);
}

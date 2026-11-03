#include "instructions/chk.hpp"

using namespace M68K;
using namespace INSTRUCTION;

Chk::Chk(uint16_t opcode) : Instruction(opcode) {
    uint16_t data_reg_part = (opcode >> 9) & 0x7;
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->data_reg = get_register_type(0, data_reg_part);
    this->src_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->src_reg = get_register_type(ea_mode_part, ea_reg_part);

    if ((this->src_mode == ADDR_MODE_UNKNOWN) || (this->src_mode == ADDR_MODE_DIRECT_ADDR)) {
        this->is_valid = false;
    }
}

void Chk::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    int16_t bound = static_cast<int16_t>(cpu_state.get_data(this->src_mode, this->src_reg, SIZE_WORD));
    int16_t value = static_cast<int16_t>(cpu_state.registers.get(this->data_reg, SIZE_WORD));

    if (value < 0) {
        cpu_state.registers.set(SR_FLAG_NEGATIVE, true);
        cpu_state.exception(VECTOR_CHK);
    } else if (value > bound) {
        cpu_state.registers.set(SR_FLAG_NEGATIVE, false);
        cpu_state.exception(VECTOR_CHK);
    }
}

std::string Chk::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "chk" << DISASSEMBLER::size_suffix(SIZE_WORD)
           << " " << DISASSEMBLER::effective_address(this->src_mode, this->src_reg, SIZE_WORD, cpu_state)
           << ", " << DISASSEMBLER::reg(this->data_reg);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Chk::create(uint16_t opcode) {
    return std::make_unique<Chk>(opcode);
}

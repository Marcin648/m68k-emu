#include "instructions/move_from_sr.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

MoveFromSR::MoveFromSR(uint16_t opcode) : Instruction(opcode) {
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->dest_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->dest_reg = get_register_type(ea_mode_part, ea_reg_part);

    if (!IS_DATA_ALTERABLE(this->dest_mode)) {
        this->is_valid = false;
    }
}

void MoveFromSR::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t sr_data = cpu_state.registers.get(REG_SR, SIZE_WORD);
    cpu_state.set_data(this->dest_mode, this->dest_reg, SIZE_WORD, sr_data);
}

std::string MoveFromSR::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "move" << DISASSEMBLER::size_suffix(SIZE_WORD)
           << " sr, " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, SIZE_WORD, cpu_state);

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> MoveFromSR::create(uint16_t opcode) {
    return std::make_unique<MoveFromSR>(opcode);
}

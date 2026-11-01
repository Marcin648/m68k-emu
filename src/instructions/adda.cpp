#include "instructions/adda.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Adda::Adda(uint16_t opcode) : Instruction(opcode) {
    uint16_t addr_reg_part = (opcode >> 9) & 0x7;
    uint16_t ea_mode_mode = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 8) & 0x1;

    this->dest_mode = ADDR_MODE_DIRECT_ADDR;
    this->dest_reg = get_register_type(1, addr_reg_part);

    this->src_mode = get_addressing_mode(ea_mode_mode, ea_reg_part);
    this->src_reg = get_register_type(ea_mode_mode, ea_reg_part);

    switch (data_size_part) {
        case 0x00: { // 0b0
            this->data_size = SIZE_WORD;
            break;
        }
        case 0x01: { // 0b1
            this->data_size = SIZE_LONG;
            break;
        }

        default: {
            this->is_valid = false;
        }
    }
}

void Adda::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = cpu_state.get_data(this->src_mode, this->src_reg, this->data_size);
    if (this->data_size == SIZE_WORD) {
        src_data = static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(src_data)));
    }
    uint32_t dest_data = cpu_state.registers.get(this->dest_reg, SIZE_LONG);
    uint32_t result = dest_data + src_data;

    cpu_state.registers.set(this->dest_reg, SIZE_LONG, result);
}

std::string Adda::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "adda" << DISASSEMBLER::size_suffix(this->data_size)
           << " " << DISASSEMBLER::effective_address(this->src_mode, this->src_reg, this->data_size, cpu_state)
           << ", " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, this->data_size, cpu_state);

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Adda::create(uint16_t opcode) {
    return std::make_unique<Adda>(opcode);
}

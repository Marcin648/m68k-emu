#include "instructions/negx.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Negx::Negx(uint16_t opcode) : Instruction(opcode) {
    uint16_t ea_mode_part = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;
    uint16_t data_size_part = (opcode >> 6) & 0x3;

    this->dest_mode = get_addressing_mode(ea_mode_part, ea_reg_part);
    this->dest_reg = get_register_type(ea_mode_part, ea_reg_part);

    if (!IS_DATA_ALTERABLE(this->dest_mode)) {
        this->is_valid = false;
    }

    switch (data_size_part) {
        case 0x00: { // 0b00
            this->data_size = SIZE_BYTE;
            break;
        }
        case 0x01: { // 0b01
            this->data_size = SIZE_WORD;
            break;
        }
        case 0x02: { // 0b10
            this->data_size = SIZE_LONG;
            break;
        }

        default: {
            this->is_valid = false;
        }
    }
}

void Negx::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t extend_data = cpu_state.registers.get(SR_FLAG_EXTEND) ? 1 : 0;
    uint32_t src_data = 0;
    uint32_t dest_data = cpu_state.get_data_silent(this->dest_mode, this->dest_reg, this->data_size);
    uint64_t result = static_cast<uint64_t>(src_data) - dest_data - extend_data;

    cpu_state.set_data(this->dest_mode, this->dest_reg, this->data_size, result);

    if (!IS_ZERO(result, this->data_size)) { // change only if non zero
        cpu_state.registers.set(SR_FLAG_ZERO, false);
    }

    cpu_state.registers.set(SR_FLAG_EXTEND, IS_CARRY(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(result, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, IS_OVERFLOW_SUB(dest_data, src_data, result, this->data_size));
    cpu_state.registers.set(SR_FLAG_CARRY, IS_CARRY(result, this->data_size));
}

std::string Negx::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << "negx" << DISASSEMBLER::size_suffix(this->data_size)
           << " " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, this->data_size, cpu_state);
    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Negx::create(uint16_t opcode) {
    return std::make_unique<Negx>(opcode);
}

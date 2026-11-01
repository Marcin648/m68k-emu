#include "instructions/div.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Div::Div(uint16_t opcode) : Instruction(opcode) {
    uint16_t data_reg_part = (opcode >> 9) & 0x7;
    uint16_t ea_mode_mode = (opcode >> 3) & 0x7;
    uint16_t ea_reg_part = (opcode >> 0) & 0x7;

    this->is_signed = (opcode >> 8) & 0x1;

    this->dest_mode = ADDR_MODE_DIRECT_DATA;
    this->dest_reg = get_register_type(0, data_reg_part);

    this->src_mode = get_addressing_mode(ea_mode_mode, ea_reg_part);
    this->src_reg = get_register_type(ea_mode_mode, ea_reg_part);

    this->data_size = SIZE_WORD;

    if ((this->src_mode == ADDR_MODE_UNKNOWN) || (this->src_mode == ADDR_MODE_DIRECT_ADDR)) {
        this->is_valid = false;
    }
}

void Div::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint16_t src_data = cpu_state.get_data(this->src_mode, this->src_reg, this->data_size);
    uint32_t dest_data = cpu_state.registers.get(this->dest_reg, SIZE_LONG);
    uint32_t result;

    if (this->is_signed) {
        int64_t quotient = static_cast<int64_t>(static_cast<int32_t>(dest_data)) / static_cast<int16_t>(src_data);
        int64_t remainder = static_cast<int64_t>(static_cast<int32_t>(dest_data)) % static_cast<int16_t>(src_data);
        if ((quotient < INT16_MIN) || (quotient > INT16_MAX)) {
            cpu_state.registers.set(SR_FLAG_OVERFLOW, true);
            cpu_state.registers.set(SR_FLAG_CARRY, false);
            return;
        }
        result = MASK_16(static_cast<uint32_t>(quotient)) | (static_cast<uint32_t>(remainder) << 16);
    } else {
        uint32_t quotient = dest_data / src_data;
        uint32_t remainder = dest_data % src_data;
        if (quotient > UINT16_MAX) {
            cpu_state.registers.set(SR_FLAG_OVERFLOW, true);
            cpu_state.registers.set(SR_FLAG_CARRY, false);
            return;
        }
        result = quotient | (remainder << 16);
    }

    cpu_state.set_data(this->dest_mode, this->dest_reg, SIZE_LONG, result);

    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(MASK_16(result), SIZE_WORD));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(MASK_16(result), SIZE_WORD));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
    cpu_state.registers.set(SR_FLAG_CARRY, false);
}

std::string Div::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << (this->is_signed ? "divs" : "divu") << DISASSEMBLER::size_suffix(this->data_size)
           << " " << DISASSEMBLER::effective_address(this->src_mode, this->src_reg, this->data_size, cpu_state)
           << ", " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, this->data_size, cpu_state);

    return output.str();
}


std::unique_ptr<INSTRUCTION::Instruction> Div::create(uint16_t opcode) {
    return std::make_unique<Div>(opcode);
}

#include "instructions/move.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;

Move::Move(uint16_t opcode) : Instruction(opcode) {
    uint16_t dest_mode_part = (opcode >> 6) & 0x7;
    uint16_t dest_reg_part = (opcode >> 9) & 0x7;
    uint16_t src_mode_part = (opcode >> 3) & 0x7;
    uint16_t src_reg_part = (opcode >> 0) & 0x7;

    this->dest_mode = get_addressing_mode(dest_mode_part, dest_reg_part);
    this->src_mode = get_addressing_mode(src_mode_part, src_reg_part);

    this->dest_reg = get_register_type(dest_mode_part, dest_reg_part);
    this->src_reg = get_register_type(src_mode_part, src_reg_part);

    if ((this->src_mode == ADDR_MODE_UNKNOWN) || (!IS_ALTERABLE(this->dest_mode))) {
        this->is_valid = false;
    }

    if (
        (this->src_mode == ADDR_MODE_UNKNOWN) ||
        (this->dest_mode == ADDR_MODE_UNKNOWN) ||
        (this->dest_mode == ADDR_MODE_PC_DISPLACEMENT) ||
        (this->dest_mode == ADDR_MODE_PC_INDEX) ||
        (this->dest_mode == ADDR_MODE_IMMEDIATE)

    ) {
        this->is_valid = false;
    }

    uint16_t data_size_part = (opcode >> 12) & 0x3;
    switch (data_size_part) {
        case 0x01: { // 0b01
            this->data_size = SIZE_BYTE;
            break;
        }
        case 0x02: { // 0b10
            this->data_size = SIZE_LONG;
            break;
        }
        case 0x03: { // 0b11
            this->data_size = SIZE_WORD;
            break;
        }
        default: {
            this->is_valid = false;
        }
    }

    this->is_movea = (this->dest_mode == ADDR_MODE_DIRECT_ADDR);

    //movea
    if (this->is_movea && (this->data_size == SIZE_BYTE)) {
        this->is_valid = false;
    }
}

void Move::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t src_data = 0;

    src_data = cpu_state.get_data(this->src_mode, this->src_reg, this->data_size);

    if (this->is_movea) {
        if (this->data_size == SIZE_WORD) {
            src_data = static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(src_data)));
        }
        cpu_state.set_data(this->dest_mode, this->dest_reg, SIZE_LONG, src_data);
    } else {
        cpu_state.set_data(this->dest_mode, this->dest_reg, this->data_size, src_data);
        cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(src_data, this->data_size));
        cpu_state.registers.set(SR_FLAG_ZERO, src_data == 0);
        cpu_state.registers.set(SR_FLAG_OVERFLOW, false);
        cpu_state.registers.set(SR_FLAG_CARRY, false);
    }
}

std::string Move::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;
    output << (this->is_movea ? "movea" : "move") << DISASSEMBLER::size_suffix(this->data_size)
           << " " << DISASSEMBLER::effective_address(this->src_mode, this->src_reg, this->data_size, cpu_state)
           << ", " << DISASSEMBLER::effective_address(this->dest_mode, this->dest_reg, this->data_size, cpu_state);

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> Move::create(uint16_t opcode) {
    return std::make_unique<Move>(opcode);
}

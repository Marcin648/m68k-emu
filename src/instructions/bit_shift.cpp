#include "instructions/bit_shift.hpp"
#include "helpers.hpp"
#include <stdexcept>

using namespace M68K;
using namespace INSTRUCTION;
BitShift::BitShift(uint16_t opcode) : Instruction(opcode) {
    uint16_t addr_mode_part = (opcode >> 3) & 0x7;
    uint16_t addr_reg_part = (opcode >> 0) & 0x7;
    uint16_t direction_part = (opcode >> 8) & 0x1;
    uint16_t data_size_part = (opcode >> 6) & 0x3;

    this->direction_left = direction_part == 1;
    this->is_memory = false;

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
        case 0x03: { // 0b11
            this->data_size = SIZE_WORD;
            this->is_memory = true;
            break;
        }
        default: {
            this->is_valid = false;
        }
    }

    // register form: type in bits 4-3, memory form: type in bits 10-9
    uint16_t instruction_type_part = (opcode >> (this->is_memory ? 9 : 3)) & 0x3;
    uint16_t is_reg_part = (opcode >> 5) & 0x1;
    uint16_t count_part = (opcode >> 9) & 0x7;

    if (this->is_memory) {
        this->addr_mode = get_addressing_mode(addr_mode_part, addr_reg_part);
        this->addr_reg = get_register_type(addr_mode_part, addr_reg_part);
        this->is_imm = true;
        this->imm_shift = 1;
        if (!IS_MEMORY_ALTERABLE(this->addr_mode)) {
            this->is_valid = false;
        }
    } else {
        this->is_imm = (is_reg_part == 0);
        if (this->is_imm) {
            this->imm_shift = (count_part == 0) ? 8 : count_part;
        } else {
            this->shift_reg = get_register_type(0, count_part);
        }
        this->addr_mode = ADDR_MODE_DIRECT_DATA;
        this->addr_reg = get_register_type(0, addr_reg_part);
    }

    this->instruction_type = static_cast<InstructionType>(instruction_type_part);
    switch (this->instruction_type) {
        case InstructionType::AS: this->instruction_str = "as"; break;
        case InstructionType::LS: this->instruction_str = "ls"; break;
        case InstructionType::ROX: this->instruction_str = "rox"; break;
        case InstructionType::RO: this->instruction_str = "ro"; break;
    }
    this->instruction_str += (this->direction_left ? "l" : "r");
}

void BitShift::execute(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    uint32_t shift = this->imm_shift;
    if (!this->is_imm) {
        shift = cpu_state.get_data(ADDR_MODE_DIRECT_DATA, this->shift_reg, SIZE_LONG) % 64;
    }

    uint32_t data = cpu_state.get_data_silent(this->addr_mode, this->addr_reg, this->data_size);
    const uint32_t msb_mask = 1u << (this->data_size * 8 - 1);

    bool extend = cpu_state.registers.get(SR_FLAG_EXTEND);
    bool carry = false;
    bool overflow = false;

    // shift one bit at a time, carry holds the last bit shifted out
    for (uint32_t i = 0; i < shift; i++) {
        bool msb = data & msb_mask;
        bool lsb = data & 0x1;

        if (this->direction_left) {
            carry = msb;
            data <<= 1;
            switch (this->instruction_type) {
                case InstructionType::AS: {
                    if (static_cast<bool>(data & msb_mask) != msb) {
                        overflow = true;
                    }
                    break;
                }
                case InstructionType::LS: break;
                case InstructionType::ROX: data |= extend ? 0x1 : 0x0; break;
                case InstructionType::RO: data |= msb ? 0x1 : 0x0; break;
            }
        } else {
            carry = lsb;
            data >>= 1;
            switch (this->instruction_type) {
                case InstructionType::AS: data |= msb ? msb_mask : 0x0; break;
                case InstructionType::LS: break;
                case InstructionType::ROX: data |= extend ? msb_mask : 0x0; break;
                case InstructionType::RO: data |= lsb ? msb_mask : 0x0; break;
            }
        }
        data = MASK(data, this->data_size);

        if (this->instruction_type != InstructionType::RO) {
            extend = carry;
        }
    }

    if ((this->instruction_type == InstructionType::ROX) && (shift == 0)) {
        carry = extend;
    }

    cpu_state.set_data(this->addr_mode, this->addr_reg, this->data_size, data);

    cpu_state.registers.set(SR_FLAG_EXTEND, extend);
    cpu_state.registers.set(SR_FLAG_NEGATIVE, IS_NEGATIVE(data, this->data_size));
    cpu_state.registers.set(SR_FLAG_ZERO, IS_ZERO(data, this->data_size));
    cpu_state.registers.set(SR_FLAG_OVERFLOW, overflow);
    cpu_state.registers.set(SR_FLAG_CARRY, carry);
}

std::string BitShift::disassembly(CPUState& cpu_state) const {
    uint32_t pc = cpu_state.registers.get(REG_PC, SIZE_LONG);
    pc += SIZE_WORD;
    cpu_state.registers.set(REG_PC, SIZE_LONG, pc);

    std::ostringstream output;

    output << this->instruction_str << DISASSEMBLER::size_suffix(this->data_size) << " ";
    if (this->is_memory) {
        output << DISASSEMBLER::effective_address(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    } else if (this->is_imm) {
        output << "#$" << std::hex << static_cast<uint32_t>(this->imm_shift);
        output << ", " << DISASSEMBLER::effective_address(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    } else {
        output << DISASSEMBLER::effective_address(ADDR_MODE_DIRECT_DATA, this->shift_reg, this->data_size, cpu_state);
        output << ", " << DISASSEMBLER::effective_address(this->addr_mode, this->addr_reg, this->data_size, cpu_state);
    }

    return output.str();
}

std::unique_ptr<INSTRUCTION::Instruction> BitShift::create(uint16_t opcode) {
    return std::make_unique<BitShift>(opcode);
}
